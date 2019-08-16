//
// Created by acco on 5/20/18.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <random>
#include <mm.hpp>
#include <Instance.hpp>
#include <Solution.hpp>

using testing::Eq;

namespace {

    class SolutionTest : public testing::Test {

    private:



    public:

        std::string path = "../../../instances/X/X-n106-k14.vrp";
        Instance instance;
        Solution solution;

        // make sure the instance is valid
        SolutionTest() : instance(path), solution(instance) {

            solution.reset();

        }
    };

    TEST_F(SolutionTest, Initialization) {

        solution.reset();
        
        ASSERT_EQ(0, solution.get_cost());
        ASSERT_EQ(solution.get_routes_num(), 0);

    }

    TEST_F(SolutionTest, ClarkeAndWright) {

        solution.clarke_and_wright();

        auto load_sum = 0;
        for(int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
            load_sum += instance.get_demand(i);
        }

        auto minNumRoutes = std::ceil(load_sum / instance.get_vehicle_capacity());

        ASSERT_GE(solution.get_routes_num(), minNumRoutes);

        for(int route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route)) {
            ASSERT_LE(solution.get_route_load(route) , instance.get_vehicle_capacity());
        }


    }


    TEST_F(SolutionTest, OneCustomerRoutesCreation) {

        solution.clarke_and_wright(1.0f, 0);

        ASSERT_EQ(solution.get_routes_num(), instance.get_customers_num());

        auto cost = 0.0f;
        for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route)) {
            ASSERT_EQ(solution.get_route_size(route), 1);
            ASSERT_EQ(solution.get_route_load(route), instance.get_demand(solution.get_first_customer(route)));
            cost += 2.0f * instance.get_cost(instance.get_depot(), solution.get_first_customer(route));
        }

        ASSERT_NEAR(solution.get_cost(), cost, FLT_EPSILON);

        const auto route = solution.get_first_route();
        const auto customer = solution.get_first_customer(route);
        solution.remove_vertex(route, customer);
        solution.remove_route(route);

        ASSERT_EQ(solution.get_routes_num(), instance.get_customers_num() - 1);

        ASSERT_EQ(solution.is_route_in_solution(route), false);
        ASSERT_EQ(solution.is_customer_in_solution(customer), false);

    }

    TEST_F(SolutionTest, LongerRoutesCreation) {

        solution.clarke_and_wright();

        auto cost = 0.0;
        for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route)) {
            auto load = 0;
            auto len = 0;
            cost += instance.get_cost(instance.get_depot(), solution.get_first_customer(route));
            for(auto customer = solution.get_first_customer(route); customer != instance.get_depot(); customer = solution.get_next_vertex(customer)) {
                load += instance.get_demand(customer);
                len++;
                cost += instance.get_cost(customer, solution.get_next_vertex(customer));
            }
            ASSERT_EQ(solution.get_route_size(route), len);
            ASSERT_EQ(solution.get_route_load(route), load);
        }

        ASSERT_NEAR(solution.get_cost(), cost, 0.01);

    }

    TEST_F(SolutionTest, Reverse) {

        solution.clarke_and_wright();

        std::vector<int> vec;

        const auto route = solution.get_first_route();

        for(auto i = solution.get_first_customer(route); i != instance.get_depot(); i = solution.get_next_vertex(i)) {
            vec.push_back(i);
        }
        vec.push_back(instance.get_depot());

        solution.reverse_route_path(route, solution.get_next_vertex(solution.get_first_customer(route)), solution.get_last_customer(route));
        std::reverse(std::next(vec.begin()), std::prev(vec.end()));

        for(auto n = 0u; n < vec.size(); n++) {
            const auto vertex = vec[n];
            const auto prev_index = n == 0 ? vec.size() - 1 : n - 1;
            const auto next_index = (n + 1) % vec.size();
            ASSERT_EQ(solution.get_prev_vertex(route, vertex), vec[prev_index]);
            ASSERT_EQ(solution.get_next_vertex(route, vertex), vec[next_index]);
        }

    }

    TEST_F(SolutionTest, Concat) {

        solution.clarke_and_wright();

        const auto route1 = solution.get_first_route();
        const auto route2 = solution.get_next_route(route1);

        const auto load1 = solution.get_route_load(route1);
        const auto load2 = solution.get_route_load(route2);

        const auto len1 = solution.get_route_size(route1);
        const auto len2 = solution.get_route_size(route2);

        solution.append_route(route1, route2);

        ASSERT_EQ(solution.get_route_load(route1), load1 + load2);
        ASSERT_EQ(solution.get_route_size(route1), len1 + len2);
        ASSERT_EQ(solution.is_route_in_solution(route2), false);

    }

    TEST_F(SolutionTest, CumulativeLoads) {

        solution.clarke_and_wright();

        for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route)) {
            solution.update_cumulative_route_loads(route);
            auto before = 0;
            auto after = solution.get_route_load(route);

            for(auto i = solution.get_first_customer(route); i != instance.get_depot(); i = solution.get_next_vertex(i)) {
                ASSERT_EQ(solution.get_route_load_before_included(i), before + instance.get_demand(i));
                ASSERT_EQ(solution.get_route_load_after_included(i), after);
                before += instance.get_demand(i);
                after -= instance.get_demand(i);
            }
        }

    }

    TEST_F(SolutionTest, Copy) {

        solution.clarke_and_wright();

        Solution copy(instance);

        copy = solution;

        for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route)) {
            ASSERT_EQ(solution.to_string(route), copy.to_string(route));
        }

    }

    TEST_F(SolutionTest, DepotMovement) {

        solution.clarke_and_wright();

        const auto route = solution.get_first_route();

        const auto customer = solution.get_first_customer(route);

        const auto old_cost = solution.get_cost();

        solution.remove_vertex(route, instance.get_depot());

        solution.insert_vertex_before(route, customer, instance.get_depot());

        const auto new_cost = solution.get_cost();

        ASSERT_NEAR(old_cost, new_cost, FLT_EPSILON);

    }

    TEST_F(SolutionTest, BasicClarkeAndWright) {

        solution.reset();

        for (auto i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
            solution.build_one_customer_route(i);
        }

        struct Saving {
            int i;
            int j;
            float value;
        };

        const auto savings_num = instance.get_customers_num() * (instance.get_customers_num() - 1) / 2;
        auto savings = std::vector<Saving>();
        savings.reserve(static_cast<unsigned long>(savings_num));

        for(auto i = instance.get_customers_begin(); i < instance.get_customers_end() - 1; i++) {
            for(auto j = i+1; j < instance.get_customers_end(); j++) {

                const auto value = + instance.get_cost(i, instance.get_depot())
                                   + instance.get_cost(instance.get_depot(), j)
                                   - instance.get_cost(i, j);

                savings.push_back({i, j, value});

            }
        }

        std::sort(savings.begin(), savings.end(), [](const Saving &a, const Saving &b) { return a.value > b.value; });

        for (auto &saving : savings) {

            const auto i = saving.i;
            const auto j = saving.j;

            const auto i_route = solution.get_route_index(i);
            const auto j_route = solution.get_route_index(j);

            if (i_route == j_route) { continue; }

            if (solution.get_last_customer(i_route) == i && solution.get_first_customer(j_route) == j &&
                solution.get_route_load(i_route) + solution.get_route_load(j_route) <= instance.get_vehicle_capacity()) {

                solution.append_route(i_route, j_route);


            } else if (solution.get_last_customer(j_route) == j && solution.get_first_customer(i_route) == i &&
                       solution.get_route_load(i_route) + solution.get_route_load(j_route) <= instance.get_vehicle_capacity()) {

                solution.append_route(j_route, i_route);

            }

        }

        solution.is_feasible();


    }


}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}