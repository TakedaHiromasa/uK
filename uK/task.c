#include "syscall.h"

void init_task()
{
	}

void diag_task()
{
	}

void task1()
{
	}

void task2()
{
	}

void task3()
{
	}

void task4()
{
	}

void task5()
{
	}

void task6()
{
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