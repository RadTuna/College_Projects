#pragma once

struct UserInfo
{
	const char* AcUserName;
	int Level;
	int Exp;

	UserInfo(const char* acUserName, int level, int exp)
		: AcUserName(acUserName)
		, Level(level)
		, Exp(exp) {}
};
