    

       
         
        
          
    
          


 

      

	GLOBAL	gcd
gcd:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, [ebp+12]
	cmp	eax, [ebp-4]
	sete	al
	movzx	eax, al
	cmp	eax, 0
	je	L1
	mov	eax, [ebp+8]
	jmp	R1
	jmp	L2
L1:
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, [ebp+12]
	cmp	eax, [ebp-4]
	setl	al
	movzx	eax, al
	cmp	eax, 0
	je	L3
	mov	eax, [ebp+12]
	push	eax
	mov	eax, [ebp+12]
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	sub	eax, [ebp-4]
	push	eax
	call	gcd
	add	esp, 8
	jmp	R1
	jmp	L4
L3:
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, [ebp+12]
	sub	eax, [ebp-4]
	push	eax
	mov	eax, [ebp+8]
	push	eax
	call	gcd
	add	esp, 8
	jmp	R1
L4:
L2:
R1:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, 189
	push	eax
	mov	eax, 315
	push	eax
	call	gcd
	add	esp, 8
	push	eax
	mov	eax, 63
	push	eax
	EXTERN	f
	call	f
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
R2:	mov	esp, ebp
	pop	ebp
	ret
