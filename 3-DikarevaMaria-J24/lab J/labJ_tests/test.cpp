#include "pch.h"
#include "brenthash.h"
#include <gtest/gtest.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

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

TEST_F(FunctionalTest, insert_3_nodes) {
	TestFunc("test_data/insert_3_nodes/input.txt",
		"test_data/insert_3_nodes/output.txt", "test_data/insert_3_nodes/factres.txt");
}


TEST_F(FunctionalTest, tree_5_nodes) {
	TestFunc("test_data/5_nodes/input.txt",
		"test_data/5_nodes/output.txt", "test_data/5_nodes/factres.txt");
}


TEST_F(FunctionalTest, add_alreadyexisting_key) {
	TestFunc("test_data/add_alreadyexisting_key/input.txt",
		"test_data/add_alreadyexisting_key/output.txt", "test_data/add_alreadyexisting_key/factres.txt");
}

TEST_F(FunctionalTest, example_test) {
	TestFunc("test_data/example_test/input.txt",
		"test_data/example_test/output.txt", "test_data/example_test/factres.txt");
}


TEST_F(FunctionalTest, del_nonexistent_node) {
	TestFunc("test_data/del_nonexistent_node/input.txt",
		"test_data/del_nonexistent_node/output.txt", "test_data/del_nonexistent_node/factres.txt");
}

TEST_F(FunctionalTest, empty_tree) {
	TestFunc("test_data/empty_tree/input.txt",
		"test_data/empty_tree/output.txt", "test_data/empty_tree/factres.txt");
}
TEST_F(FunctionalTest, del_empty_tree) {
	TestFunc("test_data/del_empty_tree/input.txt",
		"test_data/del_empty_tree/output.txt", "test_data/del_empty_tree/factres.txt");
}