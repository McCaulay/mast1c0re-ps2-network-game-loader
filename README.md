# mast1c0re - PS2 Network Game Loader

Load PS2 ISO games over the network using the [mast1c0re](https://github.com/McCaulay/mast1c0re) vulnerability.

Requires the [mast1c0re-ps2-network-elf-loader](https://github.com/McCaulay/mast1c0re-ps2-network-elf-loader) game save to loader the ELF file.

Currently the sent game is stored temporarily on disk, and is removed once the game is closed. Therefore, it requires you to transfer the ISO each time you load the game loader.

## Sending an ISO
You can send an ISO file by using one of the following methods:
* (GUI exe) Download "[mast1c0re-file-loader.exe](https://github.com/mast1c0re-ps2-network-elf-loader/releases/download/v0.1.1/mast1c0re-file-loader.exe)" in [releases](https://github.com/McCaulay/mast1c0re-ps2-network-elf-loader/tags)
* (Python GUI) python3 [mast1c0re-file-loader.py](https://github.com/mast1c0re-ps2-network-elf-loader/blob/master/scripts/mast1c0re-file-loader.py)
* (Command Line) python3 [mast1c0re-send-file.py](https://github.com/mast1c0re-ps2-network-elf-loader/blob/master/scripts/mast1c0re-send-file.py) --ip <ip> --file <path-to-iso>

## Supported Systems & Firmware Versions
* PS4
  * 5.05
  * 6.72
  * 9.00
  * 10.01
* PS5
  * 6.50

## Improvements
* Upload and load config file "config-emu-ps4.txt" (https://www.psdevwiki.com/ps4/Talk:PS2_Classics_Emulator_Compatibility_List)
* Emulator load lua file
* Load PS2 games from USB