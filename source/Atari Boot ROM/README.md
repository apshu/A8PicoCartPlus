# How to compile the CART code

* Use MADs assembler [GitHub release page](https://github.com/tebe6502/Mad-Assembler/releases)
* Install Altirra 4.21 or newer [Download Page](https://www.virtualdub.org/altirra.html)
* Use VSCode with [MADS plugin](https://marketplace.visualstudio.com/items?itemName=mirao.mads) and [Atasm Altirra Bridge](https://marketplace.visualstudio.com/items?itemName=cerebus.atasm-altirra-bridge)
* Install [Python](https://www.python.org/) 3.12 or newer

The current setup is available for Windows only. Linux porting should be fairly easy.

Set the Altirra executeable path [-> VSCODE setting](vscode://settings/atasm-altirra-bridge.emulator.altirra.path) to bridge.bat

Add the Altirra bridge specific settings to atasm-build.json
```json
	"_7": "Altirra command line parameters for developing a cartridge",
	"bridgeParams": {
		"altirraExecutable": "D:\\Games\\Altirra\\Altirra64.exe",
		"bptList": ["A001","read_joystick.done"],
		"altirraDebugArgumentTemplate": "/cartmapper 1 /cart {' '.join(args[1:])} /debugcmd:\\\".sourcemode on\\\" {' '.join(map('/debugcmd:\"bp {}\"'.format, getattr(config.bridgeParams, 'bptList', [])))}",
		"altirraArgumentTemplate": "/cartmapper 1 /cart {' '.join(args[1:])}"
	}
```
> [!IMPORTANT]
> Adjust the "_altirraExecutable_" to match the full path and executeable of your selected Altirra

> [!NOTE]
> The example above is for developing cartridges

> [!TIP]
> "_bptList_" section may contain Address or symbol name to set breakpoints on load.

# Convert the compiled code to _.h_

When the Cart code is finished, run the _convert.bat_, review the generated _rom.h_ and copy the _rom.h_ to the Pico source folder. 

Recompile the Pico Code and the new CART is incorporated as default.