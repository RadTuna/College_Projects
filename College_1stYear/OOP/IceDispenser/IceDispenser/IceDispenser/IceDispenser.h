#pragma once


class IceDispenser
{
public:

	void Initialize();
	void Release();
	void InsertMoney(int inMoney);
	void ReturnMoney();
	void SetMoney(int inMoney);
	void RefreshButtonState();

private:

	class Button* WaterButton;
	class Button* SlushButton;
	class Button* IceButton;
	class Motor* MainMotor;

	int CurrentMoney;

public:

	inline Button* GetWaterButton() const { return WaterButton; }
	inline Button* GetSlushButton() const { return SlushButton; }
	inline Button* GetIceButton() const { return IceButton; }
	inline Motor* GetMainMotor() const { return MainMotor; }
	inline int GetMoney() const { return CurrentMoney; }
};