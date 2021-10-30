echo on

echo Deploy ModbusSim ...

cd release\bin\win
dir

cd Release
dir

mkdir ModbusSim
copy modbussim.exe ModbusSim

cd ModbusSim

windeployqt.exe ModbusSim.exe --compiler-runtime -verbose 2

# Back to build dir
cd ..
7z a ModbusSim.zip ModbusSim
