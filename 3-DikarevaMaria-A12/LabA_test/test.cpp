#include "pch.h"
#include "labA.h"


TEST(init_list_test, InitList_returnNotNullptr)
{
	XOR_list* head;

	head = InitList();

	EXPECT_NE(head, nullptr);

	DestroyList(head);
}

TEST(XOR_operation, XOR_1element_returnValidVal)
{
	XOR_list* one = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = XOR(one, NULL);

	EXPECT_EQ(one->link, (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL)));

	free(one);
}

TEST(XOR_operation, XOR_2elements_returnValidVal)
{
	XOR_list* one = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* link;
	link = XOR(one, two);

	EXPECT_EQ(link, (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(two)));

	free(one);
	free(two);
}


TEST(Add_Element_To_List_Test, AddToList_List1element_returnValidVal)
{
	XOR_list* list = InitList();
	XOR_list* next;

	char* str = "first";
	next = AddToList(list, str);

	EXPECT_STREQ((char*)(next->str), "first");
	EXPECT_EQ((XOR_list*)((uintptr_t)(list->link) ^ (uintptr_t)(NULL)), next);

	free(list);
	free(next);
}

TEST(Add_Element_To_List_Test, AddToList_List2elements_returnValidVal)
{
	XOR_list* list = InitList();
	XOR_list* one;
	XOR_list* two;
	char* str1 = "first";
	char* str2 = "second";

	one = AddToList(list, str1);
	two = AddToList(one, str2);

	EXPECT_STREQ((char*)(one->str), "first");
	EXPECT_STREQ((char*)(two->str), "second");
	EXPECT_EQ((XOR_list*)((uintptr_t)(list->link) ^ (uintptr_t)(NULL)), one);
	EXPECT_EQ((XOR_list*)((uintptr_t)(one->link) ^ (uintptr_t)(list)), two);
	EXPECT_EQ((XOR_list*)((uintptr_t)(one->link) ^ (uintptr_t)(two)), list);
	EXPECT_EQ((XOR_list*)((uintptr_t)(two->link) ^ (uintptr_t)(NULL)), one);

	free(list);
	free(one);
	free(two);
}

TEST(Iteration_test, Iteration_list2ElementsIterFromHead_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* before_head = NULL;
	XOR_list* head = one;

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL));

	Iteration(&one, &before_head);

	EXPECT_EQ(two, one);
	EXPECT_EQ(before_head, head);

	free(one);
	free(head);
}

TEST(Iteration_test, Iteration_list3ElementsMiddleIteration_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* previous_two = two;
	XOR_list* head = one;

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(three));
	three->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));

	Iteration(&two, &one);

	EXPECT_EQ(two, three);
	EXPECT_EQ(one, previous_two);

	free(head);
	free(one);
	free(two);
}

TEST(Iteration_test, Iteration_list2ElementsIterFromLastElement_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* previous_two = two;
	XOR_list* head = one;

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL));

	Iteration(&two, &one);

	EXPECT_EQ(two, nullptr);
	EXPECT_EQ(previous_two, one);

	free(one);
	free(head);
}

TEST(Iteration_test, Iteration_EmptyList_returnnullprt)
{
	XOR_list* one = InitList();
	XOR_list* prev = NULL;

	Iteration(&one, &prev);

	EXPECT_EQ(one, nullptr);

	free(one);
}

TEST(FindElement_test, FindElement_Emptylist_returnnullptr)
{
	XOR_list* one = InitList();
	one->str = NULL;

	XOR_list* previous = NULL;
	XOR_list* found;

	found = FindElement(one, &previous, "first");

	EXPECT_EQ(found, nullptr);

	free(one);
}

TEST(FindElement_test, FindElement_List2ElementsNoKeyElement_returnNullptr)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	one->str = "first";
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL));
	two->str = "second";

	XOR_list* previous = NULL;
	XOR_list* found;
	found = FindElement(one, &previous, "third");

	EXPECT_EQ(found, nullptr);

	free(one);
	free(two);
}

TEST(FindElement_test, FindElement_List2ElementsWithKeyElement_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	one->str = "first";
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL));
	two->str = "second";

	XOR_list* previous = NULL;
	XOR_list* found;
	found = FindElement(one, &previous, "second");

	EXPECT_EQ(found, two);

	free(one);
	free(two);
}

/*TEST(Delete_test, List_1_element_Expected_nullptr)
{
	XOR_list* one = (XOR_list*)malloc(sizeof(XOR_list));
	one->link = NULL;
	XOR_list* prev_next = NULL;

	DeleteElement(one, prev_next, prev_next);

	EXPECT_EQ(one, nullptr);
}*/

TEST(DeleteElement_test, DeleteElement_List3Elements_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(three));
	three->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));

	DeleteElement(two, one, three);

	EXPECT_EQ(one->link, (XOR_list*)((uintptr_t)(three) ^ (uintptr_t)(NULL)));
	EXPECT_EQ(three->link, (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL)));

	free(one);
	free(three);
}

TEST(DeleteElementKey_Test, DeleteElementKey_List3Elements_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(three));
	three->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));

	one->str = "first";
	two->str = "second";
	three->str = "third";

	DeleteElementKey(one, "second");

	EXPECT_EQ(one->link, (XOR_list*)((uintptr_t)(three) ^ (uintptr_t)(NULL)));
	EXPECT_EQ(three->link, (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL)));

	free(one);
	free(three);
}

TEST(DeleteElementKey_Test, DeleteElementKey_List2ElementsNoKeyElements_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL));

	one->str = "first";
	two->str = "second";

	DeleteElementKey(one, "third");

	EXPECT_EQ(one->link, (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL)));
	EXPECT_EQ(two->link, (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL)));
	EXPECT_STREQ((char*)(one->str), "first");
	EXPECT_STREQ((char*)(two->str), "second");

	free(one);
	free(two);
}

TEST(DeleteElementKey_Test, DeleteElementKey_EmptyList_returnnullptr)
{
	XOR_list* one = InitList();
	one->str = NULL;
	DeleteElementKey(one, "second");

	EXPECT_EQ(one->link, nullptr);
	EXPECT_EQ(one->str, nullptr);
	EXPECT_NE(one, nullptr);

	free(one);
}

TEST(DeleteElementAddress_Test, DeleteElementAddress_List3Elements_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(three));
	three->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));

	DeleteElementAddress(one, two);

	EXPECT_EQ(one->link, (XOR_list*)((uintptr_t)(three) ^ (uintptr_t)(NULL)));
	EXPECT_EQ(three->link, (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL)));

	free(one);
	free(three);
}

TEST(DeleteElementAddress_Test, DeleteElementAddress_List2Elements_returnValidVal)
{
	XOR_list* one = InitList();
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));

	one->link = (XOR_list*)((uintptr_t)(two) ^ (uintptr_t)(NULL));
	two->link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(NULL));

	DeleteElementAddress(one, two);

	EXPECT_EQ(one->link, nullptr);

	free(one);
}

