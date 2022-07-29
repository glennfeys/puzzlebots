#include "mythread.h"
#include <iostream>

std::list<MyThread*> MyThread::m_active_threads;
std::mutex MyThread::m_mtx;

MyThread::MyThread(Threadable* thr, void* args) : m_threadable(thr), m_arg(args), m_thread(&MyThread::run, this)
{
}

void MyThread::run()
{
	MyThread::RegisterThread(this);
	m_threadable->run(m_arg);
	m_thread.detach();
	MyThread::UnregisterThread(this);
	delete this;
}

void MyThread::RegisterThread(MyThread* thr)
{
	m_mtx.lock();
	MyThread::m_active_threads.push_back(thr);
	m_mtx.unlock();
}
void MyThread::UnregisterThread(MyThread* thr)
{
	m_mtx.lock();
	MyThread::m_active_threads.remove(thr);
	m_mtx.unlock();
}