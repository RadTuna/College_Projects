#pragma once

class Cell
{
public:

	Cell();
	~Cell();

	void SetCurrentRow(int InRow);
	void SetCurrentColume(int InColume);
	void SetIsOpen(bool InOpen);
	void SetIsMine(bool InMine);
	void AddNearMine(int InAdd);

private:

	int NearMine;
	bool IsOpen;
	bool IsMine;

	int CurrentRow;
	int CurrentColume;

public:

	inline int GetCurrentRow() const { return CurrentRow; }
	inline int GetCurrentColume() const { return CurrentColume; }
	inline bool GetIsOpen() const { return IsOpen; }
	inline bool GetIsMine() const { return IsMine; }
	inline int GetNearMine() const { return NearMine; }
};

