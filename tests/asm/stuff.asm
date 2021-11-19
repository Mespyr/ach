print:
    mov     r9, -3689348814741910323
    sub     rsp, 40
    mov     BYTE [rsp+31], 10
    lea     rcx, [rsp+30]
.L2:
    mov     rax, rdi
    lea     r8, [rsp+32]
    mul     r9
    mov     rax, rdi
    sub     r8, rcx
    shr     rdx, 3
    lea     rsi, [rdx+rdx*4]
    add     rsi, rsi
    sub     rax, rsi
    add     eax, 48
    mov     BYTE [rcx], al
    mov     rax, rdi
    mov     rdi, rdx
    mov     rdx, rcx
    sub     rcx, 1
    cmp     rax, 9
    ja      .L2
    lea     rax, [rsp+32]
    mov     edi, 1
    sub     rdx, rax
    lea     rsi, [rsp+32+rdx]
    mov     rdx, r8
    mov     rax, 1
    syscall
    add     rsp, 40
    ret

segment .text
global _start

exit:
    mov rax, 60
    mov rdi, 0
    syscall

_start:
    ;; push
    push 35
    push 34
    ;; add
    pop rax
    pop rbx
    add rax, rbx
    push rax
    ;; print
    pop rdi
    call print
    ;; push
    push 500
    push 80
    ;; subtract
    pop rax
    pop rbx
    sub rbx, rax
    push rbx
    ;; print
    pop rdi
    call print
    ;; exit
    call exit

