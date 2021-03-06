// Thread.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "Command.h"
#include "Thread.h"

class SampleServices
{
public:

	Result countUp(long& s)
	{
		long i = s;
		s = ++i;
		return Result(Result::OK);
	}

	Result countUpLoop(long s)
	{
		_cencal = false;
		long i = s;
		for (;;)
		{
			std::cout << "count " << i++ << std::endl;

			Sleep(100);

			if (true == _cencal)
			{
				return Result(Result::Cancel);
			}			
		}

		return Result(Result::OK);
	}

	void callBack(Result result)
	{
		std::cout << "result " << result.code() << std::endl;
	}

	Result cancel()
	{
		_cencal = true;

		return Result(Result::OK);
	}

private:

	bool _cencal;
};

int main()
{

	SampleServices sample;
	//スレッド起動
	Thread thread;
	thread.start("testThread");

	thread.add(createCommand(
		std::bind(&SampleServices::countUpLoop, &sample, 5),
		std::bind(&SampleServices::callBack, &sample, std::placeholders::_1),
		std::bind(&SampleServices::cancel, &sample)
	));

	long count = 9;
	thread.add(createCommand(
		std::bind(&SampleServices::countUp, &sample, std::ref(count)),
		std::bind(&SampleServices::callBack, &sample, std::placeholders::_1),
		std::bind(&SampleServices::cancel, &sample)
	));
	for (;;)
	{
		Sleep(1200);
		thread.cancelCurrent();

		if (true == thread.empty())
		{
			break;
		}
	}

	std::cout << "count " << count << std::endl;
    return 0;
}

