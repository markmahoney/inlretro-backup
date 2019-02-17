
===================================
INSTALLING DEVICE DRIVERS:
===================================

Plug the device into a USB port.

Open a windows file explorer and navigate to:
  INL-retro-progdump\WindowsDriverPackage

In that folder there's a "InstallDriver.exe" application.
Double click it to run it.

Windows should ask if you want to allow it to make changes to your PC, say YES.

Click next in the wizard, it should install the drivers to your PC.

If it worked properly you'll get a "install successful" page, click FINISH.

In windows you should be able to see "INLretro-prog" listed in "devices and printers" from
the control panel.



===================================
RUNNING THE HOST APPLICATION:
===================================

On windows install device drivers by running InstallDriver.exe from WindowsDriverPackage folder.

An easy way to open the commandline in windows 10 is to open the host folder in explorer.
Then click on the address bar at the top, the address should highlight, simply type the
3 letters "cmd" in the address bar and hit enter.

From the host folder, run the main application from the commandline by typing:
inlretro.exe -s scripts\inlretro.lua

This will run the main application and after the device is detected and USB comms established,
control is passed to the main lua script.

See the USER notes at the top of scripts\inlretro.lua file for how to select different mappers,
cartridges, etc.  You'll need to modify inlretro.lua with your favorite text editor or even
something as simple as notepad included with windows.  If you don't have a favorite text editor,
I recommend something like notepad++ which color codes lua files/code for easier reading.
https://notepad-plus-plus.org/download

If the device errors out, or is not responding you may have to press the RESET button near the
USB connector on the INLretro programmer/dumper to reset the device, and try again.




===================================
UPDATING DEVICE FIRMWARE
===================================


----------------------------------------------------------
STM32 ARM based device versions "INLretro" V2.0 and later
----------------------------------------------------------
If you purchased your device in 2018 or later you have this version
This includes devices aquired with NESmaker kits.


0) DO I NEED TO FOLLOW THESE STEPS?

Devices sold Oct 2018 or earlier shipped with firmware v2.1 or v2.2
These builds did not include a 'switchless' USB firmware updater.
So you will need to update to v2.3 or later using STmicro DfuSeDemo.
Alternatively if you somehow bricked your device with any firmware,
you can use STmicro's DfuSeDemo to easily recover it via USB.

Once upgraded to firmware v2.3.0 or later, you shouldn't need to use
the DfuSeDemo to upgrade your device's firmware anymore.  The INLretro
host software is capable of updating the device's firmware on builds
v2.3.0 or later.

INLretro devices purchased in Nov 2018 were shipped with a mix of firmware versions.
INLretro devices purchased in Dec 2018 and later ship with firmware version
v2.3.0 or later.
Kickstarter NESmaker kits with the large fancy "NES" enclosure shipped with v2.1
All other Kickstarter NESmaker kits used the smaller v2.0N PCB and
shipped with v2.3.0

I'm working on updating the INLretro host software to report which 
firmware version your device is using to take out the guess work.
Once this is done the command prompt will report which build version
you have.  Updating from v2.3.0 to later versions will be pretty easy
as the host application will ask you if you'd like to let it update
the firmware version for you.  In that case you can ignore these DfuSeDemo
instructions unless you somehow brick your device.  Or maybe you make
your own firmware builds and use the Dfu file manager to create your 
own .dfu builds that may or may not be compatible with the switchless
firmware updater.

So at this point I'm assuming you need/want to use the DfuSeDemo
to update your device because it has firmware v2.2 or earlier, or you
bricked your device, etc.


1) GET THE SOFTWARE

On Windows:
Go to the st.com link below.  At the bottom there's a "GET SOFTWARE"
section, download the STSW-STM32080 item.  This is written for v3.0.6
https://www.st.com/en/development-tools/stsw-stm32080.html

Alternatively you can save the hassle of creating an STmicro account 
by downloading directly from the following link if you would like:
https://www.dropbox.com/s/gwcvd3dqwkbzsv9/en.stsw-stm32080.zip?dl=0

On Linux:
The dfusedemo is windows only, there is a linux alternative called
dfu-util this may work for mac too.  Read through the following 
instructions to understand the basic steps.  Then
See the "Linux dfu-util" instructions at the end.



2) INSTALL THE SOFTWARE

As far as I know this application is windows only.  You will probably
have to run it in a virtual machine if you're on linux/windows.  If
this is a big problem I can look into creating my own dfuse application
that runs on linux/mac.  This should be a one time thing, so I'm hoping
gaining access to a windows machine isn't too much to ask..

Extract the .zip downloaded in step 1.  Click next, select users, etc..
Click install and let it make the changes to your PC.
Once complete it should give you an option to launch it.


3) LAUNCH DFUSEDEMO

You can probably find it in the start menu searching "dfusedemo"
It didn't give me any choices on install path and put it here:
C:\Program Files (x86)\STMicroelectronics\Software\DfuSe v3.0.6\Bin
From that folder you can create shortcuts of dfusedemo to your desktop
If you think you may like to make your own firmware builds and want
to convert your hex/bin files to .dfu you can use the DFU file manager
in the same installation folder as dfusedemo.


4) GET THE INLRETRO INTO DFU MODE

At this point chances are there's nothing listed under 
"Available DFU Devices", it takes some steps to get the programmer 
in DFU mode so the dfusedemo application can find & communicate to it.

This step varies slightly depending on your exact device & when you
purchased it.  

-PCB VERSION 2.0 purchase prior to Dec 2018
   If your device shipped with firmware v2.1 or v2.2
   I soldered a "BL/RUN" switch to aid in this process.
   
   The switch is next to the USB socket, place it in "BL" nearest
   the USB socket.  Then plug the USB cable into your PC.  If it
   was already plugged in, just replug it, or hit the RESET button
   soldered right next to the BL/RUN switch.

-PCB VERSION 2.0 purchased Dec 2018 or later
   or the big NES enclosure from NESmaker kickstarter
   You may not have a BL/RUN switch.  Unplug the USB cable.
   Get a tweezers, paperclip, or something conductive like that.
   There are 5 holes in the PCB edge with "RUN" and "BL" on either side
   Ignore the bigger outer 2 holes.  Short the center hole to the small
   hole next to it closest to the BL label (and USB connector)
   Plug the USB cable in while those two pads are shorted together.
   It might help to have a freind plug in the USB cable while you short the pins.
   Once it's plugged in you don't have to keep the pins shorted anymore.

-PCB VERSION 2.0N smaller device with NES Connector alone
   These are the majority of the basic NESmaker kickstarter kits.
   You really shouldn't be going through this process unless you
   bricked your device or I personally instructed you to do so.
   Reason being is had an error with the PCB BL/RUN switch.
   I specifically released the switchless firmware updater in time
   to avoid you from going through this trouble.  All V2.0N PCBs
   shipped with firmware v2.3.0 or later and thus don't need dfusedemo
   to update.
   If you're still here, with PCB v2.0N dated "AUG2018" get a tweezers
   or something like a metal paperclip.
   On the top corner of the PCB there's a place for a "BL/RUN" switch.
   I didn't solder it on though, and the silk is backwards.
   Ignore the bigger outer 2 holes.  Short the center hole to the small
   hole next to it closest to the RUN label (and USB connector).
   Plug in the USB cable with those pads shorted together.
   It might help to have a freind plug in the USB cable while you short the pins.
   Once it's plugged in you don't have to keep the pins shorted anymore.

-Other PCB versions:  there are none at this time... (12/1/2018)
   If you've got a PCB that says v1.1, v1.2, v1.3, or v1.4 on the bottom
   You're in the wrong place.  None of these steps will work for you.
   Go down a little to the instructions:
	"AVR based devices versions "KAZZO" V1.4 and eariler"


5) PERFORM THE UPDATE

The device should be in DFU mode now from the step above.

With the device is in DFU mode the dfusedemo application should
list it under available devices as "STM Device in DFU Mode"
You can also see it listed in windows "devices & printers" as
"STM32 BOOTLOADER".  If you see "INL Retro-Prog" the device isn't
in DFU mode, try this step again..

-Choose Button
   Choose the right Choose button!  There are 2 "Choose" buttons.
   Naturally, you'll probably select the wrong one...
   CLICK THE "Choose" button in the BOTTOM CENTER in the 
   "Upgrade or Verify Action" section.
   
   The Choose button will popup an Open window, navigate to
   INL-retro-progdump\firmware\DFU_release folder

-Load .dfu file
   Select the proper .dfu file:
   All versions publicly released are here, sort by date and select
   the latest one that matches your PCB version.
   
   If your PCB is big and square (v2.0) select the one that
   starts with INLretro6  (currently INLretro6_PCBv2_0_FWv2_03_0.dfu)
   
   If your PCB is smaller and only has place for NES connector
   select the one that starts with "INL_NES"
   (currently INL_NES_PCBv2_0N_FWv2_03_0.dfu)
   
   If you're following these instructions later down the road
   I've probably got later versions you should be using.
   Just make sure the PCB version of the name matches what's printed
   in silk screen on the bottom of your PCB.  In the end it's easy
   to change if you select the wrong one.  Or the firmware updater
   will fix it for you once you've got it unbricked.

-Optional settings
   You can check the "Verify after download" box just above the 
   Choose button, it will verify the flash after programming.
   You can also check the "Optimize Upgrade duration" but it really
   doesn't matter, it only takes a couple seconds anyway..

-Upgrade 
   Click the "Upgrade" button just to the right of that "Choose"
   button you just used.

   NOTE!  You WILL GET A WARNING POPUP, it's expected!
   Click "Yes" to the warning: "Your device was plugged in DFU mode.
   "So it is impossible to make sure this file is correct for this device."
   "Continue however?"

   CLICK YES.

-Watch the status bar
   It should click across a few times and say 
   "Target 00: Verify successful!"  or just
   "Target 00: Upgrade successful!" depending on if you chose
   to verify or not in the optional step.

   If you have problems, make sure the device is in DFU mode 
   and appears in windows "devices & printers" as "STM32 BOOTLOADER"
   before trying to upgrade.  Please contact me if you're 
   having problems.

-Exit DFU mode
   If you have a BL/RUN switch, place it back in RUN
   Unplug the USB cable and plug it back in.
   You should see "INL Retro-Prog" in devices & printers
   and the device should be running the latest firmware now!




LINUX DFU-UTIL STEPS:
  
  sudo apt-get dfu-util
  
  Put device into dfu mode as explained in step 4 above
  
  verify you can see the device
  
  dfu-util -l
  
  get the "alt" with name of @Internal Flash use that number in the -a for example:
  
  Found DFU: [0483:df11] ver=2200, devnum=18, cfg=1, intf=0, path="1-1", alt=1, name="@Option Bytes  /0x1FFFF800/01*016 e", serial="FFFFFFFEFFFF"
  Found DFU: [0483:df11] ver=2200, devnum=18, cfg=1, intf=0, path="1-1", alt=0, name="@Internal Flash  /0x08000000/064*0002Kg", serial="FFFFFFFEFFFF"
  
  (navigate to the firmware/DFU_release directory)
  Upgrade Command example:
  sudo dfu-util -d 0483:* -a 0 -D INLretro6_PCBv2_0_FWv2_03_01.dfu



----------------------------------------------------------
AVR based devices versions "KAZZO" V1.4 and eariler
----------------------------------------------------------
If you purchased your device in 2017 or earlier you have this version
This should also apply if you made your own kazzo based on the open
source design.  Although I can't assure this software works with
any of those versions, especially versions with something besides
the atmega164.

These devices sold by Infinite NES Lives included a bootloader to 
update the flash via USB.

There are effectively 3 main versions of firmware that run on your device:

A) original kazzo firmware created by naruko that is needed to use
   anago/unagi dumping software.  See old instructions to get that running
   if you want it.  You're in the wrong place.

B) INL released firmware for use with "INL-retro prog v1.0beta" this
   was effectively my "version 1" release of firmware & software.
   It wasn't very good, but got the job done for far too many years.
   I don't recommend using that old firmware & software anymore
   You'll be able to use the latest software and firmware 
   provided here instead.

C) INLretro v2 firmare & software, the reason you're here, and
   the software & firmware included with this download/project.
   Upgrading the firmware is similar to previously, but here's
   a quick run through to get you going with the latest firmware
   that's required in order to use this INLretro software.


You're here because you want to install option C from above
Smart choice...


1) GET THE SOFTWARE

The bootloader software is already included with this project/download
which you're reading this file from.  It also doesn't require installation.
These instructions are for windows, but you can build the bootloader to
work on linux/mac as well.


2) PUT DEVICE IN BOOTLOADER MODE

Place the BL/RUN switch in the BL position.  Plug in the USB cable, or
Hit the RESET button on the PCB.

The device should now be in bootloader mode.  Look in windows "devices
and printers" and you should see "HIDBoot".  If not, try this step again.
The next steps won't work if you still see "INLRetro-Prog" "kazzo" or something
like that..


3) UPDATE THE FIRMWARE

In a windows file explorer navigate to:

   INL-retro-progdump\avr_bootloader

You should see a file in that folder named:

   "click to load v1 INLkazzo with INLretro v2.bat"

Double click on that file to run it.

If you didn't get the device in HIDBoot properly you'll probably see a message like:
   "Error opening HIDBoot device: The specified device was not found"
   Go back and retry step 2, this step only works if you got step 2 working properly

If you were successful you'll see a message like this:

	Page size   = 128 (0x80)
	Device size = 16384 (0x4000); 14336 bytes remaining
	Uploading 13312 (0x3400) bytes starting at 0 (0x0)
	0x03380 ... 0x03400
	
	C:\Users\...\INL-retro-progdump\avr_bootloader>pause
	Press any key to continue . . .

Hit the any key, or just close that popup window, you've updated the firmware!


4) EXIT BOOTLOADER MODE

Place the BL/RUN switch in RUN
You should see "INL Retro-Prog" in devices & printers
and the device should be running the latest firmware ready to use the latest software!



===================================
SOFTWARE & FIRMWARE BUILDING
===================================
Some build instructions follow, but they shouldn't be needed if you're running the
released firmware on Windows.

This section needs some updating....  Don't be afraid to contact me if you'd like to build
your own firmware.  Or are having issues building the host application on linux/mac


===================
Linux
===================

HOST APPLICATION:
install libusb:
sudo apt-get install libusb-1.0-0-dev

make:
gcc -I include inlretro.c -o inlretro -lusb-1.0
or just run make unix from host folder.
As noted in windows instructions below, lua must be built first
-cd host/source/lua, make o a, then make from host/source

run:
./inlretro


AVR FIRMWARE:
install avr-gcc and avr-libc:
sudo apt-get install gcc-avr
sudo apt-get install avr-libc


AVR BOOTLOADER:
bootloadHID-master:
https://github.com/ajd4096/bootloadHID
fork of original obdev bootloader has option to remove BL switch with timeout.

bootloadHID.2012-12-08:
https://www.obdev.at/products/vusb/bootloadhid.html
the original believe it has more upto date V-USB drivers.

Both have identical commandline folders so they're identical on the host side.
need to have libusb-dev installed can check by typing "libusb-config" in terminal
will present usage options if installed on your system
if not installed should report so with suggestion for apt-get:
sudo apt-get install libusb-dev
then just run 'make' should build successfully

With the bootloader commandline app built, the firmware can be loaded
onto the INL retro-programmer via USB:
-place BL/RUN switch in BL
-hit RESET button or plug into USB for first time
-run 'make program_unix' from firmware dir.

If bootloader commandline app was successfully built and you have permission
to access HIDbootloader should have successful output similar to this:
$ make program_unix
../bootloader/commandline/bootloadHID -r main.hex
Warning: could not set configuration: could not set config 1: Device or resource busy
Page size   = 128 (0x80)
Device size = 16384 (0x4000); 14336 bytes remaining
Uploading 1920 (0x780) bytes starting at 0 (0x0)
0x00700 ... 0x00780

-take BL switch back to RUN
-enjoy new firmware

STM32 FIRMWARE:
Need arm-none-eabi-gcc


===================
Windows
===================
HOST APPLICATION:
Install minGW:
	download: http://www.mingw.org/wiki/Getting_Started
	launch: Installation manager default settings
	select: mingw32-base 
		(primary need is gcc)
	select: msys-base 
		(primary need is make, basic unix commands are nice to have)
	optional: msys-openssh 
		(helpful if using gitlab to pull updates)
		lua for host app dev
	minGW utilities can be easily added or removed at any time with minGW installation manager.
	Add C:\MinGW\bin & C:\MinGW\msys\1.0\bin to your PC's enviroment PATH variable
		-control panel search: "edit system environment variables
		-System properties window, Advanced tab, click Environment Variables...
		-System Variables pane: Select and edit "PATH"
		-Add new entries below assuming you used default location for minGW
			C:\MinGW\bin
			C:\MinGW\msys\1.0\bin
		-I had troubles once with cp (copy) commands in Makefile
causing a crash, even though the commands works outside of make.  Bumping the
mingw path variables to the top of all my path variables corrected this issue.
So that might help if you have similar issues...
	
Now host app can be built from windows command prompt command make when in host directory

Currently setup to compile lua separate from host app.  Need to get better at writing makefiles..
But whatever it works and saves ~12sec of compile time with current setup.
-go to host/source/lua
-make o a
-go back to host
-make
This way lua is compiled separately using it's provided make file.
make clean still deletes lua object files, so this process must be reperformed if cleaned.

AVR FIRMWARE:
Download and Install WinAVR
	optional: install programmer's notepad has handy feature to make clean, all, program in tools menu
		this is nifty if you are scared of the command prompt for some strange reason...
	installation process should modify PATH environment variables for you.
	incase they don't add them just like MinGW above
		C:\WinAVR-20100110\bin
		C:\WinAVR-20100110\utils\bin

Now firmware can be built from windows command prompt with command "make" when in firmware directory

There is a bootloader installed on all "kazzo" INL retro programmer-dumper's which allows new firmware
builds to be easily flashed on to the device without a avr programmer.
Place BL/RUN switch in BL, then hit reset button in INL retro-prog
from firmware folder run command "make program" this will flash target build onto device using bootloadHID.exe
Take BL/RUN switch back to RUN and device will reset into INL retro-prog you just built.

AVR BOOTLOADER:
If you wish to build bootloader for kazzo (shouldn't be necessary for most ppl) follow the following
Requires you to have an avr programmer to reflash atmega164a mcu
Helpful to download more recent version of avrdude than included with WinAVR.
Download latest version with mingw32.zip from following link:
http://download.savannah.gnu.org/releases/avrdude/
nzip and copy paste both avrdude.exe and avrdude.conf to C:\WinAVR-20100110\bin directory
Assuming the recent build still doesn't support atmega164a..
You'll also have to add a definition for atmega164a in the avrdude.conf file
Copy paste the atmega324P section and rename it atmega164A
Then change the following lines to match:
    id               = "m164a";
    desc             = "ATmega164A";
    signature        = 0x1e 0x94 0x0f;

Now the bootloader can be built and flashed from the bootloadHID/firmware directory with make
If trying to flash mcu from make file you'll have to modify AVRDUDE line to match your avr programmer
Once completed you can make the bootloader and flash it with command "make flash"


STM32 FIRMWARE:

Need arm-none-eabi-gcc:
	https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads

Download .zip file and place in folder like C:\ARM and create environment variables to point to bin folder

