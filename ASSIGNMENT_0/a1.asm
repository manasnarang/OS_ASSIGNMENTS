global main
extern printf, scanf


section .data
        abc: db "%s", 0
        num: db "%d", 0
        strPrint: db "String is: %s",10,0
        Numprint: db "Number is: %d",10,0

section .bss
        string resb 20
        number resb 4

section .text
main:
        push rbp
        mov rbp, rsp
        sub rsp, 16


        mov eax, 0
        lea rdi, [abc]
        lea rsi, [string]
        call scanf

        mov eax, 0
        lea rdi, [num]
        lea rsi, [number]
        call scanf


        xor eax,eax
        lea rdi, [strPrint]
        lea rsi, [string]
        call printf


        xor eax,eax
        lea rdi, [Numprint]
        mov rsi, [number]
        call printf

        add rsp,16
        leave
        ret