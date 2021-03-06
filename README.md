# HackEM

## Design Goals

**The first goal** of this project is to become the major successor of Slash'EM and port over most of Slash'EM to a modern platform. I chose EvilHack as a base because it is based on 3.6, it's inheritly more difficult, and it contains many Quality Of Life features already baked in. See here:

	- Nice HP Bar
	- Enhance menu shows max and % progress on skills.
	- Extended commands: #terrain, #annotate, #overview, #tip, #give, #adjust splitting and merging
	- Peaceful displacing, paranoid swim, Smart shop autopickup
	- Full bag altar BUC identification
	- New conducts: Elbereth, alignment, touched art, petless
	- New soko levels, new room shapes and themed rooms.
	- object materials, quick wand wresting
	
Evil also has some challenging aspects that are well worth keeping:

	- New roles: Infidel, convict
	- New Races: giant, tortle, centaur, and illithid
	- Elbereth: Can't use unless you found/discovered it.
	- Partial intrinsics; Partial eflection
	- No randomly generated magic markers
	- Strong monsters push through weaker monsters to get to you.
	- Zombies ressurect, mummies wither, spellcasters are meaner.
	- Monster steeds

## Other design goals:

* **Goal 2:** Remove/edit Evil content
* **Goal 3:** Integrate slashem updates and bugfixes from other Slash'EM forks
* **Goal 4:** Import content from other forks.
* **Goal 5:** Homemade Ideas/Improvements for Slashem
* **Goal 6:** HackEM specific content
* **Goal 7:** Rethink dungeon texture
* **Goal 8:** Rethink Gehennom

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
  extra CFLAGS for debugging in a development scenario

  - With either hints file, edit the install paths to your liking
- Navigate back to the root HackEM folder, and `make all && make install`
- Execute the `HackEM` binary
- In the home directory of the account used to install HackEM, create your
  rc config file - `touch .hackemrc` and then edit as necessary

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

