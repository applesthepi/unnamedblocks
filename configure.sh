#!/bin/bash
rm -rf build && mkdir build;
cd build;
cmake -DCMAKE_INSTALL_PREFIX=install -DVER_CLIENT="\"0.0.0\"" -DVER_SERVER="\"0.0.0\"" -DVER_MOD_VIN="\"0.0.0\"" -DBETA=ON -DAVX=OFF -DMODS=ON -DCMAKE_BUILD_TYPE=Debug .. && echo done;
