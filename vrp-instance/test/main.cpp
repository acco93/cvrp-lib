//
// Created by acco on 5/29/18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Instance.hpp>
#include <SubInstance.hpp>

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

using testing::Eq;

namespace {

    class InstanceTests : public testing::Test {

    public:


        InstanceTests() = default;

    };


    void test_instance(AbstractInstance &instance, int vehicle_capacity, int customers_num, int vertices_num) {


        ASSERT_EQ(instance.get_vehicle_capacity(), vehicle_capacity);
        ASSERT_EQ(instance.get_customers_num(), customers_num);
        ASSERT_EQ(instance.get_vertices_num(), vertices_num);

        for(auto i = instance.get_vertices_begin(); i < instance.get_vertices_end(); i++) {
            const auto neighbors = instance.get_neighbors_of(i);
            ASSERT_EQ(neighbors.size(), instance.get_vertices_num());
            auto previous_neighbor_distance = 0.0f;
            for(int j : neighbors) {
                const auto current_neighbor_distance = instance.get_cost(i, j);
                ASSERT_GE(current_neighbor_distance, previous_neighbor_distance);
                previous_neighbor_distance = current_neighbor_distance;
            }
        }

    }

    TEST_F(InstanceTests, WrongPath) {

        std::string wrong_path = "wrong_path";

        auto instance = Instance(wrong_path);

        ASSERT_EQ(instance.is_valid(), false);

    }

    TEST_F(InstanceTests, Initialization)

    {

        std::string right_path = "../../../instances/X/X-n106-k14.vrp";

        auto instance = Instance(right_path);

        ASSERT_EQ(instance.is_valid(), true);

        test_instance(instance, 600, 105, 106);

    }

    TEST_F(InstanceTests, ZKInstance) {

        std::string right_path = "../../../instances/ZK/21.txt";

        auto instance = Instance(right_path, false);

        ASSERT_EQ(instance.is_valid(), true);

        test_instance(instance, 1200, 560, 561);

    }

    TEST_F(InstanceTests, BigInstance) {

        std::string right_path = "../../../instances/VRP_Instances_Belgium/Leuven1.txt";

        auto instance = Instance(right_path);

        ASSERT_EQ(instance.is_valid(), true);

        test_instance(instance, 25, 3000, 3001);

    }

    TEST_F(InstanceTests, SubInstance) {

        std::string right_path = "../../../instances/X/X-n106-k14.vrp";

        auto instance = Instance(right_path);

        ASSERT_EQ(instance.is_valid(), true);

        auto subset = std::vector<int>({59, 32, 45, 11, 7, 1});

        auto sub_instance = SubInstance(instance, subset);

        ASSERT_EQ(sub_instance.is_valid(), true);

        test_instance(sub_instance, 600, 6, 7);


    }


}