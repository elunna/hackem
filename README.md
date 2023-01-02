# Hack'EM

Welcome, traveler, to Hack'EM!  

## Design Goals

**The first goal** of this project is to become the major successor of Slash'EM and port over most of Slash'EM to a modern platform. I chose EvilHack as a base because it's based on 3.6, inheritly more difficult and contains many Quality Of Life features already baked in. See here:

	- Nice HP Bar
	- Enhance menu shows max and % progress on skills
	- Extended commands: #terrain, #annotate, #overview, #tip, #give, #adjust splitting and merging
	- Peaceful displacing, paranoid swim, smart shop autopickup
	- Full bag altar BUC identification
	- New conducts: Elbereth, alignment, touched art, petless
	- New soko levels, new room shapes and themed rooms
	- Object materials, dtsund patch for dragon scales

Evil also has some challenging aspects that complement Slash'EM:

	- New roles: Infidel, convict
	- New Races: giant, tortle, centaur, and illithid
	- Elbereth: Can't use unless you found/discovered it
	- Partial intrinsics and partial reflection
	- No randomly generated magic markers
	- Strong monsters push through weaker monsters to get to you
	- Zombies ressurect, mummies wither, spellcasters are meaner
	- Monster steeds
    - Revamped Gehennom and new branches

Originally, I intended this variant to be a modern port of Slash'EM. However, once I reached close to the completion of that goal I found that there was quite a lot in other variants that I loved and just could not pass on including. Here are some of the goals that have been achieved so far:

- [x] Port over most of Slash'EM
- [x] Improve on monster, objects, and mechanics from Slash'EM - fixing or adding mechanics as needed.
- [x] Port over most of the SpliceHack monsters and objects that fit
- [x] Port over the Quality-of-Life features from UnNetHack
- [x] Port over the best changes from slashem-up
- [x] Create brand new material, mechanics, and levels.
- [x] Remove/edit Evil content so that users can more easily transition from vanilla to HackEM

## Remaining goals:

- [x] Use the in-game database to create a built-in wiki for players.
- [ ] Stabilize the current version and eliminate all major bugs.
- [ ] Integrate slashem updates and bugfixes from slashem9.
- [ ] Port over the best-of slashTHEM (requires creating tiles for all objects/monsters)
  - Jedi Role, Warrior Role, Bard Role
- [ ] Port over material from UnNethack that fits
  - Possibly Sheol, Dragon caverns, parts of Moria
- [ ] Port over material from dNetHack that fits
- [ ] Port over updates from 3.7 that fit
  - Extrinsic item protection
- [ ] Port over remaining stuff from SpliceHack that fits
  - Pirate role, Cartomancer role
  - Grass, blood tiles. Vents, new traps. 

## How to play

Hack'EM is now being hosted on the HardFought server!  Visit https://www.hardfought.org/ for details.

To install locally, follow the instructions below.

## Installation

Each OS type found under the `sys` folder has an installation guide for that
particular operating system. Pre-compiled binaries (linux and windows) can be
found here - https://github.com/elunna/HackEM/releases

For Linux (TL;DR version):
- Dependencies needed: `make` `gcc` `gdb` `flex` `bison` `libncurses-dev`
- From the desired directory, `git clone https://github.com/elunna/HackEM`
- Navigate to the `HackEM/sys/unix` folder, then `./setup.sh hints/linux` or
  `./setup.sh hints/linux-debug` depending on what you intend to do

  - Using the standard `linux` hints file assumes running as a normal user, and
  game folders and files will reside in `/home/$USER` based on the account used.
  Invoking `sudo` should not be necessary

  - Using the `linux-debug` hints file assumes installing as root, and includes
  extra CFLAGS for debugging in a development scenario. If you prefer using clang
  as your compiler and have it installed, see `clang-linux-debug` as an alternative
  hints file to use

  - With either hints file, edit the install paths to your liking
- Navigate back to the root HackEM folder, and `make all && make install`
- Execute the `HackEM` binary
- In the home directory of the account used to install HackEM, create your
  rc config file - `touch .hackemrc` and then edit as necessary

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.