echo on

echo Building ModbusSim using MinGW ...
mkdir release
cd release

echo %PATH%

g++ -v

cmake -G "Ninja" ..
IF ERRORLEVEL 1 GOTO errorHandling

ninja
IF ERRORLEVEL 1 GOTO errorHandling

cd ..

EXIT /B 0

:errorHandling
EXIT /B 1
