/*
 * queue.c
 *
 *  Created on: Jun 3, 2021
 *      Author: hi
 *      copy by hisi
 */
#include "quqe.h"
#include <stdlib.h>

Handle_Quqe create_quqe(int nodesize,int maxlen){
    Node *quqe = malloc(sizeof(Node));

    if(!quqe)
        return -1;

    quqe->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    quqe->curlen = 0;
    quqe->maxlen = maxlen;
    quqe->nodesize = nodesize;
    quqe->data = (void **)malloc(sizeof(void *) * maxlen);

    int i = 0;
    for(i = 0;i<quqe->maxlen;i++){
        quqe->data[i] = (void *)malloc(quqe->nodesize);
    }

    printf("create handle:%d\n",(Handle_Quqe)quqe);
    return (Handle_Quqe)quqe;
}

int Push_quqe(Handle_Quqe quqeIdx,const void *data){

    if(quqeIdx == 0)
        return -1;

    Node *quqe = quqeIdx;

    pthread_mutex_lock(&quqe->mutex);
    if(quqe->curlen >= quqe->maxlen)
    {
        printf("quqe is full!\n");
        pthread_mutex_unlock(&quqe->mutex);
        return -1;
    }

    if(data)
        memcpy(quqe->data[quqe->rearIdx],data,quqe->nodesize);

    quqe->curlen++;
    quqe->rearIdx = (quqe->rearIdx +1) % quqe->maxlen;
    pthread_mutex_unlock(&quqe->mutex);
    return 0;
}


int Pop_quqe(Handle_Quqe quqeIdx,void *data){

    if(quqeIdx == 0)
        return -1;

    Node *quqe = quqeIdx;

    pthread_mutex_lock(&quqe->mutex);
    if(quqe->curlen >= quqe->maxlen)
    {
        printf("quqe is full!\n");
        pthread_mutex_unlock(&quqe->mutex);
        return -1;
    }

    if(data)
        memcpy(data,quqe->data[quqe->frontIdx],quqe->nodesize);

    quqe->curlen--;
    quqe->frontIdx = (quqe->frontIdx +1) % quqe->maxlen;
    pthread_mutex_unlock(&quqe->mutex);

    return 0;
}

int Getlen_quqe(Handle_Quqe quqeIdx){

    if(quqeIdx == 0)
        return -1;

    Node *quqe = quqeIdx;

    return quqe->curlen;
}

