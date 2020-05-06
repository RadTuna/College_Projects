#pragma once

#include <memory>
#include <random>

class RandNumberGenerator final
{
public:

	explicit RandNumberGenerator(int min, int max)
	{
		mMin = min;
		mMax = max;
	}
	~RandNumberGenerator() = default;

	void Generate(int* outArray, int arraySize) const;

private:

	int mMin = 0;
	int mMax = 0;
	
};

inline void RandNumberGenerator::Generate(int* outArray, int arraySize) const
{
	if (outArray == nullptr)
	{
		return;
	}

	memset(outArray, 0, arraySize);

	std::random_device randDevice;
	std::uniform_int_distribution<int> randUniform(mMin, mMax);

	for (int index = 0; index < arraySize; ++index)
	{
		outArray[index] = randUniform(randDevice);
	}
}
