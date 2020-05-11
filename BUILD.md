Versions of vendored libs not included and where to get them:

[Leap](https://developer.leapmotion.com/setup):

- LeapDeveloperKit 4.0.0+52238

[Structure Sensor](https://structure.io/):

- CrossPlatform SDK 0.7.1
- PerceptionEngine 0.7.1

## Build steps

```
git clone git@github.com:fuag15/project_northstar_openvr_driver.git
cd project_northstar_openvr_driver
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
```

- Open the generated solution and set northstart to the startup project (right click the project and choose the set as startup where the gear icon is) and build. Make sure to target x64 and a Release build to remove any object creation slowness.

- The release will be in `build/Release/`

- Copy all these dll's to wherever you want to install from, they should be combinded into the `resources/northstar/bin` directory, make this if it does not exist and put all generated dll's inside.

- Next register the driver with steamVR (using vrpathreg tool found in SteamVR bin folder) for me this is located at `C:\Program Files (x86)\Steam\steamapps\common\SteamVR\bin\win64\vrpathreg.exe`

```
vrpathreg adddriver <full_path_to/resources/northstar>
```

- at this point vrpathreg should show it registered under "external drivers", to disable it you can either disable in StamVR developer uptions under startup and manage addons, or vrpathreg removeddriver `full_path_to/resources/northsar`

- Running steamvr with the northstar connected (and no other hmd's connected) should work at this point but probably not have the right configuration for your hmd. Take the .json file from configuring your nothstar and convert it to the format found in `resources/northstar/resources/settings/default.vrsettings`

- restart steamvr. If driver is taking a long time to start, chances are it got built in debug mode, Release mode optimizes a way a bunch of intermediate object creation during the lens distortion solve that happens at startup. If things are still going south please maka a bug, I'll do my best to get to them asap.

