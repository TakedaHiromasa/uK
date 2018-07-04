#include "uk.h"

#define IND_u8(a) (*((u8)a)) /* indirection 関節参照 */

#define INTREG ((volatile unsigned char *)0x40) /* 割り込み許可レジスタの先頭アドレスを定義する */

/* カーネルで用いるデータ */
TCB_t	tcb[NTASK];                 /* タスク制御ブロック */
U8		runreg;                     /* Running Task ID */ 
U8		resch;                      /* 再スケジューリング要求フラグ */

////後回し
//_____________ pauseQ_head;       /* pause待ちキュー */
//_____________ semaQ_head[NSEMA]; /* セマフォ待ちキュー */	
//____ it_HookTid;                 /* 割り込みタスクID */

/* ///////////////////////////
	引数:割込み番号(0～63) 
	戻り値:なし
////////////////////////////// */
void enable_interrupt(unsigned char n)
{
	/* 割り込み番号として正しければ、割り込み許可レジスタを設定 */
	if(n <= 63) INTREG[n]=6; /* 割込みレベルは 6 に統一 */
}


/* 割り込みテストコード */
void led_type1()   /* 関数名はなんでもいい*/
{	
   /*  LED を光らせて（光ったことが分かるように）終わる*/
   	int i=0;
	for(i=0;i<=20000;i++){
		GLED = 0x03;
	}
}


void kernel_start()
{
	STACK_FRAME_t *stack_frame;      /* スタックフレーム */	
	U32 pc=0;
	
	int n=0;
	
	/* カーネルデータの初期化 */
	/* (6)タスクの初期化      */
	for(n=0;n < NTASK; n++){
		tcb[n].task_start_adr = tetbl[n].task_start_adr;
		tcb[n].sp             = tetbl[n].sp;
		tcb[n].task_status    = DORMANT;
	}
	
		/* 各タスクの初期スタックフレームを構築する　*/
	for(n=0; n< NTASK; n++){
		tcb[n].sp = tetbl[n].sp - 20;
		stack_frame = (STACK_FRAME_t*)tcb[n].sp;
		
		pc = (U32)tcb[n].task_start_adr;
		stack_frame->save_pc[0] = 0xFF & pc; //PCの下位バイト
		stack_frame->save_pc[1] = 0xFF & (pc >>  8); //PCの中位バイト
		stack_frame->save_pc[3] = 0xFF & (pc >>  16); //PCの上位バイト
		stack_frame->save_pc[2] = (1 << 6); //フラグレジスタ
		stack_frame->r0 = 0x1234;
		stack_frame->r1 = 0x5678;
		stack_frame->r2 = 0x9abc;
		stack_frame->r3 = 0xdef0;
		stack_frame->a0 = 0x1234;
		stack_frame->a1 = 0x5678;
		stack_frame->sb = 0x9abc;
		stack_frame->fb = 0xdef0;
	}
	
	/* (7)runreg←0xFF（実行タスク無し）*/
	runreg = 0xff;
	
	/* 
	(9)システムタスクの起動
	INITタスクの起動
	優先度が最高位タスクのTCBのtask_statusの、actbitをオンし、
	タスクの状態をdormant→ready状態にする。
	task_statusのBITパターンは00000011になる。
	また、スタックにparamをセットする。
	*/
	tcb[0].task_status = READY;
 
	/*
	DIAGタスクの起動
	優先度が最低位タスクのTCBのtask_statusの、actbitをオンし、
	タスクの状態をdormant→ready状態にする。
	また、スタックにparamをセットする。

	＊優先度が最下位のDIAGタスクをアイドルタスクとして見れば、
	このタスクのCPU占有率がシステムのアイドル率に相当する。

	優先度が最高位レベルのINITタスクはシステムリセット後、
	最初に実行されるタスクであり、アプリケーション側の初期化処理を行い
	実行終了（exit）する。
	一方、優先度が最低位のDIAGタスクは常時ready状態のタスクとし
	システムの診断などを行う。
	*/
	tcb[NTASK-1].task_status = READY;
	
	
	enable_interrupt(29);	//割込み番号29番を許可
	
	dispatcher(tcb[2].sp); //dispatcher関数を呼ぶ
	
	while(1){
		GLED = 0x01;
		for(n=0;n<=30000;n++);
		GLED = 0x02;
		for(n=0;n<=30000;n++);
	}
	
}

