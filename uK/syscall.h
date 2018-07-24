/*多重定義を防ぐため*/
#ifndef SYSCALL_H
#define SYSCALL_H

/* ////型定義///////////////////////// */
typedef	unsigned char	U8;	 //1byte
typedef	signed char		S8;	 //1byte
typedef	unsigned int	U16; //2byte
typedef	signed int		S16; //2byte
typedef	unsigned long	U32; //4byte
typedef	signed long		S32; //4byte


/* ////タスク関係////////////////////// */
#define NTASK 8 /* タスク数 */
#define NSEMA 8 /* セマフォ数 */

typedef U8 Tid; //TASK ID

/* （戻り値無し、引数無し）関数ポインタ　*/
/* http://www.c-lang.org/typedef.html */
typedef void (*task_p)(void);

/* TASK ENTRY TABLE 
    タスク実行先頭アドレス
    タスクのスタックポインタ初期値 */
typedef struct
{
  task_p task_start_adr;
  U8 *sp;
} TETBL_t; 

extern const TETBL_t tetbl[NTASK];


/* ////エラーコード定義////////////////////// */
#define NUMBER_OF_SYSCALL 10 //システムコールの数

/* システムコール　エラーコード　*/
typedef enum{
	No_Err			= 0,  /* 正常終了 */
	Tid_Err			= 1,  /* タスクIDエラー */
	Not_Dorm_Err	= 2,  /* dormant状態でないタスクを起動した */
	Not_Wait_Err	= 5,  /* ポーズwait状態でないタスクをresumeさせようとした */
	Pause_Time_Err	= 6,  /* Pause指定時間が不正(零) */
	SEMAid_Err		= 10, /* SEMAidが不正 */
	SEMAempty_Err	= 11, /* 指定セマフォが獲得できない(tasSEMA) */
	SelfRESET_Err	= 20, /* 自タスクをreset */
	Int_Number_Err	= 27  /* 不正割込み番号 */
} SYSCALL_ERR_t;

/* システムコール */
SYSCALL_ERR_t SVC_startTASK(Tid tid, void *parameter);
#pragma PARAMETER SVC_startTASK(R1L, R2)

SYSCALL_ERR_t SVC_exit(void); //自タスクの終了

SYSCALL_ERR_t SVC_pause(S16);//自タスクの休止（指定時間又は∞時間）
#pragma PARAMETER SVC_pause(R3)

SYSCALL_ERR_t SVC_resumeTASK(Tid);//指定タスクの休止状態からの再開
#pragma PARAMETER SVC_resumeTASK(R1L)

#endif