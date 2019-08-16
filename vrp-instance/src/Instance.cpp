//
// Created by acco on 5/29/18.
//

#include <mm.hpp>
#include <cmath>
#include "Instance.hpp"
#include <algorithm>


Instance::Instance(const std::string &path, bool round_costs) {

    valid = parse_zachariadis_kironoudis_instance(path) || parse_golden_instance(path) || parse_x_instance(path);

    if(!valid) {
        return;
    }

    initialize_data_structures(round_costs);

}

Instance::~Instance() {
    mm::release_raw_contiguous_memory(costs_matrix);
}

void Instance::initialize_data_structures(bool round_costs) {

    depot = 0;
    customers_num = matrix_size - 1;
    customers_begin = 1;
    customers_end = matrix_size;
    costs_matrix = mm::request_raw_contiguous_memory<float>(matrix_size, matrix_size);

    for(auto i = 0; i < matrix_size - 1; i++) {
        costs_matrix[i][i] = 0.0f;
        for(auto j = i + 1; j < matrix_size; j++) {

            costs_matrix[i][j] = std::sqrt((x_coordinates[i] - x_coordinates[j]) * (x_coordinates[i] - x_coordinates[j]) + (y_coordinates[i] - y_coordinates[j]) * (y_coordinates[i] - y_coordinates[j]));

            if(round_costs) {
                costs_matrix[i][j] = std::round(costs_matrix[i][j]);
            }

            costs_matrix[i][j] = costs_matrix[i][j];
            costs_matrix[j][i] = costs_matrix[i][j];

        }
    }

    neighbors.resize(matrix_size);

    auto all_vertices = std::vector<int>(matrix_size);

    for(auto i = 0; i < matrix_size; i++) {
        all_vertices[i] = i;
    }

    for(auto i = 0; i < matrix_size; i++) {

        std::sort(all_vertices.begin(), all_vertices.end(), [this, i](auto j, auto k){ return costs_matrix[i][j] < costs_matrix[i][k];});

        neighbors[i] = all_vertices;

        // ensure that the first vertex is i
        if(neighbors[i][0] != i) {
            auto n = 1;
            while(n < matrix_size) {
                if(neighbors[i][n] == i) {
                    break;
                }
                n++;
            }
            std::swap(neighbors[i][0], neighbors[i][n]);
        }

        assert(neighbors[i][0] == i);

    }

    for(auto i = customers_begin; i < customers_end; i++) {
        if(get_demand(i) == 0) {
            std::cerr << "[::ERROR::] Malformed instance: customer " << i << " load is 0.\n";
            valid = false;
        }
    }

}
