
#include "MenuHandler.h"

#include <iostream>
#include <cassert>
#include <cstdlib>

#include "MainMenu.h"
#include "ControlMenu.h"
#include "SoundMenu.h"

MenuHandler::~MenuHandler()
{
	for(;!mMenus.IsEmpty();)
	{
		IMenu* pendingMenu = nullptr;
		mMenus.Get(&pendingMenu);
		delete pendingMenu;

		mMenus.Pop();
	}
}

void MenuHandler::Run()
{
	MainMenu* newMenu = new MainMenu();
	mMenus.Push(newMenu);

	EMenu currentMenu = EMenu::MainMenu;
	uint32 input = 0;
	for(;!mMenus.IsEmpty();)
	{
		// 스택의 Front 요소를 획득
		IMenu* topMenu = nullptr;
		mMenus.Get(&topMenu);
		topMenu->Activate();

		// 입력을 처리
		std::cout << "입력 : ";
		std::cin >> input;
		system("cls");

		// Front 객체의 Select 함수를 호출
		const EMenu selectedMenu = topMenu->Select(input);
		assert(selectedMenu != currentMenu);

		// enum에 따라 조건분기 (팝 또는 새 메뉴생성)
		IMenu* newMenu = nullptr;
		switch (selectedMenu)
		{
		case EMenu::Back:
			mMenus.Pop();
			continue;
		case EMenu::MainMenu:
			newMenu = new MainMenu();
			break;
		case EMenu::ControlMenu:
			newMenu = new ControlMenu();
			break;
		case EMenu::SoundMenu:
			newMenu = new SoundMenu();
			break;
		default:
			continue;
		}

		// 정상적으로 생성이 되었다면, 푸쉬수행
		currentMenu = selectedMenu;
		mMenus.Push(newMenu);
	}

	std::cout << "프로그램을 종료합니다." << std::endl;
}
