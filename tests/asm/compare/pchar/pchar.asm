section .text
    global _start

; print char at register rdi
pchar:
    mov     rax, 1    ; sys_write call number 
    mov     rdi, 1    ; write to stdout (fd=1)
    mov     rsi, rsp  ; use char on stack
    mov     rdx, 1    ; write 1 char
    syscall   
    add     rsp, 8    ; restore sp
    ret

_start:
    push 33
    ; print
    pop rdi
    call pchar

    mov rax, 60
    mov rdi, 0
    syscall
