rmdir build
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -DVER_CLIENT="0.0.0" -DVER_SERVER="0.0.0" -DVER_MOD_VIN="0.0.0" -DBETA=ON -DAVX=OFF -DMODS=ON .. && echo done