/*多重定義を防ぐため*/
#ifndef SYSCALL_H
#define SYSCALL_H

typedef	unsigned char	U8;
typedef	signed char		S8;
typedef	unsigned int	U16;
typedef	signed int		S16;
typedef	unsigned long	U32;
typedef	signed long		S32;

/* TASK ID */
typedef U8 Tid;
 
/* タスク数 */
#define NTASK 8
/* セマフォ数 */
#define NSEMA 8

/* （戻り値無し、引数無し）関数ポインタ　*/
/* http://www.c-lang.org/typedef.html */
typedef void (*task_p)(void);

#endif