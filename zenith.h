/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * CubeSat v1.0                                                                    *
 * Authors: João Matheus Siqueira Souza      (jmssouza)                            *
 *          Orlando Wozniak de Lima Nogueira ()                                    *
 *          Vinicius Eiji Sasaki             ()                                    *
 * São Carlos School of Engineering - University of São Paulo                      *
 * First semester - 2018                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CUBESAT_ZENITH_H
#define CUBESAT_ZENITH_H

//Global variables are declared here

    int global_pack_tm_number = 0;
    int global_pack_tc_number = 0;

//Libraries are declared here

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <time.h>
    #include "constants.h"
    #include "zenith.c"

//Functions body are declared here

    int headerInterface();
    int interfaceOperator(int mode, int pack_num, char *info);
    int valueGetter(char *file_name, int *value);
    int valueSetter(char *file_name, int value);
    int writeMessage(char *file_name, char *message, int position, int size, int check);
    int readMessage(char *file_name, char *message, int position, int size, int check);
    int blockBuilder(char *block, int operating_mode, int aux);
    int write_i2c(char *file_name, int packet, int qt, int addr,int chan);
    int read_i2c(char *file_name, int position, int addr,int chan);
    int packageCreator(char *pack_num_file, char *pack_cycle_file, char *block, char *message);
    int missedPackagesChecker(int expected_package, int received_package);
    int correctValue(int *values);
    int packageAnalyzer();
    void delay(unsigned int milis);
    int Base();
    int initializingCubeSat();
    int CubeSat();
    int CubeSatTest();

#endif //CUBESAT_ZENITH_H
