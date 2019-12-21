#!/bin/bash
sed -i 's:unnamedblocks_SOURCES.*$:unnamedblocks_SOURCES = '"`find UnnamedBlocks -name *.cpp -printf "%p "`"':' Makefile.am
sed -i 's:rhr_a_SOURCES.*$:rhr_a_SOURCES = '"`find RHR -name *.cpp -printf "%p "`"':' Makefile.am
sed -i 's:VinMod_la_SOURCES.*$:VinMod_la_SOURCES = '"`find VanillaMods -name *.cpp -printf "%p "`"':' Makefile.am
