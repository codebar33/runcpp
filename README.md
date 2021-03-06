**runcpp**
=======

![Status](https://img.shields.io/badge/status-active-brightgreen.svg?style=flat)
![License](https://img.shields.io/badge/license-mit-brightgreen.svg?style=flat)
[![Contact](https://img.shields.io/badge/contact-@JeBedat-blue.svg?style=flat)](https://twitter.com/JeBedat)  

Quickly run single C++ source code files
--------------------------------------------------------------------------------------------

About
-----

**runcpp** can execute C++ source code files, as you do with shell / bash scripts. No more need to build and run successively, you can quickly evaluate simple C++ code snippets.


```shell
$ ./hello.cpp
hello world!
```

How does it work?
-----------------
Add ```#!/usr/local/bin/runcpp``` at the beginning of your C++ source code and set an execute permission on your file (```chmod 755 ./hello.cpp```) to allow its execution.

```cpp
#!/usr/local/bin/runcpp

#include <iostream>

using namespace std;

int main( int argc, char ** argv )
{
	cout << "hello world!" << endl;

	return 0;
}
```

> **NOTE:** Change the path /usr/local/bin/runcpp depending on your **runcpp** installation path.


Execute simple expressions
--------------------------
Example 1
```shell
$ ./runcpp "for( int i = 0; i < 10; i++ ){ cout << i << endl; }"
0
1
2
3
4
5
6
7
8
9
```

Example 2
```shell
$ ./runcpp "(42 % 10)"
2
```


Manual
------

```shell
$ ./runcpp --help
OVERVIEW: runcpp (https://github.com/codebar33/runcpp)

Runcpp can compile and execute C or C++ source code or simple code expressions.
Once installed in /usr/local/bin, you can add "#!/usr/local/bin/runcpp" at the beginning of your source code to allow its "direct" execution.
It uses the default system C and C++ compiler but you can specify one if you like.
C and C++ file extensions are currently supported, feel free to add a support for more languages!

USAGE: runcpp [options] source code file or code expression

OPTIONS:
	-h,--help	Show this help message
	-c,--compiler	Specify the compiler (cc, c++, gcc, g++, clang, clang++...)
```


License
-------

**runcpp** is released under the terms of the MIT License.

Repository Infos
----------------

    Owner:          Jérôme Bédat
    Web:            imazing.com
    Blog:           imazing.com/blog
    Twitter:        @JeBedat
    GitHub:         github.com/codebar33
