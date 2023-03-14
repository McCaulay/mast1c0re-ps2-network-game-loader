# mast1c0re - PS2 Network Game Loader Mod

Load PS2 ISO games over the network using the [mast1c0re](https://github.com/McCaulay/mast1c0re) vulnerability.

Requires a game save with the [mast1c0re-ps2-network-elf-loader](https://github.com/McCaulay/mast1c0re-ps2-network-elf-loader) to load the ELF file.

Currently the sent game is stored temporarily on disk, and is removed once the game is closed.</br>
Therefore, it requires you to transfer the ISO each time you load the game loader.</br>

## Modifications in this version
* This modified ELF will transfer the ISO at least 15x faster as the official version due to it's increased chunk size that can be received
* Lime colored light bar to differentiate from the official Network Game Loader

## Sending an ISO with THIS modified ELF
You can send an ISO file by using the following method:
* (GUI) Download [OkageLibrary](https://github.com/SvenGDK/OkageLibrary/releases) - Supports both official and this modified ELF

## Supported Systems & Firmware Versions
* PS4
  * 5.05
  * 6.72
  * 9.00
  * 10.01
  * 10.50
* PS5
  * 6.50

## Improvements
* Upload and load config file "config-emu-ps4.txt" (https://www.psdevwiki.com/ps4/Talk:PS2_Classics_Emulator_Compatibility_List)
* Emulator load lua file

## Thanks
Big thanks to https://github.com/McCaulay and all his work!
