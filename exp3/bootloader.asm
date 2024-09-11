bits 16 ; Instrui o montador a emitir comandos 16 bits
org 0x7C00 ; Colocando o código no offset desse endereço, que é onde a BIOS vai carregar o bootloader

mov si, 0

print:
    mov ah, 0x0E
    mov al, [message + si] ; Vai mostrar letra por letra da string no endereço message
    int 0x10
    add si, 1 ; soma o contador
    cmp byte [message + si], 0 ; se a próxima letra for 0, a string acabou
    jne print ; Caso contrário, retorna o loop

jmp $ ; Mantem o programa nesse ponto depois de printar tudo na tela

message:
    db "Hello, Boot!", 0

times 510 - ($ - $$) db 0
dw 0xAA55 ; Carregar esse valor informa a BIOS que se trata de um bootloader
