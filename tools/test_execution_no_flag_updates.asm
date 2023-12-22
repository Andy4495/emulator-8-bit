; Z80 test file for 8-bit emulator
;    https://github.com/Andy4495/emulator-8-bit
; 21-Dec-2023  Andy4495 Update for z88dk compatibility
;
; 13-Mar-2023  Andy4495 Initial version
; 
; Z80 opcodes that do no affect flags
; 
; Compile with --ixcbr2 option: 
;   zasm --ixcbr2 filename.asm

    org 0
	jp	start
	defs 5,0	; fill between with zeros
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

start: 
; Main Instruction set
; Setup
	ld SP,$fffe
	ld A,$5a
; 0x00
	nop
	ld BC,results   
	ld (BC),A
	inc BC
;	inc B           
;	dec B
	ld B,$69
;	rlca
	ex AF,AF'
;	add HL,BC
	ld A,(BC)
	dec BC
;	inc C          
;	dec C
	ld C,$23
;	rrca
	

; 0x10
;	djnz $		; tested in separate file
	ld DE,$0004  
	ld (DE),A
	inc DE
;	inc D          
;	dec D
	ld D,$34
;	rla
	jr  $+3
	nop
;	add HL,DE
	ld A,(DE)
	dec DE
;	inc E           
;	dec E
	ld E,$45
;	rra
	

; 0x20
	jr  nz,$+3
	nop
	ld HL,$0006 
	ld (results+2),HL
	inc HL
;	inc H           
;	dec H
	ld H,$56
;	daa
	jr  z,$+3
	nop
;	add HL,HL
	ld HL,($2345)
	dec HL
;	inc L          
;	dec L
	ld L,$67
;	cpl
	

; 0x30
	jr  nc,$+3
	nop
	ld SP,$fffb
	ld (results+3),A
	inc SP
;	inc (HL)       
;	dec (HL)
	ld HL, results+4	;setup
	ld (HL),$78
;	scf
	jr  c,$+3
	nop
;	add HL,SP
	ld A,($0084)
	dec SP
;	inc A          
;	dec A
	ld A,$12
;	ccf
	

; 0x40
	ld  B,B         
	ld  B,C    
	ld  B,D         
	ld  B,E         
	ld  B,H        
	ld  B,L        
	ld  B,(HL)      
	ld  B,A        
	ld  C,B         
	ld  C,C    
	ld  C,D         
	ld  C,E         
	ld  C,H        
	ld  C,L        
	ld  C,(HL)      
	ld  C,A 
	

; 0x50
	ld  D,B         
	ld  D,C    
	ld  D,D         
	ld  D,E         
	ld  D,H        
	ld  D,L        
	ld  D,(HL)      
	ld  D,A        
	ld  E,B         
	ld  E,C    
	ld  E,D         
	ld  E,E         
	ld  E,H        
	ld  E,L        
	ld  E,(HL)      
	ld  E,A  
	

; 0x60
	ld  H,B         
	ld  H,C    
	ld  H,D         
	ld  H,E         
	ld  H,H        
	ld  H,L        
	ld  H,(HL)      
	ld  H,A        
	ld  L,B         
	ld  L,C    
	ld  L,D         
	ld  L,E         
	ld  L,H        
	ld  L,L        
	ld  L,(HL)      
	ld  L,A    
	

; 0x70
	ld	HL, results+5	;setup
	ld  (HL),B     
	ld	HL, results+6	;setup
	ld  (HL),C    
	ld	HL, results+7	;setup
	ld  (HL),D         
	ld	HL, results+8	;setup
	ld  (HL),E         
	ld	HL, results+9	;setup
	ld  (HL),H        
	ld	HL, results+10	;setup
	ld  (HL),L  	
;	halt		; Tested at bottom
	ld	HL, results+1	;setup
	ld  (HL),A        
	ld  A,B         
	ld  A,C    
	ld  A,D         
	ld  A,E         
	ld  A,H        
	ld  A,L        
	ld  A,(HL)      
	ld  A,A    
	

; 0x80 -- all affect flags
;	add A,B
;	add A,C
;	add A,D
;	add A,E
;	add A,H
;	add A,L
;	add A,(HL)
;	add A,A
;	adc A,B
;	adc A,C
;	adc A,D
;	adc A,E
;	adc A,H
;	adc A,L
;	adc A,(HL)
;	adc A,A

; 0x90 -- all affect flags
;	sub A,B
;	sub A,C
;	sub A,D
;	sub A,E
;	sub A,H
;	sub A,L
;	sub A,(HL)
;	sub A,A
;	sbc A,B
;	sbc A,C
;	sbc A,D
;	sbc A,E
;	sbc A,H
;	sbc A,L
;	sbc A,(HL)
;	sbc A,A

; 0xa0 -- all affect flags
;	and A,B
;	and A,C
;	and A,D
;	and A,E
;	and A,H
;	and A,L
;	and A,(HL)
;	and A,A
;	xor A,B
;	xor A,C
;	xor A,D
;	xor A,E
;	xor A,H
;	xor A,L
;	xor A,(HL)
;	xor A,A

; 0xb0 -- all affect flags
;	or  A,B
;	or  A,C
;	or  A,D
;	or  A,E
;	or  A,H
;	or  A,L
;	or  A,(HL)
;	or  A,A
;	cp  A,B
;	cp  A,C
;	cp  A,D
;	cp  A,E
;	cp  A,H
;	cp  A,L
;	cp  A,(HL)
;	cp  A,A

; 0xc0
;	ret nz		; tested in separate file
	pop  BC         
	jp  nz,$+4
	nop
	jp  $+4
	nop
	call nz,return_point
	push BC 
;	add A,$12
;	rst  $00		; tested in separate file
;	ret z		; tested in separate file
;	ret			; tested in separate file
	jp  z,$+4
    nop                 ; 0xcb - Bit instructions
	call z,return_point
	call return_point
;	adc a,$23
;	rst  $08		; tested in separate file
	

; 0xd0
;	ret nc		; tested in separate file
	pop  DE         
	jp  nc,$+4
	nop
	out ($a3),A
	call nc,return_point
	push DE 
;	sub A,$34
;	rst  $10		; tested in separate file
;	ret c		; tested in separate file
	exx
	jp  c,$+4
	nop
	in A,($d6)
	call c,return_point
    nop					; 0xdd IX index instructions
;	sbc A,$45
;	rst  $18		; tested in separate file
	

; 0xe0
;	ret po		; tested in separate file
	pop  HL        
	jp  po,$+4
	nop
	ex (SP),HL
	call po,return_point
	push HL 
;	and A,$56
;	rst  $20		; tested in separate file
;	ret pe		; tested in separate file
	ld HL,after_HL  ; Set up the jp (HL) instruction
	jp (HL)  		;This was not included in test_disassembler_1
	nop
after_HL:
	jp  pe,$+4
	nop
	ex DE,HL
	call pe,return_point
	nop				; 0xed - Miscellaneous instructions
;	xor A,$67
;	rst  $28		; tested in separate file

; 0xf0
;	ret p		; tested in separate file
	pop  AF    
	jp  p,$+4
	nop
	di
	call p,return_point
	push AF 
;	or  A,$78
;	rst  $30		; tested in separate file
;	ret m		; tested in separate file
	ld HL,$fffa	;setup
	ld SP,HL
	jp  m,$+4
	nop
	ei
	call m,return_point
	nop				; 0xfd - IY index instructions
;	cp  A,$89
;	rst  $38		; tested in separate file
	

; Bit Instructions - 0xcb prefix
; 0xcb00 -- all affect flags
;	rlc B           
;	rlc C        
;	rlc D   
;	rlc E  
;	rlc H   
;	rlc L          
;	rlc (HL)
;	rlc A
;	rrc B
;	rrc C
;	rrc D
;	rrc E
;	rrc H
;	rrc L
;	rrc (HL)
;	rrc A

; 0xcb10  -- all affect flags
;	rl  B
;	rl  C
;	rl  D
;	rl  E
;	rl  H
;	rl  L
;	rl  (HL)
;	rl  A
;	rr  B
;	rr  C
;	rr  D
;	rr  E
;	rr  H
;	rr  L
;	rr  (HL)
;	rr  A

; 0xcb20 -- all affect flags
;	sla B
;	sla C
;	sla D
;	sla E
;	sla H
;	sla L
;	sla (HL)
;	sla A
;	sra B
;	sra C
;	sra D
;	sra E
;	sra H
;	sra L
;	sra (HL)
;	sra A

; 0xcb30 -- all affect flags
;	sll B
;	sll C
;	sll D
;	sll E
;	sll H
;	sll L
;	sll (HL)
;	sll A
;	srl B
;	srl C
;	srl D
;	srl E
;	srl H
;	srl L
;	srl (HL)
;	srl A

; 0xcb40 -- all affect flags
;	bit 0,B
;	bit 0,C
;	bit 0,D
;	bit 0,E
;	bit 0,H
;	bit 0,L
;	bit 0,(HL)
;	bit 0,A
;	bit 1,B
;	bit 1,C
;	bit 1,D
;	bit 1,E
;	bit 1,H
;	bit 1,L
;	bit 1,(HL)
;	bit 1,A

; 0xcb50 -- all affect flags
;	bit 2,B
;	bit 2,C
;	bit 2,D
;	bit 2,E
;	bit 2,H
;	bit 2,L
;	bit 2,(HL)
;	bit 2,A
;	bit 3,B
;	bit 3,C
;	bit 3,D
;	bit 3,E
;	bit 3,H
;	bit 3,L
;	bit 3,(HL)
;	bit 3,A

; 0xcb60 -- all affect flags
;	bit 4,B
;	bit 4,C
;	bit 4,D
;	bit 4,E
;	bit 4,H
;	bit 4,L
;	bit 4,(HL)
;	bit 4,A
;	bit 5,B
;	bit 5,C
;	bit 5,D
;	bit 5,E
;	bit 5,H
;	bit 5,L
;	bit 5,(HL)
;	bit 5,A

; 0xcb70 -- all affect flags
;	bit 6,B
;	bit 6,C
;	bit 6,D
;	bit 6,E
;	bit 6,H
;	bit 6,L
;	bit 6,(HL)
;	bit 6,A
;	bit 7,B
;	bit 7,C
;	bit 7,D
;	bit 7,E
;	bit 7,H
;	bit 7,L
;	bit 7,(HL)
;	bit 7,A

; 0xcb80
	res 0,B
	res 0,C
	res 0,D
	res 0,E
	res 0,H
	res 0,L
	res 0,(HL)
	res 0,A
	res 1,B
	res 1,C
	res 1,D
	res 1,E
	res 1,H
	res 1,L
	res 1,(HL)
	res 1,A
	

; 0xcb90
	res 2,B
	res 2,C
	res 2,D
	res 2,E
	res 2,H
	res 2,L
	res 2,(HL)
	res 2,A
	res 3,B
	res 3,C
	res 3,D
	res 3,E
	res 3,H
	res 3,L
	res 3,(HL)
	res 3,A
	

; 0xcba0
	res 4,B
	res 4,C
	res 4,D
	res 4,E
	res 4,H
	res 4,L
	res 4,(HL)
	res 4,A
	res 5,B
	res 5,C
	res 5,D
	res 5,E
	res 5,H
	res 5,L
	res 5,(HL)
	res 5,A
	

; 0xcbb0
	res 6,B
	res 6,C
	res 6,D
	res 6,E
	res 6,H
	res 6,L
	res 6,(HL)
	res 6,A
	res 7,B
	res 7,C
	res 7,D
	res 7,E
	res 7,H
	res 7,L
	res 7,(HL)
	res 7,A
	

; 0xcbc0
	set 0,B
	set 0,C
	set 0,D
	set 0,E
	set 0,H
	set 0,L
	set 0,(HL)
	set 0,A
	set 1,B
	set 1,C
	set 1,D
	set 1,E
	set 1,H
	set 1,L
	set 1,(HL)
	set 1,A
	

; 0xcbd0
	set 2,B
	set 2,C
	set 2,D
	set 2,E
	set 2,H
	set 2,L
	set 2,(HL)
	set 2,A
	set 3,B
	set 3,C
	set 3,D
	set 3,E
	set 3,H
	set 3,L
	set 3,(HL)
	set 3,A
	

; 0xcbe0
	set 4,B
	set 4,C
	set 4,D
	set 4,E
	set 4,H
	set 4,L
	set 4,(HL)
	set 4,A
	set 5,B
	set 5,C
	set 5,D
	set 5,E
	set 5,H
	set 5,L
	set 5,(HL)
	set 5,A
	

; 0xcbf0
	set 6,B
	set 6,C
	set 6,D
	set 6,E
	set 6,H
	set 6,L
	set 6,(HL)
	set 6,A
	set 7,B
	set 7,C
	set 7,D
	set 7,E
	set 7,H
	set 7,L
	set 7,(HL)
	set 7,A
	

; Miscellaneous opcodes - 0xed prefix
; 0xed00 - 0xed3f are undefined opcodes (executed as NOPs)
; 0xed40
;	in  B,(C)
	out (C),B
;	sbc HL,BC
	ld  (results+13),BC
;	neg
;	retn		; tested in separate file
	im  0
	ld  I,A
;	in  C,(C)
	out (C),C
;	adc HL,BC
;	ld  BC,($5678)
	nop			; 0xed4c undefined
;	reti		; tested in separate file
	nop			; 0xed4e undefined
	ld  R,A
	

; 0xed50
;	in  D,(C)
	out (C),D
;	sbc HL,DE
	ld  (results+15),DE
	nop				; 0xed54 undefined
	nop				; 0xed55 undefined
	im  1
;	ld  A,I
;	in  E,(C)
	out (C),E
;	adc HL,DE
	ld  DE,($6789)
	nop				; 0xed5c undefined
	nop				; 0xed5d undefined
	im  2
;	ld  A,R
	

; 0xed60
;	in  H,(C)
	out (C),H
;	sbc HL,HL
	nop				; 0xed63 same as 0x22  /// Need to test the "same as" opcodes
    nop				; 0xed64 undefined
	nop				; 0xed65 undefined
	nop				; 0xed66 undefined
;	rrd
;	in  L,(C)
	out (C),L
;	adc HL,HL
	nop             ; 0xed6b same as 0x2a
	nop				; 0xed6c undefined
	nop				; 0xed6d undefined
	nop				; 0xed6e undefined
;	rld
	

; 0xed70
;	in  f,(C)  		; This is also written as "in (c)" -- only affects flags, input data is discarded
	out (C),0       ; for NMOS CPUs -- undocumented opcode 0xED71 acts differently depending on chip type
;	sbc HL,SP
	ld  (results+17),SP
    nop				; 0xed74 undefined
	nop				; 0xed75 undefined
	nop				; 0xed76 undefined
	nop				; 0xed77 undefined
;	in  A,(C)
	out (C),A
;	adc HL,SP
	ld  SP,(results+17)
    nop				; 0xed7c undefined
	nop				; 0xed7d undefined
	nop				; 0xed7e undefined
	nop				; 0xed7f undefined
	

; opcodes 0xed80 - 0xed9f undefined
; 0xeda0
;	ldi
;	cpi
;	ini
;	outi
	nop				; 0xeda4 undefined
	nop				; 0xeda5 undefined
	nop				; 0xeda6 undefined
	nop				; 0xeda7 undefined
;	ldd
;	cpd
;	ind
;	outd
	nop				; 0xedac undefined
	nop				; 0xedad undefined
	nop				; 0xedae undefined
	nop				; 0xedaf undefined
	

; 0xedb0
;	ldir
;	cpir
;	inir
;	otir
	nop				; 0xedb4 undefined
	nop				; 0xedb5 undefined
	nop				; 0xedb6 undefined
	nop				; 0xedb7 undefined
;	lddr
;	cpdr
;	indr
;	otdr
	nop				; 0xedbc undefined
	nop				; 0xedbd undefined
	nop				; 0xedbe undefined
	nop				; 0xedbf undefined
	

; opcodes 0xedc0 - 0xedff undefined

; IX instructions - 0xdd prefix
; 0xdd00
	nop				; 0xdd00 undefined
	nop				; 0xdd01 undefined
	nop				; 0xdd02 undefined
	nop				; 0xdd03 undefined
	nop				; 0xdd04 same as 0x04
	nop				; 0xdd05 same as 0x05
	nop				; 0xdd06 same as 0x06
	nop				; 0xdd07 undefined
	nop				; 0xdd08 undefined
;	add IX,BC
	nop				; 0xdd0a undefined
	nop				; 0xdd0b undefined
	nop				; 0xdd0c same as 0x0c
	nop				; 0xdd0d same as 0x0d
	nop				; 0xdd0e same as 0x0e
	nop				; 0xdd0f undefined
	

; 0xdd10
	nop				; 0xdd10 undefined
	nop				; 0xdd11 undefined
	nop				; 0xdd12 undefined
	nop				; 0xdd13 undefined
	nop				; 0xdd14 same as 0x14
	nop				; 0xdd15 same as 0x15
	nop				; 0xdd16 same as 0x16
	nop				; 0xdd17 undefined
	nop				; 0xdd18 undefined
;	add IX,DE
	nop				; 0xdd1a undefined
	nop				; 0xdd1b undefined
	nop				; 0xdd1c same as 0x1c
	nop				; 0xdd1d same as 0x1d
	nop				; 0xdd1e same as 0x1e
	nop				; 0xdd1f undefined
	

; 0xdd20
	nop				; 0xdd20 undefined
	ld  IX,$1234
	ld  (results+21),IX
	inc IX
;	inc IXH              
;	dec IXH
	ld  IXH,$7f
	nop				; 0xdd27 undefined
	nop				; 0xdd28 undefined
;	add IX,IX
	ld  IX,(results+23)
	dec IX
;	inc IXL               
;	dec IXL
	ld  IXL,$80
	nop				; 0xdd2f undefined
	

; 0xdd30
	nop				; 0xdd30 undefined
	nop				; 0xdd31 undefined
	nop				; 0xdd32 undefined
	nop				; 0xdd33 undefined
;	inc (IX - $01)
;	dec (IX - $03)
	ld	IX,results
	ld  (IX+$1b),$ab
	nop				; 0xdd37 undefined
	nop				; 0xdd38 undefined
;	add IX,SP
	nop				; 0xdd3a undefined
	nop				; 0xdd3b undefined
	nop				; 0xdd3c same as 0x3c
	nop				; 0xdd3d same as 0x3d
	nop				; 0xdd3e same as 0x3e
	nop				; 0xdd3f undefined
	

; 0xdd40
	nop				; 0xdd40 same as 0x40
	nop				; 0xdd41 same as 0x41
	nop				; 0xdd42 same as 0x42
	nop				; 0xdd43 same as 0x43
	ld  B,IXH
	ld  B,IXL
	ld  B,(IX-$01)
	nop				; 0xdd47 same as 0x47
	nop				; 0xdd48 same as 0x48
	nop				; 0xdd49 same as 0x49
	nop				; 0xdd4a same as 0x4a
	nop				; 0xdd4b same as 0x4b
	ld  C,IXH 
	ld  C,IXL 
	ld  C,(IX-$02)
	nop				; 0xdd4f same as 0x4f
	

; 0xdd50
	nop				; 0xdd50 same as 0x50
	nop				; 0xdd51 same as 0x51
	nop				; 0xdd52 same as 0x52
	nop				; 0xdd53 same as 0x53
	ld  D,IXH
	ld  D,IXL
	ld  D,(IX-$01)
	nop				; 0xdd57 same as 0x57
	nop				; 0xdd58 same as 0x58
	nop				; 0xdd59 same as 0x59
	nop				; 0xdd5a same as 0x5a
	nop				; 0xdd5b same as 0x5b
	ld  E,IXH 
	ld  E,IXL 
	ld  E,(IX-$02)
	nop				; 0xdd5f same as 0x5f
	

; 0xdd60
	ld  IXH,B           
	ld  IXH,C           
	ld  IXH,D           
	ld  IXH,E           
	ld  IXH,IXH           
	ld  IXH,IXL          
	ld  H,(IX-$05)
	ld  IXH,A          
	ld  IXL,B           
	ld  IXL,C           
	ld  IXL,D           
	ld  IXL,E           
	ld  IXL,IXH           
	ld  IXL,IXL          
	ld  L,(IX-$06)
	ld  IXL,A       
	

; 0xdd70
	ld	IX,results	;setup
	ld  (IX+$19),B
	ld  (IX+$1b),C
	ld  (IX+$1d),D
	ld  (IX+$1f),E
	ld  (IX+$21),H
	ld  (IX+$23),L
	nop				; 0xdd76 undefined
	ld  (IX+$25),A
	nop				; 0xdd78 same as 0x78
	nop				; 0xdd79 same as 0x79
	nop				; 0xdd7a same as 0x7a
	nop				; 0xdd7b same as 0x7b
	ld  A,IXH 
	ld  A,IXL 
	ld  A,(IX+$16)
	nop				; 0xdd7f same as 0x7f

; 0xdd80
	nop				; 0xdd80 same as 0x80
	nop				; 0xdd81 same as 0x81
	nop				; 0xdd82 same as 0x82
	nop				; 0xdd83 same as 0x83
;	add  A,IXH
;	add  A,IXL
;	add  A,(IX-$07)
	nop				; 0xdd87 same as 0x87
	nop				; 0xdd88 same as 0x88
	nop				; 0xdd89 same as 0x89
	nop				; 0xdd8a same as 0x8a
	nop				; 0xdd8b same as 0x8b
;	adc  A,IXH 
;	adc  A,IXL 
;	adc  A,(IX+$02)
	nop				; 0xdd8f same as 0x8f
	

; 0xdd90
	nop				; 0xdd90 same as 0x90
	nop				; 0xdd91 same as 0x91
	nop				; 0xdd92 same as 0x92
	nop				; 0xdd93 same as 0x93
;	sub  A,IXH
;	sub  A,IXL
;	sub  A,(IX-$07)
	nop				; 0xdd97 same as 0x97
	nop				; 0xdd98 same as 0x98
	nop				; 0xdd99 same as 0x99
	nop				; 0xdd9a same as 0x9a
	nop				; 0xdd9b same as 0x9b
;	sbc  A,IXH 
;	sbc  A,IXL 
;	sbc  A,(IX+$02)
	nop				; 0xdd9f same as 0x9f
	

; 0xdda0
	nop				; 0xdda0 same as 0xa0
	nop				; 0xdda1 same as 0xa1
	nop				; 0xdda2 same as 0xa2
	nop				; 0xdda3 same as 0xa3
;	and  A,IXH
;	and  A,IXL
;	and  A,(IX-$07)
	nop				; 0xdda7 same as 0xa7
	nop				; 0xdda8 same as 0xa8
	nop				; 0xdda9 same as 0xa9
	nop				; 0xddaa same as 0xaa
	nop				; 0xddab same as 0xab
;	xor  A,IXH 
;	xor  A,IXL 
;	xor  A,(IX+$02)
	nop				; 0xddaf same as 0xaf
	

; 0xddb0
	nop				; 0xddb0 same as 0xb0
	nop				; 0xddb1 same as 0xb1
	nop				; 0xddb2 same as 0xb2
	nop				; 0xddb3 same as 0xb3
;	or  A,IXH
;	or  A,IXL
;	or  A,(IX-$07)
	nop				; 0xddb7 same as 0xb7
	nop				; 0xddb8 same as 0xb8
	nop				; 0xddb9 same as 0xb9
	nop				; 0xddba same as 0xba
	nop				; 0xddbb same as 0xbb
;	cp  A,IXH 
;	cp  A,IXL 
;	cp  A,(IX+$02)
	nop				; 0xddbf same as 0xbf
	

; 0xddc0
	nop				; 0xddc0 undefined
	nop				; 0xddc1 undefined
	nop				; 0xddc2 undefined
	nop				; 0xddc3 undefined
	nop				; 0xddc4 undefined
	nop				; 0xddc5 undefined
	nop				; 0xddc6 undefined
	nop				; 0xddc7 undefined
	nop				; 0xddc8 undefined
	nop				; 0xddc9 undefined
	nop				; 0xddca undefined
	nop				; 0xddcb - IX Bit instructions
	nop				; 0xddcc undefined
	nop				; 0xddcd undefined
	nop				; 0xddce undefined
	nop				; 0xddcf undefined
	

; 0xddd0
	nop				; 0xddd0 undefined
	nop				; 0xddd1 undefined
	nop				; 0xddd2 undefined
	nop				; 0xddd3 undefined
	nop				; 0xddd4 undefined
	nop				; 0xddd5 undefined
	nop				; 0xddd6 undefined
	nop				; 0xddd7 undefined
	nop				; 0xddd8 undefined
	nop				; 0xddd9 undefined
	nop				; 0xddda undefined
	nop				; 0xdddb undefined
	nop				; 0xdddc undefined
	nop				; 0xdddd undefined
	nop				; 0xddde undefined
	nop				; 0xdddf undefined
	

; 0xdde0
	nop				; 0xdde0 undefined
	pop  IX
	nop				; 0xdde2 undefined
	ex   (SP),IX
	nop				; 0xdde4 undefined
	push IX
	nop				; 0xdde6 undefined
	nop				; 0xdde7 undefined
	nop				; 0xdde8 undefined
	ld IX,after_IX	; set up jp (IX) opcode
	jp   (IX)
	nop				; 0xddea undefined
	nop				; 0xddeb undefined
	nop				; 0xddec undefined
	nop				; 0xdded undefined
	nop				; 0xddee undefined
	nop				; 0xddef undefined
after_IX:
	

; 0xddf0
	nop				; 0xddf0 undefined
	nop				; 0xddf1 undefined
	nop				; 0xddf2 undefined
	nop				; 0xddf3 undefined
	nop				; 0xddf4 undefined
	nop				; 0xddf5 undefined
	nop				; 0xddf6 undefined
	nop				; 0xddf7 undefined
	nop				; 0xddf8 undefined
	ld	IX,results+$38	;setup 
	ld  SP,IX
	nop				; 0xddfa undefined
	nop				; 0xddfb undefined
	nop				; 0xddfc undefined
	nop				; 0xddfd undefined
	nop				; 0xddfe undefined
	nop				; 0xddff undefined
	

; 0xddcb - IX bit instructions
; 0xddcbss00 -- all affect flags
;	rlc (IX+$7D),B
;	rlc (IX+$7E),C
;	rlc (IX+$7F),D
;	rlc (IX-$70),E
;	rlc (IX-$72),H
;	rlc (IX-$73),L
;	rlc (IX+$22)
;	rlc (IX-$74),A
;	rrc (IX+$7D),B
;	rrc (IX+$7E),C
;	rrc (IX+$7F),D
;	rrc (IX-$70),E
;	rrc (IX-$72),H
;	rrc (IX-$73),L
;	rrc (IX+$22)
;	rrc (IX-$74),A

; 0xddcbss10 -- all affect flags
;	rl (IX+$7D),B
;	rl (IX+$7E),C
;	rl (IX+$7F),D
;	rl (IX-$70),E
;	rl (IX-$72),H
;	rl (IX-$73),L
;	rl (IX+$22)
;	rl (IX-$74),A
;	rr (IX+$7D),B
;	rr (IX+$7E),C
;	rr (IX+$7F),D
;	rr (IX-$70),E
;	rr (IX-$72),H
;	rr (IX-$73),L
;	rr (IX+$22)
;	rr (IX-$74),A

; 0xddcbss20 -- all affect flags
;	sla (IX+$7D),B
;	sla (IX+$7E),C
;	sla (IX+$7F),D
;	sla (IX-$70),E
;	sla (IX-$72),H
;	sla (IX-$73),L
;	sla (IX+$22)
;	sla (IX-$74),A
;	sra (IX+$7D),B
;	sra (IX+$7E),C
;	sra (IX+$7F),D
;	sra (IX-$70),E
;	sra (IX-$72),H
;	sra (IX-$73),L
;	sra (IX+$22)
;	sra (IX-$74),A

; 0xddcbss30 -- all affect flags
;	sll (IX+$7D),B
;	sll (IX+$7E),C
;	sll (IX+$7F),D
;	sll (IX-$70),E
;	sll (IX-$72),H
;	sll (IX-$73),L
;	sll (IX+$22)
;	sll (IX-$74),A
;	srl (IX+$7D),B
;	srl (IX+$7E),C
;	srl (IX+$7F),D
;	srl (IX-$70),E
;	srl (IX-$72),H
;	srl (IX-$73),L
;	srl (IX+$22)
;	srl (IX-$74),A

; 0xddcbss40 - 0xddcbss7f
	nop				; same as 0xddcbss46
	nop				; same as 0xddcbss46
	nop				; same as 0xddcbss46
	nop				; same as 0xddcbss46
	nop				; same as 0xddcbss46
	nop				; same as 0xddcbss46
;	bit 0,(IX+$01)  
	nop				; same as 0xddcbss46
	nop				; same as 0xddcbss4e
	nop				; same as 0xddcbss4e
	nop				; same as 0xddcbss4e
	nop				; same as 0xddcbss4e
	nop				; same as 0xddcbss4e
	nop				; same as 0xddcbss4e
;	bit 1,(IX+$02)
	nop				; same as 0xddcbss4e
	nop				; same as 0xddcbss56
	nop				; same as 0xddcbss56
	nop				; same as 0xddcbss56
	nop				; same as 0xddcbss56
	nop				; same as 0xddcbss56
	nop				; same as 0xddcbss56
;	bit 2,(IX+$03)
	nop				; same as 0xddcbss56
	nop				; same as 0xddcbss5e
	nop				; same as 0xddcbss5e
	nop				; same as 0xddcbss5e
	nop				; same as 0xddcbss5e
	nop				; same as 0xddcbss5e
	nop				; same as 0xddcbss5e
;	bit 3,(IX+$04)
	nop				; same as 0xddcbss5e
	nop				; same as 0xddcbss66
	nop				; same as 0xddcbss66
	nop				; same as 0xddcbss66
	nop				; same as 0xddcbss66
	nop				; same as 0xddcbss66
	nop				; same as 0xddcbss66
;	bit 4,(IX+$05)
	nop				; same as 0xddcbss66
	nop				; same as 0xddcbss6e
	nop				; same as 0xddcbss6e
	nop				; same as 0xddcbss6e
	nop				; same as 0xddcbss6e
	nop				; same as 0xddcbss6e
	nop				; same as 0xddcbss6e
;	bit 5,(IX+$06)
	nop				; same as 0xddcbss6e
	nop				; same as 0xddcbss76
	nop				; same as 0xddcbss76
	nop				; same as 0xddcbss76
	nop				; same as 0xddcbss76
	nop				; same as 0xddcbss76
	nop				; same as 0xddcbss76
;	bit 6,(IX+$07)
	nop				; same as 0xddcbss76
	nop				; same as 0xddcbss7e
	nop				; same as 0xddcbss7e
	nop				; same as 0xddcbss7e
	nop				; same as 0xddcbss7e
	nop				; same as 0xddcbss7e
	nop				; same as 0xddcbss7e
;	bit 7,(IX+$08)
	nop				; same as 0xddcbss7e
	

; 0xddcbss80 - 0xddcbssbf
	res 0,(IX+$01),B   
	res 0,(IX+$02),C    
	res 0,(IX+$03),D    
	res 0,(IX+$04),E    
	res 0,(IX-$40),H    
	res 0,(IX-$41),L    
	res 0,(IX+$22)
	res 0,(IX-$42),A    
	res 1,(IX+$01),B   
	res 1,(IX+$02),C    
	res 1,(IX+$03),D    
	res 1,(IX+$04),E    
	res 1,(IX-$40),H    
	res 1,(IX-$41),L    
	res 1,(IX+$22)
	res 1,(IX-$42),A    
	res 2,(IX+$01),B   
	res 2,(IX+$02),C    
	res 2,(IX+$03),D    
	res 2,(IX+$04),E    
	res 2,(IX-$40),H    
	res 2,(IX-$41),L    
	res 2,(IX+$22)
	res 2,(IX-$42),A    
	res 3,(IX+$01),B   
	res 3,(IX+$02),C    
	res 3,(IX+$03),D    
	res 3,(IX+$04),E    
	res 3,(IX-$40),H    
	res 3,(IX-$41),L    
	res 3,(IX+$22)
	res 3,(IX-$42),A    
	res 4,(IX+$01),B   
	res 4,(IX+$02),C    
	res 4,(IX+$03),D    
	res 4,(IX+$04),E    
	res 4,(IX-$40),H    
	res 4,(IX-$41),L    
	res 4,(IX+$22)
	res 4,(IX-$42),A    
	res 5,(IX+$01),B   
	res 5,(IX+$02),C    
	res 5,(IX+$03),D    
	res 5,(IX+$04),E    
	res 5,(IX-$40),H    
	res 5,(IX-$41),L    
	res 5,(IX+$22)
	res 5,(IX-$42),A    
	res 6,(IX+$01),B   
	res 6,(IX+$02),C    
	res 6,(IX+$03),D    
	res 6,(IX+$04),E    
	res 6,(IX-$40),H    
	res 6,(IX-$41),L    
	res 6,(IX+$22)
	res 6,(IX-$42),A    
	res 7,(IX+$01),B   
	res 7,(IX+$02),C    
	res 7,(IX+$03),D    
	res 7,(IX+$04),E    
	res 7,(IX-$40),H    
	res 7,(IX-$41),L    
	res 7,(IX+$22)
	res 7,(IX-$42),A    
	

; 0xddcbssc0 - 0xddcbssff
	set 0,(IX+$01),B   
	set 0,(IX+$02),C    
	set 0,(IX+$03),D    
	set 0,(IX+$04),E    
	set 0,(IX-$40),H    
	set 0,(IX-$41),L   
	set 0,(IX+$22) 
	set 0,(IX-$42),A    
	set 1,(IX+$01),B   
	set 1,(IX+$02),C    
	set 1,(IX+$03),D    
	set 1,(IX+$04),E    
	set 1,(IX-$40),H    
	set 1,(IX-$41),L   
	set 1,(IX+$22) 
	set 1,(IX-$42),A    
	set 2,(IX+$01),B   
	set 2,(IX+$02),C    
	set 2,(IX+$03),D    
	set 2,(IX+$04),E    
	set 2,(IX-$40),H    
	set 2,(IX-$41),L   
	set 2,(IX+$22) 
	set 2,(IX-$42),A    
	set 3,(IX+$01),B   
	set 3,(IX+$02),C    
	set 3,(IX+$03),D    
	set 3,(IX+$04),E    
	set 3,(IX-$40),H    
	set 3,(IX-$41),L   
	set 3,(IX+$22) 
	set 3,(IX-$42),A    
	set 4,(IX+$01),B   
	set 4,(IX+$02),C    
	set 4,(IX+$03),D    
	set 4,(IX+$04),E    
	set 4,(IX-$40),H    
	set 4,(IX-$41),L   
	set 4,(IX+$22) 
	set 4,(IX-$42),A    
	set 5,(IX+$01),B   
	set 5,(IX+$02),C    
	set 5,(IX+$03),D    
	set 5,(IX+$04),E    
	set 5,(IX-$40),H    
	set 5,(IX-$41),L   
	set 5,(IX+$22) 
	set 5,(IX-$42),A    
	set 6,(IX+$01),B   
	set 6,(IX+$02),C    
	set 6,(IX+$03),D    
	set 6,(IX+$04),E    
	set 6,(IX-$40),H    
	set 6,(IX-$41),L   
	set 6,(IX+$22) 
	set 6,(IX-$42),A    
	set 7,(IX+$01),B   
	set 7,(IX+$02),C    
	set 7,(IX+$03),D    
	set 7,(IX+$04),E    
	set 7,(IX-$40),H    
	set 7,(IX-$41),L   
	set 7,(IX+$22) 
	set 7,(IX-$42),A    
	

; IY instructions - 0xfd prefix
; 0xfd00
	nop				; 0xfd00 undefined
	nop				; 0xfd01 undefined
	nop				; 0xfd02 undefined
	nop				; 0xfd03 undefined
	nop				; 0xfd04 same as 0x04
	nop				; 0xfd05 same as 0x05
	nop				; 0xfd06 same as 0x06
	nop				; 0xfd07 undefined
	nop				; 0xfd08 undefined
;	add IY,BC
	nop				; 0xfd0a undefined
	nop				; 0xfd0b undefined
	nop				; 0xfd0c same as 0x0c
	nop				; 0xfd0d same as 0x0d
	nop				; 0xfd0e same as 0x0e
	nop				; 0xfd0f undefined

; 0xfd10
	nop				; 0xfd10 undefined
	nop				; 0xfd11 undefined
	nop				; 0xfd12 undefined
	nop				; 0xfd13 undefined
	nop				; 0xfd14 same as 0x14
	nop				; 0xfd15 same as 0x15
	nop				; 0xfd16 same as 0x16
	nop				; 0xfd17 undefined
	nop				; 0xfd18 undefined
;	add IY,DE
	nop				; 0xfd1a undefined
	nop				; 0xfd1b undefined
	nop				; 0xfd1c same as 0x1c
	nop				; 0xfd1d same as 0x1d
	nop				; 0xfd1e same as 0x1e
	nop				; 0xfd1f undefined
	

; 0xfd20
	nop				; 0xfd20 undefined
	ld  IY,$1234
	ld  (results+80),IY
	inc IY
;	inc IYH              
;	dec IYH
	ld  IYH,$7f
	nop				; 0xfd27 undefined
	nop				; 0xfd28 undefined
;	add IY,IY
	ld  IY,($3456)
	dec IY
;	inc IYL               
;	dec IYL
	ld  IYL,$80
	nop				; 0xfd2f undefined
	

; 0xfd30
	nop				; 0xfd30 undefined
	nop				; 0xfd31 undefined
	nop				; 0xfd32 undefined
	nop				; 0xfd33 undefined
;	inc (IY - $01)
;	dec (IY - $03)
	ld	IY,results+$90		; setup
	ld  (IY+42),$ab
	nop				; 0xfd37 undefined
	nop				; 0xfd38 undefined
;	add IY,SP
	nop				; 0xfd3a undefined
	nop				; 0xfd3b undefined
	nop				; 0xfd3c same as 0x3c
	nop				; 0xfd3d same as 0x3d
	nop				; 0xfd3e same as 0x3e
	nop				; 0xfd3f undefined
	

; 0xfd40
	nop				; 0xfd40 same as 0x40
	nop				; 0xfd41 same as 0x41
	nop				; 0xfd42 same as 0x42
	nop				; 0xfd43 same as 0x43
	ld  B,IYH
	ld  B,IYL
	ld  B,(IY-$01)
	nop				; 0xfd47 same as 0x47
	nop				; 0xfd48 same as 0x48
	nop				; 0xfd49 same as 0x49
	nop				; 0xfd4a same as 0x4a
	nop				; 0xfd4b same as 0x4b
	ld  C,IYH 
	ld  C,IYL 
	ld  C,(IY-$02)
	nop				; 0xfd4f same as 0x4f
	

; 0xfd50
	nop				; 0xfd50 same as 0x50
	nop				; 0xfd51 same as 0x51
	nop				; 0xfd52 same as 0x52
	nop				; 0xfd53 same as 0x53
	ld  D,IYH
	ld  D,IYL
	ld  D,(IY-$01)
	nop				; 0xfd57 same as 0x57
	nop				; 0xfd58 same as 0x58
	nop				; 0xfd59 same as 0x59
	nop				; 0xfd5a same as 0x5a
	nop				; 0xfd5b same as 0x5b
	ld  E,IYH 
	ld  E,IYL 
	ld  E,(IY-$02)
	nop				; 0xfd5f same as 0x5f
	

; 0xfd60
	ld  IYH,B           
	ld  IYH,C           
	ld  IYH,D           
	ld  IYH,E           
	ld  IYH,IYH           
	ld  IYH,IYL          
	ld  H,(IY-$05)
	ld  IYH,A          
	ld  IYL,B           
	ld  IYL,C           
	ld  IYL,D           
	ld  IYL,E           
	ld  IYL,IYH           
	ld  IYL,IYL          
	ld  L,(IY-$06)
	ld  IYL,A       
	

; 0xfd70
	ld	IY,results	;setup
	ld  (IY+43),B
	ld  (IY+44),C
	ld  (IY+45),D
	ld  (IY+46),E
	ld  (IY+47),H
	ld  (IY+48),L
	nop				; 0xfd76 undefined
	ld  (IY+49),A
	nop				; 0xfd78 same as 0x78
	nop				; 0xfd79 same as 0x79
	nop				; 0xfd7a same as 0x7a
	nop				; 0xfd7b same as 0x7b
	ld  A,IYH 
	ld  A,IYL 
	ld  A,(IY+22)
	nop				; 0xfd7f same as 0x7f
	

; 0xfd80 -- all affect flags
;	nop				; 0xfd80 same as 0x80
;	nop				; 0xfd81 same as 0x81
;	nop				; 0xfd82 same as 0x82
;	nop				; 0xfd83 same as 0x83
;	add  A,IYH
;	add  A,IYL
;	add  A,(IY-$07)
;	nop				; 0xfd87 same as 0x87
;	nop				; 0xfd88 same as 0x88
;	nop				; 0xfd89 same as 0x89
;	nop				; 0xfd8a same as 0x8a
;	nop				; 0xfd8b same as 0x8b
;	adc  A,IYH 
;	adc  A,IYL 
;	adc  A,(IY+$02)
;	nop				; 0xfd8f same as 0x8f
	

; 0xfd90 -- all affect flags
;	nop				; 0xfd90 same as 0x90
;	nop				; 0xfd91 same as 0x91
;	nop				; 0xfd92 same as 0x92
;	nop				; 0xfd93 same as 0x93
;	sub  A,IYH
;	sub  A,IYL
;	sub  A,(IY-$07)
;	nop				; 0xfd97 same as 0x97
;	nop				; 0xfd98 same as 0x98
;	nop				; 0xfd99 same as 0x99
;	nop				; 0xfd9a same as 0x9a
;	nop				; 0xfd9b same as 0x9b
;	sbc  A,IYH 
;	sbc  A,IYL 
;	sbc  A,(IY+$02)
;	nop				; 0xfd9f same as 0x9f
	

; 0xfda0 -- all affect flags
;	nop				; 0xfda0 same as 0xa0
;	nop				; 0xfda1 same as 0xa1
;	nop				; 0xfda2 same as 0xa2
;	nop				; 0xfda3 same as 0xa3
;	and  A,IYH
;	and  A,IYL
;	and  A,(IY-$07)
;	nop				; 0xfda7 same as 0xa7
;	nop				; 0xfda8 same as 0xa8
;	nop				; 0xfda9 same as 0xa9
;	nop				; 0xfdaa same as 0xaa
;	nop				; 0xfdab same as 0xab
;	xor  A,IYH 
;	xor  A,IYL 
;	xor  A,(IY+$02)
;	nop				; 0xfdaf same as 0xaf
	

; 0xfdb0 -- all affect flags
;	nop				; 0xfdb0 same as 0xb0
;	nop				; 0xfdb1 same as 0xb1
;	nop				; 0xfdb2 same as 0xb2
;	nop				; 0xfdb3 same as 0xb3
;	or  A,IYH
;	or  A,IYL
;	or  A,(IY-$07)
;	nop				; 0xfdb7 same as 0xb7
;	nop				; 0xfdb8 same as 0xb8
;	nop				; 0xfdb9 same as 0xb9
;	nop				; 0xfdba same as 0xba
;	nop				; 0xfdbb same as 0xbb
;	cp  A,IYH 
;	cp  A,IYL 
;	cp  A,(IY+$02)
;	nop				; 0xfdbf same as 0xbf
	

; 0xfdc0
	nop				; 0xfdc0 undefined
	nop				; 0xfdc1 undefined
	nop				; 0xfdc2 undefined
	nop				; 0xfdc3 undefined
	nop				; 0xfdc4 undefined
	nop				; 0xfdc5 undefined
	nop				; 0xfdc6 undefined
	nop				; 0xfdc7 undefined
	nop				; 0xfdc8 undefined
	nop				; 0xfdc9 undefined
	nop				; 0xfdca undefined
	nop				; 0xfdcb - IY Bit instructions
	nop				; 0xfdcc undefined
	nop				; 0xfdcd undefined
	nop				; 0xfdce undefined
	nop				; 0xfdcf undefined

; 0xfdd0
	nop				; 0xfdd0 undefined
	nop				; 0xfdd1 undefined
	nop				; 0xfdd2 undefined
	nop				; 0xfdd3 undefined
	nop				; 0xfdd4 undefined
	nop				; 0xfdd5 undefined
	nop				; 0xfdd6 undefined
	nop				; 0xfdd7 undefined
	nop				; 0xfdd8 undefined
	nop				; 0xfdd9 undefined
	nop				; 0xfdda undefined
	nop				; 0xfddb undefined
	nop				; 0xfddc undefined
	nop				; 0xfddd undefined
	nop				; 0xfdde undefined
	nop				; 0xfddf undefined

; 0xfde0
	nop				; 0xfde0 undefined
	pop  IY
	nop				; 0xfde2 undefined
	ex   (SP),IY
	nop				; 0xfde4 undefined
	push IY
	nop				; 0xfde6 undefined
	nop				; 0xfde7 undefined
	nop				; 0xfde8 undefined
	ld IY,after_IY	;setup jp (IY) instruction
	jp   (IY)
	nop				; 0xfdea undefined
	nop				; 0xfdeb undefined
	nop				; 0xfdec undefined
	nop				; 0xfded undefined
	nop				; 0xfdee undefined
	nop				; 0xfdef undefined
after_IY:
	

; 0xfdf0
	nop				; 0xfdf0 undefined
	nop				; 0xfdf1 undefined
	nop				; 0xfdf2 undefined
	nop				; 0xfdf3 undefined
	nop				; 0xfdf4 undefined
	nop				; 0xfdf5 undefined
	nop				; 0xfdf6 undefined
	nop				; 0xfdf7 undefined
	nop				; 0xfdf8 undefined
	ld IY,results+$a0		;setup
	ld  SP,IY
	nop				; 0xfdfa undefined
	nop				; 0xfdfb undefined
	nop				; 0xfdfc undefined
	nop				; 0xfdfd undefined
	nop				; 0xfdfe undefined
	nop				; 0xfdff undefined
	

; 0xfdcb - IY bit instructions
; 0xfdcbss00 -- all affect flags
;	rlc (IY+$7D),B
;	rlc (IY+$7E),C
;	rlc (IY+$7F),D
;	rlc (IY-$70),E
;	rlc (IY-$72),H
;	rlc (IY-$73),L
;	rlc (IY+$22)
;	rlc (IY-$74),A
;	rrc (IY+$7D),B
;	rrc (IY+$7E),C
;	rrc (IY+$7F),D
;	rrc (IY-$70),E
;	rrc (IY-$72),H
;	rrc (IY-$73),L
;	rrc (IY+$22)
;	rrc (IY-$74),A

; 0xfdcbss10 -- all affect flags
;	rl (IY+$7D),B
;	rl (IY+$7E),C
;	rl (IY+$7F),D
;	rl (IY-$70),E
;	rl (IY-$72),H
;	rl (IY-$73),L
;	rl (IY+$22)
;	rl (IY-$74),A
;	rr (IY+$7D),B
;	rr (IY+$7E),C
;	rr (IY+$7F),D
;	rr (IY-$70),E
;	rr (IY-$72),H
;	rr (IY-$73),L
;	rr (IY+$22)
;	rr (IY-$74),A

; 0xfdcbss20 -- all affect flags
;	sla (IY+$7D),B
;	sla (IY+$7E),C
;	sla (IY+$7F),D
;	sla (IY-$70),E
;	sla (IY-$72),H
;	sla (IY-$73),L
;	sla (IY+$22)
;	sla (IY-$74),A
;	sra (IY+$7D),B
;	sra (IY+$7E),C
;	sra (IY+$7F),D
;	sra (IY-$70),E
;	sra (IY-$72),H
;	sra (IY-$73),L
;	sra (IY+$22)
;	sra (IY-$74),A

; 0xfdcbss30 -- all affect flags
;	sll (IY+$7D),B
;	sll (IY+$7E),C
;	sll (IY+$7F),D
;	sll (IY-$70),E
;	sll (IY-$72),H
;	sll (IY-$73),L
;	sll (IY+$22)
;	sll (IY-$74),A
;	srl (IY+$7D),B
;	srl (IY+$7E),C
;	srl (IY+$7F),D
;	srl (IY-$70),E
;	srl (IY-$72),H
;	srl (IY-$73),L
;	srl (IY+$22)
;	srl (IY-$74),A

; 0xfdcbss40 - 0xfdcbss7f
	nop				; same as 0xfdcbss46
	nop				; same as 0xfdcbss46
	nop				; same as 0xfdcbss46
	nop				; same as 0xfdcbss46
	nop				; same as 0xfdcbss46
	nop				; same as 0xfdcbss46
;	bit 0,(IY+$01)  
	nop				; same as 0xfdcbss46
	nop				; same as 0xfdcbss4e
	nop				; same as 0xfdcbss4e
	nop				; same as 0xfdcbss4e
	nop				; same as 0xfdcbss4e
	nop				; same as 0xfdcbss4e
	nop				; same as 0xfdcbss4e
;	bit 1,(IY+$02)
	nop				; same as 0xfdcbss4e
	nop				; same as 0xfdcbss56
	nop				; same as 0xfdcbss56
	nop				; same as 0xfdcbss56
	nop				; same as 0xfdcbss56
	nop				; same as 0xfdcbss56
	nop				; same as 0xfdcbss56
;	bit 2,(IY+$03)
	nop				; same as 0xfdcbss56
	nop				; same as 0xfdcbss5e
	nop				; same as 0xfdcbss5e
	nop				; same as 0xfdcbss5e
	nop				; same as 0xfdcbss5e
	nop				; same as 0xfdcbss5e
	nop				; same as 0xfdcbss5e
;	bit 3,(IY+$04)
	nop				; same as 0xfdcbss5e
	nop				; same as 0xfdcbss66
	nop				; same as 0xfdcbss66
	nop				; same as 0xfdcbss66
	nop				; same as 0xfdcbss66
	nop				; same as 0xfdcbss66
	nop				; same as 0xfdcbss66
;	bit 4,(IY+$05)
	nop				; same as 0xfdcbss66
	nop				; same as 0xfdcbss6e
	nop				; same as 0xfdcbss6e
	nop				; same as 0xfdcbss6e
	nop				; same as 0xfdcbss6e
	nop				; same as 0xfdcbss6e
	nop				; same as 0xfdcbss6e
;	bit 5,(IY+$06)
	nop				; same as 0xfdcbss6e
	nop				; same as 0xfdcbss76
	nop				; same as 0xfdcbss76
	nop				; same as 0xfdcbss76
	nop				; same as 0xfdcbss76
	nop				; same as 0xfdcbss76
	nop				; same as 0xfdcbss76
;	bit 6,(IY+$07)
	nop				; same as 0xfdcbss76
	nop				; same as 0xfdcbss7e
	nop				; same as 0xfdcbss7e
	nop				; same as 0xfdcbss7e
	nop				; same as 0xfdcbss7e
	nop				; same as 0xfdcbss7e
	nop				; same as 0xfdcbss7e
;	bit 7,(IY+$08)
	nop				; same as 0xfdcbss7e
	

; 0xfdcbss80 - 0xfdcbssbf
	res 0,(IY+$01),B   
	res 0,(IY+$02),C    
	res 0,(IY+$03),D    
	res 0,(IY+$04),E    
	res 0,(IY-$40),H    
	res 0,(IY-$41),L    
	res 0,(IY+$22)
	res 0,(IY-$42),A    
	res 1,(IY+$01),B   
	res 1,(IY+$02),C    
	res 1,(IY+$03),D    
	res 1,(IY+$04),E    
	res 1,(IY-$40),H    
	res 1,(IY-$41),L    
	res 1,(IY+$22)
	res 1,(IY-$42),A    
	res 2,(IY+$01),B   
	res 2,(IY+$02),C    
	res 2,(IY+$03),D    
	res 2,(IY+$04),E    
	res 2,(IY-$40),H    
	res 2,(IY-$41),L    
	res 2,(IY+$22)
	res 2,(IY-$42),A    
	res 3,(IY+$01),B   
	res 3,(IY+$02),C    
	res 3,(IY+$03),D    
	res 3,(IY+$04),E    
	res 3,(IY-$40),H    
	res 3,(IY-$41),L    
	res 3,(IY+$22)
	res 3,(IY-$42),A    
	res 4,(IY+$01),B   
	res 4,(IY+$02),C    
	res 4,(IY+$03),D    
	res 4,(IY+$04),E    
	res 4,(IY-$40),H    
	res 4,(IY-$41),L    
	res 4,(IY+$22)
	res 4,(IY-$42),A    
	res 5,(IY+$01),B   
	res 5,(IY+$02),C    
	res 5,(IY+$03),D    
	res 5,(IY+$04),E    
	res 5,(IY-$40),H    
	res 5,(IY-$41),L    
	res 5,(IY+$22)
	res 5,(IY-$42),A    
	res 6,(IY+$01),B   
	res 6,(IY+$02),C    
	res 6,(IY+$03),D    
	res 6,(IY+$04),E    
	res 6,(IY-$40),H    
	res 6,(IY-$41),L    
	res 6,(IY+$22)
	res 6,(IY-$42),A    
	res 7,(IY+$01),B   
	res 7,(IY+$02),C    
	res 7,(IY+$03),D    
	res 7,(IY+$04),E    
	res 7,(IY-$40),H    
	res 7,(IY-$41),L    
	res 7,(IY+$22)
	res 7,(IY-$42),A    
	

; 0xfdcbssc0 - 0xfdcbssff
	set 0,(IY+$01),B   
	set 0,(IY+$02),C    
	set 0,(IY+$03),D    
	set 0,(IY+$04),E    
	set 0,(IY-$40),H    
	set 0,(IY-$41),L   
	set 0,(IY+$22) 
	set 0,(IY-$42),A    
	set 1,(IY+$01),B   
	set 1,(IY+$02),C    
	set 1,(IY+$03),D    
	set 1,(IY+$04),E    
	set 1,(IY-$40),H    
	set 1,(IY-$41),L   
	set 1,(IY+$22) 
	set 1,(IY-$42),A    
	set 2,(IY+$01),B   
	set 2,(IY+$02),C    
	set 2,(IY+$03),D    
	set 2,(IY+$04),E    
	set 2,(IY-$40),H    
	set 2,(IY-$41),L   
	set 2,(IY+$22) 
	set 2,(IY-$42),A    
	set 3,(IY+$01),B   
	set 3,(IY+$02),C    
	set 3,(IY+$03),D    
	set 3,(IY+$04),E    
	set 3,(IY-$40),H    
	set 3,(IY-$41),L   
	set 3,(IY+$22) 
	set 3,(IY-$42),A    
	set 4,(IY+$01),B   
	set 4,(IY+$02),C    
	set 4,(IY+$03),D    
	set 4,(IY+$04),E    
	set 4,(IY-$40),H    
	set 4,(IY-$41),L   
	set 4,(IY+$22) 
	set 4,(IY-$42),A    
	set 5,(IY+$01),B   
	set 5,(IY+$02),C    
	set 5,(IY+$03),D    
	set 5,(IY+$04),E    
	set 5,(IY-$40),H    
	set 5,(IY-$41),L   
	set 5,(IY+$22) 
	set 5,(IY-$42),A    
	set 6,(IY+$01),B   
	set 6,(IY+$02),C    
	set 6,(IY+$03),D    
	set 6,(IY+$04),E    
	set 6,(IY-$40),H    
	set 6,(IY-$41),L   
	set 6,(IY+$22) 
	set 6,(IY-$42),A    
	set 7,(IY+$01),B   
	set 7,(IY+$02),C    
	set 7,(IY+$03),D    
	set 7,(IY+$04),E    
	set 7,(IY-$40),H    
	set 7,(IY-$41),L   
	set 7,(IY+$22) 
	set 7,(IY-$42),A    

end_program:
	halt
	jp	end_program


return_point:
	ret


; Return opcodes - tested in separate file
	ret nz
	ret z
	ret
	ret nc
	ret c
	ret po
	ret pe
	ret p
	ret m
	retn
	reti

; djnz - tested in separate file
	djnz $

; rst opcodes - tested in separate file
	rst  $00
	rst  $08
	rst  $10
	rst  $18
	rst  $20
	rst  $28
	rst  $30
	rst  $38

; Define space for compatibility with previous versions of this test file
; Current PC: $0967
	defs $699,$ff
;	org $1000
	defm "Results:"

results:
	defs $ff,'.'		; Results area. Fill with dots....
