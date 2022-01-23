#!/bin/bash
echo formatting...
echo [1/5] ub_client/
find ub_client -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [2/5] ub_cappuccino/
find ub_cappuccino -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [3/5] ub_espresso/
find ub_espresso -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [4/5] ub_rhr/
find ub_rhr -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo [5/5] mods/
find mods -iname *.hpp -o -iname *.cpp | xargs clang-format -i
echo ...formatted
