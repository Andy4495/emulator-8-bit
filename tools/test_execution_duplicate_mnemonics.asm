; Z80 test file for 8-bit emulator
;    https://github.com/Andy4495/emulator-8-bit
;
; 19-Apr-2023  Andy4495 Initial version
; 21-Dec-2023  Andy4495 Update for z88dk compatibility
; 
; Z80 undocumented opcodes which have mnemonics which 
; are duplicates of documented opcodes
; 
; Compile with --ixcbr2 option: 
;   zasm --ixcbr2 filename.asm

    org 0
	ld c,0		; Clear the flags
	push bc
	pop af
	jp	start
	defs 1,0	; fill between with zeros
;	org 8
	ret
	defs 7,0	; fill between with zeros
;	org $10
	ret
	defs 7,0	; fill between with zeros
;	org $18
	ret
	defs 7,0	; fill between with zeros
;	org $20
	ret
	defs 7,0	; fill between with zeros
;	org $28
	ret
	defs 7,0	; fill between with zeros
;	org $30
	ret
	defs 7,0	; fill between with zeros
;	org $38
	ret
	defs 7,0	; fill between with zeros
	defs $26,$ff	; Fill with $ff for compatibility with previous test file versions
;	org $66		; NMI vector
	retn

start: 
	ld sp,$ffff
	ld ix,results

; $ED Miscellaneous instruction duplicate mnemonics
	ld hl,$1234
	defb $ed,$63,$08,$40	; ld (nn),hl 
	ld (ix),'P' 			; Test 1
	inc ix

	defb $ed,$6b,$00,$40	; ld hl,(nn)
	ld a,'a'
	cp h 
	jp nz,t3
	ld a,'V'
	cp L
	jp nz,t3
	ld (ix),'P' 			; Test 2

; $DD prefix duplicate mnemonics (IX opcodes)
t3:
	inc ix
	ld b,2
	defb $dd,$04		; inc b 
	ld a,$03
	cp B
	jp nz,t4
	ld (ix),'P' 			; Test 3

t4:
	inc ix
	defb $dd,$05		; dec b 
	ld a,$02
	cp b
	jp nz,t5
	ld (ix),'P' 			; Test 4

t5:
	inc ix
	defb $dd,$06,$a5	; ld b,n 
	ld a,$a5
	cp b 
	jp nz,t6
	ld (ix),'P' 			; Test 5

t6:
	inc ix
	ld c,$22
	defb $dd,$0c 		; inc c 
	ld a,$23
	cp c 
	jp nz,t7
	ld (ix),'P' 			; Test 6

t7:
	inc ix
	defb $dd,$0d		; dec c 
	ld a,$22
	cp c 
	jp nz,t8
	ld (ix),'P' 			; Test 7

t8:
	inc ix
	defb $dd,$0e,$c5	; ld c,n 
	ld a,$c5
	cp C
	jp nz,t9
	ld (ix),'P' 			; Test 8

t9:
	inc ix
	ld d,$fe
	defb $dd,$14		; inc d 
	ld a,$ff
	cp D
	jp nz,t10
	ld (ix),'P' 			; Test 9

t10:
	inc ix
	defb $dd,$15		; dec d 
	ld a,$fe 
	cp D
	jp nz,t11
	ld (ix),'P' 			; Test 10

t11: 
	inc ix
	defb $dd,$16,$12 	; ld d,n 
	ld a,$12
	cp d 
	jp nz,t12
	ld (ix),'P' 			; Test 11

t12:
	inc ix
	ld e,$ff
	defb $dd,$1c		; inc e 
	jp nz,t13
	ld (ix),'P' 			; Test 12

t13: 
	inc ix
	defb $dd,$1d		; dec e 
	ld a,$ff
	cp E
	jp nz,t14
	ld (ix),'P' 			; Test 13

t14:
	inc ix
	defb $dd,$1e,$23	; ld e,n 
	ld a,$23
	cp E
	jp nz,t15
	ld (ix),'P' 			; Test 14

t15:
	inc ix 
	defb $dd,$3c		; inc a 
	cp $24
	jp nz,t16
	ld (ix),'P' 			; Test 15

t16:
	inc ix
	defb $dd,$3d		; dec a 
	cp $23
	jp nz,t17
	ld (ix),'P' 			; Test 16

t17:
	inc ix
	defb $dd,$3e,$34	; ld a,n
	cp $34
	jp nz,t18
	ld (ix),'P' 			; Test 17

t18:
	inc ix
	ld a,$01
	ld b,$02
	ld c,$03
	ld d,$04
	ld e,$05
	defb $dd,$40		; ld b,b 
	defb $dd,$41		; ld b,c 
	defb $dd,$42		; ld b,d 
	defb $dd,$43		; ld b,e 
	defb $dd,$47		; ld b,a 
	cp B
	jp nz,t19
	ld (ix),'P' 			; Test 18

t19:
	inc ix
	defb $dd,$48		; ld c,b 
	defb $dd,$49		; ld c,c 
	defb $dd,$4a		; ld c,d 
	defb $dd,$4b		; ld c,e 
	defb $dd,$4f		; ld c,a 
	cp c 
	jp nz,t20
	ld (ix),'P' 			; Test 19

t20:
	inc ix
	defb $dd,$50		; ld d,b
	defb $dd,$51		; ld d,c 
	defb $dd,$52		; ld d,d 
	defb $dd,$53		; ld d,e 
	defb $dd,$57		; ld d,a
	cp d
	jp nz,t21
	ld (ix),'P' 			; Test 20

t21:
	inc ix
	defb $dd,$58		; ld e,b
	defb $dd,$59		; ld e,c 
	defb $dd,$5a		; ld e,d 
	defb $dd,$5b		; ld e,e 
	defb $dd,$5f		; ld e,a
	cp e
	jp nz,t22
	ld (ix),'P' 			; Test 21

t22:
	inc ix
	defb $dd,$78		; ld a,b 
	defb $dd,$79		; ld a,c 
	defb $dd,$7a		; ld a,d 
	defb $dd,$7b		; ld a,e 
	defb $dd,$7f		; ld a,a
	cp $01
	jp nz,t23
	ld (ix),'P' 			; Test 22

t23:
	inc ix
	defb $dd,$80		; add a,b 
	defb $dd,$81		; add a,c 
	defb $dd,$82		; add a,d 
	defb $dd,$83		; add a,e 
	defb $dd,$87		; add a,a 
	cp $0a
	jp nz,t24
	ld (ix),'P' 			; Test 23

t24:
	inc ix
	defb $dd,$88		; adc a,b 
	defb $dd,$89		; adc a,c
	defb $dd,$8a		; adc a,d 
	defb $dd,$8b		; adc a,e
	defb $dd,$8f		; adc a,a 
	cp $1c
	jp nz,t25
	ld (ix),'P' 			; Test 24

t25:
	inc ix
	defb $dd,$90		; sub b 
	defb $dd,$91 		; sub c 
	defb $dd,$92		; sub d 
	defb $dd,$93		; sub e 
	defb $dd,$97		; sub a -- should always result in zero 
	jp nz,t26
	ld (ix),'P' 			; Test 25


t26:
	inc ix
	defb $dd,$98		; sbc a,b 
	defb $dd,$99		; sbc a,c 
	defb $dd,$9a		; sbc a,d 
	defb $dd,$9b		; sbc a,e 
	defb $dd,$9f		; sbc a,a 
	jp nz,t27
	ld (ix),'P' 			; Test 26

t27:
	inc ix
	ld a,$01
	defb $dd,$a0		; and b 
	defb $dd,$a1		; and c 
	defb $dd,$a2		; and d 
	defb $dd,$a3 		; and e 
	defb $dd,$a7		; and a 
	cp $01
	jp nz,t28
	ld (ix),'P' 			; Test 27

t28:
	inc ix
	defb $dd,$a8		; xor b 
	defb $dd,$a9 		; xor c 
	defb $dd,$aa 		; xor d 
	defb $dd,$ab 		; xor e 
	defb $dd,$af		; xor a -- will always result in zero
	jp nz,t29
	ld (ix),'P' 			; Test 28

t29:
	inc ix
	defb $dd,$b0		; or b 
	defb $dd,$b1 		; or c 
	defb $dd,$b2 		; or d 
	defb $dd,$b3		; or e 
	defb $dd,$b7		; or a 
	cp $01
	jp nz,t30
	ld (ix),'P' 			; Test 29


t30:
	inc ix
	defb $dd,$b8		; cp b 
	defb $dd,$b9		; cp c 
	defb $dd,$ba		; cp d 
	defb $dd,$bb		; cp e 
	defb $dd,$bf		; cp a 
	jp nz,t31
	ld (ix),'P' 			; Test 30

; $DDCB prefix duplicate opcodes (IX bit opcodes)
t31:
; Start using IY as results pointer, since we are
; running IX opcode tests
	ld a,ixh
	ld iyh,a
	ld a,ixl 
	ld iyl,a
	inc iy
	ld ix,results-9		; IX+1 contains 'R' 0x52
	defb $dd,$cb,$00,$40	; bit 0,(ix+d)
	defb $dd,$cb,$01,$41	; bit 0,(ix+d)
	defb $dd,$cb,$02,$42	; bit 0,(ix+d)
	defb $dd,$cb,$03,$43	; bit 0,(ix+d)
	defb $dd,$cb,$04,$44	; bit 0,(ix+d)
	defb $dd,$cb,$05,$45	; bit 0,(ix+d)
	defb $dd,$cb,$01,$47	; bit 0,(ix+d)
	jp nz,t32
	ld (iy),'P' 			; Test 31

t32:
	inc iy
	defb $dd,$cb,$08,$48	; bit 1,(ix+d)
	defb $dd,$cb,$09,$49	; bit 1,(ix+d)
	defb $dd,$cb,$0a,$4a	; bit 1,(ix+d)
	defb $dd,$cb,$0b,$4b	; bit 1,(ix+d)
	defb $dd,$cb,$0c,$4c	; bit 1,(ix+d)
	defb $dd,$cb,$0d,$4d	; bit 1,(ix+d)
	defb $dd,$cb,$01,$4f	; bit 1,(ix+d)
	jp z,t33
	ld (iy),'P' 			; Test 32

t33:
	inc iy
	defb $dd,$cb,$10,$50	; bit 2,(ix+d)
	defb $dd,$cb,$11,$51	; bit 2,(ix+d)
	defb $dd,$cb,$12,$52	; bit 2,(ix+d)
	defb $dd,$cb,$13,$53	; bit 2,(ix+d)
	defb $dd,$cb,$14,$54	; bit 2,(ix+d)
	defb $dd,$cb,$15,$55	; bit 2,(ix+d)
	defb $dd,$cb,$01,$57	; bit 2,(ix+d)
	jp nz,t34
	ld (iy),'P' 			; Test 33

t34:
	inc iy
	defb $dd,$cb,$18,$58	; bit 3,(ix+d)
	defb $dd,$cb,$19,$59	; bit 3,(ix+d)
	defb $dd,$cb,$1a,$5a	; bit 3,(ix+d)
	defb $dd,$cb,$1b,$5b	; bit 3,(ix+d)
	defb $dd,$cb,$1c,$5c	; bit 3,(ix+d)
	defb $dd,$cb,$1d,$5d	; bit 3,(ix+d)
	defb $dd,$cb,$01,$5f	; bit 3,(ix+d)
	jp nz,t35
	ld (iy),'P' 			; Test 34

t35:
	inc iy
	ld ix,results-7			; ix-1 contains 'R' 0x52
	defb $dd,$cb,$ff,$60	; bit 4,(ix+d)
	defb $dd,$cb,$fe,$61	; bit 4,(ix+d)
	defb $dd,$cb,$fd,$62	; bit 4,(ix+d)
	defb $dd,$cb,$fc,$63	; bit 4,(ix+d)
	defb $dd,$cb,$fb,$64	; bit 4,(ix+d)
	defb $dd,$cb,$fa,$65	; bit 4,(ix+d)
	defb $dd,$cb,$ff,$67	; bit 4,(ix+d)
	jp z,t36
	ld (iy),'P' 			; Test 35

t36:
	inc iy
	defb $dd,$cb,$f8,$68	; bit 5,(ix+d)
	defb $dd,$cb,$f7,$69	; bit 5,(ix+d)
	defb $dd,$cb,$f6,$6a	; bit 5,(ix+d)
	defb $dd,$cb,$f5,$6b	; bit 5,(ix+d)
	defb $dd,$cb,$f4,$6c	; bit 5,(ix+d)
	defb $dd,$cb,$f3,$6d	; bit 5,(ix+d)
	defb $dd,$cb,$ff,$6f	; bit 5,(ix+d)
	jp nz,t37
	ld (iy),'P' 			; Test 36

t37:
	inc iy
	defb $dd,$cb,$f1,$70	; bit 6,(ix+d)
	defb $dd,$cb,$f0,$71	; bit 6,(ix+d)
	defb $dd,$cb,$ef,$72	; bit 6,(ix+d)
	defb $dd,$cb,$ee,$73	; bit 6,(ix+d)
	defb $dd,$cb,$ed,$74	; bit 6,(ix+d)
	defb $dd,$cb,$ec,$75	; bit 6,(ix+d)
	defb $dd,$cb,$ff,$77	; bit 6,(ix+d)
	jp z,t38
	ld (iy),'P' 			; Test 37

t38:
	inc iy
	defb $dd,$cb,$ea,$78	; bit 7,(ix+d)
	defb $dd,$cb,$e9,$79	; bit 7,(ix+d)
	defb $dd,$cb,$e8,$7a	; bit 7,(ix+d)
	defb $dd,$cb,$e7,$7b	; bit 7,(ix+d)
	defb $dd,$cb,$e6,$7c	; bit 7,(ix+d)
	defb $dd,$cb,$e5,$7d	; bit 7,(ix+d)
	defb $dd,$cb,$ff,$7f	; bit 7,(ix+d)
	jp nz,t39
	ld (iy),'P' 			; Test 38

; $FD prefix duplicate mnemonics (IY opcodes)
; These run the same code as $DD prefix opcodes, 
; so no specific tests besides executing the opcode
t39:
	ld a,iyh
	ld ixh,a 
	ld a,iyl 
	ld ixl,a
	inc ix
	defb $fd,$04		; inc b 
	defb $fd,$05		; dec b 
	defb $fd,$06,$a5	; ld b,n 
	defb $fd,$0c 		; inc c 
	defb $fd,$0d		; dec c 
	defb $fd,$0e,$c5	; ld c,n 

	defb $fd,$14		; inc d 
	defb $fd,$15		; dec d 
	defb $fd,$16,$12 	; ld d,n 
	defb $fd,$1c		; inc e 
	defb $fd,$1d		; dec e 
	defb $fd,$1e,$23	; ld e,n 

	defb $fd,$3c		; inc a 
	defb $fd,$3d		; dec a 
	defb $fd,$3e,$34	; ld a,n

	defb $fd,$40		; ld b,b 
	defb $fd,$41		; ld b,c 
	defb $fd,$42		; ld b,d 
	defb $fd,$43		; ld b,e 
	defb $fd,$47		; ld b,a 
	defb $fd,$48		; ld c,b 
	defb $fd,$49		; ld c,c 
	defb $fd,$4a		; ld c,d 
	defb $fd,$4b		; ld c,e 
	defb $fd,$4f		; ld c,a 

	defb $fd,$50		; ld d,b
	defb $fd,$51		; ld d,c 
	defb $fd,$52		; ld d,d 
	defb $fd,$53		; ld d,e 
	defb $fd,$57		; ld d,a
	defb $fd,$58		; ld e,b
	defb $fd,$59		; ld e,c 
	defb $fd,$5a		; ld e,d 
	defb $fd,$5b		; ld e,e 
	defb $fd,$5f		; ld e,a

	defb $fd,$78		; ld a,b 
	defb $fd,$79		; ld a,c 
	defb $fd,$7a		; ld a,d 
	defb $fd,$7b		; ld a,e 
	defb $fd,$7f		; ld a,a

	defb $fd,$80		; add a,b 
	defb $fd,$81		; add a,c 
	defb $fd,$82		; add a,d 
	defb $fd,$83		; add a,e 
	defb $fd,$87		; add a,a 
	defb $fd,$88		; adc a,b 
	defb $fd,$89		; adc a,c
	defb $fd,$8a		; adc a,d 
	defb $fd,$8b		; adc a,e
	defb $fd,$8f		; adc a,a 

	defb $fd,$90		; sub b 
	defb $fd,$91 		; sub c 
	defb $fd,$92		; sub d 
	defb $fd,$93		; sub e 
	defb $fd,$97		; sub a 
	defb $fd,$98		; sbc a,b 
	defb $fd,$99		; sbc a,c 
	defb $fd,$9a		; sbc a,d 
	defb $fd,$9b		; sbc a,e 
	defb $fd,$9f		; sbc a,a 

	defb $fd,$a0		; and b 
	defb $fd,$a1		; and c 
	defb $fd,$a2		; and d 
	defb $fd,$a3 		; and e 
	defb $fd,$a7		; and a 
	defb $fd,$a8		; xor b 
	defb $fd,$a9 		; xor c 
	defb $fd,$aa 		; xor d 
	defb $fd,$ab 		; xor e 
	defb $fd,$af		; xor a 

	defb $fd,$b0		; or b 
	defb $fd,$b1 		; or c 
	defb $fd,$b2 		; or d 
	defb $fd,$b3		; or e 
	defb $fd,$b7		; or a 
	defb $fd,$b8		; cp b 
	defb $fd,$b9		; cp c 
	defb $fd,$ba		; cp d 
	defb $fd,$bb		; cp e 
	defb $fd,$bf		; cp a 

; $FDCB prefix duplicate opcodes (IY bit opcodes)
; These run the same code as $DD prefix opcodes, 
; so no specific tests besides executing the opcode
	defb $fd,$cb,$00,$40	; bit 0,(ix+d)
	defb $fd,$cb,$01,$41	; bit 0,(ix+d)
	defb $fd,$cb,$02,$42	; bit 0,(ix+d)
	defb $fd,$cb,$03,$43	; bit 0,(ix+d)
	defb $fd,$cb,$04,$44	; bit 0,(ix+d)
	defb $fd,$cb,$05,$45	; bit 0,(ix+d)
	defb $fd,$cb,$07,$47	; bit 0,(ix+d)

	defb $fd,$cb,$08,$48	; bit 1,(ix+d)
	defb $fd,$cb,$09,$49	; bit 1,(ix+d)
	defb $fd,$cb,$0a,$4a	; bit 1,(ix+d)
	defb $fd,$cb,$0b,$4b	; bit 1,(ix+d)
	defb $fd,$cb,$0c,$4c	; bit 1,(ix+d)
	defb $fd,$cb,$0d,$4d	; bit 1,(ix+d)
	defb $fd,$cb,$0f,$4f	; bit 1,(ix+d)

	defb $fd,$cb,$10,$50	; bit 2,(ix+d)
	defb $fd,$cb,$11,$51	; bit 2,(ix+d)
	defb $fd,$cb,$12,$52	; bit 2,(ix+d)
	defb $fd,$cb,$13,$53	; bit 2,(ix+d)
	defb $fd,$cb,$14,$54	; bit 2,(ix+d)
	defb $fd,$cb,$15,$55	; bit 2,(ix+d)
	defb $fd,$cb,$17,$57	; bit 2,(ix+d)

	defb $fd,$cb,$18,$58	; bit 3,(ix+d)
	defb $fd,$cb,$19,$59	; bit 3,(ix+d)
	defb $fd,$cb,$1a,$5a	; bit 3,(ix+d)
	defb $fd,$cb,$1b,$5b	; bit 3,(ix+d)
	defb $fd,$cb,$1c,$5c	; bit 3,(ix+d)
	defb $fd,$cb,$1d,$5d	; bit 3,(ix+d)
	defb $fd,$cb,$1f,$5f	; bit 3,(ix+d)

	defb $fd,$cb,$ff,$60	; bit 4,(ix+d)
	defb $fd,$cb,$fe,$61	; bit 4,(ix+d)
	defb $fd,$cb,$fd,$62	; bit 4,(ix+d)
	defb $fd,$cb,$fc,$63	; bit 4,(ix+d)
	defb $fd,$cb,$fb,$64	; bit 4,(ix+d)
	defb $fd,$cb,$fa,$65	; bit 4,(ix+d)
	defb $fd,$cb,$f9,$67	; bit 4,(ix+d)

	defb $fd,$cb,$f8,$68	; bit 5,(ix+d)
	defb $fd,$cb,$f7,$69	; bit 5,(ix+d)
	defb $fd,$cb,$f6,$6a	; bit 5,(ix+d)
	defb $fd,$cb,$f5,$6b	; bit 5,(ix+d)
	defb $fd,$cb,$f4,$6c	; bit 5,(ix+d)
	defb $fd,$cb,$f3,$6d	; bit 5,(ix+d)
	defb $fd,$cb,$f2,$6f	; bit 5,(ix+d)

	defb $fd,$cb,$f1,$70	; bit 6,(ix+d)
	defb $fd,$cb,$f0,$71	; bit 6,(ix+d)
	defb $fd,$cb,$ef,$72	; bit 6,(ix+d)
	defb $fd,$cb,$ee,$73	; bit 6,(ix+d)
	defb $fd,$cb,$ed,$74	; bit 6,(ix+d)
	defb $fd,$cb,$ec,$75	; bit 6,(ix+d)
	defb $fd,$cb,$eb,$77	; bit 6,(ix+d)

	defb $fd,$cb,$ea,$78	; bit 7,(ix+d)
	defb $fd,$cb,$e9,$79	; bit 7,(ix+d)
	defb $fd,$cb,$e8,$7a	; bit 7,(ix+d)
	defb $fd,$cb,$e7,$7b	; bit 7,(ix+d)
	defb $fd,$cb,$e6,$7c	; bit 7,(ix+d)
	defb $fd,$cb,$e5,$7d	; bit 7,(ix+d)
	defb $fd,$cb,$e4,$7f	; bit 7,(ix+d)

end_program:
	halt
	jp	end_program

; Define space for compatiblity with previous version of test Program
; Current PC: $0545
	defs $2abb,$ff
;	org $3000
	defm "Results:"

results:
	defs $ff,'.'		; Results area. 'P' ($50): Pass. 'F' ($46): FAIL. '.' - no result recorded
	defs $ff,'.'

; Define space for compatibility with previous version of test Program
; Current PC: $3206
	defs $0dfa,$ff
;	org $4000
	defm "Values->"
values:
	defs $ff,$00
	defs $ff,$00

testing_area:
	defb $aa
	defb $55

