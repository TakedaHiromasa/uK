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
extern pauseQ_t pauseQ_head;

task_p syscall_tbl[]={
	startTASK, exitTASK, pauseTASK, resumeTASK, resetTASK,
	getTID, takeSEMA, giveSEMA, tasSEMA, hookItTASK };

static STACK_FRAME_t *sf;
static SYSCALL_ERR_t *sts;

static Tid  tid;
static void *param;
	
void syscall_entry(void)
{ 
	U8 call_num = 0;
	
	sf	= (STACK_FRAME_t*)tcb[runreg].sp;
	sts = (SYSCALL_ERR_t*)&(sf->r0);
	
	tid	= (Tid)(sf->r1)&0xFF; //必ずR1L
	param = (void *)(sf->r2); //必ずR2
	
	call_num = *((SYSCALL_ERR_t*)&(sf->r0));
	
	syscall_tbl[call_num]();
}	

void startTASK(void)
{
	//①Tidの範囲チェック 
	//0～NTASK-1ならば②、その他なら⑤ 
	//注.自タスクの起動は④の起動エラーになる
	if(tid < NTASK){
		//②起動されたタスクがdormant状態かどうか判別、
		//  dormantであれば次③、そうでなければ④へ
		if(tcb[tid].task_status == DORMANT){
			//③の処理
			//task_statusのactbitをON
			tcb[tid].task_status = READY;	//actbitではなくablebitではないか。 杉野(2009.8.3)? DORMANT 0b01 (act_on 0b10)-> RADY 0b11
			
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
	//7/17 actbitではなくablebitではないか。 竹田
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
	S32 time = -1;
	//①runregを参照し、当該TCBの ｔask_status の ablebitをオフにする。 
	//ｔask_status のBITパターンは”00000010”となる。
	//7/17 0b10はDORMANTのはず。WAITにするならactbitをOFFって「0b01:wait」にすべき
	tcb[runreg].task_status = WAIT;
	
	//②TCBのpause_counter値にpause時間Tをセット。
	tcb[runreg].pause_counter = time;
	/*
	//③pause時間Tが0の場合⑪へ
	if(time == 0){
		//⑪の処理（ pause時間T =0）
		*sts = 6; //ERR6：システムコール失敗
		resch = 0;
		scheduler(); //スケジューラへjump
	}else if(time < 0){ //④pause時間Tが負の値の場合は⑧へ、そうでなければ次⑤へ
		//⑤既に他のタスクが有限時間pause中なら、次⑥の処理へ、そうでなければ⑦へ
		//⑥pause中のタスクのウェイト時間を、キューの最初のレコードから調べ、 ウェイト時間が小さい順に、レコードが並ぶように キュー及びヘッダーを更新する。(次の「pause_countの値」参照) TCBのpauseカウンタに前のレコードの差分値を入れる。⑧へ
		//⑦当該タスクを先頭レコードとし、キュー、ヘッダーを更新する。次⑧へ
	}
	*/
	
	*sts = No_Err; //⑤システムコール成功
	resch = 1;
	scheduler(); //スケジューラへjump
}

void resumeTASK(void)
{
	//①Tidの範囲チェック 
	//0～NTASK-1ならば②、その他なら⑦
	if(tid < NTASK){
		//②Tidより、再開TASKのTCBをもとめ当該タスクがwait状態かどうか判別する。
		//もしそうならば次③へ、そうでなければ⑥に
		if(tcb[tid].task_status == WAIT){
			//③再開タスクがpauseQの中にリンクされている場合は、次④へ、
			//そうでなければTCBのpauseカウンタが負の値かどうか調べ負の値なら⑤へ、そうでなければ（セマフォ待ちの場合などに該当）⑥へ、
			if(tid == pauseQ_head.next_Tid){
				//④再開タスクをキューから外し、ヘッダを更新する。次⑤へ
			}
			
			if(tcb[tid].pause_counter < 0 || tid == pauseQ_head.next_Tid){
				//⑤の処理内容
				tcb[tid].task_status = READY;	//task_statusをablebitをONにする。
				tcb[tid].pause_counter = 0;		//この処理は念のため
				*sts = No_Err;					//システムコール成功
				
				//タスクの優先度の比較
				if(tid < runreg)
					resch = 1; //リスケする
				else
					resch = 0; //リスケしない
					
				scheduler(); //スケジューラへjump
			}
		}
		
		//⑥の処理（ポーズwait状態でないタスクをresumeさせた）
		*sts = 5;	//sts←ERR5：システムコール失敗
		resch = 0;	//リスケしない
		//スケジューラへｊｕｍｐ

	}else{
		//⑦下記処理（不正Tid)
		*sts = 1;	//sts←ERR1：システムコール失敗
		resch = 0;	//リスケしない
		//スケジューラへｊｕｍｐ
	}
	
	scheduler(); //スケジューラへjump
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