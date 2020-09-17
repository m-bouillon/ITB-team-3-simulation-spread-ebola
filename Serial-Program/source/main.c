#include <stdio.h>
#include "tab.h"

int main(int argc, char **argv){

    // Check arguments
    if(argc != 2){
        printf("Please specify a 'config' file as a parameter\n");
        printf("Or enter 'serial -h' or 'serial --help' to find out more\n");
        return 0;
    }

    if(checkArgvParameter(argv[1]) == 1){
        displayHelp();
        return 0;
    }

    config conf = readConfigFile(argv[1]);
    if(checkConfFile(conf) == -1){ 
        fprintf(stderr, "Error : The config file is incorrect\nPlease Check it\n");
        return 0;
    }
    // START 
    //Init random
    srand(time(NULL));

    //The test space in 2d that we will use throughout the program
    cell **tab;
    initCellSpace(&tab, conf);
    //data dt = calculateData(&tab, conf);

    printf("t suceptible incubation infectious isolated immunity dead \n");

    positon pos = {0,0};
    for(int t = 0; t < conf.lapsNumber; t++){
        for(int i = 0; i < conf.sizeX; i++){
            for(int j = 0; j < conf.sizeY ; j++){
                pos.x = i;
                pos.y = j;
                seirProcessing(&tab,pos,conf);
            }
        }
        displayData(calculateData(&tab,conf),t+1);
    }


    return 0;
}