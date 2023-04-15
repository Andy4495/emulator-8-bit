; Z80 test file for 8-bit emulator
;    https://github.com/Andy4495/emulator-8-bit
;
; 13-Apr-2023  Andy4495 Initial version
; 
; Run through DAA combinations
; 
; Compile with --ixcbr2 option: 
;   zasm --ixcbr2 filename.asm

    org 0
	jp	start

start: 
	ld sp,$ffff
	ld c,$00			; Clear out the flags
	push bc
	pop af
	ld ix,tests_1		; Table of test values and results
	ld iy,results 
	ld h,0				; Used to track the test number
	ld a,(ix)

; Test ADD case (N Flag = 0)
loop1:
	add (ix+1)
	daa
	push af
	pop bc
	ld d,(ix+2)			; Check the result
	cp d
	jp nz,end_program
	ld a,(ix+3)			; Check flags
	cp c
	jp nz,end_program
	ld (iy),h 
	inc h
	inc ix
	inc ix 
	inc ix
	inc ix
	ld a,(ix)
	cp $ff				; $ff is end-of-table marker
	jp nz,loop1

; Test SUB case (N Flag = 1)
	ld ix,tests_2		; Table of test values and results
	ld h,$80			; Start the SUB test case number at 128
	ld a,(ix)
loop2: 
	sub (ix+1)
	daa
	push af
	pop bc
	ld d,(ix+2)			; Check the result
	cp d
	jp nz,end_program
	ld a,(ix+3)			; Check flags
	cp c
	jp nz,end_program
	ld (iy),h 
	inc h
	inc ix
	inc ix 
	inc ix
	inc ix
	ld a,(ix)
	cp $ff				; $ff is end-of-table marker
	jp nz,loop2

end_program:
	halt
	jp	end_program

; Table of test values and results
; addend, addend, results, flags
; Flags: SZ5H 3PNC
; N is not affected by DAA (previous SUB->N=1; ADD->N=0)
; Case 1: N = 0 (Addition)
; H = 0, C = 0 before DAA unless noted otherwise 
tests_1:
	defb $00, $00, $00, $44 ; $00. 
	defb $01, $01, $02, $00
	defb $02, $02, $04, $00
	defb $03, $03, $06, $04
	defb $04, $04, $08, $00
	defb $05, $05, $10, $10
	defb $06, $06, $12, $14 
	defb $07, $07, $14, $14 
	defb $08, $08, $16, $00 ; $08. H = 1, C = 0 before DAA
	defb $09, $09, $18, $04 ;      H = 1, C = 0 before DAA
	defb $10, $10, $20, $00
	defb $20, $20, $40, $00
	defb $30, $30, $60, $04
	defb $40, $40, $80, $80
	defb $50, $50, $00, $45
	defb $60, $60, $20, $01
	defb $70, $70, $40, $01 ; $10. 
	defb $80, $80, $60, $05 ;      H = 0, C = 1 before DAA
	defb $90, $90, $80, $81 ;      H = 0, C = 1 before DAA
	defb $11, $11, $22, $04
	defb $22, $22, $44, $04
	defb $33, $33, $66, $04
	defb $44, $44, $88, $84
	defb $55, $55, $10, $11
	defb $66, $66, $32, $11 ; $18.
	defb $77, $77, $54, $11
	defb $88, $88, $76, $01 ;      H = 1, C = 1 before DAA
	defb $99, $99, $98, $81 ;      H = 1, C = 1 before DAA
	defb $00, $00, $00, $44
	defb $ff, $ff, $ff, $ff	; end of list marker
	
; Case 2: N = 1 (Subtraction)
; H = 0, C = 0 before DAA unless noted otherwise 
tests_2:
	defb $00, $00, $00, $46 ; $80. 
	defb $08, $01, $07, $02
	defb $08, $02, $06, $06
	defb $00, $01, $99, $87 ;      H = 1, C = 1 before DAA
	defb $08, $09, $99, $87 ;      H = 1, C = 1 before DAA
	defb $00, $02, $98, $83 ;      H = 1, C = 1 before DAA
	defb $21, $19, $02, $02 ;      H = 1, C = 0 before DAA
	defb $35, $43, $92, $83 ;      H = 0, C = 1 before DAA
	defb $ff, $ff, $ff, $ff	; end of list marker

	org $1000
	defm "Results:"

results:
	defs $ff,'.'		; Results area. 'P' ($50): Pass. 'F' ($46): FAIL. '.' - no result recorded
	defs $ff,'.'
