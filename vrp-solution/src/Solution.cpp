//
// Created by acco on 5/29/18.
//

#include <cmath>
#include <iostream>
#include <macro.hpp>
#include "Solution.hpp"
#include <AbstractInstance.hpp>


const int Solution::dummy_vertex = -1;
const int Solution::dummy_route = 0;

Solution::Solution(AbstractInstance &instance) :
        instance(instance),
        solution_cost(INFINITY),
        max_number_routes(instance.get_vertices_num() + 1),
        routes_pool(max_number_routes - 1, [](int index) { return index + 1; }),
        depot_node({Solution::dummy_route, 0}),
        routes_list(new RouteNode[max_number_routes]),
        customers_list(new CustomerNode[instance.get_vertices_num()]) {
}


Solution::~Solution() {
    delete[] customers_list;
    delete[] routes_list;
}

void Solution::copy(const Solution &source) {

    routes_pool = source.routes_pool;

    depot_node = source.depot_node;
    for (int i = 0; i < instance.get_vertices_num(); i++) {
        customers_list[i] = source.customers_list[i];
    }
    for (int r = 0; r < instance.get_vertices_num(); r++) {
        routes_list[r] = source.routes_list[r];
    }
    solution_cost = source.solution_cost;

    unstaged_changes = source.unstaged_changes;

}

Solution &Solution::operator=(const Solution &source) {

    copy(source);

    return *this;

}

Solution::Solution(const Solution &source) :
        instance(source.instance),
        solution_cost(INFINITY),
        max_number_routes(instance.get_vertices_num() + 1),
        routes_pool(max_number_routes - 1, [](int index) { return index + 1; }),
        depot_node({Solution::dummy_route, 0}),
        routes_list(new RouteNode[max_number_routes]),
        customers_list(new CustomerNode[instance.get_vertices_num()]) {

    copy(source);

}

void Solution::reset_route(const int route) {
    routes_list[route].load = 0;
    routes_list[route].size = 0;
    routes_list[route].cost = 0.0f;
    routes_list[route].first_customer = instance.get_depot();
    routes_list[route].last_customer = instance.get_depot();
}

void Solution::reset_vertex(const int customer) {
    customers_list[customer].next = Solution::dummy_vertex;
    customers_list[customer].prev = Solution::dummy_vertex;
    customers_list[customer].route_ptr = Solution::dummy_route;
}

void Solution::reset() {

    solution_cost = 0.0;

    routes_pool.reset();

    depot_node.first_route = Solution::dummy_route;
    depot_node.num_routes = 0;

    for (int r = 0; r < max_number_routes; r++) {
        reset_route(r);
    }

    for (int i = 0; i < instance.get_vertices_num(); i++) {
        reset_vertex(i);
    }

}

float Solution::get_cost() const {

    return static_cast<float>(solution_cost);

}

int Solution::get_routes_num() const {
    return depot_node.num_routes;
}

int Solution::request_route() {

    assert(!routes_pool.is_empty());

    const auto route = routes_pool.get();

    return route;
}

int Solution::build_one_customer_route(const int customer) {

    assert(!is_customer_in_solution(customer));
    assert(customer != instance.get_depot());

    const auto route = request_route();

    customers_list[customer].prev = instance.get_depot();
    customers_list[customer].next = instance.get_depot();
    customers_list[customer].route_ptr = route;

    // head insert the route in the list
    const auto next_route = depot_node.first_route;
    routes_list[route].next = next_route;                  // copy the previous head in the next
    depot_node.first_route = route;                       // and set the new head
    routes_list[route].prev = Solution::dummy_route;      // since it is an head insertion
    routes_list[next_route].prev = route;                  // set the prev of the next route as this new route

    depot_node.num_routes++;

    routes_list[route].first_customer = customer;
    routes_list[route].last_customer = customer;
    routes_list[route].load = instance.get_demand(customer);
    routes_list[route].size = 1;
    routes_list[route].cost = 2.0f * instance.get_cost(instance.get_depot(), customer);

    solution_cost += routes_list[route].cost;

    unstaged_changes.insert(customer);

    return route;
}

int Solution::get_route_index(const int customer) const {
    assert(customer != instance.get_depot());
    return customers_list[customer].route_ptr;
}

int Solution::get_route_index(const int vertex, const int fallback) const {
    if (unlikely(vertex == instance.get_depot())) {
        return customers_list[fallback].route_ptr;
    } else {
        return customers_list[vertex].route_ptr;
    }
}

int Solution::get_route_load(const int route) const {
    return routes_list[route].load;
}

int Solution::get_first_route() const {
    return depot_node.first_route;
}

int Solution::get_next_route(const int route) const {
    return routes_list[route].next;
}

bool Solution::is_route_empty(const int route) const {
    return routes_list[route].load == 0;
}

int Solution::get_next_vertex(const int route, const int vertex) const {

    assert(contains_vertex(route, vertex));

    if (unlikely(vertex == instance.get_depot())) {
        return routes_list[route].first_customer;
    } else {
        return customers_list[vertex].next;
    }

}

int Solution::get_prev_vertex(const int route, const int vertex) const {

    assert(contains_vertex(route, vertex));

    if (unlikely(vertex == instance.get_depot())) {
        return routes_list[route].last_customer;
    } else {
        return customers_list[vertex].prev;
    }

}

int Solution::get_prev_vertex(const int customer) const {
    assert(customer != instance.get_depot());
    return customers_list[customer].prev;
}

int Solution::get_next_vertex(const int customer) const {
    assert(customer != instance.get_depot());
    return customers_list[customer].next;
}

void Solution::set_next_vertex_ptr(const int route, const int vertex, const int next) {
    if (unlikely(vertex == instance.get_depot())) {
        routes_list[route].first_customer = next;
    } else {
        customers_list[vertex].next = next;
    }
}

void Solution::set_prev_vertex_ptr(const int route, const int vertex, const int prev) {
    if (unlikely(vertex == instance.get_depot())) {
        routes_list[route].last_customer = prev;
    } else {
        customers_list[vertex].prev = prev;
    }
}

float Solution::remove_vertex(const int route, const int vertex) {

    if (unlikely(vertex == instance.get_depot())) {

        const auto next = routes_list[route].first_customer;
        const auto prev = routes_list[route].last_customer;

        unstaged_changes.insert(vertex);
        unstaged_changes.insert(prev);
        unstaged_changes.insert(next);
        
        set_prev_vertex_ptr(route, next, prev);
        set_next_vertex_ptr(route, prev, next);

        routes_list[route].first_customer = Solution::dummy_vertex;
        routes_list[route].last_customer = Solution::dummy_vertex;

        const auto delta = +instance.get_cost(prev, next) - instance.get_cost(prev, vertex) - instance.get_cost(vertex, next);

        routes_list[route].cost += delta;

        solution_cost += delta;

        return delta;

    } else {

        assert(contains_vertex(route, vertex));
        assert(instance.get_depot() != vertex);          // ensure that vertex is not the root of the route

        const auto next = customers_list[vertex].next;
        const auto prev = customers_list[vertex].prev;

        unstaged_changes.insert(vertex);
        unstaged_changes.insert(prev);
        unstaged_changes.insert(next);

        if (vertex == routes_list[route].first_customer) {
            routes_list[route].first_customer = next;
            set_prev_vertex_ptr(route, next, instance.get_depot());           // next might be the root of the route
        } else if (vertex == routes_list[route].last_customer) {
            routes_list[route].last_customer = prev;
            set_next_vertex_ptr(route, prev, instance.get_depot());           // prev might be the root of the route
        } else {
            customers_list[prev].next = next;              // if vertex != route.first_customer => for sure prev is not the root
            customers_list[next].prev = prev;              // if vertex != route.last_customer => for sure next is not the root
        }

        routes_list[route].load -= instance.get_demand(vertex);
        routes_list[route].size -= 1;

        const auto delta = +instance.get_cost(prev, next) - instance.get_cost(prev, vertex) - instance.get_cost(vertex, next);

        routes_list[route].cost += delta;

        solution_cost += delta;

        // reset the removed vertex
        reset_vertex(vertex);

        return delta;

    }

}

int Solution::get_first_customer(const int route) const {
    return routes_list[route].first_customer;
}

int Solution::get_last_customer(const int route) const {
    return routes_list[route].last_customer;
}

void Solution::remove_route(const int route) {
    assert(is_route_empty(route));
    release_route(route);
}

void Solution::release_route(const int route) {

    const auto prevRoute = routes_list[route].prev;
    const auto nextRoute = routes_list[route].next;

    routes_list[prevRoute].next = nextRoute;
    routes_list[nextRoute].prev = prevRoute;
    depot_node.num_routes--;

    // head remove
    if (depot_node.first_route == route) {
        depot_node.first_route = nextRoute;
    }

    reset_route(route);

    routes_pool.push(route);

}

void Solution::insert_vertex_before(const int route, const int where, const int vertex) {

    assert(where != vertex);

    if (unlikely(vertex == instance.get_depot())) {

        assert(routes_list[route].first_customer == Solution::dummy_vertex);
        assert(routes_list[route].last_customer == Solution::dummy_vertex);

        assert(where != instance.get_depot());

        const auto prev = customers_list[where].prev;
        
        unstaged_changes.insert(prev);
        unstaged_changes.insert(where);

        assert(prev != instance.get_depot());

        routes_list[route].first_customer = where;
        routes_list[route].last_customer = prev;

        customers_list[prev].next = instance.get_depot();
        customers_list[where].prev = instance.get_depot();

        const auto delta  = + instance.get_cost(prev, instance.get_depot())
                           + instance.get_cost(instance.get_depot(), where)
                           - instance.get_cost(prev, where);

        routes_list[route].cost += delta;

        solution_cost += delta;

    } else {

        assert(!is_customer_in_solution(vertex));
        assert(vertex != instance.get_depot());

        const auto prev = get_prev_vertex(route, where);
        //insert vertex between prev and next

        unstaged_changes.insert(prev);
        unstaged_changes.insert(where);
        
        // vertex for sure is not the root of route
        customers_list[vertex].next = where;
        customers_list[vertex].prev = prev;
        customers_list[vertex].route_ptr = route;

        set_next_vertex_ptr(route, prev, vertex);
        set_prev_vertex_ptr(route, where, vertex);

        const auto delta = +instance.get_cost(prev, vertex) + instance.get_cost(vertex, where) - instance.get_cost(prev, where);

        solution_cost += delta;
        routes_list[route].load += instance.get_demand(vertex);
        routes_list[route].size += 1;
        routes_list[route].cost += delta;

    }


}

void Solution::reverse_route_path(const int route, const int vertex_begin, const int vertex_end) {

    assert(vertex_begin != vertex_end);

    const auto pre = get_prev_vertex(route, vertex_begin);
    const auto stop = get_next_vertex(route, vertex_end);

    unstaged_changes.insert(pre);
    unstaged_changes.insert(stop);

    auto curr = vertex_begin;
    do {

        unstaged_changes.insert(curr);
         
        const auto prev = get_prev_vertex(route, curr);
        const auto next = get_next_vertex(route, curr);

        set_prev_vertex_ptr(route, curr, next);
        set_next_vertex_ptr(route, curr, prev);

        curr = next;

    } while (curr != stop);

    if (vertex_end == pre && vertex_begin == stop) {
        // vertex_begin and vertex_end are contiguous
    } else {

        set_prev_vertex_ptr(route, vertex_end, pre);
        set_next_vertex_ptr(route, vertex_begin, stop);
        set_next_vertex_ptr(route, pre, vertex_end);
        set_prev_vertex_ptr(route, stop, vertex_begin);

    }

    const auto delta = -instance.get_cost(pre, vertex_begin)
                       - instance.get_cost(vertex_end, stop)
                       + instance.get_cost(pre, vertex_end)
                       + instance.get_cost(stop, vertex_begin);

    solution_cost += delta;

    routes_list[route].cost += delta;

}

int Solution::append_route(const int route, const int route_to_append) {

    const auto route_end = routes_list[route].last_customer;
    const auto route_to_append_start = routes_list[route_to_append].first_customer;

    assert(route_end != instance.get_depot());
    assert(route_to_append_start != instance.get_depot());

    const auto delta = +instance.get_cost(route_end, route_to_append_start)
                       - instance.get_cost(route_end, instance.get_depot())
                       - instance.get_cost(instance.get_depot(), route_to_append_start);

    solution_cost += delta;

    customers_list[route_end].next = route_to_append_start;
    customers_list[route_to_append_start].prev = route_end;

    routes_list[route].last_customer = routes_list[route_to_append].last_customer;
    routes_list[route].load += routes_list[route_to_append].load;
    routes_list[route].size += routes_list[route_to_append].size;

    routes_list[route].cost += routes_list[route_to_append].cost + delta;

    unstaged_changes.insert(route_end);
    
    for (auto curr = route_to_append_start; curr != instance.get_depot(); curr = customers_list[curr].next) {
        customers_list[curr].route_ptr = route;

        unstaged_changes.insert(curr);
        
    }

    release_route(route_to_append);

    return route;
}

std::string Solution::to_string(const int route) const {
    std::string str;
    str += "[" + std::to_string(route) + "] ";
    str += std::to_string(instance.get_depot()) + " ";
    for (int curr = routes_list[route].first_customer; curr != instance.get_depot(); curr = customers_list[curr].next) {
        str += std::to_string(curr) + " ";
    }
    str += std::to_string(instance.get_depot());
    return str;
}

void Solution::print(const int route) const {
    std::cout << to_string(route) << " (" << get_route_load(route) << ") " << get_route_cost(route) << "\n";
}

void Solution::print() const {
    for (auto route = depot_node.first_route; route != Solution::dummy_route; route = routes_list[route].next) {
        print(route);
    }
    std::cout << "Solution cost = " << solution_cost << "\n";
}

void Solution::update_cumulative_route_loads(const int route) {

    assert(!is_route_empty(route));

    auto prev = routes_list[route].first_customer;

    customers_list[prev].load_before = instance.get_demand(prev);
    customers_list[prev].load_after = routes_list[route].load;

    auto curr = customers_list[prev].next;

    while (curr != instance.get_depot()) {

        customers_list[curr].load_before = customers_list[prev].load_before + instance.get_demand(curr);
        customers_list[curr].load_after = customers_list[prev].load_after - instance.get_demand(prev);

        prev = curr;
        curr = customers_list[curr].next;
    }


}

int Solution::get_route_load_before_included(const int customer) const {
    assert(customer != instance.get_depot());
    return customers_list[customer].load_before;
}

int Solution::get_route_load_after_included(const int customer) const {
    assert(customer != instance.get_depot());
    return customers_list[customer].load_after;
}

bool Solution::is_route_in_solution(const int route) const {
    return routes_list[route].first_customer != instance.get_depot() &&
           routes_list[route].last_customer != instance.get_depot();
}

bool Solution::is_customer_in_solution(const int customer) const {
    assert(customer != instance.get_depot());
    return customers_list[customer].route_ptr != Solution::dummy_route;
}

bool Solution::contains_vertex(const int route, const int vertex) const {
    assert(vertex >= instance.get_vertices_begin() && vertex < instance.get_vertices_end() && route >= 0 && route < max_number_routes);
    return customers_list[vertex].route_ptr == route || vertex == instance.get_depot();
}

bool Solution::is_feasible() {
    bool partial = false;
    bool error = false;
    std::vector<int> unRoutedCustomers;
    std::string message;

    /**
     * Check routes structure
     */
    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {

        if (customers_list[i].prev != Solution::dummy_vertex) {
            if (customers_list[i].prev == instance.get_depot() &&
                routes_list[customers_list[i].route_ptr].first_customer != i) {
                std::cerr << "[ error ]\tcustomer " << i << " from route " << customers_list[i].route_ptr
                          << " has\n\t\t\t"
                          << "{ depot }" << " <--prev-- { " << i << " }\n\t\t\t but " << i
                          << " is not START of the route.\n";
                error = true;
            }
            if (customers_list[i].next == instance.get_depot() &&
                routes_list[customers_list[i].route_ptr].last_customer != i) {
                std::cerr << "[ error ]\tcustomer " << i << " from route " << customers_list[i].route_ptr
                          << " has\n\t\t\t{ "
                          << i << " } --next--> " << "{ depot }" << "\n\t\t\t but " << i
                          << " is not END of the route.\n";
                error = true;
            }
            if (customers_list[i].prev != instance.get_depot() && customers_list[customers_list[i].prev].next != i) {
                std::cerr << "[ error ] next[prev[ " << i << " ]] != " << i << ".\n";
                error = true;
            }
            if (customers_list[i].next != instance.get_depot() && customers_list[customers_list[i].next].prev != i) {
                std::cerr << "[ error ] prev[next[ " << i << " ]] != " << i << ".\n";
                error = true;
            }
        } else {
            partial = true;
            unRoutedCustomers.push_back(i);
        }
        if ((customers_list[i].next == Solution::dummy_vertex && customers_list[i].prev != Solution::dummy_vertex) ||
            (customers_list[i].prev == Solution::dummy_vertex && customers_list[i].next != Solution::dummy_vertex)) {
            std::cerr << "[ error ] Node " << i << " not completely removed.\n";
            error = true;
        }
    }

    std::vector<int> prevCount(instance.get_vertices_num(), 0);
    std::vector<int> nextCount(instance.get_vertices_num(), 0);

    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        if (customers_list[i].prev == Solution::dummy_vertex ||
            customers_list[i].next == Solution::dummy_vertex) { continue; }
        prevCount[customers_list[i].prev]++;
        nextCount[customers_list[i].next]++;
    }

    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        if (prevCount[i] > 1) {
            std::cerr << "[ error ] Node " << i << " is prev " << prevCount[i] << " times.\n";
            error = true;
        }
        if (nextCount[i] > 1) {
            std::cerr << "[ error ] Node " << i << " is next " << nextCount[i] << " times.\n";
            error = true;
        }
    }

    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        if (customers_list[i].route_ptr == Solution::dummy_route) { continue; }
        if (routes_list[customers_list[i].route_ptr].first_customer == instance.get_depot()) {
            std::cerr << "[ error ] Node " << i << " belongs to route " << customers_list[i].route_ptr
                      << " which has no starting point.\n";
            error = true;
        }

    }

    for(auto route = get_first_route(); route != dummy_route; route = get_next_route(route)) {
        for(auto customer = get_first_customer(route); customer != instance.get_depot(); customer = get_next_vertex(customer)) {
            if(customers_list[customer].route_ptr != route) {
                std::cerr << "[ error ] Customer " << customer << " is served in route " << route << " but its route pointer points to " << customers_list[customer].route_ptr << "\n";
                error = true;
            }
        }
    }


    /**
     * Check routes capacity
     */

    for (int l = 0; l < instance.get_vertices_num(); l++) {
        int qSum = 0;
        for (int i = routes_list[l].first_customer; i != instance.get_depot(); i = customers_list[i].next) {
            qSum += instance.get_demand(i);
        }

        if (routes_list[l].load != qSum) {
            std::cerr << "[ error ] Route " << l << " ( " << to_string(l) << " ) computed ( " << qSum
                      << " ) and stored ( " << routes_list[l].load << ") load inconsistency.\n";
            error = true;
        }

        if(routes_list[l].load > instance.get_vehicle_capacity()) {
            std::cerr << "[ error ] Route " << l << " ( " << to_string(l) << " ) load ( " << qSum
                      << " ) is greater than vehicle capacity ( " << instance.get_vehicle_capacity() << ").\n";
            error = true;
        }

    }


    for (int i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
        if (customers_list[i].route_ptr == Solution::dummy_route) { continue; }
        const int route = customers_list[i].route_ptr;
        bool found = false;
        for (int curr = routes_list[route].first_customer; curr != instance.get_depot(); curr = customers_list[curr].next) {
            if (curr == i) {
                found = true;
                break;
            }
        }
        if (!found) {
            error = true;
            std::cerr << "[ error ] ";
            std::cerr << "Customer " << i << " should be from route " << route << " but cannot be found in route "
                      << route << ": { " << to_string(route) << " }.\n";
        }
    }

    double cost = 0;
    for (auto route = get_first_route(); route != Solution::dummy_route; route = get_next_route(route)) {
        auto route_cost = 0.0;
        route_cost += instance.get_cost(instance.get_depot(), get_first_customer(route));
        for (auto curr = get_first_customer(route); curr != instance.get_depot(); curr = get_next_vertex(curr)) {
            const auto next = get_next_vertex(curr);
            route_cost += instance.get_cost(curr, next);
        }
        cost+=route_cost;

        if (fabs(route_cost - get_route_cost(route)) > 0.1f) {
            error = true;
            std::cerr << "[ error ]\t";
            std::cerr.precision(6);
            std::cerr << std::fixed;
            std::cerr << "Route cost mismatch. Computed = " << route_cost << " and stored = " << get_route_cost(route) << ". Difference = "
                      << route_cost - get_route_cost(route) << "\n";
        }

    }
    if (fabs(cost - get_cost()) > 0.1) {
        error = true;
        std::cerr << "[ error ]\t";
        std::cerr.precision(6);
        std::cerr << std::fixed;
        std::cerr << "Solution cost mismatch. Computed = " << cost << " and stored = " << get_cost() << ". Difference = "
                  << cost - get_cost() << "\n";
    }

    for (auto route = get_first_route(); route != Solution::dummy_route; route = get_next_route(route)) {
        if (routes_list[route].last_customer == Solution::dummy_vertex ||
            routes_list[route].first_customer == Solution::dummy_vertex) {
            error = true;
            std::cerr << "[ error ]\t";
            std::cerr.precision(6);
            std::cerr << std::fixed;
            std::cerr << "Route " << route << " end-points are not properly set. First vertex = "
                      << routes_list[route].first_customer << ", last vertex = " << routes_list[route].last_customer;
        }
    }

    auto computed_route = 0.0f;
    for(auto route = get_first_route(); route != Solution::dummy_route; route = get_next_route(route)) {

        computed_route += get_route_cost(route);

        if(is_route_empty(route)) {
            error = true;
            std::cerr << "[ error ]\t";
            std::cerr.precision(6);
            std::cerr << std::fixed;
            std::cerr << "Route " << route << " is empty but in solution!\n";
        }

    }
    if (std::fabs(computed_route - get_cost()) > 0.1) {
        error = true;
        std::cerr << "[ error ]\t";
        std::cerr.precision(6);
        std::cerr << std::fixed;
        std::cerr << "Solution cost mismatch. Computed from routes = " << computed_route << " and stored = " << get_cost() << ". Difference = "
                  << cost - get_cost() << "\n";
    }


    if (partial) {
        std::cerr << "[ warning ]\tThe solution is incomplete.\n\t\t\tUnrouted customers = { ";
        for (int i :  unRoutedCustomers) { std::cerr << i << " "; }
        std::cerr << "}.\n";
    }

    return !error;

}

int Solution::get_route_size(const int route) const {
    return routes_list[route].size;
}

float Solution::get_route_cost(const int route) const {
    return routes_list[route].cost;
}












