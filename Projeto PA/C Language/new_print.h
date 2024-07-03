#include <windows.h>
#include <stdio.h>

void new_print(char* texto, int delay){
    int ch;
    int tamanho = strlen(texto);

    for (ch = 0; ch<tamanho; ch++){
        printf("%c",texto[ch]);
        Sleep(delay);
    }
}