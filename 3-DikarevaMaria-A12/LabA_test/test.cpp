#include "pch.h"
#include "labA.h"


TEST(init_list_test, InitList_returnNotNullptr)
{
	XOR_list* head;

	head = InitList();

	EXPECT_TRUE(head);

	DestroyList(head);
}

TEST(XOR_operation, XOR_1elementAndZero_returnValidVal)
{
	XOR_list one;

	one.link = XOR(&one, NULL);

	EXPECT_EQ(one.link, &one);

}

TEST(XOR_operation, XOR_1elementSquare_returnValidVal)
{
	XOR_list one;
	XOR_list* link;
	link = XOR(&one, &one);

	EXPECT_FALSE(link);
}

TEST(XOR_operation, XOR_2elementsCommutative_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* link1, * link2;

	link1 = XOR(&one, &two);
	link2 = XOR(&two, &one);

	EXPECT_EQ(link1, link2);
}

TEST(XOR_operation, XOR_3elementsAssociative_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list three;
	XOR_list* link1, * link2;

	link1 = XOR(&one, &two);
	link2 = XOR(&two, &three);

	EXPECT_EQ(XOR(link1, &three), XOR(link2, &one));

}

TEST(Add_Element_To_List_Test, AddToList_List1element_returnValidVal)
{
	XOR_list list;
	XOR_list* head = &list;
	list.link = NULL;
	char* str = "A";

	AddToList(&head, str);

	EXPECT_TRUE(head->link);
	EXPECT_EQ(head->str, str);

	free(head);
}

TEST(Add_Element_To_List_Test, AddToList_List2elements_returnValidVal)
{
	XOR_list list;
	list.link = NULL;
	XOR_list* head = &list;
	XOR_list* one;
	XOR_list* two;
	char* str1 = "A";
	char* str2 = "B";

	AddToList(&head, str1);
	AddToList(&head, str2);

	one = list.link;
	two = (XOR_list*)((uintptr_t)(&list) ^ (uintptr_t)(one->link));

	EXPECT_STREQ(one->str, str1);
	EXPECT_STREQ(two->str, str2);

	EXPECT_TRUE(one);
	EXPECT_TRUE(two);

	free(one);
	free(two);
}

TEST(Iteration_test, Iteration_list2ElementsIterFromHead_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* before_head = NULL;
	XOR_list* head = &one;

	one.link = &two;
	two.link = &one;

	Iteration(&head, &before_head);

	EXPECT_EQ(head, &two);
	EXPECT_EQ(before_head, &one);
}

TEST(Iteration_test, Iteration_list3ElementsMiddleIteration_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list three;
	XOR_list* two_ptr = &two;
	XOR_list* head = &one;

	one.link = &two;
	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	three.link = &two;

	Iteration(&two_ptr, &head);

	EXPECT_EQ(two_ptr, &three);
	EXPECT_EQ(head, &two);
}

TEST(Iteration_test, Iteration_list2ElementsIterFromLastElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* two_ptr = &two;
	XOR_list* head = &one;

	one.link = &two;
	two.link = &one;

	Iteration(&two_ptr, &head);

	EXPECT_FALSE(two_ptr);
	EXPECT_EQ(head, &two);
}

TEST(Iteration_test, Iteration_EmptyList_returnnullprt)
{
	XOR_list one;
	one.link = NULL;
	XOR_list* prev = NULL;
	XOR_list* head = &one;

	Iteration(&head, &prev);

	EXPECT_FALSE(head);
}

TEST(FindElement_test, FindElement_Emptylist_returnnullptr)
{
	XOR_list one;
	one.str = NULL;
	one.link = NULL;

	XOR_list* previous = NULL;
	XOR_list* found = NULL;

	found = FindElement(&one, &previous, "first");

	EXPECT_FALSE(found);
}

TEST(FindElement_test, FindElement_List2ElementsNoKeyElement_returnNullptr)
{
	XOR_list one;
	XOR_list two;

	one.link = &two;
	one.str = "first";
	two.link = &one;
	two.str = "second";

	XOR_list* previous = NULL;
	XOR_list* found;
	found = FindElement(&one, &previous, "third");

	EXPECT_FALSE(found);
}

TEST(FindElement_test, FindElement_List3ElementsMiddleKeyElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list three;

	one.link = &two;
	one.str = "first";

	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	two.str = "second";

	three.link = &two;
	three.str = "third";

	XOR_list* previous = NULL;
	XOR_list* found;
	found = FindElement(&one, &previous, "second");

	EXPECT_EQ(found, &two);
}

TEST(FindElement_test, FindElement_List3ElementsFirstKeyElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list three;

	one.link = &two;
	one.str = "first";

	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	two.str = "second";

	three.link = &two;
	three.str = "third";

	XOR_list* previous = NULL;
	XOR_list* found;
	found = FindElement(&one, &previous, "first");

	EXPECT_EQ(found, &one);
}

TEST(FindElement_test, FindElement_List3ElementsLastKeyElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list three;

	one.link = &two;
	one.str = "first";

	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	two.str = "second";

	three.link = &two;
	three.str = "third";

	XOR_list* previous = NULL;
	XOR_list* found;
	found = FindElement(&one, &previous, "third");

	EXPECT_EQ(found, &three);
}

TEST(DeleteElement_test, DeleteElement_List3ElementsDelFirstElement_returnValidVal)
{
	XOR_list* one = (XOR_list*)malloc(sizeof(XOR_list));
	assert(one);
	XOR_list two;
	XOR_list three;
	XOR_list* prev = NULL;

	one->link = &two;
	two.link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(&three));
	three.link = &two;

	DeleteElement(one, prev);

	EXPECT_EQ(two.link, &three);
	EXPECT_EQ(three.link, &two);
}

TEST(DeleteElement_test, DeleteElement_List3ElementsDelMiddleElement_returnValidVal)
{
	XOR_list one;
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	assert(two);
	XOR_list three;

	one.link = two;
	two->link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	three.link = two;

	DeleteElement(two, &one);

	EXPECT_EQ(one.link, &three);
	EXPECT_EQ(three.link, &one);
}

TEST(DeleteElement_test, DeleteElement_List3ElementsDelLastElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));
	assert(three);

	one.link = &two;
	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(three));
	three->link = &two;

	DeleteElement(three, &two);

	EXPECT_EQ(one.link, &two);
	EXPECT_EQ(two.link, &one);
}


TEST(DeleteElementKey_Test, DeleteElementKey_List3ElementsDelMiddleElement_returnValidVal)
{
	XOR_list one;
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	assert(two);
	XOR_list three;

	one.link = two;
	two->link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	three.link = two;

	one.str = "first";
	two->str = "second";
	three.str = "third";

	DeleteElementKey(&one, "second");

	EXPECT_EQ(one.link, &three);
	EXPECT_EQ(three.link, &one);

}

TEST(DeleteElementKey_Test, DeleteElementKey_List2ElementsNoKeyElements_returnValidVal)
{
	XOR_list one;
	XOR_list two;

	one.link = &two;
	one.str = "first";
	two.link = &one;
	two.str = "second";

	DeleteElementKey(&one, "third");

	EXPECT_EQ(one.link, &two);
	EXPECT_EQ(two.link, &one);
	EXPECT_STREQ((char*)(one.str), "first");
	EXPECT_STREQ((char*)(two.str), "second");
}

TEST(DeleteElementKey_Test, DeleteElementKey_EmptyList_returnnullptr)
{
	XOR_list one;
	one.str = NULL;
	one.link = NULL;
	DeleteElementKey(&one, "second");

	EXPECT_FALSE(one.link);
	EXPECT_FALSE(one.str);
}

TEST(DeleteElementKey_Test, DeleteElementKey_List3ElementsDelFirstElement_returnValidVal)
{

	XOR_list* one = (XOR_list*)malloc(sizeof(XOR_list));
	assert(one);
	XOR_list two;
	XOR_list three;

	one->link = &two;
	two.link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(&three));
	three.link = &two;

	one->str = "first";
	two.str = "second";
	three.str = "third";

	DeleteElementKey(one, "first");

	EXPECT_EQ(two.link, &three);
	EXPECT_EQ(three.link, &two);
}

TEST(DeleteElementKey_Test, DeleteElementKey_List3ElementsDelLastElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));
	assert(three);

	one.link = &two;
	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(three));
	three->link = &two;

	one.str = "first";
	two.str = "second";
	three->str = "third";

	DeleteElementKey(&one, "third");

	EXPECT_EQ(two.link, &one);
	EXPECT_EQ(one.link, &two);
}


TEST(DeleteElementAddress_Test, DeleteElementAddress_List3ElementsDelMiddleElement_returnValidVal)
{
	XOR_list one;
	XOR_list* two = (XOR_list*)malloc(sizeof(XOR_list));
	assert(two);
	XOR_list three;

	one.link = two;
	two->link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(&three));
	three.link = two;

	DeleteElementAddress(&one, two);

	EXPECT_EQ(one.link, &three);
	EXPECT_EQ(three.link, &one);
}

TEST(DeleteElementAddress_Test, DeleteElementAddress_List3ElementsDelFirstElement_returnValidVal)
{
	XOR_list* one = (XOR_list*)malloc(sizeof(XOR_list));
	assert(one);
	XOR_list two;
	XOR_list three;

	one->link = &two;
	two.link = (XOR_list*)((uintptr_t)(one) ^ (uintptr_t)(&three));
	three.link = &two;


	DeleteElementAddress(one, one);

	EXPECT_EQ(two.link, &three);
	EXPECT_EQ(three.link, &two);
}

TEST(DeleteElementAddress_Test, DeleteElementAddress_List3ElementsDelLastElement_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* three = (XOR_list*)malloc(sizeof(XOR_list));
	assert(three);


	one.link = &two;
	two.link = (XOR_list*)((uintptr_t)(&one) ^ (uintptr_t)(three));
	three->link = &two;


	DeleteElementAddress(&one, three);

	EXPECT_EQ(two.link, &one);
	EXPECT_EQ(one.link, &two);
}


TEST(DeleteElementAddress_Test, DeleteElementAddress_List2ElementsNoKeyElements_returnValidVal)
{
	XOR_list one;
	XOR_list two;
	XOR_list* three = NULL;

	one.link = &two;
	one.str = "first";
	two.link = &one;
	two.str = "second";

	DeleteElementAddress(&one, three);

	EXPECT_EQ(one.link, &two);
	EXPECT_EQ(two.link, &one);
	EXPECT_STREQ((char*)(one.str), "first");
	EXPECT_STREQ((char*)(two.str), "second");
}

