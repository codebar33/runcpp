#include <iostream>
#include <fstream>
#include <random>

#define BIN_PATH std::string( "/usr/bin" )

bool fileExists( const std::string & path )
{
    std::ifstream file( path );

	return file;
}

void removeFile( const std::string & path )
{
	std::remove( path.c_str() );
}

void execute( const std::string & command )
{
    std::system( command.c_str() );
}

std::string temporaryFilePath( void )
{
	std::string tmpFilePath;

	std::mt19937                                generator;
	std::uniform_int_distribution< uint16_t >   distribution( 97, 122 );

	try
	{
    	generator = std::mt19937( std::random_device()() );
	}
	catch( ... )
	{
	}

	for( uint32_t i = 0; i < 20; i++ )
	{
		tmpFilePath += char( distribution( generator ) );
	}

	return "/tmp/" + tmpFilePath;
}

int main( int argc, char ** argv )
{
	std::string 	tmpFilename;
	std::string 	sourceCode;
	std::string		sourceCodePath;
	std::ofstream	strippedCodeStream;

	if( argc < 2 )
	{
		std::cout << "C++ code missing!" << std::endl;

		return 0;
	}

	// Get C++ source code file path
	sourceCodePath = argv[ 1 ];

	// Create temporary filename
	tmpFilename = temporaryFilePath();

	// Read C++ source code
	std::ifstream s( sourceCodePath );
	sourceCode = std::string( std::istreambuf_iterator< char >( s ), std::istreambuf_iterator< char >() );

	// Strip first line (#!/usr/bin/bnr)
	sourceCode = sourceCode.substr( sourceCode.find( '\n' ) );

	// Write C++ code to file
	strippedCodeStream.open( tmpFilename + ".cc" );
	strippedCodeStream << sourceCode;
	strippedCodeStream.close();

	// Check if CLANG is available
	if( fileExists( BIN_PATH + "/clang++" ) )
	{
    	// Compile C++ code with clang++
    	execute( BIN_PATH + "/clang++ " + tmpFilename + ".cc -o " + tmpFilename );
	}
	else if( fileExists( BIN_PATH + "/c++" ) )
	{
    	// Compile C++ code with c++
    	execute( BIN_PATH + "/c++ " + tmpFilename + ".cc -o " + tmpFilename );
	}
	else if( fileExists( BIN_PATH + "/g++" ) )
	{
		// Compile C++ code with g++
		execute( BIN_PATH + "/g++ " + tmpFilename + ".cc -o " + tmpFilename );
	}
	else
	{
		std::cout << BIN_PATH << "/clang++, " << BIN_PATH << "/c++ or " << BIN_PATH << "/g++ are missing!" << std::endl;

		removeFile( tmpFilename + ".cc" );

		return 0;
	}

	// Execute builded app
	execute( tmpFilename );

	// Remove temporary files
	removeFile( tmpFilename + ".cc" );
	removeFile( tmpFilename );

	return 0;
}
