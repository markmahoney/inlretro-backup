# The INLretro Cartridge Backup Helper

A Python-based CLI for creating emulatable backups from your own cartridges using the [INLretro dumper-programmer](http://www.infiniteneslives.com/inlretro.php). The utility does the following:

1. searches multiple cartridge databases to auto-configure cartridge backups
2. passes those backup params to the INLretro to dump the game's contents
3. for NES/Famicom games, adds an INES header to the dumped contents so that the game is emulatable

## Requirements

You'll need to have `virtualenv` installed. On Unix-like systems with Python installed you should be able to do the following:

```sh
easy_install virtualenv
```

You will also need to be able to compile the host source code for the INLretro. That entire codebase is included as a subtree in this repo, which may or may not have been a dumb choice on my part, but regardless, see the INLretro's [README](vendor/inlretro/README) and [host Makefile unix target](vendor/inlretro/host/Makefile) for details.

## Setup

```sh
make && source venv/bin/activate
```

## Running the Thing

```sh
./backup
```

You can run `deactivate` when you're done using the backup utility to disable virtualenv.

## Cart Support

- NES/Famicom
- Genesis/Mega Drive
- Nintendo 64

NES/Famicom cartridge support is tethered to the mapper sets supported by the INLretro, though that list is pretty substantial at this point. However, you won't have much luck backing up weirdo games yet.

As of January 27, 2019, you may also need to manually update the INLretro's firmware to a nightly verison to support cartidge types beyond NES/Famicom. If you have the 2.3 firmware installed, you can uncomment the line `fwupdate.update_firmware("../firmware/build_stm6/inlretro_stm.bin", 0x6DC, false) --nightly build` in the [host inlretro.lua file](vendor/inlretro/host/scripts/inlretro.lua), then run `inlretro` from the `vendor/inlretro/host/scripts` directory. I believe future updates to the host software will begin auto-updating firmware versions, at which point this suggestion will be obsolete.

If you _don't_ have the 2.3 firmware installed, you'll have to follow directions in the [INLretro README](vendor/inlretro/README) to update to 2.3 first.

## Issues

- There are probably a lot of games that cannot be backed up yet. NES/Famicom in particular requires the generation of a 16-byte INES header. I wrote my own INES/NES 2.0 header generation code, and I'm still learning the ins and outs of that crazy schema.
- The menu system is bad and I feel bad about it. I'll continue to iterate on solutions.
- I don't really know Python but wanted to learn, so this code might be terrible.

## TODOs

- Windows support?
- Improve XML parser for no-intro db systems (names/regions are sometimes inconsistent)
- Support the remaining console cartridge adapters
- Perform CRC checks on the backed up ROMs
- Improve INES header generation
- Make menus better
  - Implement menu system in `curses` maybe?
- Commandline params for bypassing search, naming output files, etc.

## Credit Where Credit Is Due

- [The INLretro Dumper-Programmer](http://www.infiniteneslives.com/inlretro.php) because this wouldn't do anything without it!
- [NES Cart DB](http://bootgod.dyndns.org:7777/) for NES/Famicom data
- [No-Intro](http://no-intro.org/) for all other consoles
