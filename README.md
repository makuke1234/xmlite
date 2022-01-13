# xmlite

![Release version](https://img.shields.io/badge/alpha-v0.9.2-green.svg)
![C++ version](https://img.shields.io/badge/version-C++11-blue.svg)

A light-weight single-include C++ XML library, relying on KISS (Keep It Stupid Simple)
principle and C++11 STL (similar to my other json library [jsonlite](https://github.com/makuke1234/jsonlite)).


# Overview

Yes, this library is designed to aim for fast compilation times, simplicity and
ease of use and almost full compliancy with XML standard, nothing else.

This library also supports exceptions, they are designed to give hints about
syntax errors and to be comprehensive. Every object in this libary uses the
xmlite::exception as the exception class.

Internally all files with any other BOM-marked encoding (listed in *Features* section)
other than UTF-8 are internally converted to BOM-less UTF-8 (DOM is UTF-8). All non-BOM-marked encodings
forwards-compatible with UTF-8 should be OK.


# Features

* DOM-compatible encoding support by default (UTF-8)
* BOM support:
	* UTF-1
	* UTF-7
	* UTF-8
	* UTF-16 (little endian & big endian)
	* UTF-32 (little endian & big endian)
* BOM to DOM conversion
* Quite fool-proof, XML files are checked for correctness before parsing.
* Supports tag attributes, e.g `<tag name="John" age="55"></tag>`
* Supports multiple values inside tag, e.g `<tag>1st value<anotherTag>Value inside child tag.</anotherTag>3rd value</tag>`
* Supports value-less tags, e.g `<tag attr1="attribute 1" attr2="some other attribute" />`
* DOM to XML dumping support
* CRLF/LF/CR neutrality -> all dumps are LF


# C bindings

The neccessary source files, to compile this C++ library yourself, are loacted in
`C_bindings`. Also, a makefile is provided to compile with GNU C++ compiler. To use this
library in C, you have to include the header file [`C_bindings/include/xmlite.h`](https://github.com/makuke1234/xmlite/blob/master/C_bindings/include/xmlite.h) and link against 64-bit library [`C_bindings/bin/libxmlite.a`](https://github.com/makuke1234/xmlite/raw/master/C_bindings/bin/libxmlite.a) (so long as you use GCC's linker to link also against C++ STL).
If you want a 32-bit version you have to compile it from source.

The extra documentation for C bindings is located in the C header file `xmlite.h`.


# Testing

This project is by no means perfect. It is open to improvement and suggestion under
the *Issues* tab. Feel free to use this option whenever my library breaks down.
Issues regarding the parsing of syntactically correct XML are first priority.
Also feel free to suggest any encoding support, that you may feel are neccessary.
Currently every encoding, that is forwards-compatible with UTF-8, should be supported.


# Licensing

This project uses the MIT license.
