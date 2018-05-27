//
// Created by joaom on 25/05/2018.
//

#define PACK_SIZE 255
char nomi[14] = "numPack.dat";
#include <stdlib.h>
#include <conio.h>

int packNum = 0;


int interfaceOperator();
int packNumGetter(char* name);
int packNumSetter(char* name);
void writeMessage(char *name);
void readMessage(char *name, int numPack);
int Operator (char * name);


int interfaceOperator(){
    int option;
    system("cls");
    printf("============================================================================================================\n");
    printf("=                                            CubeSat v0 - ZSat                                             =\n");
    printf("============================================================================================================\n");
    printf(" Choose:\n");
    printf("   1. to send a message;\n");
    printf("   2. to read messages already sended;\n");
    printf("   3. to read new messages received;\n"  );
    printf(" Your choice: ");
    scanf ("%d", &option);
    return option;
}

int packNumGetter(char* name){
    FILE * file;
    file = fopen(name, "r");
    if (file!=NULL){
        fscanf(file, "%d", &packNum);
        fclose(file);
        return 1;
    }
    else{
        return 0;
    }
}

int packNumSetter(char* name){
    FILE * file;

    file = fopen(name, "w");
    if (file!=NULL){
        fprintf(file,"%d", packNum);
        fclose(file);
        return 1;
    }
    else{
        system("cls");
        printf("ERROR - packNum.dat cannot be created");
        system("pause");
        return 0;
    }
}

void writeMessage(char *name){

    FILE * file;
    char message[PACK_SIZE];
    int i;

    for (i=0;i<PACK_SIZE;i++){
        message[i]=0;
    }

    system("cls");
    printf("Digite o texto com ate 254 caracteres para ser enviado :\n");
    scanf("%s", message);


    file = fopen(name, "r+b");
    if (file != NULL){
        fseek(file, packNum*PACK_SIZE, SEEK_SET);
        fwrite(message, PACK_SIZE, 1, file);
        fclose(file);
        packNum++;
        packNumSetter(nomi);
    }
    else{
        printf("Erro de abertura de arquivo");
    }
}

void readMessage(char *name, int num_pack){

    FILE * file;
    char wait;
    char message[PACK_SIZE];
    int i;

    for(i=0;i<PACK_SIZE;i++){
        message[i]=0;
    }

    file = fopen(name, "r+b");
    if (file != NULL){
        fseek(file, num_pack*PACK_SIZE, SEEK_SET);
        fread(message, PACK_SIZE, 1, file);
        fclose(file);
        printf("%s \n", message);
        printf("Type something to exit.");
        wait = getch();
    }
    else{
        printf("Erro de abertura de arquivo");
    }
}

int Operator(char * name){

    int mode;

    mode = interfaceOperator();
    packNumGetter(nomi);
    switch(mode) {
        case 1: {
            writeMessage(name);
            //transmiter();
            break;
        }
        case 2: {

            int Num;

            system("cls");
            printf("Digite o numero do pacote enviado que deseja ler: ");
            scanf("%d", &Num);
            Num--;

            readMessage(name,Num);
            //receiver();
            break;
        }
        default: {
            system("cls");
            printf("Invalid entry.");
        }
    }
    return 1;
}