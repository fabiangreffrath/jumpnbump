[GLOBAL _get_block]
[GLOBAL _put_block]

[EXTERN ___djgpp_base_address]


[SECTION .text]


; void get_block(char page, long x, long y, long width, long height, char *buffer);

_get_block
	push	ebp
	mov	ebp,esp
	pusha

	mov	eax,[ebp+12]
	mov	[.x],eax
	mov	eax,[ebp+16]
	mov	[.y],eax
	mov	eax,[ebp+20]
	mov	[.draw_width],eax
	mov	eax,[ebp+24]
	mov	[.draw_height],eax
	mov	eax,[ebp+28]
	mov	[.buf_ptr],eax

	mov	eax,[.x]
	or	eax,eax
	jns	.11

	neg	eax
	cmp	eax,[ebp+20]
	jge	near .exit

	mov	dword [.x],0
	sub	[.draw_width],eax
	add	[.buf_ptr],eax

.11
	cmp	dword [.x],400
	jge	near .exit

	mov	eax,[.y]
	or	eax,eax
	jns	.12

	neg	eax
	cmp	eax,[ebp+24]
	jge	near .exit

	mov	dword [.y],0
	sub	[.draw_height],eax
	imul	dword [ebp+20]
	add	[.buf_ptr],eax

.12
	cmp	eax,256
	jge	near .exit

	mov	edi,0xa0000
	sub	edi,[___djgpp_base_address]
	xor	eax,eax
	mov	al,[ebp+8]
	shl	eax,15
	add	edi,eax
	mov	eax,100
	imul	dword [.y]
	add	edi,eax
	mov	[.vga_ptr],edi

	mov	dword [.c1],0

.1
	mov	edx,0x3ce
        mov	eax,[.x]
	add	eax,[.c1]
	shl	eax,8
	and	ah,3
	mov	al,4
	out	dx,ax

	mov	esi,[.vga_ptr]
	mov	eax,[.x]
	add	eax,[.c1]
	sar	eax,2
	add	esi,eax

	mov	edi,[.buf_ptr]
	add	edi,[.c1]

	mov	ebx,[.draw_height]

.2
	mov	ecx,[.draw_width]
	add	ecx,3
	sub	ecx,[.c1]
	shr	ecx,2
	mov	edx,ecx
	cmp	ecx,0
	jle	.4

.3
	mov	al,[esi]
	mov	[edi],al
	inc	esi
	add	edi,4
	dec	cx
	jnz	.3

.4
	add	esi,100
	sub	esi,edx

	add	edi,[ebp+20]
	mov	eax,edx
	shl	eax,2
	sub	edi,eax

	dec	ebx
	jnz	.2

	inc	byte [.c1]
	cmp	byte [.c1],4
	jl	near .1

.exit
	popa
	pop	ebp
	ret

.c1		dd	0
.vga_ptr	dd	0
.x		dd	0
.y		dd	0
.draw_width	dd	0
.draw_height	dd	0
.buf_ptr	dd	0


; void put_block(char page, long x, long y, long width, long height, char *buffer);

_put_block
	push	ebp
	mov	ebp,esp
	pusha

	mov	eax,[ebp+12]
	mov	[.x],eax
	mov	eax,[ebp+16]
	mov	[.y],eax
	mov	eax,[ebp+20]
	mov	[.draw_width],eax
	mov	eax,[ebp+24]
	mov	[.draw_height],eax
	mov	eax,[ebp+28]
	mov	[.buf_ptr],eax

	mov	eax,[.x]
	or	eax,eax
	jns	.11

	neg	eax
	cmp	eax,[ebp+20]
	jge	near .exit

	mov	dword [.x],0
	sub	[.draw_width],eax
	add	[.buf_ptr],eax

.11
	cmp	dword [.x],400
	jge	near .exit

	mov	eax,[.y]
	or	eax,eax
	jns	.12

	neg	eax
	cmp	eax,[ebp+24]
	jge	near .exit

	mov	dword [.y],0
	sub	[.draw_height],eax
	imul	dword [ebp+20]
	add	[.buf_ptr],eax

.12
	cmp	eax,256
	jge	near .exit

	mov	edi,0xa0000
	sub	edi,[___djgpp_base_address]
	xor	eax,eax
	mov	al,[ebp+8]
	shl	eax,15
	add	edi,eax
	mov	eax,100
	imul	dword [.y]
	add	edi,eax
	mov	[.vga_ptr],edi

	mov	dword [.c1],0

.1
	mov	edx,0x3c4
	mov	eax,0x100
        mov	ecx,[.x]
	add	ecx,[.c1]
	and	ecx,3
	shl	eax,cl
	mov	al,2
	out	dx,ax

	mov	esi,[.buf_ptr]
	add	esi,[.c1]

	mov	edi,[.vga_ptr]
	mov	eax,[.x]
	add	eax,[.c1]
	sar	eax,2
	add	edi,eax

	mov	ebx,[.draw_height]

.2
	mov	ecx,[.draw_width]
	add	ecx,3
	sub	ecx,[.c1]
	shr	ecx,2
	mov	edx,ecx
	cmp	ecx,0
	jle	.4

.3
	mov	al,[esi]
	mov	[edi],al
	add	esi,4
	inc	edi
	dec	ecx
	jnz	.3

.4
	add	esi,[ebp+20]
	mov	eax,edx
	shl	eax,2
	sub	esi,eax

	add	edi,100
	sub	edi,edx

	dec	ebx
	jnz	.2

	inc	byte [.c1]
	cmp	byte [.c1],4
	jl	near .1

.exit
	popa
	pop	ebp
	ret

.c1		dd	0
.vga_ptr	dd	0
.x		dd	0
.y		dd	0
.draw_width	dd	0
.draw_height	dd	0
.buf_ptr	dd	0

