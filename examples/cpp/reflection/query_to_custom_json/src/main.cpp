#include <query_to_custom_json.h>
#include <iostream>

// Same example as query_to_json, but with customized serializer parameters

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Mass {
    float value;
};

int main(int, char *[]) {
    flecs::world ecs;

    // Register components with reflection data
    ecs.component<Position>()
        .member<float>("x")
        .member<float>("y");

    ecs.component<Velocity>()
        .member<float>("x")
        .member<float>("y");

    ecs.component<Mass>()
        .member<float>("value");

    ecs.entity("a").set<Position>({10, 20}).set<Velocity>({1, 2});
    ecs.entity("b").set<Position>({20, 30}).set<Velocity>({2, 3});
    ecs.entity("c").set<Position>({30, 40}).set<Velocity>({3, 4}).set<Mass>({10});
    ecs.entity("d").set<Position>({30, 40}).set<Velocity>({4, 5}).set<Mass>({20});

    // Query for components
    flecs::query<Position, const Velocity> q = 
        ecs.query<Position, const Velocity>();

    // Serialize query to JSON. Only serialize entity names.
    flecs::iter_to_json_desc_t desc = ECS_ITER_TO_JSON_INIT;
    desc.serialize_fields = false;
    std::cout << q.iter().to_json(&desc).c_str() << "\n";

    // Iterator returns 2 sets of results, one for each table.

    // {
    //   "results": [
    //     {
    //       "name": "a"
    //     },
    //     {
    //       "name": "b"
    //     },
    //     {
    //       "name": "c"
    //     },
    //     {
    //       "name": "d"
    //     }
    //   ]
    // }
}
