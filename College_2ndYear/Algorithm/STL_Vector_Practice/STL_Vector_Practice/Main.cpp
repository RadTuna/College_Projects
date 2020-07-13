
#include <iostream>

#include "GameRoomUser.h"

#define LINE_SEPARATOR std::cout << "==============================" << std::endl

void PrintUserInfo(const UserInfo& userInfo)
{
	std::cout << "Name : " << userInfo.AcUserName << std::endl;
	std::cout << "Level : " << userInfo.Level << std::endl;
	std::cout << "Exp : " << userInfo.Exp << std::endl;
	std::cout << std::endl;
}

int main()
{
	GameRoomUser gameRoom;

	gameRoom.AddUser(UserInfo("User1", 35, 450));
	gameRoom.AddUser(UserInfo("User2", 87, 870));
	gameRoom.AddUser(UserInfo("User3", 48, 480));
	gameRoom.AddUser(UserInfo("User4", 32, 320));
	gameRoom.AddUser(UserInfo("User5", 2, 20));

	std::cout << "Default" << std::endl;
	gameRoom.PrintInfo();

	LINE_SEPARATOR;

	std::cout << "GetMaster" << std::endl;
	PrintUserInfo(gameRoom.GetMasterUser());

	LINE_SEPARATOR;

	std::cout << "AfterRemoveGetMaster" << std::endl;
	gameRoom.DelUser("User1"); // del master;
	PrintUserInfo(gameRoom.GetMasterUser());

	LINE_SEPARATOR;

	std::cout << "GetLast" << std::endl;
	PrintUserInfo(gameRoom.GetUserOfLastOrder());

	LINE_SEPARATOR;

	std::cout << "GetSpecificName" << std::endl;
	PrintUserInfo(gameRoom.GetUserOfName("User3"));

	LINE_SEPARATOR;

	gameRoom.AddUser(UserInfo("User6", 28, 280));
	gameRoom.AddUser(UserInfo("User7", 92, 920));
	gameRoom.AddUser(UserInfo("User8", 11, 110));

	std::cout << "Over6" << std::endl;
	gameRoom.PrintInfo();

	LINE_SEPARATOR;

	std::cout << "Ban at 3" << std::endl;
	gameRoom.BanUser(3);
	gameRoom.PrintInfo();

	return 0;
}
