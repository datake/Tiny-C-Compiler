 

      
      
      
      
              
    
    


 

     

	GLOBAL	lab
lab:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 1
	mov	[ebp-4], eax
	mov	eax, 0
	mov	[ebp-8], eax
	mov	eax, [ebp-4]
	cmp	eax, 0
	je	L1
	mov	eax, [ebp-8]
	cmp	eax, 0
	je	L2
	mov	eax, 1
	jmp	R1
	jmp	L3
L2:
	mov	eax, 2
	jmp	R1
L3:
L1:
R1:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 2
	push	eax
	call	lab
	add	esp, 0
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
R2:	mov	esp, ebp
	pop	ebp
	ret
