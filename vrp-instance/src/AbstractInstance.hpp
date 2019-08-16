//
// Created by acco on 4/12/19.
//

#ifndef VRP_ABSTRACTINSTANCE_HPP
#define VRP_ABSTRACTINSTANCE_HPP

#include <vector>
#include <string>

/**
 * Interface defining the functionalities an instance-like class must define.
 * Being an abstract class it cannot be directly instantiated.
 */
class AbstractInstance {

public:

    /**
     * Returns whether or not the instance is valid, i.e. the instance file has been successfully parsed and the 
     * corresponding instance is a feasible CVRP.
     * @return true if valid, false otherwise
     */
    virtual bool is_valid() const = 0;
    
    /**
     * Returns the depot node index.
     * @return depot
     */
    virtual int get_depot() const = 0;
    
    /**
     * Returns the vehicle capacity.
     * @return vehicle capacity
     */
    virtual int get_vehicle_capacity() const = 0;

    /**
     * Returns the number of customers.
     * @return number of customers
     */
    virtual int get_customers_num() const = 0;

    /**
     * Returns the index of the first customer.
     * @return first customer index
     */
    virtual int get_customers_begin() const = 0;

    /**
     * Returns the index after the last customer.
     * It is commonly used as follows
     * for (auto i = customers_begin(); i < customers_end(); i++) { ... }
     * @return index after the last customer
     */
    virtual int get_customers_end() const = 0;

    /**
     * Returns the total number of vertices (depot + customers)
     * @return total number of vertices
     */
    virtual int get_vertices_num() const = 0;

    /**
     * Returns the index of the first vertex.
     * @return first vertex index
     */
    virtual int get_vertices_begin() const = 0;

    /**
     * Returns the index after the last vertex.
     * It is commonly used as follows
     * for (auto i = vertices_begin(); i < vertices_end(); i++) { ... }
     * @return index after the last vertex
     */
    virtual int get_vertices_end() const = 0;

    /**
     * Returns the cost of the edge between i and j.
     * @param i vertex
     * @param j vertex
     * @return cost of the (i, j) edge
     */
    virtual float get_cost(int i, int j) const = 0;

    /**
     * Returns the vertex demand.
     * @param i vertex
     * @return vertex demand
     */
    virtual int get_demand(int i) const = 0;

    /**
     * Returns the x-coordinate of a vertex.
     * @param i vertex
     * @return vertex x-coordinate
     */
    virtual float get_x_coordinate(int i) const  = 0;

    /**
     * Returns the y-coordinate of a vertex.
     * @param i vertex
     * @return vertex y-coordinate
     */
    virtual float get_y_coordinate(int i) const  = 0;

    /**
     * Returns a reference to an array of vertices sorted according to increasing cost from i. The array includes i
     * itself in the first position. Note that this must be enforced, it is not always automatically obtained just by
     * sorting the vertices (e.g. when i and some other vertices overlap).
     * @param i vertex
     * @return reference to the neighbors array
     */
    virtual std::vector<int>& get_neighbors_of(int i) = 0;

    /**
     * Save to file the instance following the TSPLIB format. If the json parameter is set to true, the instance is
     * instead saved in a json format.
     * @param path where to save the file
     * @param json whether to save the instance in a json format
     */
    void serialize(std::string& path, bool json);

};

#endif //VRP_ABSTRACTINSTANCE_HPP
