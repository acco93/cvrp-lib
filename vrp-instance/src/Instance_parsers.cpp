//
// Created by acco on 1/25/19.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "Instance.hpp"


bool Instance::parse_golden_instance(const std::string& path) {

    try {

        std::ifstream stream(path);

        if (!stream) {
            throw std::exception();
        }

        std::string line;
        std::string::size_type sz = 0;

        // skip 3 lines
        std::getline(stream, line);
        std::getline(stream, line);
        std::getline(stream, line);

        // dimension
        std::getline(stream, line);
        std::stringstream ss(line);
        std::getline(ss, line, ':');
        std::getline(ss, line, '\n');
        matrix_size = stoi(line, &sz);
        line.erase(0, sz);

        // capacity
        std::getline(stream, line);
        std::stringstream ss2(line);
        std::getline(ss2, line, ':');
        std::getline(ss2, line, '\n');
        capacity = stoi(line, &sz);
        line.erase(0, sz);

        // vehicles
        std::getline(stream, line);

        // skip 4 lines
        std::getline(stream, line);
        std::getline(stream, line);
        std::getline(stream, line);
        std::getline(stream, line);

        x_coordinates.resize(matrix_size);
        y_coordinates.resize(matrix_size);
        demands.resize(matrix_size);

        // coord section
        // reserve index 0 for the depot (it is not define in this section)
        for (auto n = 1; n < matrix_size; n++) {

            std::getline(stream, line);

            stoi(line, &sz);    // index
            line.erase(0, sz);

            x_coordinates[n] = stof(line, &sz);
            line.erase(0, sz);

            y_coordinates[n] = stof(line, &sz);
            line.erase(0, sz);

        }

        // demand section
        std::getline(stream, line);
        // depot not in this section  too
        for (auto n = 1; n < matrix_size; n++) {

            std::getline(stream, line);

            stoi(line, &sz);        // index
            line.erase(0, sz);

            demands[n] = stoi(line, &sz);
            line.erase(0, sz);

        }

        // depot section
        std::getline(stream, line);

        std::getline(stream, line);

        x_coordinates[0] = stof(line, &sz);
        line.erase(0, sz);

        y_coordinates[0] = stof(line, &sz);
        line.erase(0, sz);

        demands[0] = 0;

    } catch (std::exception &e) {

        return false;

    }

    return true;
}

bool Instance::parse_x_instance(const std::string& path) {

    try {

        std::ifstream stream(path);

        if(!stream) {
            throw std::exception();
        }

        std::string line;
        std::string::size_type sz = 0;

        // skip 3 lines
        std::getline(stream, line);
        std::getline(stream, line);
        std::getline(stream, line);

        // dimension
        std::getline(stream, line);
        std::stringstream ss(line);
        std::getline(ss, line, ':');
        std::getline(ss, line, '\n');
        matrix_size = stoi(line, &sz);
        line.erase(0, sz);

        // skip 1 line
        std::getline(stream, line);

        // capacity
        std::getline(stream, line);
        std::stringstream ss2(line);
        std::getline(ss2, line, ':');
        std::getline(ss2, line, '\n');
        capacity = stoi(line, &sz);
        line.erase(0, sz);

        // skip 1 line
        std::getline(stream, line);

        x_coordinates.resize(matrix_size);
        y_coordinates.resize(matrix_size);
        demands.resize(matrix_size);

        // coord section
        for (auto n = 0; n < matrix_size; n++) {

            std::getline(stream, line);

            stoi(line, &sz);    // index
            line.erase(0, sz);

            x_coordinates[n] = stof(line, &sz);
            line.erase(0, sz);

            y_coordinates[n] = stof(line, &sz);
            line.erase(0, sz);

        }

        // demand section
        std::getline(stream, line);
        for (auto n = 0; n < matrix_size; n++) {

            std::getline(stream, line);

            stoi(line, &sz);        // index
            line.erase(0, sz);

            demands[n] = stoi(line, &sz);
            line.erase(0, sz);

        }


    } catch (std::exception& e) {
        return false;
    }

    return true;

}

bool Instance::parse_zachariadis_kironoudis_instance(const std::string& path) {

    try {

        std::ifstream stream(path);

        if (!stream) {
            throw std::exception();
        }

        std::string line;
        std::string::size_type sz = 0;

        // number of customers \t capacity \t route-lenght limit (optional)
        std::getline(stream, line);
        std::stringstream ss(line);

        std::getline(ss, line);
        matrix_size = stoi(line, &sz) + 1;
        line.erase(0, sz);

        capacity = static_cast<int>(stof(line, &sz));
        line.erase(0, sz);


        try {
            stof(line, &sz);
            line.erase(0, sz);
        } catch (std::exception& e) {
        }




        x_coordinates.resize(matrix_size);
        y_coordinates.resize(matrix_size);
        demands.resize(matrix_size);

        for (auto n = 0; n < matrix_size; n++) {

            std::getline(stream, line);

            stoi(line, &sz);    // index
            line.erase(0, sz);

            x_coordinates[n] = stof(line, &sz);
            line.erase(0, sz);

            y_coordinates[n] = stof(line, &sz);
            line.erase(0, sz);

            demands[n] = stoi(line, &sz);
            line.erase(0, sz);

        }

    } catch (std::exception &e) {

        return false;

    }

    return true;

}


