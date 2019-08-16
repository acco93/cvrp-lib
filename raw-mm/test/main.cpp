//
// Created by acco on 5/25/18.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mm.hpp>

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using testing::Eq;

namespace {

    class MemTests : public testing::Test {

    private:



    public:


        MemTests() {

        }


    };

    TEST_F(MemTests, Usage) {

        const auto dim = 100;
        auto ptr = mm::request_raw_contiguous_memory<int>(dim);
        ASSERT_NE(ptr, nullptr);
        mm::release_raw_contiguous_memory(ptr);
        ASSERT_EQ(ptr, nullptr);

        auto ptr2d = mm::request_raw_contiguous_memory<double>(dim, dim);
        ASSERT_NE(ptr2d, nullptr);
        mm::release_raw_contiguous_memory(ptr2d);
        ASSERT_EQ(ptr2d, nullptr);


    }



}