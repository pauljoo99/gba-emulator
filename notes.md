## Investigation 1

Stuck on

```
     fa4:	e2577001 	subs	r7, r7, #1
     fa8:	ba000015 	blt	0x1004
     fac:	e3a0b0ff 	mov	fp, #255	@ 0xff
     fb0:	e1a05a5b 	asr	r5, fp, sl
     fb4:	e4d09001 	ldrb	r9, [r0], #1
     fb8:	e3a04000 	mov	r4, #0
     fbc:	e3540008 	cmp	r4, #8
     fc0:	aafffff7 	bge	0xfa4
     fc4:	e009b005 	and	fp, r9, r5
     fc8:	e1b0c43b 	lsrs	ip, fp, r4
     fcc:	03580000 	cmpeq	r8, #0
     fd0:	0a000001 	beq	0xfdc
     fd4:	e59db004 	ldr	fp, [sp, #4]
     fd8:	e08cc00b 	add	ip, ip, fp
     fdc:	e18ee31c 	orr	lr, lr, ip, lsl r3
     fe0:	e0833002 	add	r3, r3, r2
     fe4:	e3530020 	cmp	r3, #32
     fe8:	ba000002 	blt	0xff8
     fec:	e481e004 	str	lr, [r1], #4
     ff0:	e3a0e000 	mov	lr, #0
     ff4:	e3a03000 	mov	r3, #0
     ff8:	e1a05615 	lsl	r5, r5, r6
     ffc:	e0844006 	add	r4, r4, r6
    1000:	eaffffed 	b	0xfbc
```

This is storing into 0x6000040 which ends up in the gameboy logo tile space. There is some LZ77 decompression alg going on and that decoding is probably the issue.

## Investigation 2

Normal run of Pokemon Emerald is causing

```
[LOG] src/memory.h:107: Invalid memory address: 0xFFFFFDF8
```

This is caused by a branch to a weird address that doesn't have any instructions.

```
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5647, Dispatch num: 26170574, Instr Type Normal, Opcode AND, Raw Instr: 0x0000, Addr: 0x3002758
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5648, Dispatch num: 26170573, Instr Type Normal, Opcode AND, Raw Instr: 0x0000, Addr: 0x3002754
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5649, Dispatch num: 26170572, Instr Type Normal, Opcode AND, Raw Instr: 0x0000, Addr: 0x3002750
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5650, Dispatch num: 26170569, Instr Type Normal, Opcode LDR, Raw Instr: 0xe510f004, Addr: 0x0134
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5651, Dispatch num: 26170568, Instr Type Normal, Opcode ADD, Raw Instr: 0xe28fe000, Addr: 0x0130
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5652, Dispatch num: 26170567, Instr Type Normal, Opcode MOV, Raw Instr: 0xe3a00301, Addr: 0x012c
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5653, Dispatch num: 26170566, Instr Type Normal, Opcode STM, Raw Instr: 0xe92d500f, Addr: 0x0128
[1m[LOG] tools/logging/log_reader_bin.cpp:83:[0m Step 5654, Dispatch num: 26170563, Instr Type Normal, Opcode B, Raw Instr: 0xea000042, Addr: 0x0018
```

The following code is where 0x3002750 gets set.

```
[1m[LOG] tools/logging/log_reader_bin.cpp:119:[0m Store 49, Dispatch num: 26170501, Execute Addr: 0x80006f2, Raw Instr: 0x60c8, Addr: 0x30022cc, Value: 0x0000
[1m[LOG] tools/logging/log_reader_bin.cpp:119:[0m Store 50, Dispatch num: 26170494, Execute Addr: 0x80006a8, Raw Instr: 0x6004, Addr: 0x3007ffc, Value: 0x3002750
[1m[LOG] tools/logging/log_reader_bin.cpp:119:[0m Store 51, Dispatch num: 26170491, Execute Addr: 0x80006a2, Raw Instr: 0x6081, Addr: 0x40000dc, Value: 0x84000200
```

These are the instructions in the pokemon rom assembly that sets the branch.

```
     69a:	4811      	ldr	r0, [pc, #68]	@ (0x6e0)
     69c:	6005      	str	r5, [r0, #0]
     69e:	6044      	str	r4, [r0, #4]
     6a0:	4910      	ldr	r1, [pc, #64]	@ (0x6e4)
     6a2:	6081      	str	r1, [r0, #8]
     6a4:	6880      	ldr	r0, [r0, #8]
     6a6:	4810      	ldr	r0, [pc, #64]	@ (0x6e8)
     6a8:	6004      	str	r4, [r0, #0]
```
