#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

#ifndef TPBOOL
typedef int TPBOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define BUSY_THRESHOLD 0.5	//(busy thread)/(all thread threshold)
#define MANAGE_INTERVAL 5	//tp manage thread sleep interval

typedef struct tp_work_desc_s tp_work_desc;
typedef struct tp_work_s tp_work;
typedef struct tp_thread_info_s tp_thread_info;
typedef struct tp_thread_pool_s tp_thread_pool;

//thread parm
struct tp_work_desc_s{
	char *inum;	//call in
	char *onum;	//call out
	int chnum;	//channel num
};

//base thread struct
struct tp_work_s{
	//main process function. user interface
	void (*process_job)(tp_work *this, tp_work_desc *job);
};

//thread info
struct tp_thread_info_s{
	pthread_t		thread_id;	//thread id num
	TPBOOL  		is_busy;	//thread status:true-busy;flase-idle
	pthread_cond_t          thread_cond;	
	pthread_mutex_t		thread_lock;
	tp_work			*th_work;
	tp_work_desc		*th_job;
};

//main thread pool struct
struct tp_thread_pool_s{
	TPBOOL (*init)(tp_thread_pool *this);
	void (*close)(tp_thread_pool *this);
	void (*process_job)(tp_thread_pool *this, tp_work *worker, tp_work_desc *job);
	int  (*get_thread_by_id)(tp_thread_pool *this, int id);
	TPBOOL (*add_thread)(tp_thread_pool *this);
	TPBOOL (*delete_thread)(tp_thread_pool *this);
	int (*get_tp_status)(tp_thread_pool *this);
	
	int min_th_num;		//min thread number in the pool
	int cur_th_num;		//current thread number in the pool
	int max_th_num;         //max thread number in the pool
	pthread_mutex_t tp_lock;
	pthread_t manage_thread_id;	//manage thread id num
	tp_thread_info *thread_info;	//work thread relative thread info
};

tp_thread_pool *creat_thread_pool(int min_num, int max_num);
