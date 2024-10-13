This is the portion of the code that executes in real mode. The goals of this code are as follows:
- Land, cleanup segment registers and stack
- Setup whats necessary to get to pmode (setup GDT, enable A20, etc.)
- Save addresses of image bases
- Jump to protected mode
- In protected mode an indirect jump is encoded which goes to the PE entry point of MINTLDR.sys

HOWEVER:
Extra steps are required if using FAT12/16 boot sectors, because those have to be small so they don't load the full file.
Currently this isn't supported right now.