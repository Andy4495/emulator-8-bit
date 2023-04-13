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
	inc ix
	ld (ix),'P' ; Test 91
	ld a,$55
	add A,$12
	cp $67
	jp z,adcan
addanfail:
	ld (ix),'F'	

adcan:
	inc ix
	ld (ix),'P' ; Test 92
	scf 
	ld a,$95
	adc a,$23
	cp $b9
	jp z,suban
adcanfail:
	ld (ix),'F'	

; 0xd0
suban:
	inc ix
	ld (ix),'P' ; Test 93
	scf
	ld a,$99
	sub A,$34
	cp $65
	jp z,sbcan
subanfail:
	ld (ix),'F'	

sbcan:
	inc ix
	ld (ix),'P' ; Test 94
	or a ; clear carry flag
	ld a,$f9
	sbc A,$45
	cp $b4
	jp z,andan
sbcanfail:
	ld (ix),'F'	

; 0xe0
andan:
	inc ix
	ld (ix),'P' ; Test 95
	ld a,$f0
	and A,$56
	cp $50
	jp z,xoran
andanfail:
	ld (ix),'F'	

xoran:
	inc ix
	ld (ix),'P' ; Test 96
	ld a,$91
	xor A,$67
	cp $f6
	jp z,oran
xoranfail:
	ld (ix),'F'	

; 0xf0
oran:
	inc ix
	ld (ix),'P' ; Test 97
	ld a,$17
	or  A,$78
	cp $7f 
	jp z,cpan
oranfail:
	ld (ix),'F'	

cpan:
	inc ix
	ld (ix),'P' ; Test 98
	ld a,$99
	cp  A,$89
	jp z,cpanfail
	cp a,$99
	jp z,rlcb
cpanfail:
	ld (ix),'F'

; Bit Instructions - 0xcb prefix
; 0xcb00 -- all affect flags
rlcb:
	inc ix
	ld (ix),'P' ; Test 99
	ld b,$01
	rlc B   
	ld a,$02
	cp b
	jp z,rlcc
rlcbfail:
	ld (ix),'F'	

rlcc:
	inc ix
	ld (ix),'P' ; Test 100
	ld c,$80
	rlc C
	ld a,$01
	cp c
	jp z,rlcd
rlccfail:
	ld (ix),'F'	

rlcd:
	inc ix
	ld (ix),'P' ; Test 101
	scf
	ld d,$78
	rlc D
	ld a,$f0
	cp d
	jp z,rlce
rlcdfail:
	ld (ix),'F'	

rlce:
	inc ix
	ld (ix),'P' ; Test 102
	ld e,$fe
	rlc E  
	ld a,$fd
	cp e
	jp z,rlch
rlcefail:
	ld (ix),'F'	

rlch:
	inc ix
	ld (ix),'P' ; Test 103
	ld h,$55
	rlc H  
	ld a,$aa
	cp h
	jp z,rlcl
rlchfail:
	ld (ix),'F'	

rlcl:
	inc ix
	ld (ix),'P' ; Test 104
	ld l,$aa
	rlc L 
	ld a,$55
	cp l
	jp z,rlcmhl     
rlclfail:
	ld (ix),'F'	

rlcmhl:
	inc ix
	ld (ix),'P' ; Test 105
	ld hl,results-1 ; contains $3a
	rlc (HL)
	ld a,$74
	cp (hl)
	jp nz,rlcmhlfail
	ld (hl),':'  ; change it back to a colon
	jr rlcav
rlcmhlfail:
	ld (ix),'F'	

rlcav:
	inc ix
	ld (ix),'P' ; Test 106
	ld a,$71
	rlc A
	jr c,rlcavfail
	cp $e2
	jp nz,rlcavfail
	rlc a 
	jr nc,rlcavfail 
	jp p,rlcavfail
	jr z,rlcavfail
	jp po,rlcavfail
	cp $c5
	jr nz,rlcavfail
	ld a,$00
	rlc a
	jr nz,rlcavfail
	ld a,$88
	rlc a
	push af
	pop de
	ld a,$05
	cp e
	jr z,rrcb
rlcavfail:
	ld (ix),'F'	

rrcb:
	inc ix
	ld (ix),'P' ; Test 107
	ld b,$99
	rrc b
	jp nc,rrcbfail
	jp z,rrcbfail
	jp p,rrcbfail
	jp po,rrcbfail
	ld a,$cc
	cp b
	jp nz,rrcbfail
	ld b,$00
	rrc b 
	jp c,rrcbfail
	jp nz,rrcbfail
	jp m,rrcbfail
	ld b,$01
	rrc b 
	jp po,rrcc
rrcbfail:
	ld (ix),'F'	

rrcc:
	inc ix
	ld (ix),'P' ; Test 108
	ld c,$10
	rrc C
	ld a,$08
	cp c 
	jp z,rrcd
rrccfail:
	ld (ix),'F'	

rrcd:
	inc ix
	ld (ix),'P' ; Test 109
	ld d,$f0
	rrc D
	ld a,$78
	cp d 
	jp z,rrce
rrcdfail:
	ld (ix),'F'	

rrce:
	inc ix
	ld (ix),'P' ; Test 110
	ld e,$55
	rrc E
	ld a,$aa
	cp e 
	jp z,rrch
rrcefail:
	ld (ix),'F'	

rrch:
	inc ix
	ld (ix),'P' ; Test 111
	ld h,$aa
	rrc H
	ld a,$55
	cp h 
	jp z,rrcl
rrchfail:
	ld (ix),'F'	

rrcl:
	inc ix
	ld (ix),'P' ; Test 112
	ld l,$12
	rrc L
	ld a,$09
	cp l
	jp z,rrcmhl
rrclfail:
	ld (ix),'F'	

rrcmhl:
	inc ix
	ld (ix),'P' ; Test 113
	ld hl,results-1 ; contains ':' (0x3a)
	rrc (HL)
	ld a,$1d
	cp (hl)
	jp nz,rrcmhlfail
	ld (hl),':' ; change it back to ':'
	jr rrcav
rrcmhlfail:
	ld (ix),'F'	

rrcav:
	inc ix
	ld (ix),'P' ; Test 114
	ld a,$7f
	rrc A
	cp $bf
	jp z,rlb1
rrcavfail:
	ld (ix),'F'	

; 0xcb10  -- all affect flags
rlb1:
	inc ix
	ld (ix),'P' ; Test 115
	ld b,$81
	rl  B
	jp nc,rlb1fail
	ld a,$02
	cp b
	jp nz,rlb1fail
	ld b,$81
	rl b 
	rl b
	jp c,rlb1fail
	ld a,$05
	cp b
	jp z,rlc1
rlb1fail:
	ld (ix),'F'	

rlc1:
	inc ix
	ld (ix),'P' ; Test 116
	ld c,$aa
	rl  C
	ld a,$54
	cp c 
	jp z,rld1
rlc1fail:
	ld (ix),'F'	

rld1:
	inc ix
	ld (ix),'P' ; Test 117
	ld d,$ff
	rl  D
	ld a,$fe 
	cp d 
	jp z,rle1
rld1fail: 
	ld (ix),'F'	

rle1:
	inc ix
	ld (ix),'P' ; Test 118
	scf 
	ld e,$11
	rl  E
	ld a,$23
	cp e 
	jp z,rlh1 
rle1fail:
	ld (ix),'F'	

rlh1:
	inc ix
	ld (ix),'P' ; Test 119
	scf 
	ld h,$00 
	rl  H
	ld a,$01
	cp h 
	jp z,rll1
rlh1fail:
	ld (ix),'F'	

rll1:
	inc ix
	ld (ix),'P' ; Test 120
	ld l,$00
	rl  L
	jp z,rlmhl1
rll1fail:
	ld (ix),'F'	

rlmhl1:
	inc ix
	ld (ix),'P' ; Test 121
	ld hl,results-1 ; contains ':' (0x3a)
	rl  (HL)
	ld a,$74
	cp (hl)
	jp nz,rlmhl1fail
	ld (hl),':' ; restore value
	jr rla1v
rlmhl1fail:
	ld (ix),'F'	

rla1v:
	inc ix
	ld (ix),'P' ; Test 122
	ld a,$80
	rl  A
	jp nc,rla1vfail 
	jp z,rrb1
rla1vfail:
	ld (ix),'F'	

rrb1:
	inc ix
	ld (ix),'P' ; Test 123
	or a  ; clear the carry flag
	ld b,$01
	rr  B
	jp nz,rrb1fail
	jp nc,rrb1fail
	jp rrc1
rrb1fail:
	ld (ix),'F'	

rrc1:
	inc ix
	ld (ix),'P' ; Test 124
	or a 	; clear the carry flag
	ld c,$aa
	rr  C
	ld a,$55
	cp c 
	jp z,rrd1
rrc1fail:
	ld (ix),'F'	

rrd1:
	inc ix
	ld (ix),'P' ; Test 125
	scf 
	ld d,$00
	rr  D
	ld a,$80
	cp d 
	jp z,rre1 
rrd1fail:
	ld (ix),'F'	

rre1:
	inc ix
	ld (ix),'P' ; Test 126
	ld e,$ff 
	rr  E
	ld a,$7f 
	cp e 
	jp z,rrh1 
rre1fail:
	ld (ix),'F'	

rrh1:
	inc ix
	ld (ix),'P' ; Test 127
	ld h,$02
	rr  H
	ld a,$01
	cp h
	jp z,rrl1
rrh1fail:
	ld (ix),'F'	

rrl1:
	inc ix
	ld (ix),'P' ; Test 128
	ld l,$81
	rr  L
	jp nc,rrl1fail
	ld a,$40
	cp l 
	jp z,rrmhl1
rrl1fail:
	ld (ix),'F'	

rrmhl1:
	inc ix
	ld (ix),'P' ; Test 129
	ld hl,results-1 ; contains ':' (0x3a)
	rr  (HL)
	ld a,$1d
	cp (hl)
	jp nz,rrmhl1fail
	ld (hl),':' ; restore previous value
	jr rra1v
rrmhl1fail:
	ld (ix),'F'	

rra1v:
	inc ix
	ld (ix),'P' ; Test 130
	ld a,$42
	rr  A
	cp $21
	jp z,slab
rra1vfail:
	ld (ix),'F'	

; 0xcb20 -- all affect flags
slab:
	inc ix
	ld (ix),'P' ; Test 131
	ld b,$80
	sla B
	jp nz,slabfail
	jp nc,slabfail
	ld a,$00
	cp B
	jp z,slac
slabfail:
	ld (ix),'F'	

slac:
	inc ix
	ld (ix),'P' ; Test 132
	ld c,$ff
	sla C
	ld a,$fe
	cp C
	jp z,slad
slacfail:
	ld (ix),'F'	

slad:
	inc ix
	ld (ix),'P' ; Test 133
	scf
	ld d,$00
	sla D
	jp z,slae
sladfail:
	ld (ix),'F'	

slae:
	inc ix
	ld (ix),'P' ; Test 134
	ld e,$55
	sla E
	ld a,$aa
	cp E
	jp z,slah
slaefail:
	ld (ix),'F'	

slah:
	inc ix
	ld (ix),'P' ; Test 135
	ld h,$aa 
	sla H
	ld a,$54
	cp h 
	jp z,slal
slahfail:
	ld (ix),'F'	

slal:
	inc ix
	ld (ix),'P' ; Test 136
	ld l,$02
	sla L
	ld a,$04
	cp l 
	jp z,slamhl
slalfail:
	ld (ix),'F'	

slamhl:
	inc ix
	ld (ix),'P' ; Test 137
	ld hl,results-2 ; contains 's' (0x73)
	sla (HL)
	ld a,$e6
	cp (hl)
	jp nz,slamhlfail
	ld (hl),'s' 	; restore previous value 
	jr slaa 
slamhlfail:
	ld (ix),'F'	

slaa:
	inc ix
	ld (ix),'P' ; Test 138
	ld a,$77
	sla A
	cp $ee
	jp z,srab
slaafail:
	ld (ix),'F'	

srab:
	inc ix
	ld (ix),'P' ; Test 139
	ld b,$ff
	sra B
	jp nc,srabfail
	jp z,srabfail
	jp po,srabfail
	ld a,$ff
	cp b 
	jp z,srac
srabfail:
	ld (ix),'F'	

srac:
	inc ix
	ld (ix),'P' ; Test 140
	ld c,$7f
	sra C
	ld a,$3f
	cp C
	jp z,srad
sracfail:
	ld (ix),'F'	

srad:
	inc ix
	ld (ix),'P' ; Test 141
	ld d,$40
	sra D
	ld a,$20
	cp d 
	jp z,srae 
sradfail:
	ld (ix),'F'	

srae:
	inc ix
	ld (ix),'P' ; Test 142
	ld e,$20
	sra E
	ld a,$10
	cp E
	jp z,srah
sraefail:
	ld (ix),'F'	

srah:
	inc ix
	ld (ix),'P' ; Test 143
	ld h,$10
	sra H
	ld a,$08
	cp H
	jp z,sral
srahfail:
	ld (ix),'F'	

sral:
	inc ix
	ld (ix),'P' ; Test 144
	ld l,$08
	sra L
	ld a,$04
	cp L
	jp z,sramhl
sralfail:
	ld (ix),'F'	

sramhl:
	inc ix
	ld (ix),'P' ; Test 145
	ld hl,results-3	; contains 't' (0x74)
	sra (HL)
	ld a,$3a
	cp (hl)
	jp nz,sramhlfail
	ld (hl),'t'		; restore previous
	jr sraa
sramhlfail:
	ld (ix),'F'	

sraa:
	inc ix
	ld (ix),'P' ; Test 146
	ld a,$04
	sra A
	cp $02
	jp z,sllb
sraafail:
	ld (ix),'F'	

; 0xcb30 -- all affect flags
sllb:
	inc ix
	ld (ix),'P' ; Test 147
	ld b,$80
	sll B
	jp z,sllbfail
	jp nc,sllbfail
	jr sllc
sllbfail:
	ld (ix),'F'	

sllc:
	inc ix
	ld (ix),'P' ; Test 148
	ld c,$ff
	sll C
	ld a,$ff
	cp C
	jp z,slld
sllcfail:
	ld (ix),'F'	

slld:
	inc ix
	ld (ix),'P' ; Test 149
	ld d,$55
	sll D
	ld a,$ab 
	cp D
	jp z,slle
slldfail:
	ld (ix),'F'	

slle:
	inc ix
	ld (ix),'P' ; Test 150
	ld e,$01
	sll E
	ld a,$03
	cp e 
	jp z,sllh 
sllefail:
	ld (ix),'F'	

sllh:
	inc ix
	ld (ix),'P' ; Test 151
	ld h,$02
	sll H
	ld a,$05
	cp h
	jp z,slll
sllhfail:
	ld (ix),'F'	

slll:
	inc ix
	ld (ix),'P' ; Test 152
	ld l,$07
	sll L
	ld a,$0f 
	cp l 
	jp sllmhl
slllfail:
	ld (ix),'F'	

sllmhl:
	inc ix
	ld (ix),'P' ; Test 153
	ld hl,results-1	; contains ':' (0x3a)
	sll (HL)
	ld a,$75
	cp (hl)
	jp nz,sllmhlfail
	ld (hl),':'		; restore previous value
	jr slla
sllmhlfail:
	ld (ix),'F'	

slla:
	inc ix
	ld (ix),'P' ; Test 154
	ld a,$10
	sll A
	cp $21
	jp z,srlb
sllafail:
	ld (ix),'F'	

srlb:
	inc ix
	ld (ix),'P' ; Test 155
	ld b,$01
	srl B
	jp nz,srlbfail
	jp nc,srlbfail
	jr srlc
srlbfail:
	ld (ix),'F'	

srlc:
	inc ix
	ld (ix),'P' ; Test 156
	ld c,$ff
	srl C
	ld a,$7f
	cp c 
	jp z,srld
srlcfail:
	ld (ix),'F'	

srld:
	inc ix
	ld (ix),'P' ; Test 157
	ld d,$02
	srl D
	jp c,srldfail
	ld a,$01
	cp D
	jp z,srle
srldfail:
	ld (ix),'F'	

srle:
	inc ix
	ld (ix),'P' ; Test 158
	ld e,$aa
	srl E
	ld a,$55
	cp E
	jp z,srlh
srlefail:
	ld (ix),'F'	

srlh:
	inc ix
	ld (ix),'P' ; Test 159
	ld h,$10
	srl H
	ld a,$08
	cp H
	jp z,srll
srlhfail:
	ld (ix),'F'	

srll:
	inc ix
	ld (ix),'P' ; Test 160
	ld l,$01
	srl L
	jp z,srlmhl
srllfail:
	ld (ix),'F'	

srlmhl:
	inc ix
	ld (ix),'P' ; Test 161
	ld hl,results-1	; contains ':' (0x3a)
	srl (HL)
	ld a,$1d
	cp (hl)
	jp nz,srlmhlfail
	ld (hl),':' 	; restore previous value
	jr srla
srlmhlfail:
	ld (ix),'F'	

srla:
	inc ix
	ld (ix),'P' ; Test 162
	ld a,$7f
	srl A
	cp $3f 
	jp z,bit0b
srlafail:
	ld (ix),'F'	

; 0xcb40 -- all affect flags
bit0b:
	inc ix
	ld (ix),'P' ; Test 163
	ld b,$01
	bit 0,B
	jp z,bit0bfail
	ld b,$fe
	bit 0,B
	jp nz,bit0bfail
	ld b,$ff
	bit 0,b
	jp z,bit0bfail
	ld b,$00
	bit 0,b
	jp z,bit0c
bit0bfail:
	ld (ix),'F'	

bit0c:
	inc ix
	ld (ix),'P' ; Test 164
	ld c,$01
	bit 0,C
	jp z,bit0cfail
	ld c,$fe
	bit 0,C
	jp z,bit0d
bit0cfail:
	ld (ix),'F'	

bit0d:
	inc ix
	ld (ix),'P' ; Test 165
	ld d,$01
	bit 0,d
	jp z,bit0dfail
	ld d,$fe
	bit 0,d
	jp z,bit0e
bit0dfail:
	ld (ix),'F'	

bit0e:
	inc ix
	ld (ix),'P' ; Test 166
	ld e,$01
	bit 0,e
	jp z,bit0efail
	ld e,$fe
	bit 0,e
	jp z,bit0h
bit0efail:
	ld (ix),'F'	

bit0h:
	inc ix
	ld (ix),'P' ; Test 167
	ld h,$01
	bit 0,h
	jp z,bit0hfail
	ld h,$fe
	bit 0,h
	jp z,bit0l
bit0hfail:
	ld (ix),'F'	

bit0l:
	inc ix
	ld (ix),'P' ; Test 168
	ld l,$01
	bit 0,l
	jp z,bit0lfail
	ld l,$fe
	bit 0,l
	jp z,bit0mhl
bit0lfail:
	ld (ix),'F'	

bit0mhl:
	inc ix
	ld (ix),'P' ; Test 169
	ld hl,testing_area
	ld (hl),$01
	bit 0,(hl)
	jp z,bit0mhlfail
	ld (hl),$fe
	bit 0,(hl)
	jp z,bit0a
bit0mhlfail:
	ld (ix),'F'	

bit0a:
	inc ix
	ld (ix),'P' ; Test 170
	ld a,$01
	bit 0,a
	jp z,bit0afail
	ld a,$fe
	bit 0,a
	jp z,bit1b
bit0afail:
	ld (ix),'F'	

bit1b:
	inc ix
	ld (ix),'P' ; Test 171
	ld b,$02
	bit 1,B
	jp z,bit1bfail
	ld b,$fd 
	bit 1,b 
	jp z,bit1c
bit1bfail:
	ld (ix),'F'	

bit1c:
	inc ix
	ld (ix),'P' ; Test 172
	ld c,$02
	bit 1,C
	jp z,bit1cfail
	ld c,$fd
	bit 1,C
	jp z,bit1d
bit1cfail:
	ld (ix),'F'	

bit1d:
	inc ix
	ld (ix),'P' ; Test 173
	ld d,$02
	bit 1,D
	jp z,bit1dfail
	ld d,$fd
	bit 1,D
	jp z,bit1e
bit1dfail:
	ld (ix),'F'	

bit1e:
	inc ix
	ld (ix),'P' ; Test 174
	ld e,$02
	bit 1,E
	jp z,bit1efail
	ld e,$fd
	bit 1,E
	jp z,bit1h
bit1efail:
	ld (ix),'F'	

bit1h:
	inc ix
	ld (ix),'P' ; Test 175
	ld h,$02
	bit 1,H
	jp z,bit1hfail
	ld h,$fd
	bit 1,H
	jp z,bit1l
bit1hfail:
	ld (ix),'F'	

bit1l:
	inc ix
	ld (ix),'P' ; Test 176
	ld l,$02
	bit 1,L
	jp z,bit1lfail
	ld l,$fd
	bit 1,L
	jp z,bit1mhl
bit1lfail:
	ld (ix),'F'	

bit1mhl:
	inc ix
	ld (ix),'P' ; Test 177
	ld hl,testing_area
	ld (hl),$02
	bit 1,(HL)
	jp z,bit1mhlfail
	ld (hl),$fd
	bit 1,(hl)
	jp z,bit1a
bit1mhlfail:
	ld (ix),'F'	

bit1a:
	inc ix
	ld (ix),'P' ; Test 178
	ld a,$02
	bit 1,A
	jp z,bit1afail
	ld a,$fd
	bit 1,A
	jp z,bit2b
bit1afail:
	ld (ix),'F'	

; 0xcb50 -- all affect flags
bit2b:
	inc ix
	ld (ix),'P' ; Test 179
	ld b,$04
	bit 2,B
	jp z,bit2bfail
	ld b,$fb
	bit 2,B
	jp nz,bit2bfail
	ld b,$ff
	bit 2,b
	jp z,bit2bfail
	ld b,$00
	bit 2,b
	jp z,bit2c
bit2bfail:
	ld (ix),'F'	

bit2c:
	inc ix
	ld (ix),'P' ; Test 180
	ld c,$04
	bit 2,C
	jp z,bit2cfail
	ld c,$fb
	bit 2,C
	jp z,bit2d
bit2cfail:
	ld (ix),'F'	

bit2d:
	inc ix
	ld (ix),'P' ; Test 181
	ld d,$04
	bit 2,d
	jp z,bit2dfail
	ld d,$fb
	bit 2,d
	jp z,bit2e
bit2dfail:
	ld (ix),'F'	

bit2e:
	inc ix
	ld (ix),'P' ; Test 182
	ld e,$04
	bit 2,e
	jp z,bit2efail
	ld e,$fb
	bit 2,e
	jp z,bit2h
bit2efail:
	ld (ix),'F'	

bit2h:
	inc ix
	ld (ix),'P' ; Test 183
	ld h,$04
	bit 2,h
	jp z,bit2hfail
	ld h,$fb
	bit 2,h
	jp z,bit2l
bit2hfail:
	ld (ix),'F'	

bit2l:
	inc ix
	ld (ix),'P' ; Test 184
	ld l,$04
	bit 2,l
	jp z,bit2lfail
	ld l,$fb
	bit 2,l
	jp z,bit2mhl
bit2lfail:
	ld (ix),'F'	

bit2mhl:
	inc ix
	ld (ix),'P' ; Test 185
	ld hl,testing_area
	ld (hl),$04
	bit 2,(hl)
	jp z,bit2mhlfail
	ld (hl),$fb
	bit 2,(hl)
	jp z,bit2a
bit2mhlfail:
	ld (ix),'F'	

bit2a:
	inc ix
	ld (ix),'P' ; Test 186
	ld a,$04
	bit 2,a
	jp z,bit2afail
	ld a,$fb
	bit 2,a
	jp z,bit3b
bit2afail:
	ld (ix),'F'	

bit3b:
	inc ix
	ld (ix),'P' ; Test 187
	ld b,$08
	bit 3,B
	jp z,bit3bfail
	ld b,$f7
	bit 3,b 
	jp z,bit3c
bit3bfail:
	ld (ix),'F'	

bit3c:
	inc ix
	ld (ix),'P' ; Test 188
	ld c,$08
	bit 3,C
	jp z,bit3cfail
	ld c,$f7
	bit 3,C
	jp z,bit3d
bit3cfail:
	ld (ix),'F'	

bit3d:
	inc ix
	ld (ix),'P' ; Test 189
	ld d,$08
	bit 3,D
	jp z,bit3dfail
	ld d,$f7
	bit 3,D
	jp z,bit3e
bit3dfail:
	ld (ix),'F'	

bit3e:
	inc ix
	ld (ix),'P' ; Test 190
	ld e,$08
	bit 3,E
	jp z,bit3efail
	ld e,$f7
	bit 3,E
	jp z,bit3h
bit3efail:
	ld (ix),'F'	

bit3h:
	inc ix
	ld (ix),'P' ; Test 191
	ld h,$08
	bit 3,H
	jp z,bit3hfail
	ld h,$f7
	bit 3,H
	jp z,bit3l
bit3hfail:
	ld (ix),'F'	

bit3l:
	inc ix
	ld (ix),'P' ; Test 192
	ld l,$08
	bit 3,L
	jp z,bit3lfail
	ld l,$f7
	bit 3,L
	jp z,bit3mhl
bit3lfail:
	ld (ix),'F'	

bit3mhl:
	inc ix
	ld (ix),'P' ; Test 193
	ld hl,testing_area
	ld (hl),$08
	bit 3,(HL)
	jp z,bit3mhlfail
	ld (hl),$f7
	bit 3,(hl)
	jp z,bit3a
bit3mhlfail:
	ld (ix),'F'	

bit3a:
	inc ix
	ld (ix),'P' ; Test 194
	ld a,$08
	bit 3,A
	jp z,bit3afail
	ld a,$f7
	bit 3,A
	jp z,bit4b
bit3afail:
	ld (ix),'F'	

; 0xcb60 -- all affect flags
bit4b:
	inc ix
	ld (ix),'P' ; Test 195
	ld b,$10
	bit 4,B
	jp z,bit4bfail
	ld b,$ef
	bit 4,B
	jp nz,bit4bfail
	ld b,$ff
	bit 4,b
	jp z,bit4bfail
	ld b,$00
	bit 4,b
	jp z,bit4c
bit4bfail:
	ld (ix),'F'	

bit4c:
	inc ix
	ld (ix),'P' ; Test 196
	ld c,$10
	bit 4,C
	jp z,bit4cfail
	ld c,$ef
	bit 4,C
	jp z,bit4d
bit4cfail:
	ld (ix),'F'	

bit4d:
	inc ix
	ld (ix),'P' ; Test 197
	ld d,$10
	bit 4,d
	jp z,bit4dfail
	ld d,$ef
	bit 4,d
	jp z,bit4e
bit4dfail:
	ld (ix),'F'	

bit4e:
	inc ix
	ld (ix),'P' ; Test 198
	ld e,$10
	bit 4,e
	jp z,bit4efail
	ld e,$ef
	bit 4,e
	jp z,bit4h
bit4efail:
	ld (ix),'F'	

bit4h:
	inc ix
	ld (ix),'P' ; Test 199
	ld h,$10
	bit 4,h
	jp z,bit4hfail
	ld h,$ef
	bit 4,h
	jp z,bit4l
bit4hfail:
	ld (ix),'F'	

bit4l:
	inc ix
	ld (ix),'P' ; Test 200
	ld l,$10
	bit 4,l
	jp z,bit4lfail
	ld l,$ef
	bit 4,l
	jp z,bit4mhl
bit4lfail:
	ld (ix),'F'	

bit4mhl:
	inc ix
	ld (ix),'P' ; Test 201
	ld hl,testing_area
	ld (hl),$10
	bit 4,(hl)
	jp z,bit4mhlfail
	ld (hl),$ef
	bit 4,(hl)
	jp z,bit4a
bit4mhlfail:
	ld (ix),'F'	

bit4a:
	inc ix
	ld (ix),'P' ; Test 202
	ld a,$10
	bit 4,a
	jp z,bit4afail
	ld a,$ef
	bit 4,a
	jp z,bit5b
bit4afail:
	ld (ix),'F'	

bit5b:
	inc ix
	ld (ix),'P' ; Test 203
	ld b,$20
	bit 5,B
	jp z,bit5bfail
	ld b,$df
	bit 5,b 
	jp z,bit5c
bit5bfail:
	ld (ix),'F'	

bit5c:
	inc ix
	ld (ix),'P' ; Test 204
	ld c,$20
	bit 5,C
	jp z,bit5cfail
	ld c,$df
	bit 5,C
	jp z,bit5d
bit5cfail:
	ld (ix),'F'	

bit5d:
	inc ix
	ld (ix),'P' ; Test 205
	ld d,$20
	bit 5,D
	jp z,bit5dfail
	ld d,$df
	bit 5,D
	jp z,bit5e
bit5dfail:
	ld (ix),'F'	

bit5e:
	inc ix
	ld (ix),'P' ; Test 206
	ld e,$20
	bit 5,E
	jp z,bit1efail
	ld e,$df
	bit 5,E
	jp z,bit5h
bit5efail:
	ld (ix),'F'	

bit5h:
	inc ix
	ld (ix),'P' ; Test 207
	ld h,$20
	bit 5,H
	jp z,bit5hfail
	ld h,$df
	bit 5,H
	jp z,bit5l
bit5hfail:
	ld (ix),'F'	

bit5l:
	inc ix
	ld (ix),'P' ; Test 208
	ld l,$20
	bit 5,L
	jp z,bit5lfail
	ld l,$df
	bit 5,L
	jp z,bit5mhl
bit5lfail:
	ld (ix),'F'	

bit5mhl:
	inc ix
	ld (ix),'P' ; Test 209
	ld hl,testing_area
	ld (hl),$20
	bit 5,(HL)
	jp z,bit5mhlfail
	ld (hl),$df
	bit 5,(hl)
	jp z,bit5a
bit5mhlfail:
	ld (ix),'F'	

bit5a:
	inc ix
	ld (ix),'P' ; Test 210
	ld a,$20
	bit 5,A
	jp z,bit5afail
	ld a,$df
	bit 5,A
	jp z,bit6b
bit5afail:
	ld (ix),'F'	

; 0xcb70 -- all affect flags
bit6b:
	inc ix
	ld (ix),'P' ; Test 211
	ld b,$40
	bit 6,B
	jp z,bit6bfail
	ld b,$bf
	bit 6,B
	jp nz,bit6bfail
	ld b,$ff
	bit 6,b
	jp z,bit6bfail
	ld b,$00
	bit 6,b
	jp z,bit6c
bit6bfail:
	ld (ix),'F'	

bit6c:
	inc ix
	ld (ix),'P' ; Test 212
	ld c,$40
	bit 6,C
	jp z,bit6cfail
	ld c,$bf
	bit 6,C
	jp z,bit6d
bit6cfail:
	ld (ix),'F'	

bit6d:
	inc ix
	ld (ix),'P' ; Test 213
	ld d,$40
	bit 6,d
	jp z,bit6dfail
	ld d,$bf
	bit 6,d
	jp z,bit6e
bit6dfail:
	ld (ix),'F'	

bit6e:
	inc ix
	ld (ix),'P' ; Test 214
	ld e,$40
	bit 6,e
	jp z,bit6efail
	ld e,$bf
	bit 6,e
	jp z,bit6h
bit6efail:
	ld (ix),'F'	

bit6h:
	inc ix
	ld (ix),'P' ; Test 215
	ld h,$40
	bit 6,h
	jp z,bit6hfail
	ld h,$bf
	bit 6,h
	jp z,bit6l
bit6hfail:
	ld (ix),'F'	

bit6l:
	inc ix
	ld (ix),'P' ; Test 216
	ld l,$40
	bit 6,l
	jp z,bit6lfail
	ld l,$bf
	bit 6,l
	jp z,bit6mhl
bit6lfail:
	ld (ix),'F'	

bit6mhl:
	inc ix
	ld (ix),'P' ; Test 217
	ld hl,testing_area
	ld (hl),$40
	bit 6,(hl)
	jp z,bit6mhlfail
	ld (hl),$bf
	bit 6,(hl)
	jp z,bit6a
bit6mhlfail:
	ld (ix),'F'	

bit6a:
	inc ix
	ld (ix),'P' ; Test 218
	ld a,$40
	bit 6,a
	jp z,bit6afail
	ld a,$bf
	bit 6,a
	jp z,bit7b
bit6afail:
	ld (ix),'F'	

bit7b:
	inc ix
	ld (ix),'P' ; Test 219
	ld b,$80
	bit 7,B
	jp z,bit7bfail
	ld b,$7f
	bit 7,b 
	jp z,bit7c
bit7bfail:
	ld (ix),'F'	

bit7c:
	inc ix
	ld (ix),'P' ; Test 220
	ld c,$80
	bit 7,C
	jp z,bit7cfail
	ld c,$7f
	bit 7,C
	jp z,bit7d
bit7cfail:
	ld (ix),'F'	

bit7d:
	inc ix
	ld (ix),'P' ; Test 221
	ld d,$80
	bit 7,D
	jp z,bit7dfail
	ld d,$7f
	bit 7,D
	jp z,bit7e
bit7dfail:
	ld (ix),'F'	

bit7e:
	inc ix
	ld (ix),'P' ; Test 222
	ld e,$80
	bit 7,E
	jp z,bit7efail
	ld e,$7f
	bit 7,E
	jp z,bit7h
bit7efail:
	ld (ix),'F'	

bit7h:
	inc ix
	ld (ix),'P' ; Test 223
	ld h,$80
	bit 7,H
	jp z,bit7hfail
	ld h,$7f
	bit 7,H
	jp z,bit7l
bit7hfail:
	ld (ix),'F'	

bit7l:
	inc ix
	ld (ix),'P' ; Test 224
	ld l,$80
	bit 7,L
	jp z,bit7lfail
	ld l,$7f
	bit 7,L
	jp z,bit7mhl
bit7lfail:
	ld (ix),'F'	

bit7mhl:
	inc ix
	ld (ix),'P' ; Test 225
	ld hl,testing_area
	ld (hl),$80
	bit 7,(HL)
	jp z,bit7mhlfail
	ld (hl),$7f
	bit 7,(hl)
	jp z,bit7a
bit7mhlfail:
	ld (ix),'F'	

bit7a:
	inc ix
	ld (ix),'P' ; Test 226
	ld a,$80
	bit 7,A
	jp z,bit7afail
	ld a,$7f
	bit 7,A
	jp z,inbmc
bit7afail:
	ld (ix),'F'	

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
inbmc:
	inc ix
	ld (ix),'P' ; Test 227
	ld c,$96		; port $96 contains 0x69
	in  B,(C)
	jp po,inbmcfail
	ld a,0x69
	cp B
	jp nz,inbmcfail
	ld c,$97		; port $97 contains 0x68
	in  B,(C)
	jp pe,inbmcfail
	ld a,$68
	cp b
	jp nz,inbmcfail
	ld c,$98		; port $98 contains 0x80
	in  B,(C)
	jp p,inbmcfail
	ld a,$80
	cp b
	jp nz,inbmcfail
	ld c,$99		; port $99 contains 0x00
	in  B,(C)
	jp z,sbchlbc
inbmcfail:
	ld (ix),'F'	

sbchlbc:
	inc ix
	ld (ix),'P' ; Test 228
	ld hl,$ffff
	ld bc,$0001
	sbc HL,BC
	jp p,sbchlbcfail
	ld hl,$7fff
	ld bc,$7ffe
	sbc hl,bc
	jp m,sbchlbcfail
	or a	; clear carry flag
	ld hl,$8000
	ld bc,$8000
	sbc hl,bc
	jp nz,sbchlbcfail
	scf
	ld hl,$8001
	ld bc,$8000
	sbc hl,bc
	jp nz,sbchlbcfail
	or a	; clear carry flag
	ld hl,$8000
	ld bc,$0fff 
	sbc hl,bc 
	push af
	pop de
	ld a,$16	; check for H, N, P/V flags set
	cp e
	jp nz,sbchlbcfail
	ld hl,$000f
	ld bc,$0001
	sbc hl,bc
	push af
	pop de
	ld a,$02	; check for H flag clear and N flag set
	cp E
	jp nz,sbchlbcfail
	ld hl,$7fff
	ld bc,$8000
	sbc hl,bc
	push af
	pop de
	ld a,$87	; check for S, PV, N, C flags set
	cp e
	jp z,neg1
sbchlbcfail: 
	ld (ix),'F'	

neg1:
	inc ix
	ld (ix),'P' ; Test 229
	ld a,$00
	neg
	jp nz,neg1fail
	jp m,neg1fail
	cp $00
	jp nz,neg1fail
	ld a,$22
	neg
	jp z,neg1fail
	jp p,neg1fail
	jp nc,neg1fail
	cp $de
	jp nz,neg1fail
	ld a,$80
	neg ; should set the P/V flag
	jp po,neg1fail ; P/V flag set if 0x80 before neg
	cp $80
	jp nz,neg1fail
	ld a,$08 ; half-carry
	neg
	push af
	pop bc
	ld a,$93 ; S, H, N, C set
	cp C
	jp nz,neg1fail
	ld a,$f8
	cp b
	jp z,incmc
neg1fail:
	ld (ix),'F'	

incmc:
	inc ix
	ld (ix),'P' ; Test 230
	ld c,$96	; contains $69
	in  C,(C)
	ld a,$69
	cp c 
	jp z,adchlbc
incmcfail:
	ld (ix),'F'	

adchlbc:	; flags tested more thoroughly with ADC HL,DE below
	inc ix
	ld (ix),'P' ; Test 231
	scf
	ld hl,$1234
	ld bc,$5555
	adc hl,bc 
	ld a,$67
	cp H
	jp nz,adchlbcfail
	ld a,$8a
	jp z,ldbcmn
adchlbcfail:
	ld (ix),'F'	

ldbcmn:		; does not update flags
	inc ix
	ld (ix),'P' ; Test 232
	ld  BC,(results-2)	; contains 's'(n), ':'(n+1)
	ld a,'s'
	cp C
	jp nz,ldbcmnfail
	ld a,':'
	jp z,indmc
ldbcmnfail:
	ld (ix),'F'	

; 0xed50
indmc:
	inc ix
	ld (ix),'P' ; Test 233
	ld c,$97	; contains 0x68
	in  D,(C)
	ld a,$68
	cp D
	jp z,sbchlde
indmcfail:
	ld (ix),'F'	

sbchlde:
	inc ix
	ld (ix),'P' ; Test 234
	ld HL,$0100
	ld de,$00a0
	sbc HL,DE
	ld a,$00
	cp H
	jp nz,sbchldefail
	ld a,$60
	jp z,ldai
sbchldefail:
	ld (ix),'F'	

ldai:
	inc ix
	ld (ix),'P' ; Test 235
	ld a,$80
	ld i,a 
	ld A,I
	jp p,ldaifail
	jp z,ldaifail
	jp pe,ldaifail	; P/V contains IFF2, which should be zero at this point
	push af
	pop bc
	ld a,$80	; S set, all other flags clear
	cp C
	jp nz,ldaifail
	ld a,$00
	ld i,a 
	ld A,I 
	jp m,ldaifail
	jp nz,ldaifail
	jp pe,ldaifail
	ei 			; sets IFF2
	ld  A,I
	push af
	di			; sets IFF2 to zero
	pop bc 
	ld a,$44
	cp C
	jp z,inemc
ldaifail:
	ld (ix),'F'	

inemc:
	inc ix
	ld (ix),'P' ; Test 236
	ld c,$98	; contains 0x80
	in  E,(C)
	ld a,$80
	cp E
	jp z,adchlde
inemcfail:
	ld (ix),'F'	

adchlde:
	inc ix
	ld (ix),'P' ; Test 237
	ld hl,$4000
	ld de,$3fff
	adc HL,DE
	jp m,adchldefail
	jp z,adchldefail
	jp pe,adchldefail
	jp c,adchldefail
	push af
	pop bc
	ld a,$00
	cp C
	jp nz,adchldefail
	ld hl,$7fff
	ld de,$0001
	adc hl,de
	jp p,adchldefail
	jp z,adchldefail
	jp po,adchldefail
	jp c,adchldefail
	push af
	pop bc
	ld a,$94	; S, H, OV set 
	cp c 
	jp nz,adchldefail
	scf
	ld hl,$ffff
	ld de,$0000
	adc hl,de
	push af
	pop bc
	ld a,$51	; Z, H, C flags
	cp C
	jp z,ldar
adchldefail:
	ld (ix),'F'	

ldar:
	inc ix
	ld (ix),'P' ; Test 238
	ld a,$80	; R will increment a couple times, but OK since msb of R doesn't update
	ld r,a 
	ld a,r
	jp p,ldarfail
	jp z,ldarfail
	jp pe,ldarfail	; P/V contains IFF2, which should be zero at this point
	push af
	pop bc
	ld a,$80	; S set, all other flags clear
	cp C
	jp nz,ldarfail
	ld a,$7e	; R will increment twice between loading R and then back into A
	ld r,a 
	ld A,R
	jp m,ldarfail
	jp nz,ldarfail
	jp pe,ldarfail
	ei 			; sets IFF2
	ld  A,R
	push af
	di			; sets IFF2 to zero
	pop bc 
	ld a,$04	; check if P flag set with IFF2
	cp C
	jp z,inhmc
ldarfail:
	ld (ix),'F'	

; 0xed60
inhmc:
	inc ix
	ld (ix),'P' ; Test 239
	ld c,$96	; contains 'i'
	in  H,(C)
	ld a,'i'
	cp H
	jp z,sbchlhl
inhmcfail:
	ld (ix),'F'	

sbchlhl:
	inc ix
	ld (ix),'P' ; Test 240
	sbc HL,HL
	jp z,rrd2
sbchlhlfail:
	ld (ix),'F'	

rrd2:
	inc ix
	ld (ix),'P' ; Test 241
	ld HL,results-9		; contains '1' (0x31)
	ld A,$56
	rrd
	push af
	cp $51
	jp nz,rrd2fail
	ld a,(hl)
	cp $63
	jp nz,rrd2fail
	pop bc
	ld a,$00
	cp C
	jp nz,rrd2fail
	ld (hl),0
	ld a,0
	rrd
	push af
	jp m,rrd2fail
	jp po,rrd2fail
	jp nz,rrd2fail
	ld a,(hl)
	jp nz,rrd2fail
	ld (hl),$22
	ld a,$81
	rrd
	jp m,inlmc
rrd2fail:
	ld (ix),'F'	

inlmc:
	inc ix
	ld (ix),'P' ; Test 242
	ld c,$96 ; contains 'i' (0x69)
	in  L,(C)
	ld a,$69
	cp l 
	jp z,adchlhl
inlmcfail:
	ld (ix),'F'	

adchlhl:
	inc ix
	ld (ix),'P' ; Test 243
	scf
	ld hl,$1234
	adc HL,HL
	ld bc,$2469
	sbc hl,bc
	jp z,rld2
adchlhlfail:
	ld (ix),'F'	

rld2:
	inc ix
	ld (ix),'P' ; Test 244
	ld hl,results-11
	ld (hl),$ab
	ld a,$cd
	rld
	cp $ca 
	jp nz,rld2fail 
	ld a,(hl)
	cp $bd
	jp z,infmc
rld2fail:
	ld (ix),'F'	

; 0xed70
infmc:
	inc ix
	ld (ix),'P' ; Test 245
	ld c,$98 	; contains $80
	in  f,(C)  		; This is also written as "in (c)" -- only affects flags, input data is discarded
	jp p,infmcfail
	jp z,infmcfail
	jp pe,infmcfail
	ld c,$99	; contains $00
	in f,(c)
	push af
	pop bc
	ld a,$44	; Z and P flags set
	cp c 
	jp z,sbchlsp
infmcfail: 
	ld (ix),'F'	

sbchlsp:
	inc ix
	ld (ix),'P' ; Test 246
	ld hl,$ffff
	sbc HL,SP	; SP should be $fffd
	ld bc,$0002
	or a 		; clear carry flag
	sbc hl,bc
	jp z,inamc
sbchlspfail:
	ld (ix),'F'	

inamc:
	inc ix
	ld (ix),'P' ; Test 247
	ld c,$97
	in  A,(C)
	cp 'h'
	jp z,outmca
inamcfail:
	ld (ix),'F'	

outmca:
;  does not impact flags;
	inc ix
	ld (ix),'P' ; Test 248
	or a 	; clear carry flag
	out (C),A
	jp nc,adchlsp
outmcafail:
	ld (ix),'F'	

adchlsp:
	inc ix
	ld (ix),'P' ; Test 249
	ld hl,$8003
	adc HL,SP
	ld bc,$7fff		; Note that Carry was set from adc 
	sbc hl,bc
	jp z,ldi1
adchlspfail:
	ld (ix),'F'	

; opcodes 0xed80 - 0xed9f undefined
; 0xeda0
ldi1:
	inc ix
	ld (ix),'P' ; Test 250
	ld bc,$0002
	ld hl,results-7		; contains 'e'
	ld de,results-12
	ldi
	jp po,ldi1fail
	ld iy,results-12
	ld a,(iy)
	cp 'e'
	jp nz,ldi1fail
	ldi
	jp po,cpi1
ldi1fail:
	ld (ix),'F'	

cpi1:
	inc ix
	ld (ix),'P' ; Test 251
	ld hl,results-2
	ld a,':'
	ld bc,$0002
	cpi
	jp p,cpi1fail
	jp z,cpi1fail
	jp po,cpi1fail
	cpi
	jp pe,cpi1fail
	jp z,ini1
cpi1fail:
	ld (ix),'F'	

ini1:
	inc ix
	ld (ix),'P' ; Test 252
	ld hl,results-16
	ld iy,results-16
	ld b,2
	ld c,$97
	ini
	jp z,ini1fail
	ld a,'h'
	cp (iy)
	jp nz,ini1fail
	ini
	jp z,outi1
ini1fail:
	ld (ix),'F'	

outi1:
	inc ix
	ld (ix),'P' ; Test 253
	ld hl,results
	ld b,2
	ld c,$12
	outi
	jp z,outi1fail
	ld de,results+2
	outi
	jp nz,outi1fail
	or A	; clear carry flag
	sbc hl,de
	jp z,ldd1 
outi1fail:
	ld (ix),'F'	

ldd1:
	inc ix
	ld (ix),'P' ; Test 254
	ld bc,$0002
	ld hl,results-2		; contains 'e'
	ld de,results-13
	ldd
	jp po,ldd1fail
	ld iy,results-13
	ld a,(iy)
	cp 's'
	jp nz,ldd1fail
	ldd
	jp po,cpd1
ldd1fail:
	ld (ix),'F'	

cpd1:
	inc ix
	ld (ix),'P' ; Test 255
	ld hl,results-2
	ld a,'t'
	ld bc,$0002
	cpd
	jp m,cpd1fail
	jp z,cpd1fail
	jp po,cpd1fail
	cpd
	jp pe,cpd1fail
	jp z,ind1
cpd1fail:
	ld (ix),'F'	

ind1:
	inc ix
	ld (ix),'P' ; Test 256
	ld hl,results-17
	ld iy,results-17
	ld b,2
	ld c,$96
	ind
	jp z,ind1fail
	ld a,'i'
	cp (iy)
	jp nz,ind1fail
	ind
	jp z,outd1
ind1fail:
	ld (ix),'F'	

outd1:
	inc ix
	ld (ix),'P' ; Test 257
	ld hl,results
	ld b,2
	ld c,$14
	outd
	jp z,outd1fail
	ld de,results-2
	outd
	jp nz,outd1fail
	or A	; clear carry flag
	sbc hl,de
	jp z,ldir1
outd1fail:
	ld (ix),'F'	

; 0xedb0
ldir1:
	inc ix
	ld (ix),'P' ; Test 258
	ld bc,$0005
	ld hl,results-8
	ld de,results-24
	ldir
	jp po,cpir1
ldir1fail:
	ld (ix),'F'	

cpir1:
	inc ix
	ld (ix),'P' ; Test 259
	ld bc,$0005
	ld hl,results-8
	ld a,'s'
	cpir
	jp nz,cpir1fail		; Should have found 's', so Z flag set 
	jp po,cpir1fail		; Did not count all the way down, so P flag set
	ld bc,$0005
	ld hl,results-8
	ld a,'x'
	cpir
	jp z,cpir1fail		; Should not find 'x', so Z should be clear
	jp po,inir1			; counted all the way down, so P should be clear
cpir1fail:
	ld (ix),'F'	

inir1:
	inc ix
	ld (ix),'P' ; Test 260
	ld hl,results-32
	ld b,6
	ld c,0x97	; contains 'h'
	inir
	jp z,otir1
irir1fail:
	ld (ix),'F'	

otir1:
	inc ix
	ld (ix),'P' ; Test 261
	ld hl,results-8
	ld b,4
	ld c,$99
	otir
	jp z,lddr1
otir1fail:
	ld (ix),'F'	

lddr1:
	inc ix
	ld (ix),'P' ; Test 262
	ld hl,results-1
	ld de,results-33
	ld bc,$0005
	lddr
	jp po,cpdr1
ldddr1fail:
	ld (ix),'F'	

cpdr1:
	inc ix
	ld (ix),'P' ; Test 263
	ld bc,$0005
	ld hl,results-1
	ld a,'s'
	cpdr
	jp nz,cpdr1fail		; Should have found 's', so Z flag set 
	jp po,cpdr1fail		; Did not count all the way down, so P flag set
	ld bc,$0005
	ld hl,results-1
	ld a,'x'
	cpdr
	jp z,cpdr1fail		; Should not find 'x', so Z should be clear
	jp po,indr1			; counted all the way down, so P should be clear	cpdr
cpdr1fail:
	ld (ix),'F'	

indr1:
	inc ix
	ld (ix),'P' ; Test 264
	ld hl,results-41
	ld b,6
	ld c,0x96	; contains 'i'
	indr
	jp z,otdr1
indr1fail:
	ld (ix),'F'	

otdr1:
	inc ix
	ld (ix),'P' ; Test 265
	ld hl,results-8
	ld b,4
	ld c,$9a
	otdr
	jp z,addixbc
otdr1fail:
	ld (ix),'F'	
	
; opcodes 0xedc0 - 0xedff undefined

; IX instructions - 0xdd prefix
; 0xdd00
addixbc:
; This group of tests uses IX register, so need to use IY as pointer to results area
	ld b,ixh
	ld c,ixl 
	ld iyh,b 
	ld iyl,c
	inc iy
	ld (iy),'P' ; Test 266
	ld ix,$1234
	ld bc,$5432
	add IX,BC	; only H and C flags affected by this opcode
	push af
	pop de
	ld a,ixh
	cp $66
	jp nz,addixbcfail
	ld a,ixl
	cp $66
	jp nz,addixbcfail
	ld a,$11	; H and C bit positions
	and e 		; both should be zero
	jp nz,addixbcfail
	ld ix,$f300
	ld bc,$1d11
	add ix,bc	; should set both H and C bits
	push af
	pop de
	ld a,$10
	and e 
	jp z,addixbcfail
	ld a,$01
	and e 
	jp z,addixbcfail
	jp addixde
addixbcfail:
	ld (iy),'F'	

; 0xdd10
addixde:
	inc iy
	ld (iy),'P' ; Test 267
	scf
	ld ix,$ffff
	ld de,$0001
	add IX,DE
	push af
	pop bc
	ld a,c 
	and $11
	cp $11
	jp z,incixh
addixdefail:
	ld (iy),'F'	

; 0xdd20
incixh:
	inc iy
	ld (iy),'P' ; Test 268
	ld ixh,$ff
	inc IXH
	push af
	pop bc
	ld a,$50
	cp c 
	jp nz,incixhfail
	inc ixh
	jp z,incixhfail
	ld ixh,$7F
	inc ixh
	jp pe,decixh
incixhfail:
	ld (iy),'F'	

decixh:
	inc iy
	ld (iy),'P' ; Test 269
	ld ixh,$00
	dec IXH
	ld a,$ff
	cp ixh
	jp z,addixix
decixhfail:
	ld (iy),'F'	

addixix:
	inc iy
	ld (iy),'P' ; Test 270
	ld ix,$4321
	add IX,IX
	ld a,$86
	cp ixh
	jp nz,addixixfail
	ld a,$42
	jp z,incixl
addixixfail:
	ld (iy),'F'	

incixl:
	inc iy
	ld (iy),'P' ; Test 271
	ld ixl,$7f
	inc IXL   
	ld a,$80
	cp ixl
	jp z,decixl 
incixlfail:
	ld (iy),'F'	

decixl:
	inc iy
	ld (iy),'P' ; Test 272
	ld ixl,$43
	dec IXL
	ld a,$42
	cp ixl
	jp z,incixd
decixlfail:
	ld (iy),'F'	
	
; 0xdd30
incixd:
	inc iy
	ld (iy),'P' ; Test 273
	ld ix,results
	inc (IX - $11)	; contains 'i'
	ld a,(results-$11)
	cp 'j'
	jp z,decixd
incixdfail:
	ld (iy),'F'	

decixd: 
	inc iy
	ld (iy),'P' ; Test 274
	dec (IX - $0f) ; contains 'h'
	ld a,(results-$0f)
	cp 'g'
	jp z,addixsp
decixdfail:
	ld (iy),'F'	

addixsp:
	inc iy
	ld (iy),'P' ; Test 275
	ld ix,$3456	; SP contains $fffd at this point
	add IX,SP
	ld a,$34
	cp ixh
	jp nz,addixspfail
	ld a,$53
	jp z,addaixh
addixspfail:
	ld (iy),'F'	
	
; 0xdd40

; 0xdd50

; 0xdd60

; 0xdd70

; 0xdd80
addaixh:
	inc iy
	ld (iy),'P' ; Test 276
	ld a,$5a 
	ld ixh,$a5
	add  A,IXH
	cp $ff
	jp z,addaixl
addaixhfail:
	ld (iy),'F'	

addaixl:
	inc iy
	ld (iy),'P' ; Test 277
	ld a,$11
	ld ixl,$44
	add  A,IXL
	cp $55
	jp z,addaixd
addaixlfail:
	ld (iy),'F'	

addaixd:
	inc iy
	ld (iy),'P' ; Test 278
	ld ix,results
	ld a,$65
	add  A,(IX-$01) ; contains $3a
	cp $9f
	jp z,adcixh
addaixdfail:
	ld (iy),'F'	

adcixh:
	inc iy
	ld (iy),'P' ; Test 279
	scf
	ld a,$12
	ld ixh,$34
	adc  A,IXH 
	cp $47
	jp z,adcixl
adcixhfail:
	ld (iy),'F'	

adcixl:
	inc iy
	ld (iy),'P' ; Test 280
	or A	; clear carry flag
	ld a,$ff
	ld ixl,$19
	adc  A,IXL 
	cp $18
	jp z,adcixd
adcixlfail:
	ld (iy),'F'	

adcixd:
	inc iy
	ld (iy),'P' ; Test 281
	ld ix,results-8
	ld a,$03
	adc  A,(IX+$04)		; contains 'l'
	cp 'o'
	jp z,subaixh
adcixdfail:
	ld (iy),'F'	

; 0xdd90
subaixh:
	inc iy
	ld (iy),'P' ; Test 282
	ld a,$7F
	ld ixh,$44
	sub  A,IXH
	push af
	cp $3b
	jp nz,subaixhfail
	pop de
	ld a,$02
	jp nz,subaixhfail
	ld a,$01
	ld ixh,$01
	sub a,ixh
	jp nz,subaixhfail
	jp c,subaixhfail
	ld a,$02
	ld ixh,$03
	sub a,ixh
	push af
	jp z,subaixhfail
	jp p,subaixhfail
	jp nc,subaixhfail
	pop de
	ld a,$93
	cp e
	jp z,subaixl
subaixhfail:
	ld (iy),'F'	

subaixl:
	inc iy
	ld (iy),'P' ; Test 283
	scf
	ld a,$44
	ld ixl,$43
	sub  A,IXL
	cp $01
	jp z,subaixd
subaixlfail:
	ld (iy),'F'	

subaixd:
	inc iy
	ld (iy),'P' ; Test 284
	ld ix,results
	ld a,'h'
	sub  A,(IX-$07)	; contains 'e'
	cp $03
	jp z,sbcixh
subaixdfail:
	ld (iy),'F'	

sbcixh:
	inc iy
	ld (iy),'P' ; Test 285
	or A	; clear carry flag
	ld a,$99
	ld ixh,$55
	sbc  A,IXH 
	cp $44
	jp z,sbcixl
sbcaixhfail:
	ld (iy),'F'	

sbcixl:
	inc iy
	ld (iy),'P' ; Test 286
	scf 
	ld a,$f3
	ld ixl,$f2
	sbc  A,IXL 
	jp z,sbcixd
sbcaixlfail:
	ld (iy),'F'	

sbcixd:
	inc iy
	ld (iy),'P' ; Test 287
	ld ix,results-8
	or A		; clear carry flag
	ld a,'S'	; capital 'S'
	sbc  A,(IX+$02)	; contains 's' (lower case)
	cp $e0	; capital letter is $20 less than lower case ($E0 -> -$20)
	jp nz,sbcaixdfail
	scf
	ld a,'z'
	sbc A,(IX+$03) ; contains 'u'
	cp $04	; 'z' - 'u' = 5, but also subtract the carry
	jp z,andaixh
sbcaixdfail:
	ld (iy),'F'	

; 0xdda0
andaixh:
	inc iy
	ld (iy),'P' ; Test 288
	ld a,$ff
	ld ixh,$00
	and a,ixh 
	jp nz,andaixhfail
	jp po,andaixhfail
	jp m,andaixhfail
	ld a,$ff
	ld ixh,$88
	and a,ixh
	jp po,andaixhfail
	jp p,andaixhfail
	jp z,andaixhfail
	ld a,$0f 
	and a,ixh 
	cp $08
	jp z,andaixl
andaixhfail:
	ld (iy),'F'	

andaixl:
	inc iy
	ld (iy),'P' ; Test 289
	ld a,$55
	ld ixl,$f2
	and  A,IXL
	cp $50
	jp z,andaixd
andaixlfail:
	ld (iy),'F'	

andaixd:
	inc iy
	ld (iy),'P' ; Test 290
	ld ix,results
	ld a,$13
	and  A,(IX-$01)	; contains $3a
	cp $12
	jp z,xoraixh
andaixdfail:
	ld (iy),'F'	

xoraixh:
	inc iy
	ld (iy),'P' ; Test 291
	ld a,$ff
	ld ixh,$12
	xor  A,IXH 
	jp z,xoraixhfail
	jp p,xoraixhfail
	jp po,xoraixhfail
	cp $ed
	jp z,xoraixl
xoraixhfail:
	ld (iy),'F'	

xoraixl:
	inc iy
	ld (iy),'P' ; Test 292
	ld a,$55
	ld ixl,$aa
	xor  A,IXL 
	jp z,xoraixlfail
	cp $ff
	jp z,xoraixd
xoraixlfail:
	ld (iy),'F'	

xoraixd:
	inc iy
	ld (iy),'P' ; Test 293
	ld ix,results-8
	ld a,$34
	xor  A,(IX+$02)	; contains $73
	cp $47
	jp z,oraixh
xoraixdfail:
	ld (iy),'F'	

; 0xddb0
oraixh:
	inc iy
	ld (iy),'P' ; Test 294
	ld a,$4f
	ld ixh,$12
	or  A,IXH
	push af
	cp $5f
	jp nz,oraixhfail
	pop bc
	ld a,$04
	cp c
	jp nz,oraixhfail
	ld ixh,$81
	or a,ixh 
	push af
	cp $85
	jp nz,oraixhfail
	pop bc 
	ld a,$80
	cp c
	jp nz, oraixhfail
	ld a,$00
	ld ixh,$00
	or a,ixh
	jp z,oraixl
oraixhfail:
	ld (iy),'F'	

oraixl:
	inc iy
	ld (iy),'P' ; Test 295
	ld a,$01
	ld ixl,$10
	or  A,IXL
	cp $11
	jp z,oraixd
oraixlfail:
	ld (iy),'F'	

oraixd:
	inc iy
	ld (iy),'P' ; Test 296
	ld ix,results-8
	ld a,$01
	or  A,(IX+$07)	; contains $3a
	cp $3b
	jp z,cpaixh
oraixdfail:
	ld (iy),'F'	

cpaixh:
	inc iy
	ld (iy),'P' ; Test 297
	ld a,$34
	ld ixh,$80
	cp  A,IXH
	push af
	cp $34
	jp nz,cpaixhfail
	pop bc
	ld a,$87
	cp c
	jp nz,cpaixhfail
	ld a,$80
	ld ixh,$81
	cp a,ixh
	push af
	pop bc
	ld a,$93
	cp c
	jp nz,cpaixhfail
	ld a,$05
	ld ixh,$05
	cp a,ixh 
	push af
	pop bc
	ld a,$42
	cp c
	jp z,cpaixl
cpaixhfail:
	ld (iy),'F'	

cpaixl:
	inc iy
	ld (iy),'P' ; Test 298
	ld a,$55
	ld ixl,$54
	cp  A,IXL 
	push af
	pop bc
	ld a,$02
	cp c
	jp z,cpaixd
cpaixlfail:
	ld (iy),'F'	

cpaixd:
	inc iy
	ld (iy),'P' ; Test 299
	ld a,'t'
	ld ix,results-8
	cp  A,(IX+$05)	
	jp z,ixbitsetup
cpaixdfail:
	ld (iy),'F'	

; 0xddc0	

; 0xddd0

; 0xdde0
	
; 0xddf0

; 0xddcb - IX bit instructions
; 0xddcbss00 -- all affect flags
; First, set up some test data (tested OK)
ixbitsetup:
	ld ix,results-$38
	ld b,$08
	ld a,$01
ixbitsetuploop:
	ld (ix),A
	sla a
	inc ix
	djnz ixbitsetuploop

rlcixdb:
	inc iy
	ld (iy),'P' ; Test 300
	ld ix,results
	rlc (IX-$38),B
	ld a,$02
	cp b 
	jp z,rlcixdc
rlcixdbfail:
	ld (iy),'F'	

rlcixdc:
	inc iy
	ld (iy),'P' ; Test 301
	rlc (IX-$37),C
	ld a,$04
	cp c 
	jp z,rlcixdd
rlcixdcfail:
	ld (iy),'F'	

rlcixdd:
	inc iy
	ld (iy),'P' ; Test 302
	rlc (IX-$36),D
	ld a,$08
	cp d 
	jp z,rlcixde
rlcixddfail:
	ld (iy),'F'	

rlcixde:
	inc iy
	ld (iy),'P' ; Test 303
	rlc (IX-$35),E
	ld a,$10
	cp e 
	jp z,rlcixdh
rlcixdefail:
	ld (iy),'F'	

rlcixdh:
	inc iy
	ld (iy),'P' ; Test 304
	rlc (IX-$34),H
	ld a,$20
	cp h
	jp z,rlcixdl
rlcixdhfail:
	ld (iy),'F'	

rlcixdl:
	inc iy
	ld (iy),'P' ; Test 305
	rlc (IX-$33),L
	ld a,$40
	cp l
	jp z,rlcixd
rlcixdlfail:
	ld (iy),'F'	

rlcixd:
	inc iy
	ld (iy),'P' ; Test 306
	rlc (IX-$32)	; contains $40
	push af
	ld a,(ix-$32)
	cp $80
	jp nz,rlcixdfail
	pop bc
	ld a,$80
	cp C
	jp nz,rlcixdfail
	ld a,$80
	ld (ix-$28),A
	rlc (IX-$28)
	jp m,rlcixdfail
	jp z,rlcixdfail
	jp pe,rlcixdfail
	jp nc,rlcixdfail
	ld a,$00
	ld (IX-$28),A
	rlc (IX-$28)	
	jp z,rlcixda
rlcixdfail:
	ld (iy),'F'	

rlcixda:
	inc iy
	ld (iy),'P' ; Test 307
	rlc (IX-$31),A
	cp $01
	jp z,rrcixdb
rlcixdafail:
	ld (iy),'F'	

rrcixdb:
	inc iy
	ld (iy),'P' ; Test 308
	rrc (IX-$38),B		; contains $02 before RRC
	jp c,rrcixdbfail
	ld a,$01
	cp b
	jp nz,rrcixdc
	rrc (IX-$38),b
	jp nc,rrcixdbfail
	jp z,rrcixdbfail
	ld (ix-$38),$01
	jp rrcixdc
rrcixdbfail:
	ld (iy),'F'	

rrcixdc:
	inc iy
	ld (iy),'P' ; Test 309
	rrc (IX-$37),C
	jp c,rrcixdcfail
	ld a,$02
	cp C
	jp z,rrcixdd
rrcixdcfail:
	ld (iy),'F'	

rrcixdd:
	inc iy
	ld (iy),'P' ; Test 310
	rrc (IX-$36),D
	ld a,$04
	cp d
	jp z,rrcixde
rrcixddfail:
	ld (iy),'F'	

rrcixde:
	inc iy
	ld (iy),'P' ; Test 311
	rrc (IX-$35),E
	ld a,$08
	cp e
	jp z,rrcixdh
rrcixdefail:
	ld (iy),'F'	

rrcixdh:
	inc iy
	ld (iy),'P' ; Test 312
	rrc (IX-$34),H
	ld a,$10
	cp h
	jp z,rrcixdl
rrcixdhfail:
	ld (iy),'F'	

rrcixdl:
	inc iy
	ld (iy),'P' ; Test 313
	rrc (IX-$33),L
	ld a,$20
	cp l
	jp z,rrcixd
rrcixdlfail:
	ld (iy),'F'	

rrcixd:
	inc iy
	ld (iy),'P' ; Test 314
	rrc (IX-$32)
	ld a,(ix-$32)
	cp $40
	jp z,rrcixda
rrcixdfail:
	ld (iy),'F'	

rrcixda:
	inc iy
	ld (iy),'P' ; Test 315
	rrc (IX-$31),A
	cp $80
	jp z,rlixdb
rrcixdafail:
	ld (iy),'F'	

; 0xddcbss10 -- all affect flags
rlixdb:
	inc iy
	ld (iy),'P' ; Test 316
	rl (IX-$38),B	; contains $01 before RL
	ld a,$02
	cp b
	jp z,rlixdc
rlixdbfail:
	ld (iy),'F'	

rlixdc:
	inc iy
	ld (iy),'P' ; Test 317
	rl (IX-$37),C
	ld a,$04
	cp c 
	jp z,rlixdd
rlixdcfail:
	ld (iy),'F'	

rlixdd:
	inc iy
	ld (iy),'P' ; Test 318
	rl (IX-$36),D
	ld a,$08
	cp d 
	jp z,rlixde
rlixddfail:
	ld (iy),'F'	

rlixde:
	inc iy
	ld (iy),'P' ; Test 319
	rl (IX-$35),E
	ld a,$10
	cp e 
	jp z,rlixdh
rlixdefail:
	ld (iy),'F'	

rlixdh:
	inc iy
	ld (iy),'P' ; Test 320
	rl (IX-$34),H
	ld a,$20
	cp H
	jp z,rlixdl
rlixdhfail:
	ld (iy),'F'	

rlixdl:
	inc iy
	ld (iy),'P' ; Test 321
	rl (IX-$33),L
	ld a,$40 
	cp L
	jp z,rlixd
rlixdlfail:
	ld (iy),'F'	

rlixd:
	inc iy
	ld (iy),'P' ; Test 322
	rl (IX-$32)	; contains $40 before RL
	push af
	ld a,(IX-$32)
	cp $80
	jp nz,rlixdfail
	pop bc
	ld a,$80
	cp C
	jp nz,rlixdfail
	rl (IX-$32)
	push af
	pop bc
	ld a,$45
	cp C
	jp nz,rlixdfail
	ld (IX-$32),$80
	jp rlixda
rlixdfail:
	ld (iy),'F'	

rlixda:
	inc iy
	ld (iy),'P'		; Test 323
	rl (IX-$31),A	; Contains $80 before RL
	jp z,rrixdb
rlixdafail:
	ld (iy),'F'	

rrixdb:
	inc iy
	ld (iy),'P' ; Test 324
	scf	
	rr (IX-$38),B	; Contains $02 before RR
	ld a,$81
	cp b 
	jp z,rrixdc
rrixdbfail:
	ld (iy),'F'	

rrixdc:
	inc iy
	ld (iy),'P' ; Test 325
	or A	; clear C flag
	rr (IX-$37),C
	ld a,$02
	cp c 
	jp z,rrixdd
rrixdcfail:
	ld (iy),'F'	

rrixdd:
	inc iy
	ld (iy),'P' ; Test 326
	rr (IX-$36),D
	ld a,$04
	cp d 
	jp z,rrixde
rrixddfail:
	ld (iy),'F'	

rrixde:
	inc iy
	ld (iy),'P' ; Test 327
	rr (IX-$35),E
	ld a,$08
	cp e 
	jp z,rrixdh
rrixdefail:
	ld (iy),'F'	

rrixdh:
	inc iy
	ld (iy),'P' ; Test 328
	rr (IX-$34),H
	ld a,$10
	cp h 
	jp z,rrixdl
rrixdhfail:
	ld (iy),'F'	

rrixdl:
	inc iy
	ld (iy),'P' ; Test 329
	rr (IX-$33),L
	ld a,$20
	cp l 
	jp z,rrixd
rrixdlfail:
	ld (iy),'F'	

rrixd:
	inc iy
	ld (iy),'P' ; Test 330
	rr (IX-$32)
	ld a,(IX-$32)
	cp $40
	jp z,rrixda
rrixdfail:
	ld (iy),'F'	

rrixda:
	inc iy
	ld (iy),'P' ; Test 331
	scf
	rr (IX-$31),A
	cp $80
	jp z,slaixdb
rrixdafail:
	ld (iy),'F'	

; 0xddcbss20 -- all affect flags
slaixdb:
	inc iy
	ld (iy),'P' ; Test 332
	sla (IX-$38),B		; contains $81 before SLA
	jp nc,slaixdbfail
	ld a,$02
	cp b
	jp z,slaixdc
slaixdbfail:
	ld (iy),'F'	

slaixdc:
	inc iy
	ld (iy),'P' ; Test 333
	sla (IX-$37),C 		; Contains $02 before SLA
	jp c,slaixdcfail
	ld a,$04
	cp c
	jp z,slaixdd
slaixdcfail:
	ld (iy),'F'	

slaixdd:
	inc iy
	ld (iy),'P' ; Test 334
	sla (IX-$36),D 		; Contains $04 before SLA
	ld a,$08
	cp D
	jp z,slaixde
slaixddfail:
	ld (iy),'F'	

slaixde:
	inc iy
	ld (iy),'P' ; Test 335
	sla (IX-$35),E
	ld a,$10
	cp e 
	jp z,slaixdh
slaixdefail:
	ld (iy),'F'	

slaixdh:
	inc iy
	ld (iy),'P' ; Test 336
	sla (IX-$34),H
	ld a,$20
	cp H
	jp z,slaixdl
slaixdhfail:
	ld (iy),'F'	

slaixdl:
	inc iy
	ld (iy),'P' ; Test 337
	sla (IX-$33),L
	ld a,$40
	cp L
	jp z,slaixd
slaixdlfail:
	ld (iy),'F'	

slaixd:
	inc iy
	ld (iy),'P' ; Test 338
	scf
	sla (IX-$32) 	; Contains $40 before SLA
	push af
	jp z,slaixdfail
	jp c,slaixdfail
	jp p,slaixdfail
	jp pe,slaixdfail
	ld a,(IX-$32)
	cp $80
	jp nz,slaixdfail
	pop bc
	ld a,$80	; S flag set
	cp C
	jp nz,slaixdfail
	ld (IX-$32),$80
	sla (IX-$32)
	jp nz,slaixdfail
	jp nc,slaixdfail
	ld (IX-$32),$21
	sla (IX-$32)
	push af
	ld a,(IX-$32)
	cp $42
	jp nz,slaixdfail
	pop bc
	ld a,$04
	jp nz,slaixdfail
	ld (IX-$32),$80
	jp slaixda	
slaixdfail:
	ld (iy),'F'	

slaixda:
	inc iy
	ld (iy),'P' ; Test 339
	sla (IX-$31),A
	jp nc,slaixdafail
	cp $00
	jp nz,slaixdafail
	ld (IX-$31),$01
	jp sraixdb
slaixdafail:
	ld (iy),'F'	

sraixdb:
	inc iy
	ld (iy),'P' ; Test 340
	sra (IX-$38),B		; contains $02 before SRA
	jp z,sraixdbfail
	jp c,sraixdbfail
	jp m,sraixdbfail
	jp pe,sraixdbfail
	ld a,$01
	cp b
	jp z,sraixdc
sraixdbfail:
	ld (iy),'F'	

sraixdc:
	inc iy
	ld (iy),'P' ; Test 341
	sra (IX-$37),C		; contains $04 before SRA 
	ld a,$02
	cp c
	jp z,sraixdd
sraixdcfail:
	ld (iy),'F'	

sraixdd:
	inc iy
	ld (iy),'P' ; Test 342
	sra (IX-$36),D 
	ld a,$04
	cp D
	jp z,sraixde
sraixddfail:
	ld (iy),'F'	

sraixde:
	inc iy
	ld (iy),'P' ; Test 343
	sra (IX-$35),E
	ld a,$08
	cp e 
	jp z,sraixdh
sraixdefail:
	ld (iy),'F'	

sraixdh:
	inc iy
	ld (iy),'P' ; Test 344
	sra (IX-$34),H
	ld a,$10
	cp h 
	jp z,sraixdl
sraixdhfail:
	ld (iy),'F'	

sraixdl:
	inc iy
	ld (iy),'P' ; Test 345
	sra (IX-$33),L 
	ld a,$20
	cp l 
	jp z,sraixd
sraixdlfail:
	ld (iy),'F'	

sraixd:
	inc iy
	ld (iy),'P' ; Test 346
	sra (IX-$32)	; contains $80 before SRA
	jp p,sraixdfail
	jp po,sraixdfail
	ld a,(IX-$32)
	cp $c0
	jp z,sraixda
sraixdfail:
	ld (iy),'F'	

sraixda:
	inc iy
	ld (iy),'P' ; Test 347
	sra (IX-$31),A	; contains $01 before SRA
	jp nc,sraixdafail
	jp nz,sraixdafail
	ld (IX-$31),$80
	jp sllixdb
sraixdafail:
	ld (iy),'F'	

; 0xddcbss30 -- all affect flags
sllixdb:
	inc iy
	ld (iy),'P' ; Test 348
	sll (IX-$38),B	; Contains $01 before SLL
	push af
	pop de
	ld a,$03
	cp b
	jp nz,sllixdbfail
	ld a,$04
	cp E
	jp nz,sllixdbfail
	sll (ix-$38),b 
	jp po,sllixdc
sllixdbfail:
	ld (iy),'F'	

sllixdc:
	inc iy
	ld (iy),'P' ; Test 349
	sll (IX-$37),C	; Contains $02 before SLL
	ld a,$05
	cp c 
	jp z,sllixdd
sllixdcfail:
	ld (iy),'F'	

sllixdd:
	inc iy
	ld (iy),'P' ; Test 350
	sll (IX-$36),D	; Contains $04 before SLL
	ld a,$09
	cp D
	jp z,sllixde
sllixddfail:
	ld (iy),'F'	

sllixde:
	inc iy
	ld (iy),'P' ; Test 351
	sll (IX-$35),E	; Contains $08 before SLL
	ld a,$11
	cp e 
	jp z,sllixdh
sllixdefail:
	ld (iy),'F'	

sllixdh:
	inc iy
	ld (iy),'P' ; Test 352
	sll (IX-$34),H	; Contains $10 before SLL
	ld a,$21
	cp H
	jp z,sllixdl
sllixdhfail:
	ld (iy),'F'	

sllixdl:
	inc iy
	ld (iy),'P' ; Test 353
	sll (IX-$33),L	; Contains $20 before SLL
	ld a,$41
	cp L
	jp z,sllixd
sllixdlfail:
	ld (iy),'F'	

sllixd:
	inc iy
	ld (iy),'P' ; Test 354
	sll (IX-$32)	; Contains $c0 before SLL
	jp nc,sllixdfail
	jp p,sllixdfail
	ld a,$81
	cp (ix-$32)
	jp z,sllixda
sllixdfail:
	ld (iy),'F'	

sllixda:
	inc iy
	ld (iy),'P' ; Test 355
	sll (IX-$31),A	; Contains $80 before SLL
	jp nc,sllixdafail
	jp z,sllixdafail
	cp $01
	jp z,srlixdb
sllixdafail:
	ld (iy),'F'	

srlixdb:
	inc iy
	ld (iy),'P' ; Test 356
	srl (IX-$38),B	; Contains $07 before SLL
	push af
	jp z,srlixdbfail
	jp po,srlixdbfail
	jp m,srlixdbfail
	jp nc,srlixdbfail
	ld a,$03
	cp b
	jp nz,srlixdbfail
	pop bc
	ld a,$05
	jp z,srlixdc
srlixdbfail:
	ld (iy),'F'	

srlixdc:
	inc iy
	ld (iy),'P' ; Test 357
	srl (IX-$37),C	; Contains $05 before SLL
	ld a,$02
	cp c
	jp z,srlixdd
srlixdcfail:
	ld (iy),'F'	

srlixdd:
	inc iy
	ld (iy),'P' ; Test 358
	srl (IX-$36),D	; Contains $09 before SLL
	ld a,$04
	cp D
	jp z,srlixde
srlixddfail:
	ld (iy),'F'	

srlixde:
	inc iy
	ld (iy),'P' ; Test 359
	srl (IX-$35),E 	; Contains $11 before SLL
	ld a,$08
	cp E
	jp z,srlixdh
srlixdefail:
	ld (iy),'F'	

srlixdh:
	inc iy
	ld (iy),'P' ; Test 360
	srl (IX-$34),H	; Contains $21 before SLL
	ld a,$10
	cp h 
	jp z,srlixdl
srlixdhfail:
	ld (iy),'F'	

srlixdl:
	inc iy
	ld (iy),'P' ; Test 361
	srl (IX-$33),L	; Contains $41 before SLL
	ld a,$20
	cp l 
	jp z,srlixd
srlixdlfail:
	ld (iy),'F'	

srlixd:
	inc iy
	ld (iy),'P' ; Test 362
	srl (IX-$32)	; Contains $81 before SLL
	jp pe,srlixdfail
	ld a,$40
	cp (IX-$32)
	jp z,srlixda 
srlixdfail:
	ld (iy),'F'	

srlixda:
	inc iy
	ld (iy),'P' ; Test 363
	srl (IX-$31),A	; Contains $01 before SLL
	jp nz,srlixdafail
	jp po,srlixdafail
	jp nc,srlixdafail
	jp p,bit0ixd
srlixdafail:
	ld (iy),'F'	

; 0xddcbss40 - 0xddcbss7f
bit0ixd:
	inc iy
	ld (iy),'P' ; Test 364
	bit 0,(IX-$38)  ; contains 0x03 before BIT
	push af
	pop bc
	jp z,bit0ixdfail
	ld a,C
	and $52		; only check Z, H, N bits
	cp $10		; H flag should be set
	jp z,bit1ixd
bit0ixdfail:
	ld (iy),'F'	

bit1ixd:
	inc iy
	ld (iy),'P' ; Test 365
	bit 1,(IX-$37)  ; contains 0x02 before BIT
	jp nz,bit2ixd
bit1ixdfail:
	ld (iy),'F'	

bit2ixd:
	inc iy
	ld (iy),'P' ; Test 366
	bit 2,(IX-$36)  ; contains 0x04 before BIT
	jp nz, bit3ixd
bit2ixdfail:
	ld (iy),'F'	

bit3ixd:
	inc iy
	ld (iy),'P' ; Test 367
	bit 3,(IX-$35)  ; contains 0x08 before BIT
	jp nz,bit4ixd
bit3ixdfail:
	ld (iy),'F'	

bit4ixd:
	inc iy
	ld (iy),'P' ; Test 368
	bit 4,(IX-$34)  ; contains 0x10 before BIT
	jp nz,bit5ixd
bit4ixdfail:
	ld (iy),'F'	

bit5ixd:
	inc iy
	ld (iy),'P' ; Test 369
	bit 5,(IX-$33)  ; contains 0x20 before BIT
	jp nz,bit6ixd
bit5ixdfail:
	ld (iy),'F'	

bit6ixd:
	inc iy
	ld (iy),'P' ; Test 370
	bit 6,(IX-$33)  ; contains 0x20 before BIT
	jp z,bit7ixd
bit6ixdfail:
	ld (iy),'F'	

bit7ixd:
	inc iy
	ld (iy),'P' ; Test 371
	bit 7,(IX-$31)	; contains 0x00 before BIT
	jp z,testiyopcodes
bit7ixdfail:
	ld (iy),'F'	

; 0xddcbss80 - 0xddcbssbf

; 0xddcbssc0 - 0xddcbssff

; IY instructions - 0xfd prefix
; These run the same code as the IX instructions,
; except that a different register is used for the index.
;
; All opcodes will be executed, but just a couple tests
; will be run to confirm that the correct index register (IY)
; is selected for the main index and bit index instructions 
;

testiyopcodes:
; switch back to using IX for the test results pointer
	ld a,iyh
	ld ixh,a 
	ld a,iyl
	ld ixl,a

; 0xfd00
addiybc:
	inc ix
	ld (ix),'P' ; Test 372
	ld iy,$1357
	ld bc,$2468
	add IY,BC
	ld a,$bf
	cp iyl
	jp nz,addiybcfail
	ld a,$37
	cp iyh
	jp z,addiyde
addiybcfail:
	ld (ix),'F'	

; 0xfd10
addiyde:
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
; Test to make sure bit instructions select correct index register (IY)
rlciydb:
	inc ix
	ld (ix),'P' ; Test 372
	ld IY,results-$20
	rlc (IY+$02),B	; contains 'h' ($68)
	jp c,rlciydbfail
	ld a,$D0
	cp B
	jp nz,rlciydbfail
	cp (IY+$02)
	jp z,rlciydc
rlciydbfail:
	ld (ix),'F'	

rlciydc:
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

	org $3000
	defm "Results:"

results:
	defs $ff,'.'		; Results area. 'P' ($50): Pass. 'F' ($46): FAIL. '.' - no result recorded
	defs $ff,'.'

testing_area:
	defb $aa
	defb $55

