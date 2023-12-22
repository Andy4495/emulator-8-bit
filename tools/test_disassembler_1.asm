; Z80 test file for 8-bit emulator
;    https://github.com/Andy4495/emulator-8-bit
;
; 01-Mar-2023  Andy4495 Initial version
; 21-Dec-2023  Andy4495 Update for z88dk compatibility
; 
; See list of opcodes from http://k1.spdns.de/Develop/Projects/zasm/Documentation/z79.htm
; 

    org 0
    
; 8080 instructions supported by Z80
	nop
;   RR = BC DE HL SP
	ld BC,$0002       
	ld DE,$0004  
	ld HL,$0006 
	ld SP,$0008 
	add hl,BC
	add hl,DE
	add hl,HL
	add hl,SP
	inc BC
	inc DE
	inc HL
	inc SP
	dec BC
	dec DE
	dec HL
	dec SP
	 
	ld (bc),a
	ld a,(bc)
	ld (de),a
	ld a,(de)
	ld ($1234),hl
	ld hl,($2345)
	ld ($3456),a
	ld a,($4567)
	
;   R = B C D E H L (HL) A
	inc B           
	inc C          
	inc D          
	inc E           
	inc H           
	inc L          
	inc (HL)       
	inc A          
	dec B
	dec C
	dec D
	dec E
	dec H
	dec L
	dec (HL)
	dec A
	ld B,$89
	ld C,$23
	ld D,$34
	ld E,$45
	ld H,$56
	ld L,$67
	ld (HL),$78
	ld A,$12
	 
	rlca
	rrca
	rla
	rra
	daa
	cpl
	scf
	ccf

	halt

; R = B C D E H L (HL) A
; except ld (hl),(hl) 
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
	ld  (HL),B         
	ld  (HL),C    
	ld  (HL),D         
	ld  (HL),E         
	ld  (HL),H        
	ld  (HL),L        
;	ld  (HL),(HL)      Illegal opcode
	ld  (HL),A        
	ld  A,B         
	ld  A,C    
	ld  A,D         
	ld  A,E         
	ld  A,H        
	ld  A,L        
	ld  A,(HL)      
	ld  A,A        
	                
; R = B C D E H L (HL) A	 
	add a,B
	add a,C
	add a,D
	add a,E
	add a,H
	add a,L
	add a,(HL)
	add a,A
	adc a,B
	adc a,C
	adc a,D
	adc a,E
	adc a,H
	adc a,L
	adc a,(HL)
	adc a,A
	sub a,B
	sub a,C
	sub a,D
	sub a,E
	sub a,H
	sub a,L
	sub a,(HL)
	sub a,A
	sbc a,B
	sbc a,C
	sbc a,D
	sbc a,E
	sbc a,H
	sbc a,L
	sbc a,(HL)
	sbc a,A
	and a,B
	and a,C
	and a,D
	and a,E
	and a,H
	and a,L
	and a,(HL)
	and a,A
	xor a,B
	xor a,C
	xor a,D
	xor a,E
	xor a,H
	xor a,L
	xor a,(HL)
	xor a,A
	or  a,B
	or  a,C
	or  a,D
	or  a,E
	or  a,H
	or  a,L
	or  a,(HL)
	or  a,A
	cp  a,B
	cp  a,C
	cp  a,D
	cp  a,E
	cp  a,H
	cp  a,L
	cp  a,(HL)
	cp  a,A
	 
	ret
; CC = NZ Z NC C PO PE P M
	ret NZ
	ret Z
	ret NC
	ret C
	ret PO
	ret PE
	ret P
	ret M
	jp  $1234
	jp  NZ,$2345
	jp  Z,$3456
	jp  NC,$4567
	jp  C,$5678
	jp  PO,$6789
	jp  PE,$789a
	jp  P,$89ab
	jp  M,$9abc
	call $abcd
	call NZ,$bcde
	call Z,$cdef
	call NC,$4321
	call C,$5432
	call PO,$6543
	call PE,$7654
	call P,$8765
	call M,$9876
	rst  0
	rst  8
	rst  $10
	rst  $18
	rst  $20
	rst  $28
	rst  $30
	rst  $38

; RR = BC DE HL AF	 
	pop  BC         
	pop  DE         
	pop  HL        
	pop  AF         
	push BC 
	push DE 
	push HL 
	push AF 
	 
	add a,$12
	adc a,$23
	sub a,$34
	sbc a,$45
	and a,$56
	xor a,$67
	or  a,$78
	cp  a,$89
	 
	out ($a3),a
	in a,($d6)
	 
	ex (sp),hl
	ex de,hl
	di
	ld sp,hl
	ei

	; Z80-specific instructions not included above
	ex af,af'
	exx
	djnz $
	jr  $
	jr  nz,$
	jr  z,$
	jr  nc,$
	jr  c,$

; R = B C D E H L (HL) A
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
	srl B
	srl C
	srl D
	srl E
	srl H
	srl L
	srl (HL)
	srl A
	bit 0,B
	bit 1,B
	bit 2,B
	bit 3,B
	bit 4,B
	bit 5,B
	bit 6,B
	bit 7,B
	bit 0,C
	bit 1,C
	bit 2,C
	bit 3,C
	bit 4,C
	bit 5,C
	bit 6,C
	bit 7,C
	bit 0,D
	bit 1,D
	bit 2,D
	bit 3,D
	bit 4,D
	bit 5,D
	bit 6,D
	bit 7,D
	bit 0,E
	bit 1,E
	bit 2,E
	bit 3,E
	bit 4,E
	bit 5,E
	bit 6,E
	bit 7,E
	bit 0,H
	bit 1,H
	bit 2,H
	bit 3,H
	bit 4,H
	bit 5,H
	bit 6,H
	bit 7,H
	bit 0,L
	bit 1,L
	bit 2,L
	bit 3,L
	bit 4,L
	bit 5,L
	bit 6,L
	bit 7,L
	bit 0,(HL)
	bit 1,(HL)
	bit 2,(HL)
	bit 3,(HL)
	bit 4,(HL)
	bit 5,(HL)
	bit 6,(HL)
	bit 7,(HL)
	bit 0,A
	bit 1,A
	bit 2,A
	bit 3,A
	bit 4,A
	bit 5,A
	bit 6,A
	bit 7,A
	res 0,B
	res 1,B
	res 2,B
	res 3,B
	res 4,B
	res 5,B
	res 6,B
	res 7,B
	res 0,C
	res 1,C
	res 2,C
	res 3,C
	res 4,C
	res 5,C
	res 6,C
	res 7,C
	res 0,D
	res 1,D
	res 2,D
	res 3,D
	res 4,D
	res 5,D
	res 6,D
	res 7,D
	res 0,E
	res 1,E
	res 2,E
	res 3,E
	res 4,E
	res 5,E
	res 6,E
	res 7,E
	res 0,H
	res 1,H
	res 2,H
	res 3,H
	res 4,H
	res 5,H
	res 6,H
	res 7,H
	res 0,L
	res 1,L
	res 2,L
	res 3,L
	res 4,L
	res 5,L
	res 6,L
	res 7,L
	res 0,(HL)
	res 1,(HL)
	res 2,(HL)
	res 3,(HL)
	res 4,(HL)
	res 5,(HL)
	res 6,(HL)
	res 7,(HL)
	res 0,A
	res 1,A
	res 2,A
	res 3,A
	res 4,A
	res 5,A
	res 6,A
	res 7,A
	set 0,B
	set 1,B
	set 2,B
	set 3,B
	set 4,B
	set 5,B
	set 6,B
	set 7,B
	set 0,C
	set 1,C
	set 2,C
	set 3,C
	set 4,C
	set 5,C
	set 6,C
	set 7,C
	set 0,D
	set 1,D
	set 2,D
	set 3,D
	set 4,D
	set 5,D
	set 6,D
	set 7,D
	set 0,E
	set 1,E
	set 2,E
	set 3,E
	set 4,E
	set 5,E
	set 6,E
	set 7,E
	set 0,H
	set 1,H
	set 2,H
	set 3,H
	set 4,H
	set 5,H
	set 6,H
	set 7,H
	set 0,L
	set 1,L
	set 2,L
	set 3,L
	set 4,L
	set 5,L
	set 6,L
	set 7,L
	set 0,(HL)
	set 1,(HL)
	set 2,(HL)
	set 3,(HL)
	set 4,(HL)
	set 5,(HL)
	set 6,(HL)
	set 7,(HL)
	set 0,A
	set 1,A
	set 2,A
	set 3,A
	set 4,A
	set 5,A
	set 6,A
	set 7,A

; R = B C D E H L A
	in  B,(c)
	in  C,(c)
	in  D,(c)
	in  E,(c)
	in  H,(c)
	in  L,(c)
	in  A,(c)
	out (c),B
	out (c),C
	out (c),D
	out (c),E
	out (c),H
	out (c),L
	out (c),A

; This is also written as "in (c)" -- only affects flags, input data is discarded
	in  f,(c)  

; RR = BC DE HL SP
	sbc hl,BC
	sbc hl,DE
	sbc hl,HL
	sbc hl,SP
	adc hl,BC
	adc hl,DE
	adc hl,HL
	adc hl,SP
	ld  ($1234),BC
	ld  ($2345),DE
	ld  ($3456),HL
	ld  ($4567),SP
	ld  BC,($5678)
	ld  DE,($6789)
	ld  HL,($789a)
	ld  SP,($89ab)

	neg
	retn
; N = 0 1 2
	im  0
	im  1
	im  2
	ld  i,a
	ld  r,a
	ld  a,i
	ld  a,r
	reti
	rrd
	rld

	ldi
	cpi
	ini
	outi
	ldd
	cpd
	ind
	outd
	ldir
	cpir
	inir
	otir
	lddr
	cpdr
	indr
	otdr

; RR = IX IY
	ld  IX,$1234
	ld  IY,$5678
	add IX,BC
	add IX,DE
	add IX,IX
	add IX,SP
	add IY,BC
	add IY,DE
	add IY,IY
	add IY,SP
	inc IX
	inc IY
	dec IX
	dec IY
	ld  ($1234),IX
	ld  ($2345),IY
	ld  IX,($3456)
	ld  IY,($4567)
	pop  IX
	pop  IY
	push IX
	push IY
	ex  (sp),IX
	ex  (sp),IY
	ld  sp,IX
	ld  sp,IY

; R = (IX+N) (IY+N)
	inc (IX - $01)
	inc (IY - $02)
	dec (IX - $03)
	dec (IY - $04)
	ld  (IX-$05),$ab
	ld  (IY-$06),$cd
	add a,(IX-$07)
	add a,(IY-$08)
	adc a,(IX-$09)
	adc a,(IY-$0a)
	sub a,(IX-$0b)
	sub a,(IY-$0c)
	sbc a,(IX-$0d)
	sbc a,(IY-$0e)
	and a,(IX-$0f)
	and a,(IY-$24)
	xor a,(IX-$35)
	xor a,(IY-$46)
	or  a,(IX-$57)
	or  a,(IY-$68)
	cp  a,(IX-$79)
	cp  a,(IY-$80)

; R1 = B C D E H L A  and  R2 = (IX+N) (IY+N)
	ld  B,(IX-$01)
	ld  C,(IX-$02)
	ld  D,(IX-$03)
	ld  E,(IX-$04)
	ld  H,(IX-$05)
	ld  L,(IX-$06)
	ld  A,(IX-$07)
	ld  B,(IY-$11)
	ld  C,(IY-$12)
	ld  D,(IY-$13)
	ld  E,(IY-$14)
	ld  H,(IY-$15)
	ld  L,(IY-$16)
	ld  A,(IY-$17)

; R1 = (IX+N) (IY+N)  and  R2 = B C D E H L A
	ld  (IX-$21),B
	ld  (IX-$22),C
	ld  (IX-$23),D
	ld  (IX-$24),E
	ld  (IX-$25),H
	ld  (IX-$26),L
	ld  (IX-$27),A
	ld  (IY-$31),B
	ld  (IY-$32),C
	ld  (IY-$33),D
	ld  (IY-$34),E
	ld  (IY-$35),H
	ld  (IY-$36),L
	ld  (IY-$37),A
	
; Undocumented opcodes

; R = b c d e h l (hl) a
	sll B
	sll C
	sll D
	sll E
	sll H
	sll L
	sll (HL)
	sll A

; opcode 0xED71 -- undocumented. Outputs zero to port number reference by C register
;   Some references on the web mention that the CMOS version of Z80 outputs $FF instead of 0
;   The standard mnemonic of this instruction is still listed as "out (c),0" regardless
;   The emulator disasembles 0xED71 as "out (c),0"
	out (c),0        
; affects flags only   
	in  F,(c)
; syntax variant -- this is the syntax decoded by Andy4495 emulator
	in  (c)             

; R = xh, xl, yh, yl or syntax variant: ixh, ixl, iyh, iyl
	inc IXH              
	inc IXL               
	inc IYH               
	inc IYL               
	dec IXH
	dec IXL
	dec IYH
	dec IYL
	ld  IXH,$7f
	ld  IXL,$80
	ld  IYH,$81
	ld  IYL,$82
	add a,IXH
	add a,IXL
	add a,IYH
	add a,IYL
	adc a,IXH
	adc a,IXL
	adc a,IYH
	adc a,IYL
	sub a,IXH
	sub a,IXL
	sub a,IYH
	sub a,IYL
	sbc a,IXH
	sbc a,IXL
	sbc a,IYH
	sbc a,IYL
	and a,IXH
	and a,IXL
	and a,IYH
	and a,IYL
	xor a,IXH
	xor a,IXL
	xor a,IYH
	xor a,IYL
	or  a,IXH
	or  a,IXL
	or  a,IYH
	or  a,IYL
	cp  a,IXH
	cp  a,IXL
	cp  a,IYH
	cp  a,IYL

; R1 = b c d e xh xl a  and  R2 = xh or xl
	ld  B,IXH
	ld  C,IXH 
	ld  D,IXH 
	ld  E,IXH 
	ld  IXH,IXH 
	ld  IXL,IXH 
	ld  A,IXH 
	ld  B,IXL
	ld  C,IXL 
	ld  D,IXL 
	ld  E,IXL 
	ld  IXH,IXL 
	ld  IXL,IXL 
	ld  A,IXL 
; R1 = b c d e yh yl a  and  R2 = yh or yl
	ld  B,IYH
	ld  C,IYH 
	ld  D,IYH 
	ld  E,IYH 
	ld  IYH,IYH 
	ld  IYL,IYH 
	ld  A,IYH 
	ld  B,IYL
	ld  C,IYL 
	ld  D,IYL 
	ld  E,IYL 
	ld  IYH,IYL 
	ld  IYL,IYL 
	ld  A,IYL 
; R1 = xh or xl  and  R2 = b c d e xh xl a
	ld  IXH,B           
	ld  IXH,C           
	ld  IXH,D           
	ld  IXH,E           
	ld  IXH,IXH           
	ld  IXH,IXL          
	ld  IXH,A          
	ld  IXL,B           
	ld  IXL,C           
	ld  IXL,D           
	ld  IXL,E           
	ld  IXL,IXH           
	ld  IXL,IXL          
	ld  IXL,A          
; R1 = yh or yl  and  R2 = b c d e yh yl a
	ld  IYH,B           
	ld  IYH,C           
	ld  IYH,D           
	ld  IYH,E           
	ld  IYH,IYH           
	ld  IYH,IYL          
	ld  IYH,A          
	ld  IYL,B           
	ld  IYL,C           
	ld  IYL,D           
	ld  IYL,E           
	ld  IYL,IYH           
	ld  IYL,IYL          
	ld  IYL,A          

	; ixcbr2:
	; RR = ix iy,  R = b c d e h l a
	rlc (IX+$7D),B
	rlc (IX+$7E),C
	rlc (IX+$7F),D
	rlc (IX-$70),E
	rlc (IX-$72),H
	rlc (IX-$73),L
	rlc (IX-$74),A
	rlc (IY+$03),B
	rlc (IY+$02),C
	rlc (IY+$01),D
	rlc (IY+$00),E
	rlc (IY-$01),H
	rlc (IY-$02),L
	rlc (IY-$03),A

	rrc (IX+$7D),B
	rrc (IX+$7E),C
	rrc (IX+$7F),D
	rrc (IX-$70),E
	rrc (IX-$72),H
	rrc (IX-$73),L
	rrc (IX-$74),A
	rrc (IY+$03),B
	rrc (IY+$02),C
	rrc (IY+$01),D
	rrc (IY+$00),E
	rrc (IY-$01),H
	rrc (IY-$02),L
	rrc (IY-$03),A

	rl (IX+$7D),B
	rl (IX+$7E),C
	rl (IX+$7F),D
	rl (IX-$70),E
	rl (IX-$72),H
	rl (IX-$73),L
	rl (IX-$74),A
	rl (IY+$03),B
	rl (IY+$02),C
	rl (IY+$01),D
	rl (IY+$00),E
	rl (IY-$01),H
	rl (IY-$02),L
	rl (IY-$03),A

	rr (IX+$7D),B
	rr (IX+$7E),C
	rr (IX+$7F),D
	rr (IX-$70),E
	rr (IX-$72),H
	rr (IX-$73),L
	rr (IX-$74),A
	rr (IY+$03),B
	rr (IY+$02),C
	rr (IY+$01),D
	rr (IY+$00),E
	rr (IY-$01),H
	rr (IY-$02),L
	rr (IY-$03),A

	sla (IX+$7D),B
	sla (IX+$7E),C
	sla (IX+$7F),D
	sla (IX-$70),E
	sla (IX-$72),H
	sla (IX-$73),L
	sla (IX-$74),A
	sla (IY+$03),B
	sla (IY+$02),C
	sla (IY+$01),D
	sla (IY+$00),E
	sla (IY-$01),H
	sla (IY-$02),L
	sla (IY-$03),A

	sra (IX+$7D),B
	sra (IX+$7E),C
	sra (IX+$7F),D
	sra (IX-$70),E
	sra (IX-$72),H
	sra (IX-$73),L
	sra (IX-$74),A
	sra (IY+$03),B
	sra (IY+$02),C
	sra (IY+$01),D
	sra (IY+$00),E
	sra (IY-$01),H
	sra (IY-$02),L
	sra (IY-$03),A

	sll (IX+$7D),B
	sll (IX+$7E),C
	sll (IX+$7F),D
	sll (IX-$70),E
	sll (IX-$72),H
	sll (IX-$73),L
	sll (IX-$74),A
	sll (IY+$03),B
	sll (IY+$02),C
	sll (IY+$01),D
	sll (IY+$00),E
	sll (IY-$01),H
	sll (IY-$02),L
	sll (IY-$03),A

	srl (IX+$7D),B
	srl (IX+$7E),C
	srl (IX+$7F),D
	srl (IX-$70),E
	srl (IX-$72),H
	srl (IX-$73),L
	srl (IX-$74),A
	srl (IY+$03),B
	srl (IY+$02),C
	srl (IY+$01),D
	srl (IY+$00),E
	srl (IY-$01),H
	srl (IY-$02),L
	srl (IY-$03),A
	 
; RR = ix iy,  R = b c d e h l a
; Note that while the zasm opcode list http://k1.spdns.de/Develop/Projects/zasm/Documentation/z79.htm
; indicates that the DDCB BIT instructions store a result in a register, they just test a bit and 
; do not store a result (beyond updating flags) -- see Section 3.5 in http://www.z80.info/zip/z80-documented.pdf
; So... the actual mnemonic for opcodes 0xDDCBdd40 - 0xDDCBdd7F and 0xFDCBdd40 - 0xFDCBdd7F are all the same,
; even though the zasm compiler considers them different. My emulator does not disassemble into the "register 
; result" format for the bit instructions, because that is not what the instructions do. 
; The "register result" format of the instructions are left here, but commented out, so that the automated
; test process works properly.
	bit 0,(IX+$01)  
;	bit 0,(IX+$01),B   
;	bit 0,(IX+$02),C    
;	bit 0,(IX+$03),D    
;	bit 0,(IX+$04),E    
;	bit 0,(IX-$40),H    
;	bit 0,(IX-$41),L    
;	bit 0,(IX-$42),A    
	bit 0,(IY+$01)
;	bit 0,(IY+$01),B   
;	bit 0,(IY+$02),C    
;	bit 0,(IY+$03),D    
;	bit 0,(IY+$04),E    
;	bit 0,(IY-$40),H    
;	bit 0,(IY-$41),L    
;	bit 0,(IY-$42),A    
 
	bit 1,(IX+$01)
;	bit 1,(IX+$01),B   
;	bit 1,(IX+$02),C    
;	bit 1,(IX+$03),D    
;	bit 1,(IX+$04),E    
;	bit 1,(IX-$40),H    
;	bit 1,(IX-$41),L    
;	bit 1,(IX-$42),A    
	bit 1,(IY+$01)
;	bit 1,(IY+$01),B   
;	bit 1,(IY+$02),C    
;	bit 1,(IY+$03),D    
;	bit 1,(IY+$04),E    
;	bit 1,(IY-$40),H    
;	bit 1,(IY-$41),L    
;	bit 1,(IY-$42),A    

	bit 2,(IX+$01)  
;	bit 2,(IX+$01),B   
;	bit 2,(IX+$02),C    
;	bit 2,(IX+$03),D    
;	bit 2,(IX+$04),E    
;	bit 2,(IX-$40),H    
;	bit 2,(IX-$41),L    
;	bit 2,(IX-$42),A    
	bit 2,(IY+$01)  
;	bit 2,(IY+$01),B   
;	bit 2,(IY+$02),C    
;	bit 2,(IY+$03),D    
;	bit 2,(IY+$04),E    
;	bit 2,(IY-$40),H    
;	bit 2,(IY-$41),L    
;	bit 2,(IY-$42),A    

	bit 3,(IX+$01) 
;	bit 3,(IX+$01),B   
;	bit 3,(IX+$02),C    
;	bit 3,(IX+$03),D    
;	bit 3,(IX+$04),E    
;	bit 3,(IX-$40),H    
;	bit 3,(IX-$41),L    
;	bit 3,(IX-$42),A    
	bit 3,(IY+$01)   
;	bit 3,(IY+$01),B   
;	bit 3,(IY+$02),C    
;	bit 3,(IY+$03),D    
;	bit 3,(IY+$04),E    
;	bit 3,(IY-$40),H    
;	bit 3,(IY-$41),L    
;	bit 3,(IY-$42),A    

;	bit 4,(IX+$01)  
;	bit 4,(IX+$01),B   
;	bit 4,(IX+$02),C    
;	bit 4,(IX+$03),D    
;	bit 4,(IX+$04),E    
;	bit 4,(IX-$40),H    
;	bit 4,(IX-$41),L    
;	bit 4,(IX-$42),A    
	bit 4,(IY+$01)
;	bit 4,(IY+$01),B   
;	bit 4,(IY+$02),C    
;	bit 4,(IY+$03),D    
;	bit 4,(IY+$04),E    
;	bit 4,(IY-$40),H    
;	bit 4,(IY-$41),L    
;	bit 4,(IY-$42),A    

	bit 5,(IX+$01)
;	bit 5,(IX+$01),B   
;	bit 5,(IX+$02),C    
;	bit 5,(IX+$03),D    
;	bit 5,(IX+$04),E    
;	bit 5,(IX-$40),H    
;	bit 5,(IX-$41),L    
;	bit 5,(IX-$42),A    
	bit 5,(IY+$01) 
;	bit 5,(IY+$01),B   
;	bit 5,(IY+$02),C    
;	bit 5,(IY+$03),D    
;	bit 5,(IY+$04),E    
;	bit 5,(IY-$40),H    
;	bit 5,(IY-$41),L    
;	bit 5,(IY-$42),A    

	bit 6,(IX+$01)
;	bit 6,(IX+$01),B   
;	bit 6,(IX+$02),C    
;	bit 6,(IX+$03),D    
;	bit 6,(IX+$04),E    
;	bit 6,(IX-$40),H    
;	bit 6,(IX-$41),L    
;	bit 6,(IX-$42),A    
	bit 6,(IY+$01)
;	bit 6,(IY+$01),B   
;	bit 6,(IY+$02),C    
;	bit 6,(IY+$03),D    
;	bit 6,(IY+$04),E    
;	bit 6,(IY-$40),H    
;	bit 6,(IY-$41),L    
;	bit 6,(IY-$42),A    

	bit 7,(IX+$01)  
;	bit 7,(IX+$01),B   
;	bit 7,(IX+$02),C    
;	bit 7,(IX+$03),D    
;	bit 7,(IX+$04),E    
;	bit 7,(IX-$40),H    
;	bit 7,(IX-$41),L    
;	bit 7,(IX-$42),A    
	bit 7,(IY+$01)   
;	bit 7,(IY+$01),B   
;	bit 7,(IY+$02),C    
;	bit 7,(IY+$03),D    
;	bit 7,(IY+$04),E    
;	bit 7,(IY-$40),H    
;	bit 7,(IY-$41),L    
;	bit 7,(IY-$42),A    

; RR = ix iy,  R = b c d e h l a
;	res N,(RR+dis),R	
	res 0,(IX+$01),B   
	res 0,(IX+$02),C    
	res 0,(IX+$03),D    
	res 0,(IX+$04),E    
	res 0,(IX-$40),H    
	res 0,(IX-$41),L    
	res 0,(IX-$42),A    
	res 0,(IY+$01),B   
	res 0,(IY+$02),C    
	res 0,(IY+$03),D    
	res 0,(IY+$04),E    
	res 0,(IY-$40),H    
	res 0,(IY-$41),L    
	res 0,(IY-$42),A    
 
	res 1,(IX+$01),B   
	res 1,(IX+$02),C    
	res 1,(IX+$03),D    
	res 1,(IX+$04),E    
	res 1,(IX-$40),H    
	res 1,(IX-$41),L    
	res 1,(IX-$42),A    
	res 1,(IY+$01),B   
	res 1,(IY+$02),C    
	res 1,(IY+$03),D    
	res 1,(IY+$04),E    
	res 1,(IY-$40),H    
	res 1,(IY-$41),L    
	res 1,(IY-$42),A    

	res 2,(IX+$01),B   
	res 2,(IX+$02),C    
	res 2,(IX+$03),D    
	res 2,(IX+$04),E    
	res 2,(IX-$40),H    
	res 2,(IX-$41),L    
	res 2,(IX-$42),A    
	res 2,(IY+$01),B   
	res 2,(IY+$02),C    
	res 2,(IY+$03),D    
	res 2,(IY+$04),E    
	res 2,(IY-$40),H    
	res 2,(IY-$41),L    
	res 2,(IY-$42),A    

	res 3,(IX+$01),B   
	res 3,(IX+$02),C    
	res 3,(IX+$03),D    
	res 3,(IX+$04),E    
	res 3,(IX-$40),H    
	res 3,(IX-$41),L    
	res 3,(IX-$42),A    
	res 3,(IY+$01),B   
	res 3,(IY+$02),C    
	res 3,(IY+$03),D    
	res 3,(IY+$04),E    
	res 3,(IY-$40),H    
	res 3,(IY-$41),L    
	res 3,(IY-$42),A    

	res 4,(IX+$01),B   
	res 4,(IX+$02),C    
	res 4,(IX+$03),D    
	res 4,(IX+$04),E    
	res 4,(IX-$40),H    
	res 4,(IX-$41),L    
	res 4,(IX-$42),A    
	res 4,(IY+$01),B   
	res 4,(IY+$02),C    
	res 4,(IY+$03),D    
	res 4,(IY+$04),E    
	res 4,(IY-$40),H    
	res 4,(IY-$41),L    
	res 4,(IY-$42),A    

	res 5,(IX+$01),B   
	res 5,(IX+$02),C    
	res 5,(IX+$03),D    
	res 5,(IX+$04),E    
	res 5,(IX-$40),H    
	res 5,(IX-$41),L    
	res 5,(IX-$42),A    
	res 5,(IY+$01),B   
	res 5,(IY+$02),C    
	res 5,(IY+$03),D    
	res 5,(IY+$04),E    
	res 5,(IY-$40),H    
	res 5,(IY-$41),L    
	res 5,(IY-$42),A    

	res 6,(IX+$01),B   
	res 6,(IX+$02),C    
	res 6,(IX+$03),D    
	res 6,(IX+$04),E    
	res 6,(IX-$40),H    
	res 6,(IX-$41),L    
	res 6,(IX-$42),A    
	res 6,(IY+$01),B   
	res 6,(IY+$02),C    
	res 6,(IY+$03),D    
	res 6,(IY+$04),E    
	res 6,(IY-$40),H    
	res 6,(IY-$41),L    
	res 6,(IY-$42),A    

	res 7,(IX+$01),B   
	res 7,(IX+$02),C    
	res 7,(IX+$03),D    
	res 7,(IX+$04),E    
	res 7,(IX-$40),H    
	res 7,(IX-$41),L    
	res 7,(IX-$42),A    
	res 7,(IY+$01),B   
	res 7,(IY+$02),C    
	res 7,(IY+$03),D    
	res 7,(IY+$04),E    
	res 7,(IY-$40),H    
	res 7,(IY-$41),L    
	res 7,(IY-$42),A    


; RR = ix iy,  R = b c d e h l a
;	set N,(RR+dis),R
	set 0,(IX+$01),B   
	set 0,(IX+$02),C    
	set 0,(IX+$03),D    
	set 0,(IX+$04),E    
	set 0,(IX-$40),H    
	set 0,(IX-$41),L    
	set 0,(IX-$42),A    
	set 0,(IY+$01),B   
	set 0,(IY+$02),C    
	set 0,(IY+$03),D    
	set 0,(IY+$04),E    
	set 0,(IY-$40),H    
	set 0,(IY-$41),L    
	set 0,(IY-$42),A    
 
	set 1,(IX+$01),B   
	set 1,(IX+$02),C    
	set 1,(IX+$03),D    
	set 1,(IX+$04),E    
	set 1,(IX-$40),H    
	set 1,(IX-$41),L    
	set 1,(IX-$42),A    
	set 1,(IY+$01),B   
	set 1,(IY+$02),C    
	set 1,(IY+$03),D    
	set 1,(IY+$04),E    
	set 1,(IY-$40),H    
	set 1,(IY-$41),L    
	set 1,(IY-$42),A    

	set 2,(IX+$01),B   
	set 2,(IX+$02),C    
	set 2,(IX+$03),D    
	set 2,(IX+$04),E    
	set 2,(IX-$40),H    
	set 2,(IX-$41),L    
	set 2,(IX-$42),A    
	set 2,(IY+$01),B   
	set 2,(IY+$02),C    
	set 2,(IY+$03),D    
	set 2,(IY+$04),E    
	set 2,(IY-$40),H    
	set 2,(IY-$41),L    
	set 2,(IY-$42),A    

	set 3,(IX+$01),B   
	set 3,(IX+$02),C    
	set 3,(IX+$03),D    
	set 3,(IX+$04),E    
	set 3,(IX-$40),H    
	set 3,(IX-$41),L    
	set 3,(IX-$42),A    
	set 3,(IY+$01),B   
	set 3,(IY+$02),C    
	set 3,(IY+$03),D    
	set 3,(IY+$04),E    
	set 3,(IY-$40),H    
	set 3,(IY-$41),L    
	set 3,(IY-$42),A    

	set 4,(IX+$01),B   
	set 4,(IX+$02),C    
	set 4,(IX+$03),D    
	set 4,(IX+$04),E    
	set 4,(IX-$40),H    
	set 4,(IX-$41),L    
	set 4,(IX-$42),A    
	set 4,(IY+$01),B   
	set 4,(IY+$02),C    
	set 4,(IY+$03),D    
	set 4,(IY+$04),E    
	set 4,(IY-$40),H    
	set 4,(IY-$41),L    
	set 4,(IY-$42),A    

	set 5,(IX+$01),B   
	set 5,(IX+$02),C    
	set 5,(IX+$03),D    
	set 5,(IX+$04),E    
	set 5,(IX-$40),H    
	set 5,(IX-$41),L    
	set 5,(IX-$42),A    
	set 5,(IY+$01),B   
	set 5,(IY+$02),C    
	set 5,(IY+$03),D    
	set 5,(IY+$04),E    
	set 5,(IY-$40),H    
	set 5,(IY-$41),L    
	set 5,(IY-$42),A    

	set 6,(IX+$01),B   
	set 6,(IX+$02),C    
	set 6,(IX+$03),D    
	set 6,(IX+$04),E    
	set 6,(IX-$40),H    
	set 6,(IX-$41),L    
	set 6,(IX-$42),A    
	set 6,(IY+$01),B   
	set 6,(IY+$02),C    
	set 6,(IY+$03),D    
	set 6,(IY+$04),E    
	set 6,(IY-$40),H    
	set 6,(IY-$41),L    
	set 6,(IY-$42),A    

	set 7,(IX+$01),B   
	set 7,(IX+$02),C    
	set 7,(IX+$03),D    
	set 7,(IX+$04),E    
	set 7,(IX-$40),H    
	set 7,(IX-$41),L    
	set 7,(IX-$42),A    
	set 7,(IY+$01),B   
	set 7,(IY+$02),C    
	set 7,(IY+$03),D    
	set 7,(IY+$04),E    
	set 7,(IY-$40),H    
	set 7,(IY-$41),L    
	set 7,(IY-$42),A    
