section .data
    rowhammer_data resb 10
    msg db '[ROWHAMMER]', 0xa
    len equ $ - msg

section .text   
    global _start

_start:
    ; initialize array to hammer
    mov rdi, rowhammer_data
    mov [rdi], byte 0
    mov [rdi + 1], byte 1
    mov [rdi + 2], byte 2
    mov [rdi + 3], byte 3
    mov [rdi + 4], byte 4
    mov [rdi + 5], byte 5
    mov [rdi + 6], byte 6
    mov [rdi + 7], byte 7
    mov [rdi + 8], byte 8
    mov [rdi + 9], byte 9

    ; create counter
    mov rax, 0
    mov rcx, 1700000        ; how many read/write

rowhammer_loop:
    ; rowhammer attack
    mov byte [rdi + 1], 0xAA
    mov byte [rdi + 3], 0xAA

    clflush [rdi + 1]
    clflush [rdi + 3]

    cmp rax, rcx
    inc rax

    jl rowhammer_loop

    ; check bit flips
    cmp [rdi], byte 0
    jnz detect_rowhammer

    cmp [rdi + 2], byte 2
    jnz detect_rowhammer

    cmp [rdi + 4], byte 4
    jnz detect_rowhammer

    mov rax, 0
    jmp rowhammer_loop

detect_rowhammer:
    ; print rowhammer text
    mov edx, len
    mov ecx, msg
    mov ebx, 1
    mov eax, 4
    int 0x80

    ; exit program
    mov ebx, 0
    mov eax, 1
    int 0x80