#ifndef _MYTHREAD_H
#define _MYTHREAD_H

#include <mutex>
#include <thread>
#include <list>
#include "threadable.h"

class MyThread {
private:
	Threadable* m_threadable;
	void* m_arg;
	std::thread m_thread;
protected:
	static std::mutex m_mtx;
	static std::list<MyThread*> m_active_threads;

	static void RegisterThread(MyThread* thr);
	static void UnregisterThread(MyThread* thr);
public:
	MyThread(Threadable* thr, void* arg);
	void run();

	static void FinishThreads()
	{
		bool empty = false;
		while (!empty) {
			m_mtx.lock();
			empty = m_active_threads.empty();
			m_mtx.unlock();
		}
	}
};

#endif