#pragma once

#include "Result.h"

//===========================================
class Command_T
{
public:

//lifeCycle
	Command_T() {};

	Command_T(const Command_T& rhs) = delete;

	Command_T& operator=(Command_T&&) = default;

//action
	virtual Result execute() = 0;
	virtual Result cancel() = 0;

//operator overload
	Command_T & operator=(const Command_T& rhs) = delete;
};

//===========================================
template
<
	class Action,
	class Event,
	class Cancel
>
class Command
	: public Command_T
{
public:

//lifeCycle
	Command(Action action, Event eve, Cancel cancel);

//action
	Result execute() final;
	Result cancel() final;

private:
	Action _action;
	Event  _event;
	Cancel _cancel;
};


template< class Action, class Event, class Cancel >
Command< Action, Event, Cancel >::Command(Action action, Event eve, Cancel cancel)
	: _action(action)
	, _event(eve)
	, _cancel(cancel)
{
}

template< class Action, class Event, class Cancel >
Result Command< Action, Event, Cancel >::execute()
{
	Result result = _action();

	_event(result);

	return result;
}

template< class Action, class Event, class Cancel >
Result Command< Action, Event, Cancel >::cancel()
{
	return _cancel();
}

//===========================================
template< class Action, class Event, class Cancel >
Command_T* createCommand(Action action, Event eve, Cancel cancel)
{
	typedef Command< Action, Event, Cancel > Obj;

	return new Obj(action, eve, cancel);
}