#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "labd.h"
#include <gtest/gtest.h>

#define MAX_NUM_ITEMS 10
#define STRESS_TEST_NUM_ITEMS 10000
#define STRESS_TEST_SACK_CAPACITY 40000
#define STRESS_TEST_REQUIRED_PRICE 40000
#define STRESS_TEST_MAX_VAL 5000

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Pro Ver. 20H2 Build 19042.1288
PC configuration:
	CPU:			AMD Athlon 300U with Radeon Vega Mobile Gfx		 2.40 GHz
	Motherboard:	HP 85EC 51.17
	RAM:			8.00 GB
	SSD:			477 GB

	Stress test results:
	Time:			18.197 sec
	RAM:			1.46 Gb
*/


TEST(StressTest, GenerateInputTest) {
	FILE* fileInput = fopen("test_data/StressTest/input.txt", "w");
	ASSERT_TRUE(fileInput);

	fprintf(fileInput, "%d %d %d\n", STRESS_TEST_NUM_ITEMS, STRESS_TEST_SACK_CAPACITY, STRESS_TEST_REQUIRED_PRICE);
	for (int i = 0; i < STRESS_TEST_NUM_ITEMS; i++)
		fprintf(fileInput, "%d ", (rand() % STRESS_TEST_MAX_VAL));
	fprintf(fileInput, "\n");

	for (int i = 0; i < STRESS_TEST_NUM_ITEMS; i++)
		fprintf(fileInput, "%d ", (rand() % STRESS_TEST_MAX_VAL));

	fclose(fileInput);
}


TEST(StressTest, SolutionStressTest) {
	ASSERT_TRUE(LabSolution("test_data/StressTest/input.txt", "test_data/StressTest/output.txt"));
}


unsigned ReadOutputData(const char* filenameOut, unsigned* result, unsigned size) {
	FILE* fileOut = fopen(filenameOut, "r");
	unsigned factNumVals = 0;
	if (fileOut == NULL)
		return 0;
	for (unsigned i = 0; i < size; i++)
	{
		if (fscanf(fileOut, "%d ", &result[i]) == -1)
			break;
		factNumVals++;
	}
	fclose(fileOut);
	return factNumVals;
}

//ValsNumFOut - number of values in the output file
void TestFunc(const char* filenameIn, const char* filenameOut, unsigned* correctResult, unsigned ValsNumFOut) {
	ASSERT_TRUE(LabSolution(filenameIn, filenameOut));
	
	unsigned result[MAX_NUM_ITEMS] = { 0 };

	EXPECT_EQ(ReadOutputData(filenameOut, result, MAX_NUM_ITEMS), ValsNumFOut);

	for (unsigned i = 0; i < ValsNumFOut; i++)
		EXPECT_EQ(result[i], correctResult[i]);
}

class FunctionalTest : public ::testing::Test {
protected:
	void SetUp() {
		_CrtMemCheckpoint(&m_before);
	}
	void TearDown() {
		_CrtMemCheckpoint(&m_after);
		if (_CrtMemDifference(&m_diff, &m_before, &m_after)) {
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
			_CrtMemDumpStatistics(&m_diff);
			FAIL();
		}
	}
private:
	_CrtMemState m_before, m_after, m_diff;
};

//Example from lab d25 task text
TEST_F(FunctionalTest, D25Test) {
	unsigned correctResult[] = { 0 };
	TestFunc("test_data/D25Test/input.txt", 
		"test_data/D25Test/output.txt", correctResult, 1);
}

//K = max price of items in Knapsack. 
TEST_F(FunctionalTest, requiredPriceEQMaxPrice) {
	unsigned correctResult[] = { 2, 3 };
	TestFunc("test_data/KEqualsMaxPrice/input.txt", 
		"test_data/KEqualsMaxPrice/output.txt", correctResult, 2);
}

//K > max price of items in Knapsack. 
TEST_F(FunctionalTest, requiredPriceBiggerMaxPrice) {
	unsigned correctResult[] = { 0 };
	TestFunc("test_data/KBiggerMaxPrice/input.txt",
		"test_data/KBiggerMaxPrice/output.txt", correctResult, 1);
}

//K < max price of items in Knapsack with another set of items. 
TEST_F(FunctionalTest, requiredPriceSmallerMaxPrice) {
	unsigned correctResult[] = { 1, 2, 4 };
	TestFunc("test_data/KSmallerMaxPrice/input.txt",
		"test_data/KSmallerMaxPrice/output.txt", correctResult, 3);
}

