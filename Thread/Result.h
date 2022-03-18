#pragma once

class Result
{
public:

	enum Code
	{
		OK,
		NG,
		Cancel,
	};

public:

	//lifeCycle
	explicit Result(const Code code);

	Result(const Result& rhs) = default;

	Result& operator=(Result&&) = default;

public:

	//propaty
	Code code() const;

public:

	Result& operator=(const Result& rhs) = default;

private:

	Code _code;
};