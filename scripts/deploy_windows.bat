echo on

echo "Deploy ModbusSim ..."

cd release\bin\win

echo %PATH%
g++ -v

mkdir ModbusSim
copy modbussim.exe ModbusSim

cd ModbusSim
windeployqt.exe modbussim.exe --compiler-runtime -verbose 2

REM Back to build dir
cd ..
7z a ModbusSim.zip ModbusSim
