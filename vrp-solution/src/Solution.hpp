//
// Created by acco on 5/29/18.
//

#ifndef VRP_SOLUTION_HPP
#define VRP_SOLUTION_HPP

#include <FixedSizeValueStack.hpp>
#include <vector>
#include <stack>
#include <random>
#include <unordered_set>

class AbstractInstance;

/**
 * Class used to represent and manage a solution.
 */
class Solution {

    /**
     * Deep copy of a source solution.
     * @param source the source solution
     */
    void copy(const Solution &source);

public:

    /**
     * Dummy value to identify whether a route contains real customers or it is empty.
     */
    static const int dummy_vertex;

    /**
     * Dummy value to stop route's iteration. It is commonly used as
     * for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route))
     */
    static const int dummy_route;

    /**
     * Solution constructor. Note that the instance reference ownership is not transferred.
     * @param instance instance
     */
    explicit Solution(AbstractInstance &instance);

    /**
     * Generate a new solution from a the deep copy of a source solution.
     * @param source solution
     */
    Solution(const Solution &source);

    /**
     * Solution destructor.
     */
    virtual ~Solution();

    /**
     * Deep copy a source solution
     * @param source solution
     * @return solution
     */
    Solution &operator=(const Solution &source);

    /**
     * Reset a solution.
     */
    void reset();

    /**
     * Initialize a solution with the Clarke and Wright algorithm.
     * @param lambda parameter of the algorithm
     * @param neighbors_num neighbors to consider in the merge process
     */
    void clarke_and_wright(float lambda = 1.0f, int neighbors_num = 1000);

    /**
     * Returns the solution cost.
     * @return cost
     */
    float get_cost() const;

    /**
     * Returns the number of routes in the solution.
     * @return number of routes
     */
    int get_routes_num() const;

    /**
     * Build a one-customer route
     * @param customer
     * @return route index
     */
    int build_one_customer_route(int customer);

    /**
     * Returns the index of the route serving a given customer.
     * @param customer
     * @return route index
     */
    int get_route_index(int customer) const;

    /**
     * Returns the index if the route serving a given vertex. If vertex is the depot,
     * fallback is used to identify the route index.
     * @param vertex
     * @param fallback
     * @return route index
     */
    int get_route_index(int vertex, int fallback) const;

    /**
     * Returns the route load
     * @param route
     * @return load
     */
    int get_route_load(int route) const;

    /**
     * Index of the first route. Commonly used when iterating through all the routes
     * for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route))
     * @return index of the first route
     */
    int get_first_route() const;

    /**
     * Returns the next route. Commonly used when iterating through all the routes
     * for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route))
     * @param route
     * @return index of the first route
     */
    int get_next_route(int route) const;

    /**
     * Returns whether the route contains zero customers
     * @param route
     * @return boolean
     */
    bool is_route_empty(int route) const;

    /**
     * Remove a vertex from a given route
     * @param route
     * @param vertex
     * @return the cost saving
     */
    float remove_vertex(int route, int vertex);

    /**
     * Removes an empty route. The route must be empty.
     * @param route empty route
     */
    void remove_route(int route);

    /**
     * Returns the first customer of a route. It is commonly used as
     * for(auto i = solution.get_first_customer(route); i != instance.get_depot(); i = solution.get_next_vertex(i)){ ... }
     * @param route
     * @return first customer
     */
    int get_first_customer(int route) const;

    /**
     * Returns the last customer of a route.
     * @param route
     * @return last customer
     */
    int get_last_customer(int route) const;

    /**
     * Returns the vertex after a given one.
     * @param route
     * @param vertex
     * @return vertex after a given one
     */
    int get_next_vertex(int route, int vertex) const;

    /**
     * Returns the vertex before a given one.
     * @param route
     * @param vertex
     * @return vertex before a given one
     */
    int get_prev_vertex(int route, int vertex) const;

    /**
     * Inserts a vertex before where in route.
     * @param route
     * @param where
     * @param vertex
     */
    void insert_vertex_before(int route, int where, int vertex);

    /**
     * Reverses the sub-path identified by vertex_begin and vertex_end.
     * @param route
     * @param vertex_begin
     * @param vertex_end
     */
    void reverse_route_path(int route, int vertex_begin, int vertex_end);

    /**
     * Merges two routes
     * @param route
     * @param route_to_append
     * @return index of the merge route
     */
    int append_route(int route, int route_to_append);

    /**
     * Generates a string representation of a given route.
     * @param route
     * @return string
     */
    std::string to_string(int route) const;

    /**
     * Prints a given route.
     * @param route
     */
    void print(int route) const;

    /**
     * Prints the whole soluion.
     */
    void print() const;

    /**
     * Updates the data structures containing the route's cumulative loads.
     * Commonly used in some local search operators (e.g. inter-route 2 opt)
     * @param route
     */
    void update_cumulative_route_loads(int route);

    /**
     * Accesses the route's cumulative load before and including customer
     * @param customer
     * @return cumulative load
     */
    int get_route_load_before_included(int customer) const;

    /**
     * Accesses the route's cumulative load after and including customer
     * @param customer
     * @return cumulative load
     */
    int get_route_load_after_included(int customer) const;

    /**
     * Checks whether a route is currently used in the solution.
     * @param route
     * @return boolean flag
     */
    bool is_route_in_solution(int route) const;

    /**
     * Checks whether a customer is currently served in the solution.
     * @param customer
     * @return boolean flag
     */
    bool is_customer_in_solution(int customer) const;

    /**
     * Check whether a vertex is served in a given route.
     * @param route
     * @param vertex
     * @return
     */
    bool contains_vertex(int route, int vertex) const;

    /**
     * Returns the number of customers served by the route
     * @param route
     * @return number of customers
     */
    int get_route_size(int route) const;

    /**
     * Returns the customer before a given one.
     * @param customer
     * @return customer before a given one
     */
    int get_prev_vertex(int customer) const;

    /**
     * Returns the customer after a given one. It is commonly used as
     * for(auto i = solution.get_first_customer(route); i != instance.get_depot(); i = solution.get_next_vertex(i)){ ... }
     * @param customer
     * @return customer after a given one
     */
    int get_next_vertex(int customer) const;

    /**
     * Check whether the solution is CVRP feasible.
     * @return
     */
    bool is_feasible();

    /**
     * Returns the route cost.
     * @param route
     * @return route cost
     */
    float get_route_cost(int route) const;

    /**
     * Clear the set of recently modified vertices.
     */
    inline void commit() { unstaged_changes.clear(); }

    /**
     * Returns the recently modified vertices.
     * Note that it is not safe to perform any operations while iterating the returned set.
     * @return set
     */
    inline const std::unordered_set<int>& get_unstaged_changes() const { return unstaged_changes; }

    /**
     * Load an existing solution.
     * @param path
     * @return whether the loading has been successful
     */
    bool load(const std::string& path);

private:

    struct DepotNode {
        int first_route;
        int num_routes;
    };

    struct CustomerNode {
        int next;
        int prev;
        int route_ptr;
        int load_after;
        int load_before;
    };

    struct RouteNode {
        int first_customer;
        int last_customer;
        int load;
        int next;
        int prev;
        int size;
        float cost;
    };

    AbstractInstance &instance;
    double solution_cost;
    const int max_number_routes;
    FixedSizeValueStack<int> routes_pool;
    struct DepotNode depot_node;
    struct RouteNode *routes_list;
    struct CustomerNode *customers_list;

    void reset_route(int route);
    void reset_vertex(int customer);
    void set_next_vertex_ptr(int route, int vertex, int next);
    void set_prev_vertex_ptr(int route, int vertex, int prev);
    int request_route();
    void release_route(int route);

    std::unordered_set<int> unstaged_changes;

};



#endif //VRP_SOLUTION_HPP
