#include "tab.h"
#include <pthread.h>

extern int thread_count;
extern cell **tab;
extern config conf;
extern pthread_mutex_t mutex;
extern FILE *f;

void initCellSpace(cell*** tab, config conf){
    (*tab) = malloc(sizeof(**tab) * conf.sizeX);
    if((*tab) != NULL){
        for(int i = 0; i < conf.sizeX;  i++){
            (*tab)[i] = malloc(sizeof( **tab[i]) * conf.sizeY);
        }
    }

    cell defaultcell = {SUCEPTIBLE, ADULT, 0};

    for(int i = 0; i < conf.sizeX; i++){
        for(int j = 0; j < conf.sizeY; j++){
            (*tab)[i][j] = defaultcell;
        }
    }

    //Leaving the infection
    int population = conf.sizeX * conf.sizeY;
    int peopleInIncubation = conf.incubatedPercent*population / 100;
    int peopleInfected = conf.infectedPercent*population / 100;

    int i = 0;
    while(i < peopleInIncubation){

        //Creating new random position
        positon pos = {rand() % conf.sizeX,rand() % conf.sizeY};
        //Insert a new incubation cell
        if((*tab)[pos.x][pos.y].state == SUCEPTIBLE){
            cell incubated = {INCUBATION,ADULT,INCUBATION_PERIOD};
            (*tab)[pos.x][pos.y] = incubated;
            i++;
        }
    }

    i = 0;
    while(i < peopleInfected){

        //Creating new random position
        positon pos = {rand() % conf.sizeX,rand() % conf.sizeY};
        //If position is free
        //Insert a new infected cell
        if((*tab)[pos.x][pos.y].state == SUCEPTIBLE){
            cell incubated = {INFECTIOUS,ADULT,INFECTIOUS_PERIOD};
            (*tab)[pos.x][pos.y] = incubated;
            i++;
        }
    }

}

data calculateData(cell*** tab, config conf){

    //if it is not instantiated the variables
    //take random values according to what it finds in memory
    data dt = {0,0,0,0,0,0,0};

    dt.population = conf.sizeX * conf.sizeY;

    for(int i = 0; i<conf.sizeX;i++){
        for(int j = 0; j<conf.sizeY;j++){
            if((*tab)[i][j].state == SUCEPTIBLE){
                dt.suceptible ++;
            }
            else if((*tab)[i][j].state == INCUBATION){
                dt.incubation ++;
            }
            else if((*tab)[i][j].state == INFECTIOUS){
                dt.infectious ++;
            }
            else if((*tab)[i][j].state == ISOLATED){
                dt.isolated ++;
            }
            else if((*tab)[i][j].state == IMMUNITY){
                dt.immunity ++;
            }
            else if((*tab)[i][j].state == DEATH){
                dt.death ++;
            }
        }
    }

    return dt;
}

void displayData(data dt, int turn){
    printf("%d %d %d %d %d %d %d\n", turn, dt.suceptible, dt.incubation, dt.infectious, dt.isolated, dt.immunity, dt.death);
}

void * seirProcessing(void *rank){
    long my_rank = (long) rank;
    int i, j;
    int local_size_x = conf.sizeX/thread_count;
    int my_first_row_x = my_rank*local_size_x;
    int my_last_row_x = (my_rank + 1) * local_size_x -1;

    for(int laps = 0; laps < conf.lapsNumber; laps++){

        for(i = my_first_row_x; i <= my_last_row_x;i++){
            for(j = 0; j < conf.sizeX;j++){
                positon pos = {i,j};
                //Dynamic update of the neighboring
                cell *neighboringCells[8];
                int incr = 0;
                for(int x = -1; x<=1; x++){
                    for(int y = -1; y<=1; y++){
                    //I do not want initial cell in my moore array
                        if(x != 0 && y!= 0){
                            neighboringCells[incr] = &(tab)[((conf.sizeX+pos.x)+x)%conf.sizeX][((conf.sizeY+pos.y)+y)%conf.sizeY];
                            incr++;
                        }
                    }
                }

                //Dynamic update of the cell
                cell *actualCell = &(tab)[pos.x][pos.y];

                pthread_mutex_lock(&mutex);

                switch ((*actualCell).state)
                {
                    case SUCEPTIBLE:
                        findNewStateOfSuceptibleCell(actualCell,neighboringCells);
                        break;

                    case INCUBATION:
                        if((*actualCell).statusTimeLeft != 0){
                        (*actualCell).statusTimeLeft--;
                        }
                        else{
                            (*actualCell).state = INFECTIOUS;
                            (*actualCell).statusTimeLeft = INFECTIOUS_PERIOD;
                        }
                        break;

                    case INFECTIOUS:
                        if((*actualCell).statusTimeLeft != 0){
                            (*actualCell).statusTimeLeft --;
                        }
                        if((*actualCell).statusTimeLeft == 0){
                            (*actualCell).state = DEATH;
                        }
                        else if(returnProba(P_BECOME_ISOLATED) == 1){
                            (*actualCell).state = ISOLATED;
                            (*actualCell).statusTimeLeft = ISOLATED_PERIOD;
                        }
                        else if(returnProba(C_CONTAMINATE_NEIGHBOR) == 1){
                            int neighbor = rand() % 8;
                            (*neighboringCells)[neighbor].state = INFECTIOUS;
                            (*neighboringCells)[neighbor].statusTimeLeft = INFECTIOUS_PERIOD;
                        }
                        break;

                    case ISOLATED:
                        if((*actualCell).statusTimeLeft != 0){
                            (*actualCell).statusTimeLeft--;
                        }
                        if((*actualCell).statusTimeLeft == 0){
                            (*actualCell).state = DEATH;
                        }
                        else if(returnProba(P_BECOME_IMMUNE) == 1){
                            (*actualCell).state = IMMUNITY;
                        }
                        break;

                    default:
                        break;
                }
                fprintf(f,"%d %d %d %s\n",laps, i,j,stateToStrings(actualCell->state));
                pthread_mutex_unlock(&mutex);
                //end of a for a cell
            }
        }
    }
    return NULL;
}


void findNewStateOfSuceptibleCell(cell* concernedCell, cell** neighbors){

    int P = 0;

    for(int i = 0; i <8 ; i++){
        switch((*neighbors)[i].state){
            case INCUBATION:
                P += P_INCUBATION;
                break;
            case INFECTIOUS:
                P += P_INFECTIOUS;
                break;
            case DEATH:
                P += P_DEATH;
                break;
            case ISOLATED:
                P += P_ISOLATED;
                break;
            case IMMUNITY:
                P += P_IMMUNITY;
                break;
            default:
                break;
        }
    }

    if(returnProba(P) == 1){
        concernedCell->state = INCUBATION;
        concernedCell->statusTimeLeft = INCUBATION_PERIOD;
    }

}

int returnProba(int percent){
    int res = rand() % 100;
    for(int i = 1; i<=percent;i++){
        if(i == res) return 1;
    }
    return -1;
}

char* stateToStrings(status state){
    switch (state)
    {
    case SUCEPTIBLE:
        return "SUCEPTIBLE";
        break;
    case INCUBATION:
        return "INCUBATION";
        break;
    case INFECTIOUS:
        return "INFECTIOUS";
        break;
    case ISOLATED:
        return "ISOLATED";
        break;
    case IMMUNITY:
        return "IMMUNITY";
        break;
    case DEATH:
        return "DEATH";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}