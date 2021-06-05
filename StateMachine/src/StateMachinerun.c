/*
 * StateMachinerun.c
 *
 *  Created on: Jun 3, 2021
 *      Author: hi
 */

#include "StateMachinerun.h"
#include "comm.h"
#include "quqe.h"
#include <stdio.h>
#include <unistd.h>
#include "errno.h"
#include "string.h"
#include <pthread.h>


static WorkModeContext stWorkModect;
Handle_Quqe StateQuqeHandle;

static pthread_cond_t s_StateMachineExitCond;
static pthread_mutex_t s_StateMachineExitMutex;
static pthread_t s_StateMachineThread = -1;
static int s_bStateMachineMsgThreadWork = TRUE;

int Statemng_MsgProcThead();

StatemngAttr stateregistered[STATE_BASE + 1] = {
        {
            {
                STATE_ONE,
                STATEONE,
                Statemng_StateOne_enter,
                Statemng_StateOne_exit,
                Statemng_StateOne_Msgproc
            },FALSE
        },
        {
            {
                STATE_TWO,
                STATETWO,
                Statemng_StateTwo_enter,
                Statemng_StateTwo_exit,
                Statemng_StateTwo_Msgproc
            },FALSE
        },
        {
            {
                STATE_THREE,
                STATETHREE,
                Statemng_StateThree_enter,
                Statemng_StateThree_exit,
                Statemng_StateThree_Msgproc
            },FALSE
        },
        {
            {
                STATE_BASE,
                STATEBASE,
                Statemng_Statebase_enter,
                Statemng_Statebase_exit,
                Statemng_Statebase_Msgproc
            },FALSE
        }
};


static int Statemng_quqeInit(){

    if(NULL != StateQuqeHandle)
    {
        printf("StateQuqeHandle not null!\n");
        return ERROR;
    }

    StateQuqeHandle = create_quqe(sizeof(StateMsg),10);
    if(StateQuqeHandle == NULL)
    {
        printf("StateQuqeHandle is null!\n");
        return ERROR;
    }
    return 0;
}

int Statemng_Init(WorkMode workmode){
    Statemng_Threadinit();
    stWorkModect.curworkmode = workmode;
    stWorkModect.preworkmode = workmode;
    stWorkModect.s32DestStateID = workmode;
    stWorkModect.binit = FALSE;

    StateMsg msg;
    msg.event = STATE_EVENT_INITCOMD;//通过此进行初始化
    Statemng_Sendmsg(&msg);

    return 0;
}

int Statemng_Sendmsg(StateMsg *msg){

    int ret = 0;
//    printf("send state ms event:%d\n",msg->event);
    if(NULL == StateQuqeHandle)
    {
        printf("StateQuqeHandle is null!\n");
        return ERROR;
    }

    ret = Push_quqe(StateQuqeHandle,msg);
    if(ret < 0)
    {
        printf("Push quqe fail!\n");
        return ERROR;
    }
    return 0;
}


int Statemng_waitMsg(StateMsg *msg){

    int ret = 0;
    unsigned int u32CheckDelayUs = 10 * 1000;
    do{
        if(Getlen_quqe(StateQuqeHandle) > 0)
        {
            ret = Pop_quqe(StateQuqeHandle,msg);
            if(ret < 0)
            {
                printf("pop quqe fail!\n");
                return ERROR;
            }
            printf("get statemng msg event:%d\n",msg->event);
            return 0;
        }else
            pthread_mutex_lock(&s_StateMachineExitMutex);
            HI_COND_TIMEDWAIT(s_StateMachineExitCond, s_StateMachineExitMutex, u32CheckDelayUs);
            pthread_mutex_unlock(&s_StateMachineExitMutex);
    }while(s_bStateMachineMsgThreadWork);
    return 0;
}


int Statemng_Threadinit(){

    int s32Ret = HI_SUCCESS;

    Statemng_quqeInit();
    pthread_mutex_init(&s_StateMachineExitMutex,NULL);

    /* init cond */
    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
    pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
    pthread_cond_init(&s_StateMachineExitCond,&condattr);
    pthread_condattr_destroy(&condattr);

    /** create record msg proc thread */
   s32Ret = pthread_create(&s_StateMachineThread, NULL, Statemng_MsgProcThead, NULL);
   if (HI_SUCCESS != s32Ret) {
       printf("pthread_create error:%s \n", strerror(errno));
       pthread_cond_destroy(&s_StateMachineExitCond);
//       MsgQueueDeInit();
       s_StateMachineThread = 0;
       s_bStateMachineMsgThreadWork = FALSE;
       return -1;
   }

    return 0;
}

static int MsgProcThreadDeInit(void)
{
    int s32Ret = HI_SUCCESS;

    if (0 == s_StateMachineThread && NULL == StateQuqeHandle) {
        printf("Msg Proc Thread has been deinited already.\n");
        return HI_SUCCESS;
    }

    if (0 != s_StateMachineThread) {
        s_bStateMachineMsgThreadWork = FALSE;
        pthread_mutex_lock(&s_StateMachineExitMutex);
        pthread_cond_signal(&s_StateMachineExitCond);
        pthread_mutex_unlock(&s_StateMachineExitMutex);
        pthread_join(s_StateMachineThread, NULL);
        pthread_cond_destroy(&s_StateMachineExitCond);
        s_StateMachineThread = -1;
    }

//    s32Ret = MsgQueueDeInit();
//    PDT_STATEMNG_CHECK_RET(s32Ret, s32Ret, "MsgQueueDeInit");

    return s32Ret;
}

int Statemng_MsgProcThead(){

    StateMsg msg;
    int ret = 0;

    printf("start running MsgProcThread\n");
    while(s_bStateMachineMsgThreadWork)
    {
        Statemng_waitMsg(&msg);
        if(stWorkModect.binit){//普通处理
            /** Normal path */
            printf("Normal path\n");
            if(stateregistered[stWorkModect.curworkmode].state.processMessage)
                ret = stateregistered[stWorkModect.curworkmode].state.processMessage(&msg,&stateregistered[stWorkModect.curworkmode],&stWorkModect.s32DestStateID);
            if(HANDLE_SUCESS != ret)//处理不成功交由基本状态处理
                stateregistered[STATE_BASE].state.processMessage(&msg,&stateregistered[stWorkModect.curworkmode],&stWorkModect.s32DestStateID);
        }else if(!stWorkModect.binit && STATE_EVENT_INITCOMD == msg.event){
            /** Initial one time path. */
            printf("Initial path\n");
            stWorkModect.binit = TRUE;
            stateregistered[STATE_BASE].state.enter();
            if(stateregistered[stWorkModect.curworkmode].state.enter)
                stateregistered[stWorkModect.curworkmode].state.enter();
        }else
            printf("StateMachine.handleMessage: The start method not called, received msg: %d\n", msg.event);
        //切换状态
        if(stWorkModect.curworkmode != stWorkModect.s32DestStateID)
        {
            stWorkModect.preworkmode = stWorkModect.curworkmode;
            if(stateregistered[stWorkModect.preworkmode].state.exit)
                stateregistered[stWorkModect.preworkmode].state.exit();
            if(stateregistered[stWorkModect.curworkmode].state.enter)
                stateregistered[stWorkModect.curworkmode].state.enter();
            stWorkModect.curworkmode = stWorkModect.s32DestStateID;
        }
    }
    printf("StateMachine thread exit \n");

    return 0;
}

WorkModeContext *Get_StatemngWorkmodect(){
    return &stWorkModect;
}
