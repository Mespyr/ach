; calculate average grade
    segment .data
g1  dq      98      ; grade 1
g2  dq      77      ; grade 2
g3  dq      86      ; grade 3
g4  dq      97      ; grade 4
avg dq      0       ; average grade
rem dq      0       ; remainder

    segment .text
    global _start

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

_start:
    ; grade amount
    mov rbx, 4
    ; add all grades together
    mov rax, [g1]
    add rax, [g2]
    add rax, [g3]
    add rax, [g4]
    ; divide by 4
    idiv rbx
    mov [avg], rax
    mov [rem], rdx
    ; print average
    mov rdi, [avg]
    call print
    ; print remainder
    mov rdi, [rem]
    call print
    ; exit 
    mov rax, 60
    mov rdi, 0
    syscall
