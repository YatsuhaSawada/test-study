#pragma once

#include "Result.h"
#include "Command.h"

//===========================================
class Thread
{
public:

//lifeCycle
	Thread();
	~Thread();
	Thread(const Thread& rhs) = delete;
	Thread(Thread&& rhs) = default;
	

//
	bool start(const std::string threadName);
	void stop();

	long add(Command_T* pCommand);
	bool empty();

	Result cancelCurrent();
	Result cancelAll();

//operator overload
	Thread & operator=(const Thread& rhs) = delete;
	Thread& operator=(Thread&&) = default;

private:

	void run();

public:

	std::mutex _mutex;

	bool _notify;
	std::mutex _waitMutex;
	std::condition_variable _waitObject;

	long _seq;
	Command_T*             _pCurrent;
	std::list<Command_T*>* _pReqestQueue;
	std::thread*           _pThread;
};
