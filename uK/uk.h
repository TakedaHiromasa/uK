#ifndef UK_H
#define UK_H

#include "syscall.h"
/* TCB */
typedef struct 
{
 U8 	task_status;
 U8 	next_Tid;
 void	*parameter;
 U8		reserved[2]; 
 U8		pause_counter;
 task_p	task_start_adr;   /* syscall.h内の宣言を使う */
 U8		*sp;
 char name[2];
} TCB_t;

#define ABLEBIT 0b01
#define ACTBIT  0b10

#define DORMANT      _____
#define READY        _____
#define WAIT         _____
#define NON_EXISTENT 0b00

#endif