echo on

echo Building ModbusSim using MinGW ...
mkdir release
cd release

cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..

ninja

