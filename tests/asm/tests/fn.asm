section .text
global _start

_start:
    call func
    
    mov rax, 60
    mov rdi, 0
    syscall

func:
    push 66
    mov     rax, 1    ; sys_write call number 
    mov     rdi, 1    ; write to stdout (fd=1)
    mov     rsi, rsp   ; B
    mov     rdx, 1    ; write 1 char
    syscall
    add rsp, 8
    ret
