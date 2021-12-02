Jaga's Goomba Color fork

A fork of Goomba Color with the goal of fixing bugs and incompatibilities in the original.  Based on the 2019-05-04 source.

The games I'd like to get running are:
- Donkey Kong Land: New Colors Mode, https://www.romhacking.net/hacks/6076/.  Can't reach the file select screen.
- Kirby's Dream Land DX Service Repair, https://www.romhacking.net/hacks/6224/.  Palette issues in Level 2 and onwards (should be fixed as of v0.2)
- Konami GB Collections 2 and 4.  Don't boot (should be fixed as of v0.1)
- Pokemon Crystal.  Heavy graphical corruption after displaying text (should be fixed as of v0.3)

To build:
- Install the latest DevkitPro GBA tools
- Navigate to this directory
- make
- Rename font.lz77.o to font.o and fontpal.bin.o to fontpal.o
- make

To test, I build a ROM with the resulting goomba.gba and the game I'm testing using goombafront.exe, then run it in mGBA.  You can find goombafront.exe as part of the Goomba Color releases.

Thanks to:
- Dwedit for the Goomba Color emulator, which you can find at https://www.dwedit.org/gba/goombacolor.php.  If you'd like to incorporate my changes into Goomba Color, you're more than welcome to.
- FluBBa for the Goomba emulator before that: http://goomba.webpersona.com/
- Minucce for help with ASM and pointing me in the right direction.