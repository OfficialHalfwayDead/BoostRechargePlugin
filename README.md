# BoostRechargePlugin
Custom plugin for BakkesMod that allows you to constantly recharge a given amount of boost at a desired time interval.

## Installation
Easy way: https://bakkesplugins.com/plugins/view/8

Manual:

1. Copy `BoostRechargePlugin.dll` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/plugins`
2. Copy `boostrechargeplugin.set` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/plugins/settings`
3. Add `plugin load boostrechargeplugin` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/cfg/plugins.cfg`

## Commands

**boost_recharge** - Set to 1, to enable. Will automatically disable unlimited boost. Set to 0 to disable.

**boost_recharge_amount** - The amount of boost you want to get every time it triggers.

**boost_recharge_interval** - The time interval (in seconds) at which you want the boost to be given.

**boost_recharge_groundonly** - Only recharge boost while the wheels are touching an object.