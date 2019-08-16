//
// Created by acco on 5/29/18.
//

#ifndef VRP_INSTANCE_HPP
#define VRP_INSTANCE_HPP

#include <string>
#include <vector>
#include <cfloat>
#include "AbstractInstance.hpp"

/**
 * Basic implementation of InstanceInterface.
 */
class Instance : public AbstractInstance {

    bool valid = false;

    int matrix_size = 0;
    int capacity = 0;
    std::vector<float> x_coordinates;
    std::vector<float> y_coordinates;
    std::vector<int> demands;
    int depot = 0;
    int customers_num = 0;
    int customers_begin = 0;
    int customers_end = 0;
    float** costs_matrix = nullptr;
    std::vector<std::vector<int>> neighbors;

    bool parse_golden_instance(const std::string& string);
    bool parse_x_instance(const std::string& string);
    bool parse_zachariadis_kironoudis_instance(const std::string& string);
    void initialize_data_structures(bool b);

public:

    explicit Instance(const std::string& path, bool round_costs = true);

    virtual ~Instance();

    inline bool is_valid() const override {return valid; }

    inline int get_depot() const override { return 0; }

    inline int get_vehicle_capacity() const override { return capacity; }

    inline int get_customers_num() const override { return customers_num; }

    inline int get_customers_begin() const override { return customers_begin; }

    inline int get_customers_end() const override { return customers_end; }

    inline int get_vertices_num() const override { return matrix_size; }

    inline int get_vertices_begin() const override { return 0; }

    inline int get_vertices_end() const override {return matrix_size; }

    inline float get_cost(int i, int j) const override { return costs_matrix[i][j]; }

    inline int get_demand(int i) const override { return demands[i]; }

    inline std::vector<int>& get_neighbors_of(int i) override { return neighbors[i]; }

    inline float get_x_coordinate(int vertex) const override { return x_coordinates[vertex]; }

    inline float get_y_coordinate(int vertex) const override { return y_coordinates[vertex]; }

};


#endif //VRP_INSTANCE_HPP
