### How to Run

Run by

```
mkdir build
make all
./build/emulator "games/Pokemon - Emerald Version (U).gba"
./build/emulator "games/gba_bios.bin"
```

Dump binary by

```
arm-none-eabi-objdump -D -b binary -marm games/Pokemon\ -\ Emerald\ Version\ \(U\).gba
arm-none-eabi-objdump -D -b binary -marm games/gba_bios.bin
arm-none-eabi-objdump -D -b binary -marm -Mforce-thumb games/gba_bios.bin
```

XCode Configurations

```
If not already set,
- Go to Build Settings -> Objective-C Bridging Header and set to GbaDisplay/Bridging-Header.h
- Go to Build Phases -> Link Binary With Libraries and set to build/cpu_runner.a
- Go to Build Settings -> Header Search Paths and set to include/
```
