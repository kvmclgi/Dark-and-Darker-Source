extern legitTrampoline:QWORD ; add rsp, 68h, ret
extern legitTrampoline2:QWORD ; jmp rbx

.data
RBX_Saved QWORD 0
RET_Saved QWORD 0

.code
_spoofer_stub PROC
	
	mov dl, dl
	mov rax, [rsp]
	mov dl, dl
	xor rax, rbx
	mov dl, dl
	mov RET_Saved, rax
	mov dl, dl
	mov rax, legitTrampoline2
	mov dl, dl
	mov RBX_Saved, rbx
	mov dl, dl
	mov [rsp], rax
	mov dl, dl
	lea rbx, PostCallRBX
	mov dl, dl
	sub     rsp, 68h
	mov dl, dl
	mov     r10, [rsp+144]
	mov dl, dl
	mov     rax, [rsp+200]
	mov dl, dl
	mov     [rsp+88], rax
	 
	mov dl, dl
	 
	mov     rax, [rsp+192]
	 
	mov dl, dl
	 
	mov     r11, [r10]
	 
	mov dl, dl
	 
	mov     [rsp+80], rax
	 
	mov dl, dl
	 
	mov     rax, [rsp+184]
	 
	mov dl, dl
	 
	mov     [rsp+72], rax
	 
	mov dl, dl
	 
	mov     rax, [rsp+176]
	 
	mov dl, dl
	 
	mov     [rsp+64], rax
	 
	mov dl, dl
	 
	mov     rax, [rsp+168]
	 
	mov dl, dl
	 
	mov     [rsp+56], rax
	 
	mov dl, dl
	 
	mov     rax, [rsp+160]
	 
	mov dl, dl
	 
	mov     [rsp+48], rax
	 
	mov dl, dl
	 
	mov     rax, [rsp+152]
	 
	mov dl, dl
	 
	mov     [rsp+40], rax
	 
	mov dl, dl
	 
	mov     rax, [r10+8]
	 
	mov dl, dl
	 
	mov     [rsp+32], rax
	 
	push    legitTrampoline
	 
	mov dl, dl
	 
	jmp     r11

	PostCallRBX:
	 
	mov rbx, RBX_Saved
	 
	mov dl, dl
	 
	mov rcx, RET_Saved
	  
	xor rcx, rbx
	 
	mov dl, dl
	 
	jmp rcx

_spoofer_stub ENDP
END