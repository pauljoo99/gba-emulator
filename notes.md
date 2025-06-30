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
