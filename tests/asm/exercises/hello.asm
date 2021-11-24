    section	.data
msg db 'Hello, world!', 0xa  ;string to be printed
len equ $ - msg     ;length of the string

    section	.text
    global _start     ;must be declared for linker (ld)
	
_start:	            ;tells linker entry point
    mov	rdx, len     ;message length
    mov	rcx, msg     ;message to write
    mov	rbx, 1       ;file descriptor (stdout)
    mov	rax, 4       ;system call number (sys_write)
    int	0x80        ;call kernel
	
    mov	rax, 60      ;system call number (sys_exit)
    mov rdi, 0
    syscall

