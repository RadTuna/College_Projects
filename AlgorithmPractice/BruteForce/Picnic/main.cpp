#include <iostream>

int CountPairings(bool Taken[10], int LastIndex);

bool AreFriends[10][10];

int main()
{
	int TestCase = 0;
	std::cin >> TestCase;
	for (int i = 0; i < TestCase; ++i)
	{
		int StudentNum = 0;
		int FriendNum = 0;
		std::cin >> StudentNum >> FriendNum;

		for (int j = 0; j < 10; ++j)
		{
			int First = 0;
			int Second = 0;
			if (j < FriendNum)
			{
				std::cin >> First >> Second;
			}

			for (int k = 0; k < 10; ++k)
			{
				AreFriends[j][k] = false;
			}

			if (First == Second)
			{
				continue;
			}

			AreFriends[First][Second] = true;
			AreFriends[Second][First] = true;
		}
		// 입력완료 풀이시작

		bool Taken[10];
		for (int j = 0; j < 10; ++j)
		{
			Taken[j] = false;
		}
		std::cout << CountPairings(Taken, StudentNum) << std::endl;

		// 풀이종료
	}
}

int CountPairings(bool Taken[10], int LastIndex)
{
	bool IsFinished = true;
	for (int i = 0; i < LastIndex; ++i)
	{
		if (Taken[i] == false)
		{
			IsFinished = false;
		}
	}

	if (IsFinished == true)
	{
		return 1;
	}

	int Result = 0;

	for (int i = 0; i < LastIndex; ++i)
	{
		for (int j = 0; j < LastIndex; ++j)
		{
			if (Taken[i] == false && Taken[j] == false && AreFriends[i][j] == true)
			{
				Taken[i] = Taken[j] = true;
				Result += CountPairings(Taken, LastIndex);
				Taken[i] = Taken[j] = false;
			}
		}
	}

	return Result;
}

