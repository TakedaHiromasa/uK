#include "syscall.h"
#include "uk.h"

static U32 i;

void init_task(){ //LED(0)
	SVC_startTASK(1,0);
	
	while(1){
		SEG0 = 0b10000 | 0x00;
		for(i=0;i<=90000;i++);
		SVC_pause(-1);
		SVC_resumeTASK(1);
		//SVC_exit();
	}
}

void diag_task(){ //LED(F)
	while(1){
		SEG0 = 0b10000 | 0x0F;
	}
}

void task1(){
	SVC_startTASK(2,0);
	
	while(1){
		SEG0 = 0b10000 | 0x01;
		for(i=0;i<=90000;i++);
		SVC_pause(-1);
		SVC_resumeTASK(2);
		//SVC_exit();
	}
}

void task2(){
	SVC_startTASK(3,0);
	
	while(1){
		SEG0 = 0b10000 | 0x02;
		for(i=0;i<=90000;i++);
		SVC_pause(-1);
		SVC_resumeTASK(3);
		//SVC_exit();
	}
}

void task3(){
	SVC_startTASK(4,0);
	
	while(1){
		SEG0 = 0b10000 | 0x03;
		for(i=0;i<=90000;i++);
		SVC_pause(-1);
		SVC_resumeTASK(4);
		//SVC_exit();
	}
}

void task4(){
	SVC_startTASK(5,0);
	
	while(1){
		SEG0 = 0b10000 | 0x04;
		for(i=0;i<=90000;i++);
		SVC_pause(-1);
		SVC_resumeTASK(5);
		//SVC_exit();
	}
}

void task5(){
	SVC_startTASK(6,0);

	while(1){
		SEG0 = 0b10000 | 0x05;
		for(i=0;i<=90000;i++);
		SVC_pause(-1);
		SVC_resumeTASK(6);
		//SVC_exit();
	}
}

void task6(){
	while(1){
		SEG0 = 0b10000 | 0x06;
		for(i=0;i<=90000;i++);
		SVC_resumeTASK(0);
		//SVC_pause(-1);
		//SVC_exit();
	}
}

extern U8 init_task_stack[];
extern U8 task1_stack[];
extern U8 task2_stack[];
extern U8 task3_stack[];
extern U8 task4_stack[];
extern U8 task5_stack[];
extern U8 task6_stack[];
extern U8 diag_task_stack[];

/* TASK ENTRY TABLE */
const TETBL_t tetbl[NTASK]={
	{init_task, init_task_stack},
	{task1,     task1_stack},
	{task2,     task2_stack},
	{task3,     task3_stack},
	{task4,     task4_stack},
	{task5,     task5_stack},
	{task6,     task6_stack},
	{diag_task, diag_task_stack}
};