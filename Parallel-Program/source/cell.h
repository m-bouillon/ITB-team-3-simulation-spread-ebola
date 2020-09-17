#define INCUBATION_PERIOD 3
#define INFECTIOUS_PERIOD 10
#define ISOLATED_PERIOD 15

///////////////////////
/**
 * Probability on 100%
 * TODO Make more configurations
 */ 
#define P_INCUBATION 12
#define P_INFECTIOUS 20 
#define P_DEATH 63
#define P_ISOLATED  1
#define P_IMMUNITY 5

#define P_BECOME_ISOLATED 70
#define P_BECOME_IMMUNE 15
#define C_CONTAMINATE_NEIGHBOR 50

///////////////////////

typedef enum _status
{
    SUCEPTIBLE,
    INCUBATION,
    INFECTIOUS,
    ISOLATED,
    IMMUNITY,
    DEATH
} status;

typedef enum _ages
{
    KID,
    TEEN,
    ADULT,
    ELDERLY
} ages;

typedef struct _cell
{
    status state;
    ages age;
    int statusTimeLeft;
} cell;