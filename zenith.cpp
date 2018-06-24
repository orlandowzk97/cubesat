/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * This code is a part of the first MVP created in the CubeSat Z-Sat development.  *
 * Author: jmssouza                                                                *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "zenith.h"

int interfaceOperator(){
    int option;
    system("cls");
    printf("============================================================================================================\n");
    printf("=                                            CubeSat v0 - ZSat                                             =\n");
    printf("============================================================================================================\n");
    printf(" Choose:\n");
    printf("   1. to send a message;\n");
    printf("   2. to read messages already sended;\n");
    printf("   3. to read new messages received;\n");
    printf("   9. to exit;\n\n\n");
    printf(" Your choice: ");
    scanf ("%d", &option);
    return option;
}

int valueGetter(char *file_name, int *value){
    FILE *file;

    file = fopen(file_name, "r");
    if (file!=NULL){
        fscanf(file, "%d", value);
        fclose(file);
        return 1;
    }
    else{
        printf("ERROR - file %s cannot be opened", file_name);
        return 0;
    }
}

int valueSetter(char *file_name, int value){
    FILE *file;

    file = fopen(file_name, "w");
    if (file!=NULL){
        fprintf(file,"%d", value);
        fclose(file);
        return 1;
    }
    else{
        system("cls");
        printf("ERROR - file %s cannot be created", file_name);
        return 0;
    }
}

int writeMessage(char *file_name, char *message, int position, int size){

    FILE *file;
    int i;

    file = fopen(file_name, "r+b");

    if (file != NULL){
        fseek(file, position*(size+1), SEEK_SET);
        fwrite(message, size, 1, file); //We add 1 to PACK_SIZE because we need a byte to confirm the status of the package
        fclose(file);
        return 1;
    }
    else{
        printf("Error - file %s cannot be opened.", file_name);
        return 0;
    }
}

int readMessage(char *file_name, char *message, int position, int size){

    FILE *file;
    int i;

    for(i=0;i<size;i++){
        message[i]=0;
    }

    file = fopen(file_name, "r+b");

    if (file != NULL){
        fseek(file, position*(size+1), SEEK_SET);
        fread(message, size, 1, file);
        fclose(file);
        return 1;
    }
    else{
        printf("Error - file cannot be opened.");
        return 0;
    }
}

int blockBuilder(char *block, int operating_mode, int aux){

    switch (operating_mode){
        case 1: { // Mission 1
            valueGetter(HEALTH_NUMBER, &aux);
            readMessage(HEALTH_FILE, block, aux, BLOCK_SIZE);
            break;
        }
        case 2: {
            break;
        }
        case 3: { break; }
        case 4: { break; }
        case 5: { break; }
        case 6: { break; }
        case 7: { break; }
        default:{ return 0; }
    }
    return 1;
}

int packageCreator(char *pack_num_file, char *pack_cycle_file, char *block, char *message){

    char package[PACK_SIZE];

    int i;
    int aux;
    int pack_number;
    int pack_cycle;
    int op_mode;

    for (i=0;i<PACK_SIZE+1;i++){
        package[i] = 0;
    }

    //Getting data and setting the system
    valueGetter(pack_num_file  , &pack_number);        //Getting the number of the last packet created
    valueGetter(pack_cycle_file, &pack_cycle );        //Getting the number of the current cycle in CubeSat
    valueGetter( MODE_FILE     , &op_mode    );        //Getting the number of operating mode

    pack_number++;
    if (pack_number>PACK_SIZE){
        pack_number=0;
        pack_cycle++;
    }

    valueSetter(pack_num_file  , pack_number);
    valueSetter(pack_cycle_file, pack_cycle );

    //Building the package

    package[0]=pack_number;
    package[1]=pack_cycle;
    package[2]=op_mode;

    package[123]=pack_number;
    package[124]=op_mode;

    package[252]=pack_number;
    package[253]=op_mode;

    for(i=3;i=251;i++){
        if (i<123){
            package[i]=*(block+(i-3));
        }
        else if (i>124){
            package[i] = *(block+(i-5));
        }
    }

    aux = pack_cycle*255+pack_number;
    writeMessage(file, package, aux);

    return 0;
} //finished

int missedPacketsChecker(int expected_packet, int received_packet){

    int missed_packets = 0;

    if (expected_packet == received_packet){
        valueSetter(tm_number, expected_packet);
        writeMessage(tm_file, message, expected_packet);
        return 0;
    }
    else {
        //Criar função que trate os pacotes perdidos
        missed_packets = expected_packet - received_packet;
        printf("\n\n\nWe lost %d packages\n\n\n", pack_num_received[0] - pack_num_system);
        return missed_packets;
    }
}

int packageAnalyzer(){   //Must be constructed yet

    char message[255];
    char missed_array[255];
    int aux;
    int pack_num_system       = 0;
    int operating_mode_system = 0;
    int missed_packets        = 0;
    int pack_num_received[3]  = {0,0,0};
    int operating_mode_rec[3] = {0,0,0};

    valueGetter(TC_NUMBER, &pack_num_system);
    readMessage(NEW_TM, message, 0);

    pack_num_received[0] = message[0];
    pack_num_received[1] = message[123];
    pack_num_received[2] = message[252];

    operating_mode_rec[0] = message[2];
    operating_mode_rec[1] = message[124];
    operating_mode_rec[2] = message[253];

    //If all three pack_num received are the same
    if ((pack_num_received[0] == pack_num_received[1]) && (pack_num_received[1] == pack_num_received[2])){
        aux = pack_num_received[0];
        missed_packets = missedPacketsChecker(pack_num_system+1, aux);
        pack_num_system = pack_num_received[0];
        valueSetter(TC_NUMBER, pack_num_system);
        writeMessage(TC_FILE, message, pack_num_system);
        printf("Packet %d received.", pack_num_system);

        if (missed_packets != 0){
            valueSetter(MISSED_PACKETS, missed_packets);
            for (i=0;i<missed_packets;i++){
                missed_array[i]= pack_num_system - missed_packets + i;
            }
            writeMessage(MISSED_PACKETS, missed_array, 0);
        }
        else{
            writeMessage(MISSED_PACKETS, 0, 0);
        }
    }
    else{
        printf("The packet has problems");
        //if ((pack_num_received[0] == pack_num_received[1]) && (pack_num_received[1] != pack_num_received[2])){
        //aux = pack_num_system + 1;
        //if (aux == pack_num_received[0]){
        //  valueSetter()
        //}
    }

    if ((operating_mode_rec[0] == operating_mode_rec[1]) && (operating_mode_rec[1] == operating_mode_rec[2])) {
        operating_mode_system = operating_mode_rec[0];
        valueSetter(MODE_FILE, operating_mode_system);
        printf("\nNew operating mode: %d", operating_mode_system);
    }

    return 0;
}

int Operator(){

    int aux;
    int mode;
    int *pack_tm;
    int *pack_tc;
    char tm[255];
    char tc[255];


    pack_tm = &global_pack_tm_number;
    pack_tc = &global_pack_tc_number;

    valueGetter(TM_NUMBER, pack_tm);
    mode = interfaceOperator();

    switch(mode) {
        case 1: {
            system("cls");
            printf("============================================================================================================\n");
            printf("=                                            CubeSat v0 - ZSat                                             =\n");
            printf("============================================================================================================\n");
            printf("Type a text to send (it must be smaller than 254 characters and has no spaces): \n   ");
            scanf ("%s", tc);
            writeMessage(TC_FILE, tc, global_pack_tc_number);
            global_pack_tc_number++;
            valueSetter(TC_NUMBER, global_pack_tc_number);
            break;
        }
        case 2: {
            system("cls");
            printf("============================================================================================================\n");
            printf("=                                            CubeSat v0 - ZSat                                             =\n");
            printf("============================================================================================================\n");
            printf("Type the number of the package that you want to read: ");
            scanf ("%d", &aux);
            aux--;
            readMessage(TC_FILE, tc, aux);
            break;
        }
        case 9: {
            return 0;
        }
        default: {
            system("cls");
            printf("Invalid entry.");
            break;
        }
    }
    return 1;
}

int initializingCubeSat(){
    system("cls");
    printf("Initializing Zenith CubeSat...\n");
    return 0;
}

int CubeSat(){

    int tc_pack_number = 0;
    int operating_mode = 0;
    int block_position = 0;

    char info[PACK_SIZE-7];
    char tm[PACK_SIZE];

    valueGetter(TC_NUMBER, &tc_pack_number);
    valueGetter(MODE_FILE, &operating_mode);

    initializingCubeSat();

    switch (operating_mode){

        case 1: {
            system("clear");
            printf("Operating mode 1 - Sending a simple message about Cubesat health");
            valueGetter(HEALTH_NUMBER, &block_position);
            blockBuilder(HEALTH_FILE, info, operating_mode, block_position);
            packageCreator(TM_NUMBER, TM_CYCLE, info, tm);
            printf("Message sended: \n    %s", tm);
            break;
        }
        case 2: {
            system("clear");
            printf("You are on mode 2");

            break;
        }
        case 3: {
            system("clear");
            printf("You are on mode 3");

            break;
        }
        case 4: {
            system("clear");
            printf("You are on mode 4");

            break;
        }
        case 5: {
            system("clear");
            printf("You are on mode 5");

            break;
        }
        case 6: {
            system("clear");
            printf("You are on mode 6");

            break;
        }
        case 7: {
            system("clear");
            printf("You are on mode 7");

            break;
        }
        default: {
            system("clear");
            break;
        }
    }


}

//end
