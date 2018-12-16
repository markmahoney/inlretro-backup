
You'll probably have issues where the host app can't detect the device with linux systems.

If you don't have permission to write to the device you'll get an error message such as:
libusbx: error [_get_usbfs_fd] libusbx couldn't open USB device /dev/bus/usb/002/021: Permission denied
libusbx: error [_get_usbfs_fd] libusbx requires write access to USB device nodes.
[ERROR] (source/inlprog.c:99: errno: Permission denied) Unable to open USB device: Access denied (insufficient permissions)

This can be temporarily corrected by running as sudo
But you'll want to update permissions so don't need to run as sudo
need to create udev rule to give user permissions to device when inserted.

What follows is a walkthrough of the process I used with specific commands
and outputs I recieved.  Your outputs will most likely differ getting
assigned different bus and device numbers, but the process is hopefully similar.

To start, plug into USB port and run lsusb command to verify its detected by OS
should get similar output as this:
paul@eeepc:~$ lsusb
Bus 001 Device 003: ID 05e3:0505 Genesys Logic, Inc. 
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 005 Device 002: ID 0b05:b700 ASUSTek Computer, Inc. Broadcom Bluetooth 2.1
Bus 005 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 004 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 003 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 002 Device 021: ID 16c0:05dc Van Ooijen Technische Informatica shared ID for use with libusb
Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub

The "Van Ooijen..." device is the INL-retroprog as noted with 16c0:05dc Vendor:Prod ID
you can check current permissions by checking /dev/ location
use the Bus number and Device number to check permissions on the INL retro-prog 
the initial error should point out the /dev/ location of the device
In my example:
paul@eeepc:~$ ls -ltr /dev/bus/usb/002/021
crw-rw-r-- 1 root root 189, 148 Nov 18 01:36 /dev/bus/usb/002/021

you can see it's owned by root and users don't have write access.

one easy fix is to use chmod, but this isn't persistent and will probably need to be reapplied
everytime the device is reset/plugged in.
for my example:
paul@eeepc:~$ sudo chown paul /dev/bus/usb/002/021

While that may work, let's permanently fix the issue so you always have access.

First step is to make an attempt to determine what rule is being applied to the device.
run the command udevadm test <device>
in my example:
paul@eeepc:~$ udevadm test /dev/bus/usb/002/021

this prints out a long list of all the udev rules that get applied to the device.
It's not necessarily that easy to determine what rule is getting applied.
Look through the list starting at the bottom (biggest numbered rule)
See if there's any default usb rule or a usb named rule that might apply.
Here's an exerpt of what I had:
...
read rules file: /lib/udev/rules.d/40-libsane.rules
read rules file: /lib/udev/rules.d/40-usb_modeswitch.rules
read rules file: /lib/udev/rules.d/40-virtualbox-guest-dkms.rules
read rules file: /lib/udev/rules.d/42-usb-hid-pm.rules
read rules file: /lib/udev/rules.d/50-firmware.rules
read rules file: /lib/udev/rules.d/50-udev-default.rules
read rules file: /lib/udev/rules.d/55-dm.rules
read rules file: /lib/udev/rules.d/56-lvm.rules
read rules file: /lib/udev/rules.d/60-cdrom_id.rules
read rules file: /lib/udev/rules.d/60-keyboard.rules
...

My guess was that the 50-udev-default.rules was getting applied since none of
the larger rule numbers seemed to apply to my usb device.

Another thing to check is udev rules in /etc/udev/rules.d
paul@eeepc:~$ ls /etc/udev/rules.d
70-persistent-net.rules  README

So the only non-lib rule I had in /dev/udev/rules.d was rule 70 for net devices.
So my best guess is still the default udev rule #50.
So Let's create a udev rule that's one number greater so it overrides the default rule.

paul@eeepc:/etc/udev/rules.d$ sudo vim 51-libusb-permission.rules
with the following contents:
# This file was created to give user permissions to V-USB devices

# USB devices (usbfs replacement)

SUBSYSTEM=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05dc", MODE="0666"
SUBSYSTEM=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05df", MODE="0666"

That should do the trick.  
You see the first entry had ProductID 05dc that's the INL retro-prog
The second entry with ProductID 05df is when it's in Bootloader mode

So that udev rule should give you write permission to the device in Bootloader and Run modes.

I've included a copy of this udev rule in the host/udev directory which you can copy.

Now we need to reapply all the rules so it can take effect:
sudo udevadm control --reload-rules

This applies the rules, now unplug the device or hit reset so the rule gets utilized.

run lsusb again, the device number probably incremented again:
$ lsusb
Bus 001 Device 003: ID 05e3:0505 Genesys Logic, Inc. 
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 005 Device 002: ID 0b05:b700 ASUSTek Computer, Inc. Broadcom Bluetooth 2.1
Bus 005 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 004 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 003 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 002 Device 022: ID 16c0:05dc Van Ooijen Technische Informatica shared ID for use with libusb
Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub

You can see it's now device 022 for my example.

So let's check the permissions on the device
ls -ltr /dev/bus/usb/002/022
crw-rw-rw- 1 root root 189, 149 Nov 18 02:05 /dev/bus/usb/002/022

That final 'w' means my default user now has write access to the device which is the goal.
If you don't get that final 'w' permission then something went wrong.

Perhaps you've got a larger rule number overridding the rule you created.
You can try increasing the rule number by some amount, reapplying the rules, and retrying.
I verified that rule 50 was indeed the rule I needed to override by moving my rule

paul@eeepc:/etc/udev/rules.d$ sudo mv 51-libusb-permission.rules 49-libusb-permission.rules

I then used udevadm to reapply the rules, unplugged and reinserted.
sudo udevadm control --reload-rules

After doing so I lost permission of the device proving it was rule 50 I was 'battling against'.
So I moved it back to rule 51 tested out the INL retro-programmer app and celebated.
You could apply this same methodology bumping all the way to rule 99 I believe.
If you get that high, you've probably got some other problem.
See https://wiki.archlinux.org/index.php/udev for help or good ol' google...
