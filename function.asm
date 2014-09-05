
	GLOBAL	f
f:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, [ebp+12]
	mov	[ebp-4], eax
	mov	eax, [ebp+8]
	cdq
	idiv	dword [ebp-4]
	jmp	R1
R1:	mov	esp, ebp
	pop	ebp
	ret
	GLOBAL	function
function:	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	eax, [ebp+8]
	push	eax
	mov	eax, [ebp+8]
	mov	[ebp-4], eax
	mov	eax, 3
	add	eax, [ebp-4]
	push	eax
	call	f
	add	esp, 8
	jmp	R2
R2:	mov	esp, ebp
	pop	ebp
	ret
