echo on

echo "Deploy ModbusScope ..."

g++ -v

cd release\src\bin\win
IF ERRORLEVEL 1 GOTO errorHandling

set DEPLOY_DIR=deploy

mkdir %DEPLOY_DIR%
copy modbussim.exe %DEPLOY_DIR%
IF ERRORLEVEL 1 GOTO errorHandling

cd %DEPLOY_DIR%
windeployqt.exe modbussim.exe --compiler-runtime -verbose 2
IF ERRORLEVEL 1 GOTO errorHandling

cd ..
7z a ModbusSim.zip ".\%DEPLOY_DIR%\*"
IF ERRORLEVEL 1 GOTO errorHandling

EXIT

:errorHandling
EXIT /B 1