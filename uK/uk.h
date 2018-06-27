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
 char	name[2];
} TCB_t;

/* スタックフレーム構造体 */
typedef struct
{
  U8 r0; U8 r1; U8 r2; U8 r3; U8 a0; U8 a1; U8 sb; U8 fb;
  U8 save_pc[3];    /* PC(L),PC(M),FR,PC(U)    */  
} STACK_FRAME_t;

/* レジスタ渡しのアセンブラ関数宣言　*/
void dispatcher(U8 *sp);

#pragma PARAMETER dispatcher( A0 );

#define ABLEBIT 0b01
#define ACTBIT  0b10

#define DORMANT      0b10
#define READY        0b11
#define WAIT         0b01
#define NON_EXISTENT 0b00

#endif