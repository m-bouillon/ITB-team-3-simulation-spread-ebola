#include <stdio.h>
#include <pthread.h>

#include "tab.h"

//The test space in 2d that we will use throughout the program
cell **tab;
config conf;
int thread_count;
pthread_mutex_t mutex;

FILE *f;

int main(int argc, char **argv){

    thread_count = 4;
    long thread;

    //wb for clean the data in the file
    f = fopen("out/history.txt","wb");

    pthread_t* thread_handles;
    thread_handles = malloc(thread_count*sizeof(pthread_t));
    pthread_mutex_init(&mutex,NULL);


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

    conf = readConfigFile(argv[1]);
    if(checkConfFile(conf) == -1){
        fprintf(stderr, "Error : The config file is incorrect\nPlease Check it\n");
        return 0;
    }
    // START
    //Init random
    srand(time(NULL));

    initCellSpace(&tab, conf);
    //data dt = calculateData(&tab, conf);

    printf("turn cell_pos_x cell_pos_y cell_state\n");

    for(thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread],NULL, seirProcessing,(void*) thread);
        pthread_join(thread_handles[thread],NULL);
    }

    //Bug with this, use realloc maybe
    // for (int i = 0; i < conf.sizeX; i++){
    //     cell** cellToFree = &(tab[i]);
    //     free(*cellToFree);
    // }

    free(thread_handles);

    pthread_mutex_destroy(&mutex);

    fclose(f);
    return 0;
}