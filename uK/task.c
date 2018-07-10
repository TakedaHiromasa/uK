#include "syscall.h"
#include "uk.h"

void init_task(){ //LED(0)
	SEG0 = 0b10000 | 0x00;
	SVC_startTASK(0,0);
}

void diag_task(){ //LED(F)
		while(1){
		SEG0 = 0b10000 | 0x0F;
		}
	}

void task1(){
		while(1){
		SEG0 = 0b10000 | 0x01;
		}
	}

void task2(){
		while(1){
		SEG0 = 0b10000 | 0x02;
		}
	}

void task3(){
		while(1){
		SEG0 = 0b10000 | 0x03;
		}
	}

void task4(){
		while(1){
		SEG0 = 0b10000 | 0x04;
		}
	}

void task5(){
		while(1){
		SEG0 = 0b10000 | 0x05;
		}
	}

void task6(){
		while(1){
		SEG0 = 0b10000 | 0x05;
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