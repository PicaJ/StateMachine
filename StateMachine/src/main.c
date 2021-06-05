/*
 ============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "quqe.h"
#include "StateMachinerun.h"

void quqe_test()
{
    Handle_Quqe testhandle = create_quqe(10,10);
    char putbuf[10];
    char getbuf[10];
    while(1){
        gets(putbuf);
        if(strcmp(putbuf,"stop") == 0)
            break;
        Push_quqe(testhandle,putbuf);
        Pop_quqe(testhandle,getbuf);
        printf("get frome putbuf:%s\n",getbuf);
    }
}

void State_test(){
    char putbuf[10];
    WorkModeContext *pstworkmodecontext;
    Statemng_Init(STATE_ONE);
    StateMsg msg;
    msg.event = STATE_EVENT_ONE;
    while(1){
        gets(putbuf);
        if(strcmp(putbuf,"get") == 0)
        {
            pstworkmodecontext = Get_StatemngWorkmodect();
            printf("cur workmode:%d  binit:%d\n",pstworkmodecontext->curworkmode,pstworkmodecontext->binit);
        }
        Statemng_Sendmsg(&msg);
    }
}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
//	quqe_test();
	State_test();
	return EXIT_SUCCESS;
}
