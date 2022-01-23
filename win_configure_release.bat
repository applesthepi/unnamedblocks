if not exist "build/release" mkdir "build/release"
cd "build/release"
cmake -DVER_CLIENT="\"0.0.0\"" -DVER_SERVER="\"0.0.0\"" -DVER_MOD_VIN="\"0.0.0\"" -DBETA=ON -DAVX=OFF -DMODS=ON -DCMAKE_BUILD_TYPE=Release ../.. && echo done
