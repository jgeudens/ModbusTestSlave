echo on

echo Building ModbusSim using MinGW ...
mkdir release
cd release

echo %PATH%

g++ -v

cmake -G "Ninja" ..
ninja

cd ..
