/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * For instructions, use the zenith.h tutorial.                                    *
 * Author: jmssouza                                                                *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "zenith.h"

int headerInterface(){
    system("clear");
    printf("=========================================================================\n");
    printf("=                           CubeSat v0 - ZSat                           =\n");
    printf("=========================================================================\n");
    return 0;
}

int interfaceOperator(int mode, int pack_num, char *info){

    int option;

    headerInterface();
    printf(" Current data:\n");
    printf("   Operating mode:  %d ;\n", mode);
    printf("   Package Number:  %d ;\n", pack_num);
    printf("   Info: \n     %s \n\n\n\n\n\n", info);
    printf("=========================================================================\n");
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

int writeMessage(char *file_name, char *message, int position, int size, int check){

    FILE *file;
    int i;

    file = fopen(file_name, "r+b");

    if (file != NULL){
        fseek(file, position*(size+check), SEEK_SET);
        fwrite(message, size, 1, file); //We add 1 to PACK_SIZE because we need a byte to confirm the status of the package
        fclose(file);
        return 1;
    }
    else{
        printf("Error - file %s cannot be opened.", file_name);
        return 0;
    }
}

int readMessage(char *file_name, char *message, int position, int size, int check){

    FILE *file;
    int i;

    for(i=0;i<size;i++){
        message[i]=0;
    }

    file = fopen(file_name, "r+b");

    if (file != NULL){
        fseek(file, position*(size+check), SEEK_SET);
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
        case 1 : { // Mission 1
            valueGetter(HEALTH_NUMBER, &aux);
            readMessage(HEALTH_FILE, block, aux, BLOCK_SIZE, 1);
            break;
        }
        case 2 : {
            break;
        }
        case 3 : { break; }
        case 4 : { break; }
        case 5 : { break; }
        case 6 : { break; }
        case 7 : { break; }
        case 10: {
            char temp[255];
            valueGetter(TC_NUMBER, &aux);
            readMessage(TC_FILE, temp, aux, PACK_SIZE, 1);
            for (aux=0;aux<PACK_SIZE;aux++){
                if((aux >= 3) && (aux <= 122)){
                    block[aux - 3] = temp[aux];
                }
                else if ((aux >= 125) && (aux <= 251)){
                    block[aux - 5] = temp[aux];
                }
            }
            break;
        }
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
    valueGetter(pack_num_file  , &pack_number);        //Getting the number of the last package created
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
    return 0;
} //finished

int missedPackagesChecker(int expected_package, int received_package){

    int missed_packages = 0;

    if (expected_package == received_package){
        valueSetter(TM_NUMBER, expected_package);
        return 0;
    }
    else {
        //Criar função que trate os pacotes perdidos
        missed_packages = expected_package - received_package;
        return missed_packages;
    }
}

int correctValue(int *values){
    if ((values[0]==values[1]) && (values[1]==values[2])){
        return values[0];
    }
    else if ((values[0]==values[1]) && (values[1]!=values[2])){
        return values[0];
    }
    else if ((values[0]!=values[1]) && (values[1]==values[2])){
        return  values[2];
    }
    else {
        return -1;
    }
}

int packageAnalyzer(){

    char message[255];
    char missed_array[255];
    int aux;
    int i = 0;
    int pack_num_system       = 0;
    int operating_mode_system = 0;
    int missed_packages        = 0;
    int pack_num_received[3]  = {0,0,0};
    int operating_mode_rec[3] = {0,0,0};

    valueGetter(TC_NUMBER, &pack_num_system);
    readMessage(NEW_TC, message, 0, PACK_SIZE, 1);

    pack_num_received[0] = message[0];
    pack_num_received[1] = message[123];
    pack_num_received[2] = message[252];

    operating_mode_rec[0] = message[2];
    operating_mode_rec[1] = message[124];
    operating_mode_rec[2] = message[253];

    aux = correctValue(pack_num_received);
    if (aux >= 0){

        missed_packages = missedPackagesChecker(pack_num_system+1, aux);
        pack_num_system = aux;
        valueSetter(TC_NUMBER, pack_num_system);
        writeMessage(TC_FILE, message, pack_num_system, PACK_SIZE, 1);
        printf(" - Package %d received! \n", pack_num_system);
	

        if (missed_packages != 0){
	    printf("\nI have entry in missed_packets\n");
            valueSetter(MISSED_PACKAGES, missed_packages);
            for (i=0;i<missed_packages;i++){
                missed_array[i]= pack_num_system - missed_packages + i;
            }
            writeMessage(MISSED_PACKAGES, missed_array, 0, PACK_SIZE, 1);
        }
        else{
	    printf("\nI did not lost a pack\n");
	    for (i=0;i<missed_packages;i++){
                missed_array[i] = 0;
            } 
            writeMessage(MISSED_PACKAGES, missed_array, 0, PACK_SIZE, 1);
        }
    }
    else{
        printf("\n The package has problems with pack_number");
    }

    aux = correctValue(operating_mode_rec);
    if (aux >= 0 && aux <= 8){
        operating_mode_system = aux;
        valueSetter(MODE_FILE, operating_mode_system);
        return 1;
    }
    else{
        return 0;
    }
}

void delay(unsigned int milis){
    clock_t goal = milis + clock();
    while(goal>clock());
}

int Base(){

    int control = 0;
    int counter = 0;
    int aux;
    int mode;
    int pack_tm;
    int pack_tc;
    char tm[255];
    char tc[255];

    while (control == 0){

        packageAnalyzer();
        valueGetter(MODE_FILE, &mode);
        valueGetter(TC_NUMBER, &pack_tc);
        blockBuilder(tc, 10, aux);	
        interfaceOperator(mode, pack_tc, tc);
        delay(1000000);

        counter ++;
        if (counter==1000) {control = 1;}
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
            blockBuilder( info, operating_mode, block_position);
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

int CubeSatTest(){
    int i = 0;
    int j = 0;
    int mode = 0;
    int pack = 0;
    char msg[255];
    while (i == 0){
        system("clear");
        for(j=0;j<255;j++){
		msg[i] = 0;
	}
	printf(" Digite o modo de operacao - ");
        scanf ("%d", &mode);
	printf("\n Digite o num do pack - ");
        scanf ("%d", &pack);
	printf("\n Digite a msg - ");
	scanf("%s",msg); 
        msg[0]=pack;
	msg[1]=1;
	msg[2]=mode;
	msg[123]=pack;
	msg[124]=mode;
	msg[252]=pack;
	msg[253]=mode;
	writeMessage(NEW_TC, msg, 0, PACK_SIZE, 0);
	printf("\nDigite 1 para sair.\n");
        scanf ("%d", &i);
    }
    return 0;
}

//end
