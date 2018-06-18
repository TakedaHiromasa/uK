#include "uk.h"

#define IND_u8(a) (*((u8)a)) /* indirection 関節参照 */

#define INTREG ((volatile unsigned char *)0x40) /* 割り込み許可レジスタの先頭アドレスを定義する */

#define GLED_ADDR ((volatile unsigned char*)0x4005)
#define GLED (*(GLED_ADDR))


/* カーネルで用いるデータ */
static U8	tcb[NTASK];                 /* タスク制御ブロック */
static U8	runreg;                     /* Running Task ID */ 
static U8	resch;                      /* 再スケジューリング要求フラグ */
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
	enable_interrupt(29);	/* 割込み番号29番を許可 */
	
	while(1){
		GLED = 0x01;
	}
}