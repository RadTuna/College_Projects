#include "pch.h"
#include "CppUnitTest.h"

// additional include directories
// #include "..\AlgorithmCorrectness\Calculator.h"
#include "Calculator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:

		double mStep = 0.25;
		double mLimit = 100.0;
		
		TEST_METHOD(SumTest)
		{
			auto testLambda = [](double numA, double numB) -> void {
				const double result = CCalc::sum(numA, numB);
				Assert::AreEqual(result, numA + numB);
			};

			for (double i = 0.0; i < mLimit; i += mStep)
			{
				for (double j = 0.0; j < mLimit; j += mStep)
				{
					testLambda(i, j);
				}
			}
		}

		TEST_METHOD(MinusTest)
		{
			auto testLambda = [](double numA, double numB) -> void {
				const double result = CCalc::minus(numA, numB);
				Assert::AreEqual(result, numA - numB);
			};

			for (double i = 0.0; i < mLimit; i += mStep)
			{
				for (double j = 0.0; j < mLimit; j += mStep)
				{
					testLambda(i, j);
				}
			}
		}

		TEST_METHOD(MultiTest)
		{
			auto testLambda = [](double numA, double numB) -> void {
				const double result = CCalc::multi(numA, numB);
				Assert::AreEqual(result, numA * numB);
			};

			for (double i = 0.0; i < mLimit; i += mStep)
			{
				for (double j = 0.0; j < mLimit; j += mStep)
				{
					testLambda(i, j);
				}
			}
		}

		TEST_METHOD(DivTest)
		{
			auto testLambda = [](double numA, double numB) -> void {
				const double result = CCalc::div(numA, numB);
				Assert::AreEqual(result, numA / numB);
			};

			// exclude zero divide
			for (double i = 1.0; i < mLimit; i += mStep)
			{
				for (double j = 1.0; j < mLimit; j += mStep)
				{
					testLambda(i, j);
				}
			}
		}
		
	};
}
