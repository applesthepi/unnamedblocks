#!/bin/bash
echo formatting...
echo [1] ub_client/
find ub_client -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [2] ub_cappuccino/
find ub_cappuccino -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [3] ub_espresso/
find ub_espresso -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [4] ub_rhr/
find ub_rhr -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [5] mods/
find mods -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [6] cmake files
find ub_client ub_cappuccino ub_espresso ub_rhr mods -iname CMakeLists.txt -o -iname CMakeLists.txt | xargs cmake-format -i
cmake-format CMakeLists.txt -o CMakeLists.txt
echo ...formatted
