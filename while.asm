  

     
      

        
            
            
    

     


 

     

	GLOBAL	w
w:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 0
	mov	[ebp-4], eax
L1:
	mov	eax, [ebp+8]
	mov	[ebp-8], eax
	mov	eax, 0
	cmp	eax, [ebp-8]
	setl	al
	movzx	eax, al
	cmp	eax, 0
	je	L2
	mov	eax, 1
	mov	[ebp-8], eax
	mov	eax, [ebp-4]
	add	eax, [ebp-8]
	mov	[ebp-4], eax
	mov	eax, 1
	mov	[ebp-8], eax
	mov	eax, [ebp+8]
	sub	eax, [ebp-8]
	mov	[ebp+8], eax
	jmp	L1
L2:
	mov	eax, [ebp-4]
	jmp	R1
R1:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 10
	push	eax
	mov	eax, 10
	push	eax
	call	w
	add	esp, 4
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
R2:	mov	esp, ebp
	pop	ebp
	ret
