/*
 * statebase.c
 *
 *  Created on: Jun 5, 2021
 *      Author: hi
 */
#include "StateMachinerun.h"

int Statemng_Statebase_enter(){
    printf("state base enter!\n");
    return 0;
}

int Statemng_Statebase_exit(){
    printf("State base exit!\n");
    return 0;
}

int Statemng_Statebase_Msgproc(StateMsg *msg, void * argv, WorkMode *stateID){

    printf("State base handle msg!\n");
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
        case STATE_EVENT_SWITCH_WORKMODE:
            printf("ready to transform\n");
            if(msg->arg2 > STATE_BASE && msg->arg2 < STATE_ONE){
                printf("the workmode(arg2) to set is error\n\n");
                return HANDLE_FALSE;
            }
            WorkModeContext *pstworkmodecontext = Get_StatemngWorkmodect();
            if(pstworkmodecontext->curworkmode == msg->arg2)
                printf("no need to transform!\n");
            else
                stateID = (WorkMode*)msg->arg2;
            break;
        default:
            printf("can not find this event!\n");
            return RETURN_BASESTATE;
    }

    return HANDLE_SUCESS;
}

