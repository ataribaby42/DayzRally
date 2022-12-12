# DayzRally
Dayz Rally event implementation purely made using only mission init.c. Each driver or driver/navigator crew starts one by one with some separation (we used 2 minutes separation) and crossing start point clock starts till crew crosses finish point. Then all CPs are evaluated and possible penalty for missed ones is added towards final time. Final time is shown periodicaly after finish to each player that finished race. all CP times progress are shown to all rally admins and to each players. Admins see all events notifications. All Game Messages must be set on to see that in-game.

## INSTALLATION AND SETUP
Simply look into provided init.c what is based on vanilla Dayz 1.19 Chernarus mission. Everything related to rally starts with Rally prefix.

### Settings
**vector RallyResetPos = Vector(1645.750000, 451.746002, 14190.099609);** - Position of rally event reset spot. In case any Rally Admin stands there for more than minute, rally will reset.

**float RallyResetRadius = 100;** - Radius of rally event reset spot in meters.

**RallyPenalty = 30;** - Penalty in seconds for missed Control Points, will be added towards finish time after crossing finish line.

**RallyAdmins.Insert("7xxxxxxxxxxxxxxx9");** - Rally Admin definition, use SteamID.

**RallyCPs.Insert(new RallyCP("Start", "S", Vector(547.258972, 421.897003, 11117.799805), 5));** - Start point definition coord and trigger radius in meters.

**RallyCPs.Insert(new RallyCP("CP1", "CP", Vector(1120.680054, 331.583008, 11326.200195), 10));** - Check point definition coord and trigger radius in meters. You can define as many as you wish, just use unique CP name like CP1, CP2, CP3 and so on...

**RallyCPs.Insert(new RallyCP("Finish", "F", Vector(10190.599609, 24.241400, 2543.530029), 10));** - Finish point definition coord and trigger radius in meters.

### Recomendations
We used VPPAdminTools to build race track and used color barrells for start, finish and CPs. Seachest objects was used to get wach point coords to be filled into script.

All loot spawning, events, zombies and animals shold be completly disabled. Only objects used as cars, car parts, rally scenery or needed equipement should be in types.xml with nominal and min set to 0 just have lifetime defined to keep objects from despawning.

## FINAL NOTES
Script is provided as is. It is up to you to figure out how to build your rally track or how to set-up server zombie free.

Here is example of our Rally event

https://www.youtube.com/watch?v=TapIEQ6JD_I

https://www.youtube.com/watch?v=jufVHhrzgUY

Have fun!

## CREDITS
ataribaby42 - Idea & Code 
twitch.tv/lonertao - Ideas & Track building
