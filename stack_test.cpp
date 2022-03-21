//  "Copyright [year] <Copyright Owner>"
#define NOMINMAX

#include "gtest/gtest.h"

#include <vector>

#include "stack.hpp"

#include "iutest.hpp"


// Test用　関数
namespace {
    typedef int* (Stack::* GetBody)();
    IUTEST_MAKE_PEEP(GetBody, Stack, GetBody);

    typedef int (Stack::* GetCount)();
    IUTEST_MAKE_PEEP(GetCount, Stack, GetCount);

    bool check_invaliant(const Stack& stack) {
        auto p = IUTEST_PEEP_GET(stack, Stack, GetBody)();
        int c = IUTEST_PEEP_GET(stack, Stack, GetCount)();

        bool a = (p != nullptr);
        bool b = (c >= 0);

        return a && b;
    }
}

// Test Stackクラスの一連のメソッドの確認
TEST(StackTest, SimpleStackTest) {
    // Arrange
    Stack stk;

    // Action
    stk.push(0);
    stk.push(2);

    // Assert
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

// Test push_test
TEST(StackTest, push_test) {
    // Arrange
    Stack stk;

    // Action
    stk.push(0);

    // Assert
    int out;

    ASSERT_EQ(1, stk.count());

    ASSERT_EQ(true, stk.pop(out));
    ASSERT_EQ(0, out);

    ASSERT_TRUE(check_invaliant(stk));
}

// Test push test 小さな範囲のpushをする
TEST(StackTest, push_small_range_test) {
    // Arrange
    Stack stk;

    std::vector<int> in = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    // Action
    for (const auto& v : in) {
        stk.push(v);
    }

    // Assert
    ASSERT_TRUE(check_invaliant(stk));

    for (int i = 0; i < in.size(); ++i) {
        int out;
        ASSERT_EQ(in.size() - i, stk.count());

        ASSERT_EQ(true, stk.pop(out));
        ASSERT_EQ(in.size() - i - 1, out);
    }

    ASSERT_TRUE(check_invaliant(stk));
}

// Test push test 大きな範囲のpushする
TEST(StackTest, push_large_range_test) {
    // Arrange
    Stack stk;

    std::vector<int64_t> in;
    constexpr int64_t max = std::numeric_limits<int16_t>::max();
    in.reserve(max);
    for (int64_t i = 0; i < max; ++i) {
        in.push_back(i);
    }

    // Action
    for (const auto& v : in) {
        stk.push(v);
    }

    // Assert
    ASSERT_TRUE(check_invaliant(stk));

    for (int i = 0; i < max; ++i) {
        int out;
        ASSERT_EQ(max - i, stk.count());

        ASSERT_EQ(true, stk.pop(out));
        ASSERT_EQ(max - i - 1, out);
    }

    ASSERT_TRUE(check_invaliant(stk));
}

// Test pop test empty時のpop
TEST(StackTest, pop_empty) {
    // Arrange
    Stack stk;

    // Action
    int out;
    bool act = stk.pop(out);

    // Assert
    ASSERT_EQ(0, stk.count());
    ASSERT_EQ(false, act);
    ASSERT_TRUE(check_invaliant(stk));
}

// Test pop test 一度用度を入れたのちのempty時のpop
TEST(StackTest, pop_re_empty) {
    // Arrange
    Stack stk;

    std::vector<int> in = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    for (const auto& v : in) {
        stk.push(v);
    }

    for (int i = 0; i < in.size(); ++i) {
        int out;
        stk.pop(out);
    }

    // Action
    int out;
    bool act = stk.pop(out);

    // Assert
    ASSERT_EQ(0, stk.count());
    ASSERT_EQ(false, act);
    ASSERT_TRUE(check_invaliant(stk));
}

// Test push test メモリ枯渇
TEST(StackTest, push_exhausted_test) {
    // Arrange
    Stack stk;

    constexpr int64_t max = std::numeric_limits<int64_t>::max();

    // Action
    bool ret = true;
    for (int i = 0; i < max; ++i) {
        ret = stk.push(i);
        if (!ret) break;
    }

    // Assert
    ASSERT_FALSE(ret);
    ASSERT_TRUE(check_invaliant(stk));
}
