# Table of Contents
1. [Basic Installation](#BASIC-INSTALLATION)
2. [NES](#NES)

# Basic Installation

Follow the steps in the [README](../README.md) to update the frimware if needed and install the device drivers. You will only need the drivers if you're on Windows.

If you just recieved your device, you probably won't need to update the firmware, but please check the readme if something changes.

# NES

1. First, go ahead and connect your INL-retro to your PC
1. Next you're going to need is NES game. Go ahead and plug that into the correct slot of your INL-retro board. Word of advice: when removing the game, use your index fingers to pull the game up and push on the board with your thumbs.
1. After that you'll need to grab some parameters from another git repository [http://tuxnes.sourceforge.net/nesmapper.txt](Cartridge Mappings). These are the cartridge mappings will help you rip your game. Look for it in the list. What you need is the PRG size, CHR size and the Mapper. Take note of those for your game. Some of the Mappers listed may not yet be supported, so be aware of that if you get any errors.
1. Next, clone/download this repository
1. Navigate to the INL-retro-progdump/host folder in a command prompt or terminal. This might help you [How to Open Command Prompt at Current Location](http://www.ilovefreesoftware.com/25/windows-10/open-command-prompt-folder-windows-10.html)
1. The command you're going to run is
>```inlretro -s scripts/inlretro2.lua -c NES -x 64 -y 16 -d C:/User/YOU/Documents/GAME.nes```
1. Notice the -x and -y flags. The numbers that follow them correspond to PRG size and CHR size in the previous step. The -c flag is the mapper. Replace those with the parameters for your game. The flag -d is the destination, replace that with whatever folder path you feel like.
1. Finally run it! You should get a success message and you should be able to play your new rom file! If you run into any errors, remember not every mapper is supported. It is possible the Mapping listing is incorrect. Play with the flags if you want and see if you can get to work.