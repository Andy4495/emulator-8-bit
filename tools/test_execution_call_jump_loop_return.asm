; Z80 test file for 8-bit emulator
;    https://github.com/Andy4495/emulator-8-bit
;
; 13-Mar-2023  Andy4495 Initial version
; 21-Dec-2023  Andy4495 Update for z88dk compatibility
; 
; Z80 opcodes for conditional calls, conditional jumps, loops, 
;                 conditional returns, restarts (rst)
; 
; Compile with --ixcbr2 option: 
;   zasm --ixcbr2 filename.asm

    org 0
	cp a,$01	;2
	ret	z		;1
	ld a,$a5
	jp	start	;3
;	org 8
	ld a,$02
	ret
	defs 5,0	; fill between with zeros
;	org $10
	ld 	a,$04
	ret
	defs 5,0	; fill between with zeros
;	org $18
	ld a,$08
	ret
	defs 5,0	; fill between with zeros
;	org $20
	ld a,$10
	ret
	defs 5,0	; fill between with zeros
;	org $28
	ld a,$20
	ret
	defs 5,0	; fill between with zeros
;	org $30
	ld a,$40
	ret
	defs 5,0	; fill between with zeros
;	org $38
	ld a,$80
	ret
	defs 5,0	; fill between with zeros
	defs $26,$ff	; Fill in remaining as $ff for compatiblity with previous versions of this test file	
;	org $66		; NMI vector
	jp retn

start: 
	ld sp,$ffff
	ld ix,results

; rst testing
rst1:	
	ld (ix),'P'
	ld a,$01	; Used to distinguish "rst 0" from cold reset
	rst  $00
	cp a,$01
	jp z,rst2
	ld (ix),'F'	; Test 0
rst2:	
	inc ix
	ld (ix),'P'
	rst  $08
	cp a,$02
	jp z,rst3
	ld (ix),'F'	; Test 1
rst3:	
	inc ix
	ld (ix),'P'
	rst  $10
	cp a,$04
	jp z,rst4
	ld (ix),'F'	; Test 2
rst4:	
	inc ix
	ld (ix),'P'
	rst  $18
	cp a,$08
	jp z,rst5
	ld (ix),'F'	; Test 3
rst5:	
	inc ix
	ld (ix),'P'
	rst  $20
	cp a,$10
	jp z,rst6
	ld (ix),'F'	; Test 4
rst6:	
	inc ix
	ld (ix),'P'
	rst  $28
	cp a,$20
	jp z,rst7
	ld (ix),'F'	; Test 5
rst7:	
	inc ix
	ld (ix),'P'
	rst  $30
	cp a,$40
	jp z,rst8
	ld (ix),'F' ; Test 6
rst8:	
	inc ix
	ld (ix),'P'
	rst  $38
	cp a,$80
	jp z,cc1
	ld (ix),'F' ; Test 7

; conditional call testing
cc1:
	inc ix
	ld (ix),'P'
	ld a,$99
	cp a,$00     ; clear z flag
	call nz,return_point
	cp a,$5a
	jp z,cc2
	ld (ix),'F'	; Test 8
cc2:
	inc ix
	ld (ix),'P'
	ld a,$99
	cp a,$99     ; set z flag
	call z,return_point
	cp a,$5a
	jp z,cc3
	ld (ix),'F' ; Test 9
cc3:
	inc ix
	ld (ix),'P'
	ld a,$99
	scf
	ccf
	call nc,return_point
	cp a,$5a
	jp z,cc4
	ld (ix),'F' ; Test 10
cc4:
	inc ix
	ld (ix),'P'
	ld a,$99
	scf
	call c,return_point
	cp a,$5a
	jp z,cc5
	ld (ix),'F' ; Test 11
cc5:
	inc ix
	ld (ix),'P'
	ld a,$10
	or a,$00		; parity odd
	call po,return_point
	cp a,$5a
	jp z,cc6
	ld (ix),'F'	; Test 12
cc6:
	inc ix
	ld (ix),'P'
	ld a,$99
	or a,$00		; parity even
	call pe,return_point
	cp a,$5a
	jp z,cc7
	ld (ix),'F' ; Test 13
cc7:
	inc ix
	ld (ix),'P'
	ld a,$79
	cp $00		; clear sign flag
	call p,return_point
	cp a,$5a
	jp z,cc8
	ld (ix),'F' ; Test 14
cc8:
	inc ix
	ld (ix),'P'
	ld a,$99
	cp $ff		; set sign flag
	call m,return_point
	cp a,$5a
	jp z,cj1
	ld (ix),'F' ; Test 15

; conditional jump testing
cj1:
	inc ix
	ld (ix),'P'
	ld a,$44
	cp a,$0
	jp  nz,cj2
	ld (ix),'F'	; Test 16
cj2:
	inc ix
	ld (ix),'P'
	ld a,$44
	cp a,$44
	jp  z,cj3
	ld (ix),'F'	; Test 17
cj3:
	inc ix
	ld (ix),'P'
	scf
	ccf
	jp  nc,cj4
	ld (ix),'F'	; Test 18
cj4:
	inc ix
	ld (ix),'P'
	scf
	jp  c,cj5
	ld (ix),'F'	; Test 19
cj5:
	inc ix
	ld (ix),'P'
	ld a,$11
	or a,$80
	jp  po,cj6
	ld (ix),'F'	; Test 20
cj6:
	inc ix
	ld (ix),'P'
	ld a,$11
	or a,$22
	jp  pe,cj7
	ld (ix),'F'	; Test 21
cj7:
	inc ix
	ld (ix),'P'
	ld a,$75
	cp a,$74
	jp  p,cj8
	ld (ix),'F'	; Test 22
cj8:
	inc ix
	ld (ix),'P'
	ld a,$44
	cp a,$45
	jp  m,lt1
	ld (ix),'F'	; Test 23

; loop testing
lt1:
	inc ix
	ld (ix),'P'
	ld c,$1e
	ld b,$11
	ld d,$0
dj1:
	inc d 
	dec c
	djnz dj1
	ld a,$11
	cp d
	jp nz,djerror
	ld a,$0
	cp b
	jp nz,djerror
	ld a,$d
	cp c
	jp nz,djerror
	ld (IX),'P'  
	jr lt2
djerror:
	ld (IX),'F' ; test 24
lt2:
	inc ix
	ld (ix),'P'
	ld bc,$2
	ld hl,test_string
	ld de,output_string
	ldir
	ld a,(output_string+1)
	cp 'e'
	jp z,lt3
	ld (ix),'F' ; test 25
lt3:
	inc ix
	ld (ix),'P'
	ld bc,$2
	ld hl,test_string+8
	ld de,output_string+8
	lddr
	ld a,(output_string+1)
	cp 'e'
	jp z,lt4
	ld (ix),'F' ; test 26
lt4:
	inc ix
	ld (ix),'P'
	ld bc,8		; first case -- bc goes to zero before a match
	ld a,'!'
	ld hl,test_string
	cpir
	jp nz,lt4a
	ld (ix),'F' ; test 27a
	jp lt5
lt4a:
	ld bc,$10	; second case -- match found before bc == 0
	ld hl,test_string
	cpir
	jp z,lt4b
	ld (ix),'F'	; test 27b
	jp lt5
lt4b:
	ld a,3
	cp c    	; bc should still have some counts left
	jp z,lt5
	ld (ix),'F'	; test 27c
lt5:
	inc ix
	ld (ix),'P'
	ld bc,8		; first case -- bc goes to zero before a match
	ld a,'H'
	ld hl,test_string+12
	cpdr
	jp nz,lt5a
	ld (ix),'F' ; test 28a
	jp lt6
lt5a:
	ld bc,$10	; second case -- match found before bc == 0
	ld hl,test_string+12
	cpdr
	jp z,lt5b
	ld (ix),'F' ; test 28b
	jp lt6
lt5b:
	ld a,3
	cp c    	; bc should still have some counts left
	jp z,lt6
	ld (ix),'F' ; test 28c
lt6:
	inc ix
	ld (ix),'P'
	ld b,8
	ld c,$96  ; input port $96 contains "i" (as set up in emulator)
	ld HL,input_test
	inir
	ld de,input_test+8
	sbc hl,de
	jp z,lt7
	ld (ix),'F' ; test 29
	jp lt7
lt7:
	inc ix
	ld (ix),'P'
	ld b,4
	ld c,$97 ; input port $97 contains "h" (as set up in emulator)
	ld hl,input_test-1
	indr
	ld de,input_test-5
	sbc hl,de
	jp z,lt8
	ld (ix),'F' ; test 30
	jp lt8
lt8:
	inc ix
	ld (ix),'P'
	ld b,4
	ld c,$80
	ld hl,test_string
	otir
	ld de,test_string+4
	sbc hl,de
	jp z,lt9
	ld (ix),'F' ; test 31
lt9:
	inc ix
	ld (ix),'P'
	ld b,5
	ld c,$81
	ld hl,test_string+12
	otdr
	ld de,test_string+12-5
	sbc hl,de
	jp z,cr1
	ld (ix),'F' ; test 32
	jp cr1


; Conditional return testing
cr1:
	inc ix
	ld (ix),'P'
	sub a   	 ; set Z flag 
	call retnz
	cp a,$fe
	jp z,cr1a
	ld (ix),'F' ; test 33
	jp cr2
cr1a: 
	sub $80		; clear Z flag
	call retnz
	cp a,$01
	jp z,cr2
	ld (ix),'F'
	jp cr2
cr2:
	inc ix
	ld (ix),'P'
	or $ff		; clear Z flag
	call retz
	cp a,$fd
	jp z,cr2a
	ld (ix),'F' ; test 34
	jp cr3
cr2a: 
	sub a   	 ; set Z flag 
	call retz
	cp a,$02
	jp z,cr3
	ld (ix),'F'
	jp cr3
cr3:
	inc ix
	ld (ix),'P'
	scf
	call retnc
	cp $fb
	jp z,cr3a
	ld (ix),'F' ; test 35
	jp cr4
cr3a:
	scf
	ccf			; clear carry flag (set then complement)
	call retnc
	cp $04
	jp z,cr4
	ld (ix),'F'
	jp cr4
cr4:
	inc ix
	ld (ix),'P'
	scf
	ccf 
	call retc
	cp $f7
	jp z,cr4a
	ld (ix),'F'
	jp cr5
cr4a:
	ccf
	call retc
	cp $08
	jp z,cr5
	ld (ix),'F'  ; test 36
	jp cr5
cr5:
	inc ix
	ld (ix),'P'
	ld a,$10
	or a,$01  ; even parity
	call retpo
	cp $ef
	jp z,cr5a
	ld (ix),'F' ; test 37
	jp cr6
cr5a:
	ld a,$10
	or a,$81  ; odd parity
	call retpo
	cp $10
	jp z,cr6
	ld (ix),'F'
	jp cr6
cr6:
	inc ix
	ld (ix),'P'
	or $80		; set the sign bit
	call retp
	cp $cf
	jp z,cr6a
	ld (ix),'F'  ; test 38
	jp cr7
cr6a: 
	and $00     ; clear the sign bit
	call retp
	cp $40
	jp z, cr7
	ld (ix),'F' 
	jp cr7
cr7:
	inc ix
	ld (ix),'P'
	and $00		; clear the sign bit
	call retm
	cp $7f
	jp z,cr7a
	ld (ix),'F' ; test 39
	jp retntest
cr7a:
	or $8f		; set the sign bit
	call retm
	cp $80
	jp z,retntest
	ld (ix),'F'
	jp retntest
retntest:
	inc ix
	ld (ix),'P'
	ld bc,after_nmi
	push bc
	jp $0066 	; simulate NMI which jumps to $0066
after_nmi:
	cp $12
	jp z,retitest
	ld (ix),'F' ; test 40
	jp retitest
retitest:
	inc ix
	ld (ix),'P'
	ld bc,after_int
	push bc
	jp reti
after_int:
	cp $34
	jp z,end_of_rets
	ld (ix),'F' ; test 41

end_of_rets:
	jp end_program

; Return opcodes
retnz:
	ld a,$01
	ret nz
	ld a,$fe 
	ret
retz:
	ld a,$02
	ret z
	ld a,$fd 
	ret
retnc:
	ld a,$04
	ret nc
	ld a,$fb
	ret
retc:
	ld a,$08
	ret c
	ld a,$f7
	ret 
retpo:
	ld a,$10
	ret po
	ld a,$ef
	ret
retpe:
	ld a,$20
	ret pe
	ld a,$de
	ret
retp:
	ld a,$40
	ret p
	ld a,$cf 
	ret
retm:
	ld a,$80
	ret m
	ld a,$7f
	ret
retn:
	ld a,$12
	retn
reti:
	ld a,$34
	reti

end_program:
	halt
	jp	end_program

return_point:
	ld a,$5a
	ret

; define space for compatibility with previous version of test file
; current PC at $04f2
	defs $30e,$ff
;	org $0800
	defm "Results:"
results:
	defs $30,'.'		; Results area. 'P' ($50): Pass. 'F' ($46): FAIL. '.' - no result recorded
test_string:
	defm "Hello, World!"
output_string:
	defm "............."
input_test:
	defm "................"