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
extern void scheduler(void);
extern void task_init(U8);
extern TCB_t tcb[NTASK];
extern Tid runreg;
extern U8 resch;

task_p syscall_tbl[]={
	startTASK, exitTASK, pauseTASK, resumeTASK, resetTASK,
	getTID, takeSEMA, giveSEMA, tasSEMA, hookItTASK };

void syscall_entry(void)
{ 
	STACK_FRAME_t *sf;
	U8 call_num = 0;
	
	sf	= (STACK_FRAME_t*)tcb[runreg].sp;
	call_num = *((SYSCALL_ERR_t*)&(sf->r0));
	
	syscall_tbl[call_num]();
}	

void startTASK(void)
{
	STACK_FRAME_t *sf;
	SYSCALL_ERR_t *sts;
	Tid tid;
	void *param;

	sf	= (STACK_FRAME_t*)tcb[runreg].sp;
	sts = (SYSCALL_ERR_t*)&(sf->r0);
	tid	= (Tid)(sf->r1)&0xFF;
	param = (void *)(sf->r2);
	//①Tidの範囲チェック 
	//0～NTASK-1ならば②、その他なら⑤ 
	//注.自タスクの起動は④の起動エラーになる
	if(0 <= tid && tid < NTASK){
		//②起動されたタスクがdormant状態かどうか判別、
		//  dormantであれば次③、そうでなければ④へ
		if(tcb[tid].task_status == DORMANT){
			//③の処理
			//task_statusのactbitをON
			tcb[tid].task_status = READY;	//actbitではなくablebitではないか。
			
			//&parameterの値をTCB＋２、３にｺﾋﾟｰする
			//&parameterは実際には使っていないので、TCBへのコピーも不要
			
			*sts = No_Err; //システムコール成功
			
			//タスクの優先度の比較
			if(tid < runreg)
				resch = 1; //リスケする
			else
				resch = 0; //リスケしない
		}else{
			//④の処理（dormant状態でないタスクを起動）　　
			*sts = Not_Dorm_Err;	//ERR2:システムコール失敗
			resch = 0;	//リスケしない
		}		
	}else{
		//⑤下記処理（不正Tid）
		*sts = Tid_Err;	//ERR1：システムコール失敗
		resch = 0; //リスケしない
	}
	
	scheduler(); //スケジューラへjump
}

void exitTASK(void)
{
	//①runregを参照し、当該TASKのTCB、task_status、actbitをOFFする
	//actbitではなくablebitではないか。
	tcb[runreg].task_status = DORMANT; //（ready状態→dormant状態）
	
	//②タスクの初期化（TCBの初期化）
	//タスクエントリィテーブルTETBLを参照し、 タスク実行先頭アドレス（TCB+8、9、10）、及び 初期ＳＰ（TCB+11、12、13）をセット。
	//TCBを参照し、タスクのスタックフレームを初期化する。 
	//注．システムRESET時の初期化と同様の処理である。
	task_init((U8)runreg);
	
	//③の処理
	//sts←0：システムコール成功 （これは不要、参照できない。また失敗することは有り得無い。）
	resch=1;
	scheduler(); //スケジューラへjump
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