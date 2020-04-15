
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
		// ������ Front ��Ҹ� ȹ��
		IMenu* topMenu = nullptr;
		mMenus.Get(&topMenu);
		topMenu->Activate();

		// �Է��� ó��
		std::cout << "�Է� : ";
		std::cin >> input;
		system("cls");

		// Front ��ü�� Select �Լ��� ȣ��
		const EMenu selectedMenu = topMenu->Select(input);
		assert(selectedMenu != currentMenu);

		// enum�� ���� ���Ǻб� (�� �Ǵ� �� �޴�����)
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

		// ���������� ������ �Ǿ��ٸ�, Ǫ������
		currentMenu = selectedMenu;
		mMenus.Push(newMenu);
	}

	std::cout << "���α׷��� �����մϴ�." << std::endl;
}
