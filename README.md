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

Gba Visualizer Tool

```
make to_ppm_bin
./build/to_ppm_bin "tools/visual/data/snapshot_0/memory.bin" "output.ppm"
```

Gba Logging Tool. When the code aborts, logs of the last 1024\*1024 instructions are dumped. In order to read the logs, run

```
make log_reader_bin
./build/log_reader_bin log -1 5
./build/log_reader_bin mem -1 5
```
