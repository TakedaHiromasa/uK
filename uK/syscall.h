/*多重定義を防ぐため*/
#ifndef SYSCALL_H
#define SYSCALL_H

typedef	unsigned char	U8;	 //1byte
typedef	signed char		S8;	 //1byte
typedef	unsigned int	U16; //2byte
typedef	signed int		S16; //2byte
typedef	unsigned long	U32; //4byte
typedef	signed long		S32; //4byte

/* TASK ID */
typedef U8 Tid;
 
/* タスク数 */
#define NTASK 8
/* セマフォ数 */
#define NSEMA 8

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


extern TETBL_t tetbl[NTASK];

#endif