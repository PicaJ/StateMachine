/*
 * quqe.h
 *
 *  Created on: Jun 3, 2021
 *      Author: hi
 */
#ifndef QUQE_H_
#define QUQE_H_
#include <pthread.h>

typedef int Handle_Quqe;

typedef struct node{
        pthread_mutex_t mutex;
        int frontIdx;//头部索引
        int rearIdx;//尾部索引
        int curlen;
        int maxlen;
        int nodesize;//数据大小
        void ** data;//数据
}Node;

Handle_Quqe create_quqe(int nodesize,int maxlen);

int Push_quqe(Handle_Quqe quqeIdx,const void *data);
int Pop_quqe(Handle_Quqe quqeIdx,void *data);
int Getlen_quqe(Handle_Quqe quqeIdx);

#endif /* QUQE_H_ */
