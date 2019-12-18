sed -i 's:unnamedblocks_SOURCES.*$:unnamedblocks_SOURCES = '"`find -name *.cpp -printf "%p "`"':' Makefile.am
