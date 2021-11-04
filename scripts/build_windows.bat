echo on

echo Building ModbusSim using MinGW ...
mkdir release
cd release

echo %PATH%
g++ -v

cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja

