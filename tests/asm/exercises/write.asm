    section .data
SYS_EXIT  equ 1
SYS_WRITE equ 1
STDOUT    equ 1

    section	.text
    global _start
	
_start:
    mov rdx, 66
    ; add '0' to to convert the sum from decimal to ASCII
    add rdx, '0'
    
    ; print the sum 
    mov rax, SYS_WRITE        
    mov rbx, STDOUT
    mov rcx, rdx
    mov rdx, 2
    int 0x80

exit:    
    mov eax, SYS_EXIT   
    xor ebx, ebx 
    int 0x80
