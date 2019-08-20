# cvrp-lib
An entry level set of classes for fast-prototyping CVRP algorithms.

**Table of contents**

*   [Benchmark instances](#benchmark-instances)
*   [Project structure](#project-structure)
*   [The `AbstractInstance`, `Instance` and `SubInstance` classes](#the-abstractinstance-instance-and-subinstance-classes)
*   [The `Solution` class](#the-solution-class)
*   [The Clarke and Wright savings algorithm](#the-clarke-and-wright-savings-algorithm)
*   [License](#license)

#### Benchmark instances

The most important benchmark instances used in the literature can be downloaded from the [CVRPLIB](http://vrp.galgos.inf.puc-rio.br/index.php/en/) website. I've been mainly working with the most recent Uchoa et al. (2014) and Arnold, Gendreau and Sörensen (2017) datasets.

#### Project structure

The project consists of a set of cmake sub-projects. In particular, the main ones are contained in the `vrp-instance` and `vrp-solution` directories. The `vrp-instance` directory contains a set of classes to handle CVRP instances. Following the same philosophy, the `vrp-solution` directory provides a set of classes to represent and manage CVRP solutions. The usage of those classes is the main topic of this document and it is described in the next sections.

#### The `AbstractInstance`, `Instance` and `SubInstance` classes

The `AbstractInstance` class defines the interface describing the functionalities an Instance-like class must implement. Both `Instance` and `SubInstance` inherit from `AbstractInstance` providing a concrete implementation for the required functionalities.

The `Instance` class represents an instance entity and can be easily be instantiated as follows

```cpp
auto instance = Instance("path-to-file");
```

Several parsers are automatically tried in order to parse the file identified by the given path. One can check whether the instance file has been successfully parsed by inspecting the result of the `is_valid` method

```cpp
if(!instance.is_valid()) {
    // not able to parse the instance
}
```

After a successful parsing phase, a set of data structures have been initialized within the `Instance` object and all the available methods can be safely used. Note that using any method except `is_valid` when the instance is not valid will result in undefined behaviour.

As an example, it is possible to iterate through all the vertices (depot and customers) and access some instance information as follows


```cpp
for(auto i = instance.vertices_begin(); i < instance.vertices_end(); i++) {
    if(i == instance.get_depot()) {
        // this is the depot
    } else {
        // this is a customer
    }
    for(auto n = 0; n < 25; n++) {
        // to access the 25 nearest neighbors of i
        // in ascending order of distance from i
        auto j = instance.get_neighbors_of(i)[n];
    }
}
```


Otherwise one can iterate just over the customers


```cpp
for(auto i = instance.customers_begin(); i < instance.customers_end(); i++) {
    auto demand = instance.get_demand(i);
}
```


The `SubInstance` class decorates an `AbstractInstance` object by offering a consistent partial view over it. It is a simple tool that can greatly simplify instance decomposition. Being a `SubInstance` an `AbstractInstance`, the decomposition process can be executed recursively several times. The class initialization is quite memory efficient and does not duplicate unnecessary data structures. In fact, it almost always access the original data structures by using appropriate mappings. The only exception to keep in mind are the arrays identifying the neighbors of a given vertex that are instead recomputed during the `SubInstance` object initialization. Moreover, one should take into account some minor overhead due to the indirection in accessing the data which is linear in the number of recursive decompositions.

The `SubInstance` class can be used as follows


```cpp
auto instance = Instance("path-to-file");

// define an array containing a subset of customers of the original instance
// the depot is implicitly considered and must not be inserted in the array
auto subset = std::vector<int>({ ... })

auto sub_instance = SubInstance(instance, subset);
// sub_instance now provides all the functionalities an AbstractInstance must define
```


#### The `Solution` class

The `Solution` class represents a solution entity and provides a set of functionalities to manage it. The internal representation mainly consists of the classical pair of previous and next arrays with some additional fancy stuff. Note that this may not be the best representation to work with when developing population-based algorithms or when it is necessary to support a broad range of VRP variants. Apart from that, it is quite efficient when dealing with the vanilla CVRP.

The `Solution` class can be easily instantiated by passing any reference to an `AbstractInterface` object. Note that in my code the ownership is never transferred when references are passed as parameters. The only exceptions are the `copy` methods that generate a deep copy of the object making the copied object independent from the original one but maintaining a dependency from the the references originally required by the original object. As an example consider the following `Solution` object instantiation and subsequent copy


```cpp
auto instance = Instance("path-to-file");
auto solution = Solution(instance);
auto best_solution = Solution(solution);

// the previous line is semantically the same as the following
auto best_solution = Solution(instance);
best_solution = solution;
```


The objects `solution` and `best_solution` defines two independent `Solution` objects without shared internal data structures except from the `Instance` object which is shared for efficiency purposes. Thus, the `instance` object must remain alive for both solutions lifespans.

The `Solution` class provides several methods to manage a solution entity. In the following there are some hints on how to perform the basic operations.

As a final note before diving into the code, the correct usage of the `Solution` class is not enforced with runtime checks to avoid significant overhead penalties. However, several sanity checks are enabled as assertions when the code is run in debug mode. My obvious suggestion is thus to first run any code in debug mode before switching to release.

In the following some simple use cases of the `Solution` class


```cpp
auto instance = Instance("path-to-file");

// initialize an empty solution
auto solution = Solution(instance); // you can also pass a SubInstance object

// check whether the solution is CVRP feasible (if not the problems are printed out)
auto feasible = solution.is_feasible();

// build a route for each customer
for(auto i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
    auto route = solution.build_one_customer_route(i);
}

// now the solution should be feasible
auto feasible = solution.is_feasible();

// print the solution cost
std::cout << "Solution cost = " << solution.get_cost() << "\n";

// print the solution itself
solution.print();

// number of routes
std::cout << "Routes num = " << solution.get_routes_num() << "\n";

// iterate through all the solution's routes
for(auto route = solution.get_first_route(); route != Solution::dummy_route; route = solution.get_next_route(route)) {

    std::cout << "Customers in route = " << solution.get_route_size(route) << "\n";
    std::cout << "Route load = " << solution.get_route_load(route) << "\n";
    std::cout << "Route cost = " << solution.get_route_cost(route) << "\n";
    solution.print(route);

    // iterate through the customers of a route
    for(auto i = solution.get_first_customer(route); i != instance.get_depot(); i = solution.get_next_vertex(i)){
        ...
    }
}

// check whether a customer is served by the solution
auto served = solution.is_customer_in_solution(customer);

// retrieve the route index
auto route = solution.get_route_index(customer);

// move a customer from the original position into another one
solution.remove_vertex(route, customer)
// empty routes must be manually removed to avoid undefined behaviour
if(solution.is_route_empty(route)) {
    solution.remove_route(route);
}
// insert customer before where in another_route
solution.insert_vertex_before(another_route, where, customer);
```


The depot is a quite special node and must be treated with extreme care. For example, asking what is the route index that serves the depot does not make sense since the depot is served in all routes. All of the methods that require a `route` parameter along with the `vertex` one can be safely used with the depot. As an extreme case consider the removal of the depot from a given route (which is not very common but may be useful to position the depot in a better place within the route itself).

```cpp
solution.remove_vertex(route, instance.get_depot());
// now route is in an inconsistent state since it is missing the depot!
// and... it is better to recover a consistent route state as soon as possible
solution.insert_vertex_before(route, where, instance.get_depot());
```

Each `Solution` object keeps track of the changes that happen within itself in terms of what are the nodes involved in the executed operations. This may be used to localize some procedures (e.g. local search ones) on a small subset of the nodes that recently changed. The `get_unstaged_changes` method returns the set of recently modified vertices. Keep in mind that it does not identify sequences of operations that might revert to the original solution state (e.g. removal and addition of the same vertex in the same position, the vertex is considered as changed). Note that it is not safe to perform any solution's changing operation while iterating the returned reference. The set iterator would remain valid but some elements might be overlooked. It is thus better to avoid this usage. In case, it was necessary to iterate and edit, one should deep copy the current set and use that for iterating while keeping in mind that the real set of modified vertices could change due to the newly performed edits. The `commit` method clears the changes resetting the set of vertices returned by `get_unstaged_changes`.

```cpp
auto vertices = solution.get_unstaged_changes();
...
solution.commit();
```

#### The Clarke and Wright savings algorithm

The implementation of the basic Clarke and Wright savings algorithm becomes trivial once the `Solution` class is available. A possible code might be as follows

```cpp
solution.reset();

for (auto i = instance.get_customers_begin(); i < instance.get_customers_end(); i++) {
    solution.build_one_customer_route(i);
}

struct Saving {
    int i;
    int j;
    float value;
};

const auto savings_num = instance.get_customers_num() * (instance.get_customers_num() - 1) / 2;
auto savings = std::vector<Saving>();
savings.reserve(savings_num);

for(auto i = instance.get_customers_begin(); i < instance.get_customers_end() - 1; i++) {
    for(auto j = i+1; j < instance.get_customers_end(); j++) {

        const auto value = + instance.get_cost(i, instance.get_depot())
                           + instance.get_cost(instance.get_depot(), j)
                           - instance.get_cost(i, j);

        savings.push_back({i, j, value});

    }
}

std::sort(savings.begin(), savings.end(), [](const Saving &a, const Saving &b) { return a.value > b.value; });

for (auto &saving : savings) {

    const auto i = saving.i;
    const auto j = saving.j;

    const auto i_route = solution.get_route_index(i);
    const auto j_route = solution.get_route_index(j);

    if (i_route == j_route) { continue; }

    if (solution.get_last_customer(i_route) == i && solution.get_first_customer(j_route) == j &&
        solution.get_route_load(i_route) + solution.get_route_load(j_route) <= instance.get_vehicle_capacity()) {

        solution.append_route(i_route, j_route);

    } else if (solution.get_last_customer(j_route) == j && solution.get_first_customer(i_route) == i &&
               solution.get_route_load(i_route) + solution.get_route_load(j_route) <= instance.get_vehicle_capacity()) {

        solution.append_route(j_route, i_route);

    }

}

solution.is_feasible();
```

Note that the `Solution` base class provides itself a slightly fancier implementation for the Clarke and Wright savings algorithm.


#### License

GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
