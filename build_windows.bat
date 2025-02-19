@echo off

call :set_variable BUILD_TYPE Release %BUILD_TYPE%
call :set_variable DAL_DIR "%CD%" %DAL_DIR%
call :set_variable ADDRESS_MODEL Win64 %ADDRESS_MODEL%
call :set_variable MSVC_RUNTIME dynamic %MSVC_RUNTIME%
call :set_variable MSVC_VERSION "Visual Studio 16 2019" %MSVC_VERSION%

echo BUILD_TYPE:  %BUILD_TYPE%
echo DAL_DIR: %DAL_DIR%
echo ADDRESS_MODEL: %ADDRESS_MODEL%
echo MSVC_RUNTIME: %MSVC_RUNTIME%
echo MSVC_VERSION: %MSVC_VERSION%

git submodule init
git submodule update

cd machinist2

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

echo Starting build machinist2
call build_windows.bat
echo End build machinist2

set MACHINIST_TEMPLATE_DIR=%CD%\template\
echo MACHINIST_TEMPLATE_DIR=%MACHINIST_TEMPLATE_DIR%
bin\Machinist.exe -c ../config/dal.ifc -l ../config/dal.mgl -d ../dal

if %errorlevel% neq 0 exit /b 1

cd ..

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if "%ADDRESS_MODEL%"=="Win64" (
  set PLATFORM=x64
) else (
  if "%MSVC_VERSION%"=="Visual Studio 16 2019" (
    set PLATFORM=x64
  ) else (
    set PLATFORM=Win32
  )
)

if "%ADDRESS_MODEL%"=="Win64" (
  if "%MSVC_VERSION%"=="Visual Studio 16 2019" (
    set ADDRESS_MODEL=
  )
)


if "%ADDRESS_MODEL%" =="Win64" (
cmake -G "%MSVC_VERSION% %ADDRESS_MODEL%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%DAL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
) else (
cmake -G "%MSVC_VERSION%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_INSTALL_PREFIX=%DAL_DIR% -DMSVC_RUNTIME=%MSVC_RUNTIME% ..
)

if %errorlevel% neq 0 exit /b 1

msbuild dal.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..

bin\test_suite.exe

if %errorlevel% neq 0 exit /b 1

@echo on

:set_variable
if "%~3"=="" (
 set %~1=%~2
)
EXIT /B 0