# barrett

A Rise of the Triad source port based on [ROTTEXPR](https://github.com/LTCHIPS/rottexpr) focused on QoL, moddability/ease of programming, and better support for modern hardware.

New gameplay options inherited from ROTTEXPR:

* Blitzguards being outfitted with any weapon from the missile/magic weapon
  arsenal! Yes, that means Blitzguards can use excalibats too!
* You can now pick up ammo from dropped missile weapons!
* A special respawning enemy game mode called ZomROTT, where you have to gib
  your enemies to keep them from coming back!
* Toggleable Auto Aim
* Auto Aim for Missile Weapons
* ...and more

## Requirements

Libraries:

`SDL2.dll` (https://www.libsdl.org/download-2.0.php)

`SDL2_mixer.dll` (https://www.libsdl.org/projects/SDL_mixer/)

Game Files:

```
DEMO1_3.DMO
DEMO2_3.DMO
DEMO3_3.DMO
DEMO4_3.DMO
REMOTE1.RTS
```

And

```
DARKWAR.RTC
DARKWAR.RTL
DARKWAR.WAD
```

Or

```
HUNTBGIN.RTC
HUNTBGIN.RTL
HUNTBGIN.WAD
```

Place all those files in the same directory as the barrett executable.

## Building

To build the project, you'll need the SDL2.0 development libraries
(https://www.libsdl.org/download-2.0.php) as well as the SDL_mixer developement
libaries (https://www.libsdl.org/projects/SDL_mixer/, under Binary).

For Linux users, install the respective SDL2 dependencies through your distro's package manager.

Before running make on the project, double check the first few lines of the
`Makefile` and make sure you build the right version.

On Windows, use MinGW with gcc to build the project.
