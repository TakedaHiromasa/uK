#ifndef UK_H
#define UK_H

#include "syscall.h"

#define ABLEBIT 0b01
#define ACTBIT  0b10

#define DORMANT      0b01
#define READY        0b11
#define WAIT         0b10
#define NON_EXISTENT 0b00

#define GLED_ADDR ((volatile unsigned char*)0x4005)
#define GLED (*(GLED_ADDR))

/* 7セグLED I/Oアドレス */
#define SEG0_ADDR ((volatile unsigned char*)0x4004)
#define SEG0 (*(SEG0_ADDR))

/* レジスタ渡しのアセンブラ関数宣言　*/
extern void dispatcher(U8 *sp);
#pragma PARAMETER dispatcher(A0);

/* TCB */
typedef struct 
{
 U8 	task_status;
 U8 	next_Tid;
 void	*parameter;
 U8		reserved[2]; 
 S16	pause_counter;
 task_p	task_start_adr;   /* syscall.h内の宣言を使う */
 U8		*sp;
 char	name[2];
} TCB_t;

/* スタックフレーム構造体 「計20byte、PCは3byte構成」*/
typedef struct
{
  U16 r0;
  U16 r1; 
  U16 r2;
  U16 r3;
  U16 a0;
  U16 a1;
  U16 sb;
  U16 fb;
  U8 save_pc[4];    /* PC(L),PC(M),FR,PC(U) */  
} STACK_FRAME_t;

/* TCB */
typedef struct 
{
 Tid 	next_Tid;
 U8 	n;
} pauseQ_t;

#endif