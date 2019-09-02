//
// Created by acco on 8/15/19.
//

#include <fstream>
#include "AbstractInstance.hpp"

void serialize_TSPLIB(const AbstractInstance& instance, std::string& path);
void serialize_json(const AbstractInstance& instance, std::string& path);

void AbstractInstance::serialize(std::string &path, bool json) {

    if(json) {
        serialize_json(*this, path);
    } else {
        serialize_TSPLIB(*this, path);
    }

}

void serialize_TSPLIB(const AbstractInstance &instance, std::string &path) {
    auto stream = std::ofstream(path);
    stream << "NAME : name\n";
    stream << "COMMENT : (comment)\n";
    stream << "TYPE : CVRP\n";
    stream << "DIMENSION : " << instance.get_vertices_num() << "\n";
    stream << "EDGE_WEIGHT_TYPE : EUC_2D\n";
    stream << "CAPACITY : " << instance.get_vehicle_capacity() << "\n";
    stream << "NODE_COORD_SECTION\n";
    stream << "1\t" << instance.get_x_coordinate(instance.get_depot()) << "\t" << instance.get_y_coordinate(instance.get_depot()) << "\n";
    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        stream << (i+1) << "\t" << instance.get_x_coordinate(i) << "\t" << instance.get_y_coordinate(i) << "\n";
    }
    stream << "DEMAND_SECTION\n";
    stream << "1\t0\n";
    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        stream << (i+1) << "\t" << instance.get_demand(i) << "\n";
    }
    stream << "DEPOT_SECTION\n";
    stream << "\t1\n";
    stream << "\t-1\n";
    stream << "EOF\n";
    stream.close();
}

void serialize_json(const AbstractInstance &instance, std::string &path) {
    auto stream = std::ofstream(path);
    stream << "{";
    stream << "\"x\":[";
    for (auto i = instance.get_vertices_begin(); i < instance.get_vertices_end() - 1; i++) {
        stream << instance.get_x_coordinate(i) << ",";
    }
    stream << instance.get_x_coordinate(instance.get_vertices_end() - 1) << "],\n";
    stream << "\"y\":[";
    for (auto i = instance.get_vertices_begin(); i < instance.get_vertices_end() - 1; i++) {
        stream << instance.get_y_coordinate(i) << ",";
    }
    stream << instance.get_y_coordinate(instance.get_vertices_end() - 1) << "],\n";
    stream << "\"Q\":" << instance.get_vehicle_capacity() << ",\n";
    stream << "\"q\":[";
    for (auto i = instance.get_vertices_begin(); i < instance.get_vertices_end() - 1; i++) {
        stream << instance.get_demand(i) << ",";
    }
    stream << instance.get_demand(instance.get_vertices_end() - 1) << "]\n";
    stream << "}\n";
    stream.close();
}