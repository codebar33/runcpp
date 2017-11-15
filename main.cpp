//
//  main.cpp
//  runcpp
//
//  Created by Jérôme Bédat on 14.11.2017.
//  Copyright © 2017 Jérôme Bédat. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <random>

void showUsage( void )
{
    std::cerr << "OVERVIEW: runcpp (https://github.com/codebar33/runcpp)\n\n"
              << "Runcpp can compile and execute C or C++ source code or simple code expressions.\n"
              << "Once installed in /usr/bin, you can add \"#!/usr/bin/runcpp\" at the beginning of your source code to allow its \"direct\" execution.\n"
              << "It uses the default system C and C++ compiler but you can specify one if you like.\n"
              << "C and C++ file extensions are currently supported, feel free to add a support for more languages!\n\n"
              << "USAGE: runcpp [options] source code file or code expression\n\n"
              << "OPTIONS:\n"
              << "\t-h,--help\tShow this help message\n"
              << "\t-c,--compiler\tSpecify the compiler (cc, c++, gcc, g++, clang, clang++...)"
              << std::endl;
}

bool fileExists( const std::string & path )
{
    std::ifstream file( path );

    return file.good();
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

    for( uint32_t i = 0; i < 10; i++ )
    {
        tmpFilePath += char( distribution( generator ) );
    }

    return "/tmp/" + tmpFilePath;
}

std::string findCompilerForSourceFileExtension( const std::string & sourceCodePath )
{
    std::string fileExt;
    
    // Get file extension
    fileExt = sourceCodePath.substr( sourceCodePath.find_last_of( '.' ) );
    
    // If not file extension we use the C++ compiler
    if( fileExt == "" )
    {
        return "c++";
    }

    // Extension to upper
    std::transform( fileExt.begin(), fileExt.end(), fileExt.begin(), ::toupper );
    
    if( fileExt == ".CC" || fileExt == ".CPP" || fileExt == ".C++" || fileExt == ".CXX"  )
    {
        return "c++";
    }
    else
    {
        return "cc";
    }
}

int main( int argc, char * argv[] )
{
    std::string     tmpFilename;
    std::string     tmpFilenameExt;
    std::string     sourceCode;
    std::string     envMainCode;
    std::string     sourceCodePath;
    std::string     compiler;
    std::ofstream   strippedCodeStream;
    bool            sourceCodePathRetrieved;

    if( argc < 2 )
    {
        showUsage();

        return 1;
    }
    
    sourceCodePathRetrieved = false;
    
    for ( int i = 1; i < argc; ++i )
    {
        std::string arg = argv[ i ];
        
        if( arg == "-h" || arg == "--help" )
        {
            showUsage();
            
            return 0;
        }
        else if( arg == "-c" || arg == "--compiler" )
        {
            if( i + 1 < argc )
            {
                compiler = argv[ i++ ]; // Increment 'i' so we don't get the argument as the next argv[ i ].
            }
            else
            {
                // Uh-oh, there was no argument to the compiler option.
                std::cerr << "--compiler option requires one argument." << std::endl;
                
                return 1;
            }
        }
        else
        {
            // Get source code file path or code to evaluate
            if( sourceCodePathRetrieved )
            {
                sourceCodePath += " ";
            }
            
            sourceCodePath += arg;
            
            sourceCodePathRetrieved = true;
        }
    }

    // Create temporary filename
    tmpFilename = temporaryFilePath();

    // Read C++ source code
    std::ifstream s( sourceCodePath );
    if( s.good() )
    {
        // Get source code from file
        sourceCode = std::string( std::istreambuf_iterator< char >( s ), std::istreambuf_iterator< char >() );
    
        // Strip first line (#!/usr/bin/bnr)
        sourceCode = sourceCode.substr( sourceCode.find( '\n' ) );
    }
    else
    {
        // It's not a file, so we just get the code expression
        
        sourceCode = sourceCodePath;
    }

    // Add
    if
    (
        sourceCode.find( " main(" ) == std::string::npos &&
        sourceCode.find( " main (" ) == std::string::npos
    )
    {
        // Add main if needed  !!! https://github.com/macmade/STDC
        envMainCode =   "#include <iostream>\n"
                        "using namespace std;\n"
                        "int main( int argc, char ** argv ) {\n";
        if( sourceCode.find( ';' ) == std::string::npos )
        {
            envMainCode += "std::cout << " + sourceCode + " << std::endl;\n";
        }
        else
        {
            envMainCode += sourceCode + "\n";
        }
        envMainCode += "return 0; }\n";

        sourceCode = envMainCode;

        // Compiler is C++ when evaluating expressions
        compiler = "c++";
    }
    else
    {
        // Get compiler if needed
        if( compiler.length() == 0 )
        {
            compiler = findCompilerForSourceFileExtension( sourceCodePath );
        }
    }

    // Determine source code extension, depending on compiler
    if( compiler == "cc" )
    {
        tmpFilenameExt = ".c";
    }
    else
    {
        tmpFilenameExt = ".cpp";
    }

    // Write C++ code to file
    strippedCodeStream.open( tmpFilename + tmpFilenameExt );
    strippedCodeStream << sourceCode;
    strippedCodeStream.close();

    // Compile code
    execute( compiler + " " + tmpFilename + tmpFilenameExt + " -o " + tmpFilename );

    // Execute builded app
    execute( tmpFilename );

    // Remove temporary files
    removeFile( tmpFilename + tmpFilenameExt );
    removeFile( tmpFilename );

    return 0;
}

