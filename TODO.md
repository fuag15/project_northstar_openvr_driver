# Immediate Goals
- New Configuration method
    - Blind copy is definitely wrong, investigate inconsistencies found further
- Clean up new configuration method

# General Polish
- Test clean up (add tests to account for default poses)
- remove error on startup if structure sensor is not available
- fix hmd presence checks / serial numbers
- read relevant debug flags from config file not hard coded
- build to a directory you can install to
- better hashing function
- clean up serial numbers to be more representative
- default input bindings polish
- hook into ctest
- this hasn't caused any known issues but monitor property querying is likely querying the wrong monitor and only implemented on win32 platforms. See HostProber for details

# Optics Polish
- Play around with / optimize parameters for Warp Solver

# Structure Sensor Polish
- change initial pose to 0, 0, 0
- dont need most of the data from structure sensor, configure session to only request relevant data and experiment with low precision mode
- Headset offset measure for better rotation
- vel / accel are very choppy
- standby mode
- ensure server stops tracking when driver stops

# Leap controller Polish
- better model
- vel is choppy
- standby mode
- ensure server stops tracking when driver stops
- gesture recognition system