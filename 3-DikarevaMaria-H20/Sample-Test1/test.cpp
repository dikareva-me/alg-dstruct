#include "pch.h"
#include "labh.h"
#include <gtest/gtest.h>
#include <string.h>

#define MAX_COUNT 100


void TestFunc(const char* filenameIn, const char* filenameOut, const char* factResult) {
	char resultCorrect[MAX_COUNT], resultFact[MAX_COUNT];

	FILE* fileIn = fopen(filenameIn, "r");
	ASSERT_TRUE(fileIn);

	FILE* fileOut = fopen(filenameOut, "r");
	ASSERT_TRUE(fileOut);

	FILE* result = fopen(factResult, "w");
	ASSERT_TRUE(result);

	ASSERT_TRUE(LabSol(fileIn, result));
	fclose(result);

	result = fopen(factResult, "r");
	ASSERT_TRUE(result);

	fgets(resultCorrect, MAX_COUNT, fileOut);
	ASSERT_TRUE(resultCorrect);

	fgets(resultFact, MAX_COUNT, result);
	ASSERT_TRUE(resultFact);

	EXPECT_EQ(strcmp(resultFact, resultCorrect), 0);
	fclose(fileOut);
	fclose(result);
	fclose(fileIn);
}

TEST(test, insert_3_nodes) {
	TestFunc("test_data/insert_3_nodes/input.txt",
		"test_data/insert_3_nodes/output.txt", "test_data/insert_3_nodes/factres.txt");
}


TEST(test, tree_5_nodes) {
	TestFunc("test_data/5_nodes/input.txt",
		"test_data/5_nodes/output.txt", "test_data/5_nodes/factres.txt");
}


TEST(test, example_test) {
	TestFunc("test_data/example_test/input.txt",
		"test_data/example_test/output.txt", "test_data/example_test/factres.txt");
}