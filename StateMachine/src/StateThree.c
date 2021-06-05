/*
 * StateThree.c
 *
 *  Created on: Jun 5, 2021
 *      Author: hi
 */
#include "StateMachinerun.h"

int Statemng_StateThree_enter(){
    printf("state Three enter!\n");
    return 0;
}

int Statemng_StateThree_exit(){
    printf("State Three exit!\n");
    return 0;
}

int Statemng_StateThree_Msgproc(StateMsg *msg, void * argv, WorkMode *stateID){

    printf("State Three handle msg!\n");
    switch(msg->event)
    {
        case STATE_EVENT_ONE:
            printf("handle event 1\n");
            break;
        case STATE_EVENT_TWO:
            printf("handle event 2\n");
            break;
        case STATE_EVENT_THREE:
            printf("handle event 3\n");
            break;
        default:
            printf("can not find this event!\n");
            return RETURN_BASESTATE;
    }

    return HANDLE_SUCESS;
}
