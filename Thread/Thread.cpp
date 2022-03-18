#include "Thread.h"

//===========================================
//lifeCycle
//===========================================
Thread::Thread()
	: _mutex()
	, _notify(false)
	, _waitMutex()
	, _waitObject()
	, _seq(0)
	, _pReqestQueue(new std::list<Command_T*>())
	, _pThread(nullptr)
{
}

//===========================================
Thread::~Thread()
{
	stop();
}

//===========================================
//
//===========================================
bool Thread::start(const std::string threadName)
{
	_pThread = new std::thread(&Thread::run, this);
	if (nullptr == _pThread)
	{
		return false;
	}

	return true;
}

//===========================================
void Thread::stop()
{
	if (nullptr == _pThread)
	{
		return;
	}

	_pThread->join();
	safe_delete(_pThread);
}

//===========================================
long Thread::add(Command_T* pCommand)
{
	std::unique_lock<std::mutex> lock(_mutex);

	_pReqestQueue->push_back(pCommand);

	_notify = true;
	_waitObject.notify_all();

	return _seq++;
}

//===========================================
Result Thread::cancelCurrent()
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (nullptr == _pCurrent)
	{
		return Result(Result::OK);
	}

	return _pCurrent->cancel();
}

//===========================================
Result Thread::cancelAll()
{
	std::unique_lock<std::mutex> lock(_mutex);

	FOR_BY_ITR(p, *_pReqestQueue)
	{
		delete (*p);
	}

	return _pCurrent->cancel();
}

//===========================================
bool Thread::empty()
{
	std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
	if (!lock)
	{
		//Lock‚ðŽæ‚ê‚È‚¢‚Æ‚«‚Í”»’f‚ª‚Â‚©‚È‚¢‚½‚ßfalseB
		//•Ê‚ÉTryLock‚Å‚È‚­‚Ä‚à‚¢‚¢B
		return false;
	}

	if (nullptr != _pCurrent)
	{
		return false;
	}

	return _pReqestQueue->empty();
}

//===========================================
//private
//===========================================
void Thread::run()
{
	for (;;)
	{
		if (0 == _pReqestQueue->size())
		{
			std::unique_lock<std::mutex> lk(_waitMutex);
			_notify = false;
			_waitObject.wait(lk, [&] { return _notify; });
		}

		{
			std::unique_lock<std::mutex> lock(_mutex);

			_pCurrent = *(_pReqestQueue->begin());
			_pReqestQueue->pop_front();
		}

		_pCurrent->execute();
		safe_delete(_pCurrent);
	}
}