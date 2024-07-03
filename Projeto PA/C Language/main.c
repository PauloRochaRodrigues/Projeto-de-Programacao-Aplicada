#include "new_print.h"
#include "transmissor.h"
#include "receptor.h"
#include <stdlib.h>
#include <locale.h>

void Menu(){
    
    while(TRUE){
        fflush(stdin);
        printf("\033[1;31m");
        printf(" -------------------------------------------------\n");
        printf("|    Escolha para onde a mensagem sera enviada    |\n");
        printf(" -------------------------------------------------\n");
        printf("\033[0m");

        int input;

        new_print("\n   | 1. \033[1m PC \033[0m (Receber)\n   | 2. \033[1m Arduino \033[0m (Enviar)\n",15);
        printf("\n>");
        if(scanf("%d", &input) == 0){
            break;
        }

        fflush(stdin);

        if (input == 1){
            system("cls");
            receptor("\\\\.\\COM1");
        }else if (input == 2){
            system("cls");
            transmissor("\\\\.\\COM2");
        }else{
            system("cls");
            new_print("Nao entendi.\n",10);
        }
    }
}

int main(){
    printf("\033[1;31m");
    printf(" -----------------------------------------\n");
    printf("|               Projeto de PA             |\n");
    printf(" -----------------------------------------\n");
    printf("\033[0m");
    printf("\033[1m");
    new_print("Integrantes:",10);
    printf("\033[0m");
    new_print("\n    Alex Machado",10);
    new_print("\n    Leanderson Calazans",10);
    new_print("\n    Paulo Henrique",10);
    new_print("\n    Virky Chen\n\n",10);
    system("pause");
    system("cls");
    Menu();

    return 0;
}

