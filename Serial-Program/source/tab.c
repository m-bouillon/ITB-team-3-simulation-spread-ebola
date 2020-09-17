#include "tab.h"

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
    // printf("Population : %d\n", dt.population);
    // printf("Suceptible : %d\n", dt.suceptible);
    // printf("Incubation : %d\n", dt.incubation);
    // printf("Infectious : %d\n", dt.infectious);
    // printf("Isolated : %d\n", dt.isolated);
    // printf("Immunity : %d\n", dt.immunity);
    // printf("Dead : %d\n", dt.death);

    printf("%d %d %d %d %d %d %d\n", turn, dt.suceptible, dt.incubation, dt.infectious, dt.isolated, dt.immunity, dt.death);
}

void seirProcessing(cell*** tab, positon pos, config conf){

    //Cell array to know moore neighboring of actualcell
    //Dynamic update of the neighboring
    cell *neighboringCells[8];
    int incr = 0;
    for(int i = -1; i<=1; i++){
        for(int j = -1; j<=1; j++){
            //I do not want initial cell in my moore array
            if(i != 0 && j!= 0){
                neighboringCells[incr] = &(*tab)[((conf.sizeX+pos.x)+i)%conf.sizeX][((conf.sizeY+pos.y)+j)%conf.sizeY];
                incr++;
            }
        }
    }

    //Dynamic update of the cell
    cell *actualCell = &(*tab)[pos.x][pos.y];

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

    //end of a for a cell
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



