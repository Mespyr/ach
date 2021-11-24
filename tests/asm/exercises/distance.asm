; compute the distance squared between 2 points on a plane
    segment .data
x1  dq      20       ; point 1.x
y1  dq      4       ; point 1.y
x2  dq      5       ; point 2.x
y2  dq      8       ; point 2.y

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
    ; calculate third point to form triange
    mov rax, [y1] ; y-val of third point
    mov rbx, [x2] ; x-val of third point
    ; calculate distance between point 1 and 3 (x)
    mov rcx, [x1] ; copy x value of point 1 to register
    sub rcx, rbx  ; difference between rcx (p1.x) and rbx (p3.x)
    ; calculate distance between point 2 and 3 (y)
    mov rdx, [y2] ; copy y value of point 2 to register
    sub rdx, rax  ; difference between rdx (p2.y) and rax (p3.y)
    ; pythagorean theorem
    imul rcx, rcx ; get a^2
    imul rdx, rdx ; get b^2
    add rcx, rdx  ; c^2
    ; print c^2
    mov rdi, rcx
    call print
    ; exit 
    mov rax, 60
    mov rdi, 0
    syscall
