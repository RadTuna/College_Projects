
#ifdef _DEBUG
#pragma comment(lib, "benchmark_d.lib")
#elif NDEBUG
#pragma comment(lib, "benchmark.lib")
#endif

#pragma comment(lib, "shlwapi.lib")

#include "benchmark.h"
#include "RandArrayGenerator.h"
#include "SelectionSort.h"
#include "BubbleSort.h"
#include "DoubleSelectionSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"


#define SORT_TEST_MACRO(Type) \
	for (int i = 0; i < TEST_NUM; ++i) { \
		Type sort(gTestArray[i], TEST_MAX_SIZE, ESortDirection::Ascending, ""); \
		sort.Sort(); \
	} \

constexpr unsigned int TEST_MAX_SIZE = 10000;
constexpr unsigned int TEST_NUM = 1;

static int** gTestArray = nullptr;
static RandNumberGenerator gGenerator(0, TEST_MAX_SIZE);


static void InitTestCase()
{
	gTestArray = new int*[TEST_NUM];
	for (int i = 0 ; i < TEST_NUM; ++i)
	{
		gTestArray[i] = new int[TEST_MAX_SIZE];
		gGenerator.Generate(gTestArray[i], TEST_MAX_SIZE);
	}
}

static void ReleaseTestCase()
{
	if (gTestArray != nullptr)
	{
		for (int i = 0; i < TEST_NUM; ++i)
		{
			if (gTestArray[i] != nullptr)
			{
				delete[] gTestArray[i];
			}
		}

		delete[] gTestArray;
	}
}

static void BM_SelectionSort(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		SORT_TEST_MACRO(SelectionSort);
	}
}
BENCHMARK(BM_SelectionSort);

static void BM_DoubleSelectionSort(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		SORT_TEST_MACRO(DoubleSelectionSort);
	}
}
BENCHMARK(BM_DoubleSelectionSort);

static void BM_BubbleSort(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		SORT_TEST_MACRO(BubbleSort);
	}
}
BENCHMARK(BM_BubbleSort);

static void BM_InsertionSort(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		SORT_TEST_MACRO(InsertionSort);
	}
}
BENCHMARK(BM_InsertionSort);

static void BM_MergeSort(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		SORT_TEST_MACRO(MergeSort);
	}
}
BENCHMARK(BM_MergeSort);

static void BM_QuickSort(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		SORT_TEST_MACRO(QuickSort);
	}
}
BENCHMARK(BM_QuickSort);


int main(int argc, char** argv)
{
	InitTestCase();
	benchmark::Initialize(&argc, argv);

	if (::benchmark::ReportUnrecognizedArguments(argc, argv))
	{
		return 1;
	}

	benchmark::RunSpecifiedBenchmarks();
	ReleaseTestCase();
}

