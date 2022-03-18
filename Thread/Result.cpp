#include "Result.h"

//===========================================
//lifeCycle
//===========================================
Result::Result(const Code code)
	: _code(code)
{
}

//===========================================
//propaty
//===========================================
Result::Code Result::code() const
{
	return _code;
}