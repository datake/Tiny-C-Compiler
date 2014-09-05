    

       
         
    
         


    

       
         
    
         


    

       
         
    
         


 

      
      
      

	GLOBAL	cmp1
cmp1:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 1
	mov	[ebp-8], eax
	mov	eax, [ebp+8]
	add	eax, [ebp-8]
	mov	[ebp-4], eax
	mov	eax, [ebp+12]
	cmp	eax, [ebp-4]
	setl	al
	movzx	eax, al
	cmp	eax, 0
	je	L1
	mov	eax, 1
	jmp	R1
	jmp	L2
L1:
	mov	eax, 0
	jmp	R1
L2:
R1:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	cmp2
cmp2:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, 1
	mov	[ebp-8], eax
	mov	eax, [ebp+12]
	add	eax, [ebp-8]
	cmp	eax, [ebp-4]
	setle	al
	movzx	eax, al
	cmp	eax, 0
	je	L3
	mov	eax, 1
	jmp	R2
	jmp	L4
L3:
	mov	eax, 0
	jmp	R2
L4:
R2:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	cmp3
cmp3:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, 1
	mov	[ebp-8], eax
	mov	eax, [ebp+12]
	add	eax, [ebp-8]
	cmp	eax, [ebp-4]
	setl	al
	movzx	eax, al
	cmp	eax, 0
	je	L5
	mov	eax, 1
	jmp	R3
	jmp	L6
L5:
	mov	eax, 0
	jmp	R3
L6:
R3:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	main
main:	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, 0
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 2
	push	eax
	call	cmp1
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
	mov	eax, 1
	push	eax
	mov	eax, 3
	push	eax
	mov	eax, 4
	push	eax
	call	cmp2
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
	mov	eax, 1
	push	eax
	mov	eax, 2
	push	eax
	mov	eax, 4
	push	eax
	call	cmp3
	add	esp, 8
	push	eax
	EXTERN	chk
	call	chk
	add	esp, 8
R4:	mov	esp, ebp
	pop	ebp
	ret
