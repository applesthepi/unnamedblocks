if not exist "build/debug" mkdir "build/debug"
cd "build/debug"
cmake -DVER_CLIENT="\"0.0.0\"" -DVER_SERVER="\"0.0.0\"" -DVER_MOD_VIN="\"0.0.0\"" -DBETA=ON -DAVX=OFF -DMODS=ON -DCMAKE_BUILD_TYPE=Debug ../.. && echo done
