# The INL Retro Cartridge Backup Helper
=======================================

A Python-based CLI utility for creating emulatable backups from your own NES/Famicom cartridges using the [INL Retro](http://www.infiniteneslives.com/inlretro.php). The utility does the following:

1. allows you to search the [NES Cart DB[(http://bootgod.dyndns.org:7777/) for your game (via an XML dump of the site)
2. passes cartridge settings to the INL Retro to dump the game's contents
3. adds an INES header to the dumped contents so that the game is emulatable

## Requirements
===============

You'll need to have `virtualenv` installed. On Unix-like systems with Python installed you should be able to do the following:

```sh
easy_install virtualenv
```

You will also need to be able to compile the host source code for the INL Retro. That entire codebase is included as a subtree in this repo, which may or may not have been a dumb choice on my part, but regardless see the INL Retro's [README](vendor/inlretro/README) and [host Makefile unix target](vendor/inlretro/host/Makefile) for details.

## Setup
========

```sh
make && source venv/bin/activate
```

You can run `deactivate` when you're done using the backup utility to disable virtualenv.

## Issues
=========

I don't really know Python but wanted to learn, so this code might be terrible.

I wrote the INES header generation code myself, because I wanted to, because I'm an idiot. As a result, ~35% of the resultant game backups I've created don't seem to work in an emulator. A good example is Super Mario Bros. 3: when emulated from my own backup, it displays the curtain that reveals the game title but totally locks up after that. Caveat: I've only tried emulating my backups in [OpenEmu](https://openemu.org/). 

Overall, I've had the most success dumping earlier NES games, and my testing methodology has been haphazard and extremely non-rigorous at best.

Cartridge support is tethered to the mapper sets supported by the INL Retro, though that list is pretty substantial at this point. However, you won't have much luck backing up weirdo games yet.

Eventually I'd like to support backing up cartridges for other systems.

These are early days, and also I have no idea what I'm doing!

## TODOs
========

- Support other systems besides NES/Famicom
- Iterate on INES header generation
- Make menus better
-- Collapse game region selection into a sub-menu
-- Implement menu system in `curses` maybe?
- Windows support?
- Commandline params for bypassing search with search strings or game catalog numbers, naming output files, etc.
