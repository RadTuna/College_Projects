#pragma once

#include <vector>

#include "UserInfo.h"

constexpr unsigned int MAX_ROOM_CAPACITY = 6;

class GameRoomUser
{
public:

	explicit GameRoomUser() noexcept;
	~GameRoomUser() = default;


	bool AddUser(const UserInfo& tUserInfo);
	bool DelUser(const char* pcUserName);
	bool IsEmpty() const;
	bool IsFull() const;

	const UserInfo& GetUserOfName(const char* pcName) const;
	const UserInfo& GetMasterUser() const;
	const UserInfo& GetUserOfLastOrder() const;
	bool BanUser(unsigned int orderNum);

	void Clear();

	void PrintInfo() const;

private:

	std::vector<UserInfo> mUsers;
	const char* mAcMasterUserName;
	
};
