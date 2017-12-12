
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <common_define.h>
#include <string.h>

/*初始化互斥锁*/
int init_pthread_lock(pthread_mutex_t *pthread_lock)
{
	pthread_mutexattr_t mat;
	int result;

	if ((result=pthread_mutexattr_init(&mat)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call pthread_mutexattr_init fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	/*这种类型的互斥锁会自动检测死锁。如果一个线程试图对一个互斥锁重复锁定，将会返回一个错误代码。
	 * 如果试图解锁一个由别的线程锁定的互斥锁将会返回一个错误代码。
	 * 如果一个线程试图解锁已经被解锁的互斥锁也将会返回一个错误代码。*/
	if ((result=pthread_mutexattr_settype(&mat, \
			PTHREAD_MUTEX_ERRORCHECK)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call pthread_mutexattr_settype fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}
	if ((result=pthread_mutex_init(pthread_lock, &mat)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call pthread_mutex_init fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}
	if ((result=pthread_mutexattr_destroy(&mat)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call thread_mutexattr_destroy fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	return 0;
}

/* 设置线程的栈大小,以及将线程属性设置为PTHREAD_CREATE_DETACHED状态(这样创建的线程就不用pthread_detach()函数)
 *
 * \param pattr:被赋值的线程属性
 * \param stack_size:设置的线程栈大小,0:保存线程栈大小不变
 *
 * \return 0:成功
 * */
int init_pthread_attr(pthread_attr_t *pattr, const int stack_size)
{
	size_t old_stack_size;
	size_t new_stack_size;
	int result;

	if ((result=pthread_attr_init(pattr)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call pthread_attr_init fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	if ((result=pthread_attr_getstacksize(pattr, &old_stack_size)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call pthread_attr_getstacksize fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	if (stack_size > 0)
	{
		if (old_stack_size != stack_size)
		{
			new_stack_size = stack_size;
		}
		else
		{
			new_stack_size = 0;
		}
	}
	else if (old_stack_size < 1 * 1024 * 1024)
	{
		new_stack_size = 1 * 1024 * 1024;
	}
	else
	{
		new_stack_size = 0;
	}

	if (new_stack_size > 0)
	{
		if ((result=pthread_attr_setstacksize(pattr, \
				new_stack_size)) != 0)
		{
            printf("file: "__FILE__", line: %d, " \
				"call pthread_attr_setstacksize fail, " \
				"errno: %d, error info: %s", \
				__LINE__, result, STRERROR(result));
			return result;
		}
	}

	if ((result=pthread_attr_setdetachstate(pattr, \
			PTHREAD_CREATE_DETACHED)) != 0)
	{
        printf("file: "__FILE__", line: %d, " \
			"call pthread_attr_setdetachstate fail, " \
			"errno: %d, error info: %s", \
			__LINE__, result, STRERROR(result));
		return result;
	}

	return 0;
}

int create_work_threads(int *count, void *(*start_func)(void *), \
		void *arg, pthread_t *tids, const int stack_size)
{
	int result;
	pthread_attr_t thread_attr;
	pthread_t *ptid;
	pthread_t *ptid_end;

	if ((result=init_pthread_attr(&thread_attr, stack_size)) != 0)
	{
		return result;
	}

	result = 0;
	ptid_end = tids + (*count);
	for (ptid=tids; ptid<ptid_end; ptid++)
	{
		if ((result=pthread_create(ptid, &thread_attr, \
			start_func, arg)) != 0)
		{
			*count = ptid - tids;
            printf("file: "__FILE__", line: %d, " \
				"create thread failed, startup threads: %d, " \
				"errno: %d, error info: %s", \
				__LINE__, *count, \
				result, STRERROR(result));
			break;
		}
	}

	pthread_attr_destroy(&thread_attr);
	return result;
}

int kill_work_threads(pthread_t *tids, const int count)
{
	int result;
	pthread_t *ptid;
	pthread_t *ptid_end;

	ptid_end = tids + count;
	for (ptid=tids; ptid<ptid_end; ptid++)
	{
		if ((result=pthread_kill(*ptid, SIGINT)) != 0)
		{
            printf("file: "__FILE__", line: %d, " \
				"kill thread failed, " \
				"errno: %d, error info: %s", \
				__LINE__, result, STRERROR(result));
		}
	}

	return 0;
}

