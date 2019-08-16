//
// Created by acco on 4/11/19.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Solution.hpp"
#include <AbstractInstance.hpp>

bool Solution::load(const std::string& path) {

    this->reset();

    try {

        std::ifstream stream(path);

        if (!stream) {
            throw std::exception();
        }

        std::string line;
        std::string::size_type sz = 0;

        while (std::getline(stream, line)) {

            std::stringstream ss(line);

            if (line.rfind("Route", 0) == 0) {

                auto first_customer = true;

                auto route = -1;

                std::getline(ss, line, ':');
                std::getline(ss, line, '\n');

                while(true){

                    try {

                        const auto customer = std::stoi(line, &sz);

                        if(first_customer) {
                            route = this->build_one_customer_route(customer);
                            first_customer = false;
                        } else {
                            this->insert_vertex_before(route, instance.get_depot(), customer);
                        }

                    } catch (std::exception& e) {
                        break;
                    }

                    line.erase(0, sz);

                }

            } else if (line.rfind("Cost", 0) == 0){

                // parse the cost if necessary

            }


        }


    } catch (std::exception &e) {

        return false;

    }

    return true;

}