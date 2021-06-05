/*
 * StateMachinerun.h
 *
 *  Created on: Jun 3, 2021
 *      Author: hi
 */

#ifndef STATEMACHINERUN_H_
#define STATEMACHINERUN_H_
#include <pthread.h>
#include <time.h>

#define RETURN_BASESTATE -1
#define HANDLE_FALSE -2
#define HANDLE_SUCESS 0

#define STATEONE   "state_one"
#define STATETWO   "state_two"
#define STATETHREE "state_three"
#define STATEBASE  "state_base"

#define HI_COND_TIMEDWAIT(cond, mutex, usec)                  \
    do {                                                      \
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 0 };   \
        (void)clock_gettime(CLOCK_MONOTONIC, &ts);        \
        ts.tv_sec += (usec / 1000000LL);                      \
        ts.tv_nsec += (usec * 1000LL % 1000000000LL);         \
        (void)pthread_cond_timedwait(&cond, &mutex, &ts); \
    } while (0)

typedef enum stateevent{
    STATE_EVENT_ONE = 0,
    STATE_EVENT_TWO,
    STATE_EVENT_THREE,
    STATE_EVENT_INITCOMD,
    STATE_EVENT_SWITCH_WORKMODE,
    STATE_EVENT_BUILT
}StateEvent;

typedef struct statemsg{
    StateEvent event;
    int arg1;
    int arg2;
}StateMsg;

typedef enum workmodestate{
    STATE_ONE = 0,
    STATE_TWO,
    STATE_THREE,
    STATE_BASE
}WorkMode;

typedef struct workmodecontext{
        int binit;
        WorkMode preworkmode;
        WorkMode curworkmode;
        WorkMode s32DestStateID;
        pthread_mutex_t Mutex;/**<Mutex for bInProgress*/
        int brunning;
        int StateExit;
}WorkModeContext;

typedef struct statecontext{
    WorkMode stateID;                        /* state id */
    char name[32];                          /* state name */
    int (*enter)();            /* call when state enter */
    int (*exit)();             /* call when state exit */
    /* call when process a message */
    int (*processMessage)(StateMsg *msg, void * argv, WorkMode *stateID);
}StateContext;

typedef struct statemngattr{
    StateContext state;
    int brunning;
}StatemngAttr;

int Statemng_Init(WorkMode workmode);
int Statemng_Sendmsg(StateMsg *msg);
int Statemng_Msgproc();
WorkModeContext *Get_StatemngWorkmodect();

int Statemng_StateOne_enter();
int Statemng_StateOne_exit();
int Statemng_StateOne_Msgproc(StateMsg *msg, void * argv, WorkMode *stateID);

int Statemng_StateTwo_enter();
int Statemng_StateTwo_exit();
int Statemng_StateTwo_Msgproc(StateMsg *msg, void * argv, WorkMode *stateID);

int Statemng_StateThree_enter();
int Statemng_StateThree_exit();
int Statemng_StateThree_Msgproc(StateMsg *msg, void * argv, WorkMode *stateID);

int Statemng_Statebase_enter();
int Statemng_Statebase_exit();
int Statemng_Statebase_Msgproc(StateMsg *msg, void * argv, WorkMode *stateID);

#endif /* STATEMACHINERUN_H_ */
