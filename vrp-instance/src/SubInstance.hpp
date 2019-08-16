    // Created by acco on 4/12/19.
//

#ifndef VRP_SUBINSTANCE_HPP
#define VRP_SUBINSTANCE_HPP


#include <iostream>
#include <unordered_map>
#include "Instance.hpp"

/**
 * A class used to offer a partial consistent view over an AbstractInstance object.
 */
class SubInstance : public AbstractInstance {

    AbstractInstance& instance;

    std::vector<int> mapping;

    std::unordered_map<int, int> reverse_mapping;

    int customers_num;

    std::vector<float> features;

    std::vector<std::vector<int>> neighbors;

public:

    SubInstance(AbstractInstance &instance_, std::vector<int>& customers) : instance(instance_) {


        customers_num = customers.size();

        // Define a 0-based mapping from this chunk of 'customers' to the real
        // customers. 'customers' must not contain the depot!
        mapping.resize(customers.size() + 1);
        mapping[instance.get_depot()] = instance.get_depot();

        reverse_mapping[instance.get_depot()] = instance.get_depot();

        neighbors.resize(mapping.size());

        for(auto i = 0ul; i < customers.size(); i++) {

            mapping[i+1] = customers[i];
            reverse_mapping[customers[i]] = (i+1);

        }

        auto all_vertices = std::vector<int>(customers_num+1);

        for(auto i = get_vertices_begin(); i < get_vertices_end(); i++) {
            all_vertices[i] = i;
        }

        for(auto i = get_vertices_begin(); i < get_vertices_end(); i++) {

            std::sort(all_vertices.begin(), all_vertices.end(), [this, i](auto j, auto k){ return get_cost(i,j) < get_cost(i, k);});

            neighbors[i] = all_vertices;

            // ensure that the first vertex is i
            if(neighbors[i][0] != i) {
                auto n = 1;
                while(n < get_vertices_num()) {
                    if(neighbors[i][n] == i) {
                        break;
                    }
                    n++;
                }
                std::swap(neighbors[i][0], neighbors[i][n]);
            }

            assert(neighbors[i][0] == i);

        }


}

    inline int get_mapping(int i) {
        return mapping[i];
    }

    inline int get_reverse_mapping(int vertex_original_coordinates) {
        return reverse_mapping[vertex_original_coordinates];
    }

    inline int get_customers_num() const override {
        return customers_num;
    }

    inline int get_customers_begin() const override {
        return 1;
    }

    inline int get_customers_end() const override {
        return mapping.size();
    }

    inline int get_vertices_num() const override {
        return mapping.size();
    }

    inline int get_vertices_begin() const override {
        return 0;
    }

    inline int get_vertices_end() const override {
        return mapping.size();
    }

    inline float get_cost(int i, int j) const override {
        return instance.get_cost(mapping[i], mapping[j]);
    }

    inline bool is_valid() const override { return instance.is_valid(); }

    inline int get_depot() const override { return instance.get_depot(); }

    inline int get_vehicle_capacity() const override { return instance.get_vehicle_capacity(); }

    inline int get_demand(int i) const override { return instance.get_demand(mapping[i]); }

    inline std::vector<int> &get_neighbors_of(int i) override { return neighbors[i]; }

    inline float get_x_coordinate(int customer) const override { return instance.get_x_coordinate(mapping[customer]); }

    inline float get_y_coordinate(int customer) const override { return instance.get_y_coordinate(mapping[customer]); }

};


#endif //VRP_SUBINSTANCE_HPP
