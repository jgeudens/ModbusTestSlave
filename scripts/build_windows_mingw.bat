echo on

SET project_dir="%cd%"
echo Building ModbusSim using MinGW ...
mkdir build_windows
cd build_windows

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

cd ..
