#include "uk.h"
#include "syscall.h"

void syscall_entry(void);

/* システムコール */
void startTASK	(void); //1
void exitTASK	(void); //2
void pauseTASK	(void); //3
void resumeTASK	(void); //4
void resetTASK	(void); //5
void getTID		(void); //6
void takeSEMA	(void); //7
void giveSEMA	(void); //8
void tasSEMA	(void); //9
void hookItTASK	(void); //10

/* 外部参照 */
extern TCB_t tcb[NTASK];
extern Tid runreg;
extern U8 resch;

extern task_p
syscall_tbl[]={
	startTASK, exitTASK, pauseTASK, resumeTASK, resetTASK,
	getTID, takeSEMA, giveSEMA, tasSEMA, hookItTASK };

void syscall_entry(void)
{ 
}	

void startTASK(void)
{
	//①Tidの範囲チェック 
	//0～NTASK-1ならば②、その他なら⑤ 
	//注.自タスクの起動は④の起動エラーになる

	//②起動されたタスクがdormant状態かどうか判別、dormantであれば次③、そうでなければ④へ

	//③の処理
	//task_statusのactbitをONし、&parameterの値をTCB＋２、３にｺﾋﾟｰする
	//オリジナルの仕様では上のようになっているが、actbitではなくablebitではないかと思う。 杉野(2009.8.3)
	//&parameterは実際には使っていないので、TCBへのコピーも不要だと思う。杉野(2016.7.13)
	//sts←0：システムコール成功
	//runreg > Tid　ならresch←1、そうでなければ、resch←0　 (タスクの優先度の比較)
	//スケジューラへjump

	//④の処理（dormant状態でないタスクを起動）　　
	//sts←ERR2：システムコール失敗
	//resch←0
	//スケジューラへjump

	//⑤下記処理（不正Tid）
	//sts←-ERR1：システムコール失敗
	//resch←0
	//スケジューラへjump
}

void exitTASK(void)
{
}

void pauseTASK(void)
{
}

void resumeTASK(void)
{
}

void resetTASK(void)
{
}

void getTID(void)
{
}

void takeSEMA(void)
{
}

void giveSEMA(void)
{
}

void tasSEMA(void)
{
}

void hookItTASK(void)
{
}