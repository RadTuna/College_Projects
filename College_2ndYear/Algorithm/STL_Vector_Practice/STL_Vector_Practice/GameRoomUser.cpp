
#include "GameRoomUser.h"

#include <cstring>
#include <iostream>

GameRoomUser::GameRoomUser() noexcept
{
	mUsers.reserve(MAX_ROOM_CAPACITY);
}

bool GameRoomUser::AddUser(const UserInfo& tUserInfo)
{
	if (IsFull())
	{
		return false;
	}
	
	if (IsEmpty()) // set master
	{
		mAcMasterUserName = tUserInfo.AcUserName;
	}

	mUsers.emplace_back(tUserInfo);
	return true;
}

bool GameRoomUser::DelUser(const char* pcUserName)
{
	if (IsEmpty())
	{
		return false;
	}
	
	auto findIter = std::find_if(mUsers.begin(), mUsers.end(), 
		[&pcUserName](const UserInfo& userInfo) -> bool
	{
		return std::strcmp(pcUserName, userInfo.AcUserName) == 0;
	});

	if (findIter == mUsers.end()) // not find
	{
		return false;
	}

	if (findIter == mUsers.begin()) // del master
	{
		mAcMasterUserName = findIter->AcUserName;
	}

	mUsers.erase(findIter);
	return true;
}

bool GameRoomUser::IsEmpty() const
{
	return mUsers.empty();
}

bool GameRoomUser::IsFull() const
{
	return mUsers.capacity() == mUsers.size();
}

const UserInfo& GameRoomUser::GetUserOfName(const char* pcName) const
{
	auto findIter = std::find_if(mUsers.begin(), mUsers.end(), 
		[&pcName](const UserInfo& userInfo) -> bool
		{
			return std::strcmp(pcName, userInfo.AcUserName) == 0;
		});

	if (findIter == mUsers.end())
	{
		throw std::exception("The element corresponding to 'pcName' was not found.");
	}

	return *findIter;
}

const UserInfo& GameRoomUser::GetMasterUser() const
{
	if (IsEmpty())
	{
		throw std::exception("The element does not exist in the mUsers array.");
	}

	return mUsers.front();
}

const UserInfo& GameRoomUser::GetUserOfLastOrder() const
{
	if (IsEmpty())
	{
		throw std::exception("The element does not exist in the mUsers array.");
	}

	return mUsers.back();
}

bool GameRoomUser::BanUser(unsigned int orderNum)
{
	if (mUsers.size() <= orderNum)
	{
		return false;
	}

	mUsers.erase(mUsers.begin() + orderNum);
}

void GameRoomUser::Clear()
{
	mUsers.clear();
}

void GameRoomUser::PrintInfo() const
{
	for (const UserInfo& userInfo : mUsers)
	{
		std::cout << "Name : " << userInfo.AcUserName << std::endl;
		std::cout << "Level : " << userInfo.Level << std::endl;
		std::cout << "Exp : " << userInfo.Exp << std::endl;
		std::cout << std::endl;
	}
}
