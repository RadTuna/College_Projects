#pragma once

enum class ButtonState
{
	Water,
	Slush,
	Ice,
	Disable
};

class Button
{
public:

	void PressButton();
	void SetState(ButtonState inState);
	void SetPrice(int inPrice);
	void SetDispenser(class IceDispenser* inDispenser);

private:

	class IceDispenser* Dispenser;
	ButtonState State;
	int Price;

public:

	inline ButtonState GetState() const { return State; }
	inline int GetPrice() const { return Price; }

};
