// ConcurrentQueue.h: interface for the CConcurrentQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONCURRENTQUEUE_H__7FE4092E_871D_4E5A_9F9C_5AB2430631B8__INCLUDED_)
#define AFX_CONCURRENTQUEUE_H__7FE4092E_871D_4E5A_9F9C_5AB2430631B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <deque>

template<class T, unsigned max = 512>
class CConcurrentQueue  
{
private:

	CRITICAL_SECTION mutex;
	std::deque<T> m_queue;

public:
	HANDLE space_avail;
	HANDLE data_avail;
	CConcurrentQueue(){

	space_avail = CreateSemaphore(NULL, max, max, NULL);
	data_avail = CreateSemaphore(NULL,0,max,NULL);
	InitializeCriticalSection(&mutex);
	}


	int size(){
		int size;
		EnterCriticalSection(&mutex);
		size=m_queue.size();
		LeaveCriticalSection(&mutex);
		return size;
	}

	bool offer(T data){
		//do nothing and return false if space_avail is not signalled.
		if(WaitForSingleObject(space_avail, 0) != WAIT_OBJECT_0) return false;
		EnterCriticalSection(&mutex);
		m_queue.push_back(data);		
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(data_avail,1,NULL);
		return true;
	}

	void push(T data){
		WaitForSingleObject(space_avail, INFINITE);
		EnterCriticalSection(&mutex);
		m_queue.push_back(data);		
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(data_avail,1,NULL);	
	}

	void push_front(T data){
		WaitForSingleObject(space_avail, INFINITE);
		EnterCriticalSection(&mutex);
		m_queue.push_front(data);		
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(data_avail,1,NULL);
	}

	
	T peek(){
		if(WaitForSingleObject(data_avail, 0) != WAIT_OBJECT_0) return false;
		EnterCriticalSection(&mutex);
		T retval = m_queue.front();
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(data_avail,1,NULL);
		return retval;
	}


	T peek(long timeout_ms){
		if(WaitForSingleObject(data_avail, timeout_ms) != WAIT_OBJECT_0) return false;
		EnterCriticalSection(&mutex);
		T retval = m_queue.front();
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(data_avail,1,NULL);
		return retval;
	}

	T pop(){
		WaitForSingleObject(data_avail, INFINITE);
		EnterCriticalSection(&mutex);
		T retval = m_queue.front();
		m_queue.pop_front();
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(space_avail,1,NULL);
		return retval;
	}

	T poll(bool* bSuccess){
		if(WaitForSingleObject(data_avail, 0) != WAIT_OBJECT_0) {
			EnterCriticalSection(&mutex);	
			(*bSuccess) = false;
			LeaveCriticalSection(&mutex);
			return NULL;
		}
		EnterCriticalSection(&mutex);
		//Get the front of the queue
		T data = m_queue.front();
		m_queue.pop_front();
	
		ReleaseSemaphore(space_avail,1,NULL);
		(*bSuccess)=true;
		return data;;
	}

	T poll(){
		poll(0);
	}

	T poll(long timeout_ms){
		if(WaitForSingleObject(data_avail, timeout_ms) != WAIT_OBJECT_0) {
			return NULL;
		}
		EnterCriticalSection(&mutex);
		//Get the front of the queue
		T data = m_queue.front();
		m_queue.pop_front();
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(space_avail,1,NULL);
		return data;
	}

	~CConcurrentQueue(){
		DeleteCriticalSection(&mutex);
		CloseHandle(data_avail);
		CloseHandle(space_avail);
	}


};

#endif // !defined(AFX_CONCURRENTQUEUE_H__7FE4092E_871D_4E5A_9F9C_5AB2430631B8__INCLUDED_)
