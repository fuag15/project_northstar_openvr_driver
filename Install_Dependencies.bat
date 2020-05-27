@echo Starting North Star Setup Script %time% %date%> setup.log
@echo Starting North Star Setup Script %time% %date%> output.log

@echo Unpacking cmake...
@7za x cmake-3.17.2-win64-x64.zip>> output.log

@cd dependencies
@echo Downloading Leap Drivers
@echo Downloading Leap Drivers>> ..\setup.log
@START /WAIT ..\cmake-3.17.2-win64-x64\bin\cmake .

@cd ..
@echo Installing Leap Drivers...
@echo Installing Leap Drivers>> setup.log
@START /WAIT dependencies\LeapDeveloperKit_4.0.0+52238_win\Leap_Motion_Installer_v4-multi-device_public_win_x86_4.0.0+52238_ah1889.exe /passive
@pause

@echo Installing MS build tools>> setup.log
@echo Installing MS build tools...
@START /WAIT vs_buildtools__2104452789.1549425347 --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 --add Microsoft.Net.Component.4.6.1.TargetingPack --add Microsoft.VisualStudio.Workload.VCTools --passive --promptrestart
@pause
