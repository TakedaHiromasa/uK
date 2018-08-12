#ifndef UK_H
#define UK_H

#include "syscall.h"
#include <string.h>

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

/* ///////////////
 タイマ関係
/////////////// */
/* タイマ0モードレジスタアドレス(TA0MR) */
#define TA0MR_ADDR ((volatile unsigned char*)0x0396)
#define TA0MR (*(TA0MR_ADDR))

/* タイマ0レジスタ(TA0) */
#define TA0L_ADDR ((volatile unsigned char*)0x0386)
#define TA0L (*(TA0L_ADDR))
#define TA0H_ADDR ((volatile unsigned char*)0x0387)
#define TA0H (*(TA0H_ADDR))

/* カウント開始フラグ */
#define TABSR_ADDR ((volatile unsigned char*)0x0380)
#define TABSR (*(TABSR_ADDR))

/* アップダウンフラグ */
#define UDF_ADDR ((volatile unsigned char*)0x0384)
#define UDF (*(UDF_ADDR))

/* タイマ0割り込み許可レベル */
#define TA0IC_ADDR ((volatile unsigned char*)0x0055)
#define TA0IC (*(TA0IC_ADDR))
#define IR_TA0IC ((TA0IC >> 3) & 0b00000001)

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