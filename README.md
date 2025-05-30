### How to Run

Run by

```
mkdir build
make all
./build/emulator "games/Pokemon - Emerald Version (U).gba"
```

Dump binary by

```
arm-none-eabi-objdump -D -b binary -marm games/Pokemon\ -\ Emerald\ Version\ \(U\).gba
```
