# Hack'EM

## HackEM Retirement Notice


HackeM is no longer being actively maintained or developed. Due to its numerous bugs and unfinished features, I’ve decided to retire the game and shift focus to new projects. While HackEM was a fun experiment, it became clear that it no longer aligns with my goals or resources.

### Introducing NerfHack

In the spirit of HackEM, I’m excited to announce the launch of NerfHack. This new project takes many of the core features and ideas from HackeM but approaches them with a fresh set of design goals. NerfHack aims to improve upon the original game’s foundation by addressing its issues, optimizing gameplay, and introducing new mechanics while keeping the essence of what made HackeM fun.

If you enjoyed HackeM, I encourage you to check out NerfHack and follow its development here: https://github.com/elunna/NerfHack

----

Welcome, traveler, to Hack'EM!  

## Design Goals

**The first goal** of this project is to become the major successor of SLASH'EM and port it to a modern platform. I chose EvilHack as a base because it's based on 3.6, it's inherently more difficult and contains many Quality-Of-Life features already baked in. See here:

EvilHack also has some challenging aspects that complement SLASH'EM as well as new roles, races, items, monsters, and mechanics.

Bits and pieces from other variants has been added as well: SpliceHack, UnNetHack, SlashTHEM, xNetHack, FIQHack, SporkHack, and slashem-up and SLASHEM9.

## How to play

Hack'EM is now being hosted on the HardFought server!  Visit https://www.hardfought.org/ for details.

To install locally, follow the instructions below

If you on Windows, download the latest binary here: https://github.com/elunna/hackem/releases 

* Hack'EM Wiki: https://nethackwiki.com/wiki/Hack%27EM
* Hack'EM Frequently Asked Questions: https://nethackwiki.com/wiki/Hack%27EM_Frequently_Asked_Questions
* Click here to join #hackem on IRC: https://web.libera.chat/#hackem

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
