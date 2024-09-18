bits 16 ; Instrui o montador a emitir comandos 16 bits
org 0x7C00 ; Colocando o código no offset desse endereço, que é onde a BIOS vai carregar o bootloader
_start:
    mov si, 0 ; Inicializando registradores
    mov ax, si
    mov bx, si
    mov cx, si
    mov dx, si
    mov sp, 0x7C00 ; Utilizando o espaço antes do programa como pilha
    mov bx, message
    push bx
    call print
    jmp $ ; Mantem o programa nesse ponto depois de printar tudo na tela

print:
    pop bx
    mov si, 0
.loop:
    mov ah, 0x0E
    mov al, [message + si] ; Vai mostrar letra por letra da string no endereço message
    int 0x10
    add si, 1 ; soma o contador
    cmp byte [message + si], 0 ; se a próxima letra for 0, a string acabou
    jne .loop ; Caso contrário, retorna o loop
    ret

message:
    db `Hello, Boot!\r\n`, 0

times 510 - ($ - $$) db 0
dw 0xAA55 ; Carregar esse valor informa a BIOS que se trata de um bootloader
