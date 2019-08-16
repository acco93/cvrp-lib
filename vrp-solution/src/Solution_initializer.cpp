//
// Created by acco on 5/22/18.
//

#include <cassert>
#include <algorithm>
#include <iostream>
#include "Solution.hpp"
#include "Instance.hpp"


void Solution::clarke_and_wright(float lambda, int neighbors_num) {

    this->reset();

    for (auto i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        this->build_one_customer_route(i);
    }

    neighbors_num = std::min(instance.get_customers_num() - 1, neighbors_num);

    const auto savings_num = instance.get_customers_num() * neighbors_num / 2ul;

    struct Saving {
        int i;
        int j;
        float value;
    };

    auto savings = std::vector<Saving>();
    savings.reserve(static_cast<unsigned long>(savings_num));

    for(auto i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {

        for(auto n = 1u, added=0u; added < static_cast<unsigned int>(neighbors_num) && n < instance.get_neighbors_of(i).size(); n++) {

            const auto j = instance.get_neighbors_of(i)[n];
            // avoid symmetries and depot (depot is 0 thus is the following condition is always false for it)
            if(i < j) {

                const auto value = + instance.get_cost(i, instance.get_depot())
                                   + instance.get_cost(instance.get_depot(), j)
                                   - lambda * instance.get_cost(i, j);

                savings.push_back({i, j, value});

                added++;

            }

        }
    }

    std::sort(savings.begin(), savings.end(), [](const Saving &a, const Saving &b) { return a.value > b.value; });

    for (auto &saving : savings) {

        const auto i = saving.i;
        const auto j = saving.j;

        const auto iRoute = this->get_route_index(i);
        const auto jRoute = this->get_route_index(j);

        if (iRoute == jRoute) { continue; }

        if (this->get_last_customer(iRoute) == i && this->get_first_customer(jRoute) == j &&
            this->get_route_load(iRoute) + this->get_route_load(jRoute) <= instance.get_vehicle_capacity()) {

            this->append_route(iRoute, jRoute);


        } else if (this->get_last_customer(jRoute) == j && this->get_first_customer(iRoute) == i &&
                   this->get_route_load(iRoute) + this->get_route_load(jRoute) <= instance.get_vehicle_capacity()) {

            this->append_route(jRoute, iRoute);

        }

    }

    assert(this->is_feasible());

}
