
@echo Generate build files>> setup.log
@echo Generating build files...
@cmake-3.17.2-win64-x64\bin\cmake -B build -G "Visual Studio 16 2019" -A x64 . >> output.log
@if NOT ["%errorlevel%"]==["0"] exit

@echo building libraries>> setup.log
@echo Building libraries, this will take some time...
@cmake-3.17.2-win64-x64\bin\cmake --build ./build --target ALL_BUILD --config Release >> output.log
@if NOT ["%errorlevel%"]==["0"] exit


@echo The driver has been built. The following steps copy the driver to the default SteamVR drivers folder and registers it.
@set /P copyDriver="Do you want the driver copied to the default location (C:\Program Files (x86)\Steam\steamapps\common\SteamVR\drivers\) (Y/N)?"
@if copyDriver == "Y" (
@echo Copying files to driver directory>> setup.log
@echo Copying North Star driver files to SteamVR drivers...
@xcopy  resources\northstar "C:\Program Files (x86)\Steam\steamapps\common\SteamVR\drivers\northstar\" /e /Y >> output.log
@if NOT ["%errorlevel%"]==["0"] exit

@echo Copying device libraries>> setup.log
@echo Copying device libraries... 
@xcopy build\northstar\Release\*.dll "C:\Program Files (x86)\Steam\steamapps\common\SteamVR\drivers\northstar\bin\win64\" /Y >> output.log
@if NOT ["%errorlevel%"]==["0"] exit
@set registerPath="C:\Program Files (x86)\Steam\steamapps\common\SteamVR\drivers\northstar"
) ELSE (
@set registerPath=%~dp0resources\northstar 
)

@echo Registering driver>> setup.log
@echo Registering driver 
@"C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\vrpathreg" adddriver %registerPath% >> output.log
@if NOT ["%errorlevel%"]==["0"] exit
@"C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\vrpathreg" show >> setup.log
@if NOT ["%errorlevel%"]==["0"] exit

@echo Setup Complete!
@pause