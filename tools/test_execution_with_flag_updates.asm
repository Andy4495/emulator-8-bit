; Z80 test file for 8-bit emulator
;    https://github.com/Andy4495/emulator-8-bit
;
; 13-Mar-2023  Andy4495 Initial version
; 
; Z80 opcodes that affect flags
; 
; Compile with --ixcbr2 option: 
;   zasm --ixcbr2 filename.asm

    org 0
	jp	start
	defs 5,0	; fill between with zeros
	org 8
	ret
	defs 7,0	; fill between with zeros
	org $10
	ret
	defs 7,0	; fill between with zeros
	org $18
	ret
	defs 7,0	; fill between with zeros
	org $20
	ret
	defs 7,0	; fill between with zeros
	org $28
	ret
	defs 7,0	; fill between with zeros
	org $30
	ret
	defs 7,0	; fill between with zeros
	org $38
	ret
	defs 7,0	; fill between with zeros
	org $66		; NMI vector
	retn

start: 
	ld sp,$ffff
	ld ix,results
; Main Instruction set
; 0x00
incb:
	ld (ix),'P' ; Test 0
	ld b,0
	inc B        
	push af 
	pop de
	ld a,$00	; no flags
	cp e 
	jp z,ldb1
	ld (ix),'F'
	jp decb
ldb1:
	ld b,$7F
	inc b 
	push af
	pop de
	ld a,$94	; S, H, P/V set
	cp e 
	jp z,ldb2
	ld (ix),'F'
	jp decb
ldb2:
	ld b,$ff
	inc b 
	push af
	pop de 
	ld a,$50	; Z, H set
	cp e 
	jp z, decb 
	ld (ix), 'F'
	jp decb

decb:
	inc ix
	ld (ix),'P' ; Test 1
	ld b,$01
	dec B
	push af
	pop de
	ld a,$42 ; Z, N flags
	cp e 
	jp z,decb1
	ld (ix), 'F'
	jp rlca1
decb1:
	ld b,$80
	dec b
	push af 
	pop de 
	ld a,$16 ; H, N, PV flags
	cp e
	jp z,decb2
	ld (ix), 'F'
	jp rlca1
decb2: 
	ld b,$00
	dec b
	push af 
	pop de 
	ld a,$92 ; S, H, N flags
	cp e
	jp z,rlca1
	ld (ix), 'F'
	jp rlca1

rlca1:
	inc ix
	ld (ix),'P' ; Test 2
	ld a,$80
	rlca
	push af 
	pop de 
	ld a,$41 ; C set, Z unchanged from previous
	cp e 
	jp z,rlca2
	ld (ix), 'F'
	jp addhlbc
rlca2:
	ld a,$7f
	rlca 
	push af
	ld e,$fe ; test the rotate
	cp e 
	jp z,rlca3 
	ld (ix),'F'
	jp addhlbc
rlca3:
	pop de 
	ld a,$40 ; Z unchanged from previous 
	cp e
	jp z,addhlbc 
	ld (ix),'F'
	jp addhlbc


addhlbc:
	inc ix
	ld (ix),'P' ; Test 3
	ld hl,$0f11
	ld bc,$00ff
	add hl,bc
	push af
	ld a,$10 ; Test result
	cp h 
	jp nz,addhlbcfail
	cp l 
	jp nz,addhlbcfail 
	ld a,$50 ; Z unchanged from previous, H set
	pop de 
	cp e 
	jp z, incc
addhlbcfail:
	ld (ix),'F' 

incc:
	inc ix
	ld (ix),'P' ; Test 4
	ld c,0
	inc c  
	push af 
	pop de
	ld a,$00	; no flags
	cp e 
	jp nz,inccfail
	ld c,$7F
	inc c
	push af
	pop de
	ld a,$94	; S, H, P/V set
	cp e 
	jp nz,inccfail
	ld c,$ff
	inc c
	push af
	pop de 
	ld a,$50	; Z, H set
	cp e 
	jp z, decc
inccfail:
	ld (ix),'F'

decc:
	inc ix
	ld (ix),'P' ; Test 5
	ld c,$01
	dec c
	push af
	pop de
	ld a,$42 ; Z, N flags
	cp e 
	jp nz,deccfail
	ld c,$80
	dec c
	push af 
	pop de 
	ld a,$16 ; H, N, PV flags
	cp e
	jp nz,deccfail
	ld c,$00
	dec c
	push af 
	pop de 
	ld a,$92 ; S, H, N flags
	cp e
	jp z,rrca
deccfail:
	ld (ix),'F'

rrca:
	inc ix
	ld (ix),'P' ; Test 6
	ld a,$01
	rrca
	push af 
	pop de 
	ld a,$41 ; C set, Z unchanged from previous
	cp e 
	jp nz,rrcafail
	ld a,$fe
	rrca 
	push af
	ld e,$7f ; test the rotate
	cp e 
	jp nz,rrcafail
	pop de 
	ld a,$40 ; Z unchanged from previous 
	cp e
	jp z,incd
rrcafail:
	ld (ix),'F'	

; Many of the following use the same code for flag updates
; as previously tested opcodes. So just test that result of
; operation is correct, instead of re-testing the flag 
; update code
; 0x10
incd:
	inc ix
	ld (ix),'P' ; Test 7
	ld d,$7f
	inc d
	ld a,$80
	cp d
	jp z,decd
incdfail:        
	ld (ix),'F'	

decd:
	inc ix
	ld (ix),'P' ; Test 8
	ld d,$7f
	dec D
	ld a,$7e 
	cp d 
	jp z,rla1
decdfail:
	ld (ix),'F'	

rla1:
	inc ix
	ld (ix),'P' ; Test 9
	ld a,$5a 
	rla
	jp c,rlafail 
	cp $b4
	jp nz,rlafail
	rla 
	jp nc,rlafail
	jp addhlde
rlafail:
	ld (ix),'F'	

addhlde:
	inc ix
	ld (ix),'P' ; Test 10
	ld hl,$5aa4
	ld de,$a45b
	add hl,de
	ld de,$feff   ; expected result
	or a		; clear carry flag 
	sbc hl,de
	add hl,de   ; Z is set if compare matches
	jp z,ince
addhldefail:
	ld (ix),'F'	

ince:
	inc ix
	ld (ix),'P' ; Test 11
	ld e,$39
	inc e 
	ld a,$3a 
	cp a 
	jp z,dece
incefail:
	ld (ix),'F'	

dece:
	inc ix
	ld (ix),'P' ; Test 12
	ld e,$b3
	dec E
	ld a,$b2 
	cp e
	jp z,rra1
decefail:
	ld (ix),'F'	

rra1:
	inc ix
	ld (ix),'P' ; Test 13
	ld a,$01
	rra
	push af 
	jp nc,rrafail 
	cp $00
	jp nz,rrafail 
	pop af
	rra 
	cp $80
	jp z,inch
rrafail:
	ld (ix),'F'	

; 0x20
inch:
	inc ix
	ld (ix),'P' ; Test 14
	ld h,$9f
	inc h 
	ld a,$a0
	cp h
	jp z,dech
inchfail:
	ld (ix),'F'	

dech:
	inc ix
	ld (ix),'P' ; Test 15
	ld h,$99
	dec H
	ld a,$98 
	cp h 
	jp z,addhlhl
dechfail:
	ld (ix),'F'	

;; DAA requires a much more extensive test suite
;; Test with a separate file
;	daa

addhlhl:
	inc ix
	ld (ix),'P' ; Test 16
	ld hl,$a2a2
	add hl,hl 
	ld de,$4544	; expected result
	or a		; clear carry flag 
	sbc hl,de
	add hl,de   ; Z is set if compare matches	
	jp z,incl
addhlhlfail:
	ld (ix),'F'	

incl:
	inc ix
	ld (ix),'P' ; Test 17
	ld l,$99
	inc l 
	ld a,$9a
	cp l 
	jp z,decl
inclfail:
	ld (ix),'F'	

decl:
	inc ix
	ld (ix),'P' ; Test 18
	ld l,$50
	dec l
	ld a,$4f 
	cp l 
	jp z,cpl1
declfail:
	ld (ix),'F'	

cpl1:
	inc ix
	ld (ix),'P' ; Test 19
	ld a,$5a 
	or a		; clear Z and C flags, sets PV
	cpl
	push af
	cp $a5
	jp nz,cplfail
	pop de 
	ld a,$16	; N and H set, PV set from "or a" above
	cp e 
	jp z,incmhl
cplfail:
	ld (ix),'F'	


; 0x30
incmhl:
	inc ix
	ld (ix),'P' ; Test 20
	ld hl,results-9
	ld (hl),'1'
	inc (HL)
	ld a,'2'
	cp (hl)
	jp z,decmhl
incmhlfail:
	ld (ix),'F'	
decmhl:
	inc ix
	ld (ix),'P' ; Test 21
	dec (HL)
	ld a,'1'
	cp (hl)
	jp z,scf1
decmhlfail:
	ld (ix),'F'	

scf1:
	inc ix
	ld (ix),'P' ; Test 22
	or a ; clear carry flag
	jp c,scffail
	scf
	jp c, addhlsp
scffail:
	ld (ix),'F'	
addhlsp:
	inc ix
	ld (ix),'P' ; Test 23
	ld hl,$1234
	add HL,SP
	ld de,$1233
	or a 	; clear carry flag
	sbc hl,de
	add hl,de
	jp z,inca1 
addhlspfail:
	ld (ix),'F'	

inca1:
	inc ix
	ld (ix),'P' ; Test 24
	ld a,$ff
	inc A
	jp z,deca1
incafail:     
	ld (ix),'F'	
deca1:
	inc ix
	ld (ix),'P' ; Test 25
	ld a,$01
	dec A
	jp z,ccf1
decafail:
	ld (ix),'F'	
ccf1:
	inc ix
	ld (ix),'P' ; Test 26
	scf
	ccf
	jp nc,addab
ccffail:
	ld (ix),'F'	
	

; 0x40	

; 0x50	

; 0x60	

; 0x70	

; 0x80 -- all affect flags
addab:
	inc ix
	ld (ix),'P' ; Test 27
	ld bc,0 
	push bc
	pop af	; clear flags
	ld b,0
	add A,B ; Set Z flag
	jp nz,addabfail
	ld b,$80
	add a,b ; set S flag
	jp p,addabfail 
	ld a,$0f 
	ld b,$01
	add a,b ; set H flag
	push af 
	pop bc
	ld a,$10 ; check for H flag set, N flag reset
	cp c 
	jp nz,addabfail 
	ld a,$70 
	ld b,$31
	add a,b  ; test overflow, positive operands
	jp po,addabfail	; odd parity is same as no-overflow
	ld a,$f1
	ld b,$62
	add a,b ; test overflow, mixed operands -> should not overflow
	jp pe,addabfail ; even parity represents overflow
	ld a,$81
	ld b,$82
	add a,b ; test overflow, negative operands
	jp po,addabfail; ; odd parity is same as no-overflow
	ld b,$03	; check if result is correct
	cp b
	jp z,addac
addabfail:
	ld (ix),'F'	

addac:
	inc ix
	ld (ix),'P' ; Test 28
	or a 	; clears the carry flag
	ld a,$99
	ld c,$20
	add A,C
	ld b,$b9
	cp b 
	jp z,addad
addacfail:
	ld (ix),'F'	

addad:
	inc ix
	ld (ix),'P' ; Test 29
	or a 	; clears the carry flag
	ld a,$40 
	ld d,$a5 
	add A,D
	ld b,$e5
	cp b 
	jp z,addae 
addadfail:
	ld (ix),'F'	

addae:
	inc ix
	ld (ix),'P' ; Test 30
	or a 	; clears the carry flag
	ld a,$01 
	ld e,$10
	add A,E
	cp $11
	jp z,addah 
addaefail:
	ld (ix),'F'	

addah:
	inc ix
	ld (ix),'P' ; Test 31
	or a 	; clears the carry flag
	ld a,$80
	ld h,$08
	add A,H
	cp $88
	jp z,addal
addahfail:
	ld (ix),'F'	

addal:
	inc ix
	ld (ix),'P' ; Test 32
	or a 	; clears the carry flag
	ld a,$15
	ld l,$51
	add A,L
	cp $66
	jp z,addamhl
addalfail:
	ld (ix),'F'	

addamhl:
	inc ix
	ld (ix),'P' ; Test 33
	or a 	; clears the carry flag
	ld HL,results-10
	ld (hl),'a'
	ld a,1
	add A,(HL)
	cp 'b'
	jp z,addaa
addamhlfail:
	ld (ix),'F'	

addaa:
	inc ix
	ld (ix),'P' ; Test 34
	or a 	; clears the carry flag
	ld a,$22
	add A,A
	cp $44 
	jp z,adcab
addaafail:
	ld (ix),'F'	

adcab:
	inc ix
	ld (ix),'P' ; Test 35
	or a 	; clears the carry flag
	ld a,$12
	ld b,$23 
	scf 
	adc A,B
	cp $36
	jp z,adcac
adcabfail:
	ld (ix),'F'	

adcac:
	inc ix
	ld (ix),'P' ; Test 36
	or a 	; clears the carry flag
	ld a,$12
	ld c,$33
	adc A,C
	cp $45
	jp z,adcad
adcacfail:
	ld (ix),'F'	

adcad:
	inc ix
	ld (ix),'P' ; Test 37
	or a 	; clears the carry flag
	ld a,$80
	ld d,$21
	adc A,D
	cp $a1
	jp z,adcae
adcadfail:
	ld (ix),'F'	

adcae:
	inc ix
	ld (ix),'P' ; Test 38
	or a 	; clears the carry flag
	ld a,$44
	ld e,$aa
	adc A,E
	cp $ee
	jp z,adcah
adcaefail:
	ld (ix),'F'	

adcah:
	inc ix
	ld (ix),'P' ; Test 39
	or a 	; clears the carry flag
	ld a,$01 
	ld h,$11
	adc A,H
	cp $12
	jp z,adcal
adcahfail:
	ld (ix),'F'	

adcal:
	inc ix
	ld (ix),'P' ; Test 40
	or a 	; clears the carry flag
	ld a,$55
	ld l,$12
	adc A,L
	cp $67
	jp z,adcamhl
adcafail:
	ld (ix),'F'	

adcamhl:
	inc ix
	ld (ix),'P' ; Test 41
	or a 	; clears the carry flag
	ld hl,results-8
	ld a,1
	adc A,(HL)
	cp 'S'
	jp z,adcaa
adcamhltest:
	ld (ix),'F'	

adcaa:
	inc ix
	ld (ix),'P' ; Test 42
	or a 	; clears the carry flag
	ld a,$35
	adc A,A
	cp $6a
	jp z,subab
adcaafail:
	ld (ix),'F'	


; 0x90 -- all affect flags
subab:
	inc ix
	ld (ix),'P' ; Test 43
	ld a,$22
	ld b,$10
	sub A,B
	cp $12
	jp z,subac
subabfail:
	ld (ix),'F'	

subac:
	inc ix
	ld (ix),'P' ; Test 44
	ld a,$23
	ld c,$09
	sub A,C
	cp $1a
	jp z,subad
subacfail:
	ld (ix),'F'	

subad:
	inc ix
	ld (ix),'P' ; Test 45
	ld a,$f0
	ld d,$08
	sub A,D
	cp $e8
	jp z,subae
subadfail:
	ld (ix),'F'	

subae:
	inc ix
	ld (ix),'P' ; Test 46
	ld a,$ff 
	ld e,$fe 
	sub A,E
	cp $01
	jp z,subah
subaefail:
	ld (ix),'F'	

subah:
	inc ix
	ld (ix),'P' ; Test 47
	ld a,$80 
	ld h,$01
	sub A,H
	cp $7f
	jp z,subal
subahfail:
	ld (ix),'F'	

subal:
	inc ix
	ld (ix),'P' ; Test 48
	ld a,$70
	ld l,$71
	sub A,L
	cp $ff 
	jp z,subamhl
subalfail:
	ld (ix),'F'	

subamhl:
	inc ix
	ld (ix),'P' ; Test 49
	ld hl,results-7 ; This is the ascii letter 'e'
	ld a,'g'
	sub A,(HL)
	cp $02
	jp z,subaa
subamhlfail:
	ld (ix),'F'	

subaa:
	inc ix
	ld (ix),'P' ; Test 50
	sub A,A ; should be zero regardless of what's in A 
	jp z,sbcab
subaafail:
	ld (ix),'F'	

sbcab:
	inc ix
	ld (ix),'P' ; Test 51
	scf 
	ld a,$99
	ld b,$90
	sbc A,B
	cp $08
	jp z,sbcac
sbcabfail:
	ld (ix),'F'	

sbcac:
	inc ix
	ld (ix),'P' ; Test 52
	scf
	ld a,$05
	ld c,$04
	sbc A,C
	jp z,sbcad
sbcacfail:
	ld (ix),'F'	

sbcad:
	inc ix
	ld (ix),'P' ; Test 53
	scf
	ld a,$10
	ld d,$05
	scf
	sbc A,D
	cp $0a
	jp z,sbcae
sbcadfail:
	ld (ix),'F'	

sbcae:
	inc ix
	ld (ix),'P' ; Test 54
	scf 
	ld a,$22
	ld e,$11
	sbc A,E
	cp $10
	jp z,sbcah
sbcaefail:
	ld (ix),'F'	

sbcah:
	inc ix
	ld (ix),'P' ; Test 55
	scf
	ld a,$90
	ld h,$08
	sbc A,H
	cp $87
	jp z,sbcal
sbcahfail:
	ld (ix),'F'	

sbcal:
	inc ix
	ld (ix),'P' ; Test 56
	scf
	ld a,$a0
	ld l,$10
	sbc A,L
	cp $8f
	jp z,sbcamhl 
sbcalfail:
	ld (ix),'F'	

sbcamhl:
	inc ix
	ld (ix),'P' ; Test 57
	scf
	ld hl,results-6 ; contains ascii 's'
	ld a,'w'
	sbc A,(HL)
	cp $03
	jp z,sbcaa
sbcamhlfail:
	ld (ix),'F'	

sbcaa:
	inc ix
	ld (ix),'P' ; Test 58
	scf
	sbc A,A
	cp $ff 
	jp z,andab
sbcaafail:
	ld (ix),'F'	

; 0xa0 -- all affect flags
andab:
	inc ix
	ld (ix),'P' ; Test 59
	ld a,$03
	ld b,$02
	and A,B
	cp $02
	jp z,andac
andabfail:
	ld (ix),'F'	

andac:
	inc ix
	ld (ix),'P' ; Test 60
	ld a,$f3
	ld c,$84
	and A,C
	cp $80
	jp z,andad
andacfail:
	ld (ix),'F'	

andad:
	inc ix
	ld (ix),'P' ; Test 61
	ld a,$f1
	ld d,$81
	and A,D
	cp $81
	jp z,andae
andadfail:
	ld (ix),'F'	

andae:
	inc ix
	ld (ix),'P' ; Test 62
	ld a,$fe
	ld e,$31
	and A,E
	cp $30 
	jp z,andah
andaefail:
	ld (ix),'F'	

andah:
	inc ix
	ld (ix),'P' ; Test 63
	ld a,$33
	ld h,$12
	and A,H
	cp $12
	jp z,andal
andahfail:
	ld (ix),'F'	

andal:
	inc ix
	ld (ix),'P' ; Test 64
	ld a,$55
	ld l,$aa
	and A,L
	jp z,andamhl
andalfail:
	ld (ix),'F'	

andamhl:
	inc ix
	ld (ix),'P' ; Test 65
	ld hl,results-1 ; contains ascii ':' (0x3e)
	ld a,$f1
	and A,(HL)
	cp $30
	jp z,andaa
andamhlfail:
	ld (ix),'F'	

andaa:
	inc ix
	ld (ix),'P' ; Test 66
	ld a,$5a
	and A,A
	cp $5a 
	jp z,xorab
andaafail:
	ld (ix),'F'	

xorab:
	inc ix
	ld (ix),'P' ; Test 67
	ld a,$ff
	ld b,$55
	xor A,B
	cp $aa
	jp z,xorac
xorabfail:
	ld (ix),'F'	

xorac:
	inc ix
	ld (ix),'P' ; Test 68
	ld a,$55
	ld c,$fa
	xor A,C
	cp $af
	jp z,xorad
xoracfail:
	ld (ix),'F'	

xorad:
	inc ix
	ld (ix),'P' ; Test 69
	ld a,$80
	ld d,$08
	xor A,D
	cp $88
	jp z,xorae
xoradfail:
	ld (ix),'F'	

xorae:
	inc ix
	ld (ix),'P' ; Test 70
	ld a,$11
	ld e,$22
	xor A,E
	cp $33
	jp z,xorah
xoraefail:
	ld (ix),'F'	

xorah:
	inc ix
	ld (ix),'P' ; Test 71
	ld a,$44
	ld h,$15
	xor A,H
	cp $51
	jp z,xoral
xorahfail:
	ld (ix),'F'	

xoral:
	inc ix
	ld (ix),'P' ; Test 72
	ld a,$88
	ld l,$11
	xor A,L
	cp $99
	jp z,xoramhl
xoralfail:
	ld (ix),'F'	

xoramhl:
	inc ix
	ld (ix),'P' ; Test 73
	ld HL,results-2 ; contains ascii 's' (0x73)
	ld a,0x91
	xor A,(HL)
	cp $e2
	jp z,xoraa
xoramhlfail:
	ld (ix),'F'	

xoraa:
	inc ix
	ld (ix),'P' ; Test 74
	xor A,A ; xor with itself always results in zero
	jp z,orab
xoraafail:
	ld (ix),'F'	

; 0xb0 -- all affect flags
orab:
	inc ix
	ld (ix),'P' ; Test 75
	ld a,$20
	ld b,$02
	or  A,B
	cp $22
	jp z,orac
orabfail:
	ld (ix),'F'	

orac:
	inc ix
	ld (ix),'P' ; Test 76
	ld a,$31
	ld c,$42
	or  A,C
	cp $73
	jp z,orad
oracfail:
	ld (ix),'F'	

orad:
	inc ix
	ld (ix),'P' ; Test 77
	ld a,$11
	ld d,$33
	or  A,D
	cp $33
	jp z,orae
oradfail:
	ld (ix),'F'	

orae:
	inc ix
	ld (ix),'P' ; Test 78
	ld a,$5a 
	ld e,$a5 
	or  A,E
	cp $ff
	jp z,orah
oraefail:
	ld (ix),'F'	

orah:
	inc ix
	ld (ix),'P' ; Test 79
	ld a,$01
	ld h,$fe
	or  A,H
	cp $ff 
	jp z,oral
orahfail:
	ld (ix),'F'	

oral:
	inc ix
	ld (ix),'P' ; Test 80
	ld a,$01
	ld l,$10
	or  A,L
	cp $11
	jp z,oramhl
oralfail:
	ld (ix),'F'	

oramhl:
	inc ix
	ld (ix),'P' ; Test 81
	ld hl,results-8 ; contains ascii 'R' (0x52)
	ld a,$a1
	or  A,(HL)
	cp $f3
	jp z,oraa
oramhlfail:
	ld (ix),'F'	

oraa:
	inc ix
	ld (ix),'P' ; Test 82
	ld a,$5a
	or  A,A
	cp $5a 
	jp z,cpab
oraafail:
	ld (ix),'F'	

cpab:
	inc ix
	ld (ix),'P' ; Test 83
	ld a,$05
	ld b,$06
	cp  A,B
	jp nz,cpac
cpabfail:
	ld (ix),'F'	

cpac:
	inc ix
	ld (ix),'P' ; Test 84
	ld a,$10
	ld c,$10
	cp  A,C
	jp z,cpad
cpacfail:
	ld (ix),'F'	

cpad:
	inc ix
	ld (ix),'P' ; Test 85
	ld d,$10
	cp  A,D
	jp z,cpae
cpadfail:
	ld (ix),'F'	

cpae:
	inc ix
	ld (ix),'P' ; Test 86
	ld e,$10
	cp  A,E
	jp z,cpah
cpaefail:
	ld (ix),'F'	

cpah:
	inc ix
	ld (ix),'P' ; Test 87
	ld h,$10
	cp  A,H
	jp z,cpal
cpahfail:
	ld (ix),'F'	

cpal:
	inc ix
	ld (ix),'P' ; Test 88
	ld l,$10
	cp  A,L
	jp z,cpamhl
cpalfail:
	ld (ix),'F'	

cpamhl:
	inc ix
	ld (ix),'P' ; Test 89
	ld HL,results-4 ; contains ascii 'l' (0x6c)
	ld a,$6c
	cp  A,(HL)
	jp z,cpaa
cpamhlfail:
	ld (ix),'F'	

cpaa:
	inc ix
	ld (ix),'P' ; Test 90
	cp  A,A
	jp z,addan
cpaafail:
	ld (ix),'F'	


; 0xc0
addan:
	halt ;;; temporary
	add A,$12
	adc a,$23
	

; 0xd0
	sub A,$34
	sbc A,$45
	

; 0xe0
	and A,$56
	xor A,$67

; 0xf0
	or  A,$78
	cp  A,$89
	

; Bit Instructions - 0xcb prefix
; 0xcb00 -- all affect flags
	rlc B           
	rlc C        
	rlc D   
	rlc E  
	rlc H   
	rlc L          
	rlc (HL)
	rlc A
	rrc B
	rrc C
	rrc D
	rrc E
	rrc H
	rrc L
	rrc (HL)
	rrc A

; 0xcb10  -- all affect flags
	rl  B
	rl  C
	rl  D
	rl  E
	rl  H
	rl  L
	rl  (HL)
	rl  A
	rr  B
	rr  C
	rr  D
	rr  E
	rr  H
	rr  L
	rr  (HL)
	rr  A

; 0xcb20 -- all affect flags
	sla B
	sla C
	sla D
	sla E
	sla H
	sla L
	sla (HL)
	sla A
	sra B
	sra C
	sra D
	sra E
	sra H
	sra L
	sra (HL)
	sra A

; 0xcb30 -- all affect flags
	sll B
	sll C
	sll D
	sll E
	sll H
	sll L
	sll (HL)
	sll A
	srl B
	srl C
	srl D
	srl E
	srl H
	srl L
	srl (HL)
	srl A

; 0xcb40 -- all affect flags
	bit 0,B
	bit 0,C
	bit 0,D
	bit 0,E
	bit 0,H
	bit 0,L
	bit 0,(HL)
	bit 0,A
	bit 1,B
	bit 1,C
	bit 1,D
	bit 1,E
	bit 1,H
	bit 1,L
	bit 1,(HL)
	bit 1,A

; 0xcb50 -- all affect flags
	bit 2,B
	bit 2,C
	bit 2,D
	bit 2,E
	bit 2,H
	bit 2,L
	bit 2,(HL)
	bit 2,A
	bit 3,B
	bit 3,C
	bit 3,D
	bit 3,E
	bit 3,H
	bit 3,L
	bit 3,(HL)
	bit 3,A

; 0xcb60 -- all affect flags
	bit 4,B
	bit 4,C
	bit 4,D
	bit 4,E
	bit 4,H
	bit 4,L
	bit 4,(HL)
	bit 4,A
	bit 5,B
	bit 5,C
	bit 5,D
	bit 5,E
	bit 5,H
	bit 5,L
	bit 5,(HL)
	bit 5,A

; 0xcb70 -- all affect flags
	bit 6,B
	bit 6,C
	bit 6,D
	bit 6,E
	bit 6,H
	bit 6,L
	bit 6,(HL)
	bit 6,A
	bit 7,B
	bit 7,C
	bit 7,D
	bit 7,E
	bit 7,H
	bit 7,L
	bit 7,(HL)
	bit 7,A

; 0xcb80	

; 0xcb90	

; 0xcba0	

; 0xcbb0	

; 0xcbc0

; 0xcbd0

; 0xcbe0	

; 0xcbf0	

; Miscellaneous opcodes - 0xed prefix
; 0xed00 - 0xed3f are undefined opcodes (executed as NOPs)
; 0xed40
	in  B,(C)
	sbc HL,BC
	neg
	in  C,(C)
	ld  BC,($5678)
	

; 0xed50
	in  D,(C)
	sbc HL,DE
	ld  A,I
	in  E,(C)
	adc HL,DE
	ld  A,R
	

; 0xed60
	in  H,(C)
	sbc HL,HL
	rrd
	in  L,(C)
	adc HL,HL
	rld
	

; 0xed70
	in  f,(C)  		; This is also written as "in (c)" -- only affects flags, input data is discarded
	sbc HL,SP
	in  A,(C)
	out (C),A
	adc HL,SP

; opcodes 0xed80 - 0xed9f undefined
; 0xeda0
	ldi
	cpi
	ini
	outi
	ldd
	cpd
	ind
	outd	

; 0xedb0
	ldir
	cpir
	inir
	otir
	lddr
	cpdr
	indr
	otdr
	
; opcodes 0xedc0 - 0xedff undefined

; IX instructions - 0xdd prefix
; 0xdd00
	add IX,BC
	
; 0xdd10
	add IX,DE
	
; 0xdd20
	inc IXH              
	dec IXH
	add IX,IX
	inc IXL               
	dec IXL
	
; 0xdd30
	inc (IX - $01)
	dec (IX - $03)
	add IX,SP
	
; 0xdd40

; 0xdd50

; 0xdd60

; 0xdd70

; 0xdd80
	add  A,IXH
	add  A,IXL
	add  A,(IX-$07)
	adc  A,IXH 
	adc  A,IXL 
	adc  A,(IX+$02)	

; 0xdd90
	sub  A,IXH
	sub  A,IXL
	sub  A,(IX-$07)
	sbc  A,IXH 
	sbc  A,IXL 
	sbc  A,(IX+$02)	

; 0xdda0
	and  A,IXH
	and  A,IXL
	and  A,(IX-$07)
	xor  A,IXH 
	xor  A,IXL 
	xor  A,(IX+$02)	

; 0xddb0
	or  A,IXH
	or  A,IXL
	or  A,(IX-$07)
	cp  A,IXH 
	cp  A,IXL 
	cp  A,(IX+$02)	

; 0xddc0	

; 0xddd0

; 0xdde0
	
; 0xddf0

; 0xddcb - IX bit instructions
; 0xddcbss00 -- all affect flags
	rlc (IX+$7D),B
	rlc (IX+$7E),C
	rlc (IX+$7F),D
	rlc (IX-$70),E
	rlc (IX-$72),H
	rlc (IX-$73),L
	rlc (IX+$22)
	rlc (IX-$74),A
	rrc (IX+$7D),B
	rrc (IX+$7E),C
	rrc (IX+$7F),D
	rrc (IX-$70),E
	rrc (IX-$72),H
	rrc (IX-$73),L
	rrc (IX+$22)
	rrc (IX-$74),A

; 0xddcbss10 -- all affect flags
	rl (IX+$7D),B
	rl (IX+$7E),C
	rl (IX+$7F),D
	rl (IX-$70),E
	rl (IX-$72),H
	rl (IX-$73),L
	rl (IX+$22)
	rl (IX-$74),A
	rr (IX+$7D),B
	rr (IX+$7E),C
	rr (IX+$7F),D
	rr (IX-$70),E
	rr (IX-$72),H
	rr (IX-$73),L
	rr (IX+$22)
	rr (IX-$74),A

; 0xddcbss20 -- all affect flags
	sla (IX+$7D),B
	sla (IX+$7E),C
	sla (IX+$7F),D
	sla (IX-$70),E
	sla (IX-$72),H
	sla (IX-$73),L
	sla (IX+$22)
	sla (IX-$74),A
	sra (IX+$7D),B
	sra (IX+$7E),C
	sra (IX+$7F),D
	sra (IX-$70),E
	sra (IX-$72),H
	sra (IX-$73),L
	sra (IX+$22)
	sra (IX-$74),A

; 0xddcbss30 -- all affect flags
	sll (IX+$7D),B
	sll (IX+$7E),C
	sll (IX+$7F),D
	sll (IX-$70),E
	sll (IX-$72),H
	sll (IX-$73),L
	sll (IX+$22)
	sll (IX-$74),A
	srl (IX+$7D),B
	srl (IX+$7E),C
	srl (IX+$7F),D
	srl (IX-$70),E
	srl (IX-$72),H
	srl (IX-$73),L
	srl (IX+$22)
	srl (IX-$74),A

; 0xddcbss40 - 0xddcbss7f
	bit 0,(IX+$01)  
	bit 1,(IX+$02)
	bit 2,(IX+$03)
	bit 3,(IX+$04)
	bit 4,(IX+$05)
	bit 5,(IX+$06)
	bit 6,(IX+$07)
	bit 7,(IX+$08)	

; 0xddcbss80 - 0xddcbssbf

; 0xddcbssc0 - 0xddcbssff

; IY instructions - 0xfd prefix
; 0xfd00
	add IY,BC

; 0xfd10
	add IY,DE

; 0xfd20
	inc IYH              
	dec IYH
	add IY,IY
	inc IYL               
	dec IYL

; 0xfd30
	inc (IY - $01)
	dec (IY - $03)
	add IY,SP
	
; 0xfd40

; 0xfd50

; 0xfd60    
	
; 0xfd70

; 0xfd80 -- all affect flags
;	nop				; 0xfd80 same as 0x80
;	nop				; 0xfd81 same as 0x81
;	nop				; 0xfd82 same as 0x82
;	nop				; 0xfd83 same as 0x83
	add  A,IYH
	add  A,IYL
	add  A,(IY-$07)
;	nop				; 0xfd87 same as 0x87
;	nop				; 0xfd88 same as 0x88
;	nop				; 0xfd89 same as 0x89
;	nop				; 0xfd8a same as 0x8a
;	nop				; 0xfd8b same as 0x8b
	adc  A,IYH 
	adc  A,IYL 
	adc  A,(IY+$02)
;	nop				; 0xfd8f same as 0x8f
	

; 0xfd90 -- all affect flags
;	nop				; 0xfd90 same as 0x90
;	nop				; 0xfd91 same as 0x91
;	nop				; 0xfd92 same as 0x92
;	nop				; 0xfd93 same as 0x93
	sub  A,IYH
	sub  A,IYL
	sub  A,(IY-$07)
;	nop				; 0xfd97 same as 0x97
;	nop				; 0xfd98 same as 0x98
;	nop				; 0xfd99 same as 0x99
;	nop				; 0xfd9a same as 0x9a
;	nop				; 0xfd9b same as 0x9b
	sbc  A,IYH 
	sbc  A,IYL 
	sbc  A,(IY+$02)
;	nop				; 0xfd9f same as 0x9f
	

; 0xfda0 -- all affect flags
;	nop				; 0xfda0 same as 0xa0
;	nop				; 0xfda1 same as 0xa1
;	nop				; 0xfda2 same as 0xa2
;	nop				; 0xfda3 same as 0xa3
	and  A,IYH
	and  A,IYL
	and  A,(IY-$07)
;	nop				; 0xfda7 same as 0xa7
;	nop				; 0xfda8 same as 0xa8
;	nop				; 0xfda9 same as 0xa9
;	nop				; 0xfdaa same as 0xaa
;	nop				; 0xfdab same as 0xab
	xor  A,IYH 
	xor  A,IYL 
	xor  A,(IY+$02)
;	nop				; 0xfdaf same as 0xaf
	

; 0xfdb0 -- all affect flags
;	nop				; 0xfdb0 same as 0xb0
;	nop				; 0xfdb1 same as 0xb1
;	nop				; 0xfdb2 same as 0xb2
;	nop				; 0xfdb3 same as 0xb3
	or  A,IYH
	or  A,IYL
	or  A,(IY-$07)
;	nop				; 0xfdb7 same as 0xb7
;	nop				; 0xfdb8 same as 0xb8
;	nop				; 0xfdb9 same as 0xb9
;	nop				; 0xfdba same as 0xba
;	nop				; 0xfdbb same as 0xbb
	cp  A,IYH 
	cp  A,IYL 
	cp  A,(IY+$02)
;	nop				; 0xfdbf same as 0xbf
	

; 0xfdc0

; 0xfdd0

; 0xfde0

; 0xfdf0

; 0xfdcb - IY bit instructions
; 0xfdcbss00 -- all affect flags
	rlc (IY+$7D),B
	rlc (IY+$7E),C
	rlc (IY+$7F),D
	rlc (IY-$70),E
	rlc (IY-$72),H
	rlc (IY-$73),L
	rlc (IY+$22)
	rlc (IY-$74),A
	rrc (IY+$7D),B
	rrc (IY+$7E),C
	rrc (IY+$7F),D
	rrc (IY-$70),E
	rrc (IY-$72),H
	rrc (IY-$73),L
	rrc (IY+$22)
	rrc (IY-$74),A

; 0xfdcbss10 -- all affect flags
	rl (IY+$7D),B
	rl (IY+$7E),C
	rl (IY+$7F),D
	rl (IY-$70),E
	rl (IY-$72),H
	rl (IY-$73),L
	rl (IY+$22)
	rl (IY-$74),A
	rr (IY+$7D),B
	rr (IY+$7E),C
	rr (IY+$7F),D
	rr (IY-$70),E
	rr (IY-$72),H
	rr (IY-$73),L
	rr (IY+$22)
	rr (IY-$74),A

; 0xfdcbss20 -- all affect flags
	sla (IY+$7D),B
	sla (IY+$7E),C
	sla (IY+$7F),D
	sla (IY-$70),E
	sla (IY-$72),H
	sla (IY-$73),L
	sla (IY+$22)
	sla (IY-$74),A
	sra (IY+$7D),B
	sra (IY+$7E),C
	sra (IY+$7F),D
	sra (IY-$70),E
	sra (IY-$72),H
	sra (IY-$73),L
	sra (IY+$22)
	sra (IY-$74),A

; 0xfdcbss30 -- all affect flags
	sll (IY+$7D),B
	sll (IY+$7E),C
	sll (IY+$7F),D
	sll (IY-$70),E
	sll (IY-$72),H
	sll (IY-$73),L
	sll (IY+$22)
	sll (IY-$74),A
	srl (IY+$7D),B
	srl (IY+$7E),C
	srl (IY+$7F),D
	srl (IY-$70),E
	srl (IY-$72),H
	srl (IY-$73),L
	srl (IY+$22)
	srl (IY-$74),A

; 0xfdcbss40 - 0xfdcbss7f
	bit 0,(IY+$01)  
	bit 1,(IY+$02)
	bit 2,(IY+$03)
	bit 3,(IY+$04)
	bit 4,(IY+$05)
	bit 5,(IY+$06)
	bit 6,(IY+$07)
	bit 7,(IY+$08)	

; 0xfdcbss80 - 0xfdcbssbf

; 0xfdcbssc0 - 0xfdcbssff

end_program:
	halt
	jp	end_program

	org $1000
	defm "Results:"

results:
	defs $ff,'.'		; Results area. 'P' ($50): Pass. 'F' ($46): FAIL. '.' - no result recorded

