//
// Created by acco on 5/26/18.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <FixedSizeValueStack.hpp>

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using testing::Eq;

namespace {

    class UtilsTests : public testing::Test {

    private:



    public:


        UtilsTests() {

        }


    };

    TEST_F(UtilsTests, Usage) {

        const auto dim = 100;
        const auto initializer = [](int index) { return index + 1; };

        FixedSizeValueStack<int> stack(dim-1, initializer);

        ASSERT_FALSE(stack.is_empty());

        for(int i = 0; i <dim-1; i++){
            const auto item = stack.get();
            ASSERT_EQ(item, i+1);
        }

        ASSERT_TRUE(stack.is_empty());

        stack.reset();

        for(int i = 0; i <dim - 10; i++){
            const auto item = stack.get();
            ASSERT_EQ(item, i+1);
        }

        stack.push(0);
        stack.push(1);

        ASSERT_EQ(1, stack.get());
        ASSERT_EQ(0, stack.get());

        while(!stack.is_empty()) {
            stack.get();
        }

        ASSERT_TRUE(stack.is_empty());

        stack.push(0);

        FixedSizeValueStack<int> stack2(dim-1, initializer);

        stack2 = stack;

        ASSERT_EQ(stack2.get(), 0);

        ASSERT_TRUE(stack2.is_empty());


    }



}