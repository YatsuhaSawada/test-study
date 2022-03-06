#define NOMINMAX

#include "gtest/gtest.h"

#include <vector>

#include "stack.hpp"

#include "iutest.hpp"


//Test�p�@�֐�
namespace {
    typedef int* (Stack::* GetBody)();
    IUTEST_MAKE_PEEP(GetBody, Stack, GetBody);

    typedef int (Stack::* GetCount)();
    IUTEST_MAKE_PEEP(GetCount, Stack, GetCount);

    bool check_invaliant(Stack& stack) {
        auto p = IUTEST_PEEP_GET(stack, Stack, GetBody)();
        int c = IUTEST_PEEP_GET(stack, Stack, GetCount)();

        bool a = (p != nullptr);
        bool b = (c >= 0);

        return a && b;
    }
}

//Test Stack�N���X�̈�A�̃��\�b�h�̊m�F
TEST(StackTest, SimpleStackTest) {

    //Arrange
    Stack stk;

    //Action
    stk.push(0);
    stk.push(2);

    //Assert
    int out;

    ASSERT_EQ(2, stk.count());

    ASSERT_EQ(true, stk.pop(out));
    ASSERT_EQ(2, out);
    ASSERT_EQ(1, stk.count());

    ASSERT_EQ(true, stk.pop(out));
    ASSERT_EQ(0, out);
    ASSERT_EQ(0, stk.count());

    ASSERT_TRUE(check_invaliant(stk));
}

//Test push_test
TEST(StackTest, push_test) {

    //Arrange
    Stack stk;

    //Action
    stk.push(0);

    //Assert
    int out;

    ASSERT_EQ(1, stk.count());

    ASSERT_EQ(true, stk.pop(out));
    ASSERT_EQ(0, out);

    ASSERT_TRUE(check_invaliant(stk));
}

//Test push test �����Ȕ͈͂�push������
TEST(StackTest, push_small_range_test) {

    //Arrange
    Stack stk;

    std::vector<int> in = { 0, 1,2,3,4,5,6,7,8,9,10,11 };

    //Action
    for (const auto& v : in) {
        stk.push(v);
    }

    //Assert
    ASSERT_TRUE(check_invaliant(stk));

    for (int i = 0; i < in.size(); ++i) {
        int out;
        ASSERT_EQ(in.size() - i, stk.count());

        ASSERT_EQ(true, stk.pop(out));
        ASSERT_EQ(in.size() - i - 1, out);
    }

    ASSERT_TRUE(check_invaliant(stk));
}

//Test push test �傫�Ȕ͈͂�push����
TEST(StackTest, push_large_range_test) {

    //Arrange
    Stack stk;

    std::vector<int> in;
    constexpr int max = std::numeric_limits<short>::max();
    in.reserve(max);
    for (int i = 0; i < max; ++i) {
        in.push_back(i);
    }

    //Action
    for (const auto& v : in) {
        stk.push(v);
    }
 
    //Assert
    ASSERT_TRUE(check_invaliant(stk));

    for (int i = 0; i < max; ++i) {
        int out;
        ASSERT_EQ(max - i, stk.count());

        ASSERT_EQ(true, stk.pop(out));
        ASSERT_EQ(max - i - 1, out);
    }
 
    ASSERT_TRUE(check_invaliant(stk));
}

//Test pop test empty����pop
TEST(StackTest, pop_empty) {

    //Arrange
    Stack stk;

    //Action
    int out;
    bool act = stk.pop(out);

    //Assert
    ASSERT_EQ(0, stk.count());
    ASSERT_EQ(false, act);
    ASSERT_TRUE(check_invaliant(stk));
}

//Test pop test ��x�p�x����ꂽ�̂���empty����pop
TEST(StackTest, pop_re_empty) {

    //Arrange
    Stack stk;

    std::vector<int> in = { 0, 1,2,3,4,5,6,7,8,9,10,11 };

    for (const auto& v : in) {
        stk.push(v);
    }

    for (int i = 0; i < in.size(); ++i) {
        int out;
        stk.pop(out);
    }

    //Action
    int out;
    bool act = stk.pop(out);

    //Assert
    ASSERT_EQ(0, stk.count());
    ASSERT_EQ(false, act);
    ASSERT_TRUE(check_invaliant(stk));
}

//Test push test �������͊�
TEST(StackTest, push_exhausted_test) {

    //Arrange
    Stack stk;

    constexpr int max = std::numeric_limits<int>::max();

    //Action
    bool ret = true;
    for (int i = 0; i < max; ++i) {
        ret = stk.push(i);
        if (!ret) break;
    }

    //Assert
    ASSERT_FALSE(ret);
    ASSERT_TRUE(check_invaliant(stk));
}