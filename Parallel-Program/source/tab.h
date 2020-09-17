#include "cell.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "config/config.h"


typedef struct _positon{
    int x;
    int y;
} positon;

typedef struct _data {
    int population;
    int suceptible;
    int incubation;
    int infectious;
    int isolated;
    int immunity;
    int death; 
} data;

/**
 * Initializing an array in 2d
 * With Non infectious People inside (Suceptible, Adult) first
 * And insert % people incubation and infectious as written in config file
 */ 
void initCellSpace(cell ***tab, config conf);

/**
 * Return numbers to status infection state
 */ 
data calculateData(cell ***tab, config conf);

/**
 * Print data about infection state in the console
 */ 
void displayData(data dt, int turn);

/**
 * Method that takes as parameter the array and a position and that will execute the SEIR processing
 * It's the main fucntion of the cellular automation
 * Takes tab pointer as parameter to modify dynamicly the test environment
 */
void * seirProcessing(void * rank);

/**
 * Calculates the probability of changing a cell's status from suceptible to incubation
 * Update concernedCell Status
 */ 
void findNewStateOfSuceptibleCell(cell* concernedCell, cell** neighbors);

/**
 * return 1 -> ok and -1 -> ko
 * Uses probability to dermine someting 
 */ 
int returnProba(int percent);

/**
 * Return the name of the state
 */
char* stateToStrings(status state);