#include <api.h>

void Switch_get_case_empty() {
    ecs_world_t *world = ecs_init();

    ECS_TYPE(world, Type, 0);

    ecs_entity_t e = ecs_new(world, 0);
    test_assert(e != 0);

    ecs_entity_t case_id = ecs_get_case(world, e, Type);
    test_int(case_id, 0);

    ecs_fini(world);
}

void Switch_get_case_no_switch() {
    ecs_world_t *world = ecs_init();
    
    ECS_COMPONENT(world, Position);

    ECS_TYPE(world, Type, 0);    

    ecs_entity_t e = ecs_new(world, Position);
    test_assert(e != 0);

    ecs_entity_t case_id = ecs_get_case(world, e, Type);
    test_int(case_id, 0);

    ecs_fini(world);
}

void Switch_get_case_unset() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Type, Walking, Running, Jumping);

    ecs_entity_t e = ecs_new_w_entity(world, ECS_SWITCH | Type);
    test_assert(e != 0);

    ecs_entity_t case_id = ecs_get_case(world, e, Type);
    test_int(case_id, 0);

    ecs_fini(world);
}

void Switch_get_case_set() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Type, Walking, Running, Jumping);

    ecs_entity_t e = ecs_new_w_entity(world, ECS_SWITCH | Type);
    test_assert(e != 0);

    ecs_add_entity(world, e, ECS_CASE | Walking);
    test_assert( ecs_has_entity(world, e, ECS_CASE | Walking));

    ecs_entity_t case_id = ecs_get_case(world, e, Type);
    test_int(case_id, Walking);

    ecs_fini(world);
}

void Switch_get_case_change() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Type, Walking, Running, Jumping);

    ecs_entity_t e = ecs_new_w_entity(world, ECS_SWITCH | Type);
    test_assert(e != 0);

    ecs_add_entity(world, e, ECS_CASE | Walking);
    test_assert( ecs_has_entity(world, e, ECS_CASE | Walking));

    ecs_add_entity(world, e, ECS_CASE | Running);
    test_assert( !ecs_has_entity(world, e, ECS_CASE | Walking));
    test_assert( ecs_has_entity(world, e, ECS_CASE | Running));

    ecs_entity_t case_id = ecs_get_case(world, e, Type);
    test_int(case_id, Running);

    ecs_fini(world);
}

void Switch_new_w_type() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Movement, Walking, Running, Jumping);
    ECS_TYPE(world, Type, SWITCH | Movement, CASE | Running);

    ecs_entity_t e = ecs_new(world, Type);
    test_assert(e != 0);

    test_assert( ecs_has_entity(world, e, ECS_CASE | Running));
    ecs_entity_t case_id = ecs_get_case(world, e, Movement);
    test_int(case_id, Running);

    ecs_fini(world);
}

void Switch_add_w_type() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Movement, Walking, Running, Jumping);
    ECS_TYPE(world, Type, SWITCH | Movement, CASE | Running);

    ecs_entity_t e = ecs_new(world, 0);
    test_assert(e != 0);

    ecs_add(world, e, Type);

    test_assert( ecs_has_entity(world, e, ECS_CASE | Running));
    ecs_entity_t case_id = ecs_get_case(world, e, Movement);
    test_int(case_id, Running);

    ecs_fini(world);
}

void Switch_bulk_new_w_type() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Movement, Walking, Running, Jumping);
    ECS_TYPE(world, Type, SWITCH | Movement, CASE | Running);

    ecs_entity_t e = ecs_bulk_new(world, Type, 100);
    test_assert(e != 0);

    ecs_add(world, e, Type);

    test_assert( ecs_has_entity(world, e, ECS_CASE | Running));
    ecs_entity_t case_id = ecs_get_case(world, e, Movement);
    test_int(case_id, Running);

    ecs_fini(world);
}

void Switch_3_entities_same_case() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_ENTITY(world, e1, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e3, SWITCH | Movement, CASE | Running);

    test_assert( ecs_has_entity(world, e1, ECS_CASE | Running));
    test_assert( ecs_has_entity(world, e2, ECS_CASE | Running));
    test_assert( ecs_has_entity(world, e3, ECS_CASE | Running));

    test_int(ecs_get_case(world, e1, Movement), Running);
    test_int(ecs_get_case(world, e2, Movement), Running);
    test_int(ecs_get_case(world, e3, Movement), Running);

    ecs_fini(world);
}

void Switch_2_entities_1_change_case() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_ENTITY(world, e1, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, SWITCH | Movement, CASE | Running);

    ecs_add_entity(world, e2, ECS_CASE | Jumping);
    test_assert( ecs_has_entity(world, e1, ECS_CASE | Running));
    test_assert( ecs_has_entity(world, e2, ECS_CASE | Jumping));

    test_int(ecs_get_case(world, e1, Movement), Running);
    test_int(ecs_get_case(world, e2, Movement), Jumping); 

    ecs_fini(world);
}

void Switch_3_entities_change_case() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);

    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_ENTITY(world, e1, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e3, SWITCH | Movement, CASE | Running);

    ecs_add_entity(world, e1, ECS_CASE | Walking);
    test_assert( ecs_has_entity(world, e1, ECS_CASE | Walking));
    test_assert( ecs_has_entity(world, e2, ECS_CASE | Running));
    test_assert( ecs_has_entity(world, e3, ECS_CASE | Running));

    test_int(ecs_get_case(world, e1, Movement), Walking);
    test_int(ecs_get_case(world, e2, Movement), Running);
    test_int(ecs_get_case(world, e3, Movement), Running);

    ecs_add_entity(world, e2, ECS_CASE | Jumping);
    test_assert( ecs_has_entity(world, e1, ECS_CASE | Walking));
    test_assert( ecs_has_entity(world, e2, ECS_CASE | Jumping));
    test_assert( ecs_has_entity(world, e3, ECS_CASE | Running));

    test_int(ecs_get_case(world, e1, Movement), Walking);
    test_int(ecs_get_case(world, e2, Movement), Jumping);
    test_int(ecs_get_case(world, e3, Movement), Running); 

    ecs_add_entity(world, e3, ECS_CASE | Walking);
    test_assert( ecs_has_entity(world, e1, ECS_CASE | Walking));
    test_assert( ecs_has_entity(world, e2, ECS_CASE | Jumping));
    test_assert( ecs_has_entity(world, e3, ECS_CASE | Walking));

    test_int(ecs_get_case(world, e1, Movement), Walking);
    test_int(ecs_get_case(world, e2, Movement), Jumping);
    test_int(ecs_get_case(world, e3, Movement), Walking);        

    ecs_fini(world);
}

static
void MatchSwitch(ecs_iter_t *it) {
    ecs_entity_t *movement = ecs_column(it, ecs_entity_t, 1);
    test_assert(movement != NULL);
    probe_system(it);
}

void Switch_query_switch() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, MatchSwitch, EcsOnUpdate, SWITCH | Movement);

    ECS_ENTITY(world, e1, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, SWITCH | Movement, CASE | Walking);
    ECS_ENTITY(world, e3, SWITCH | Movement, CASE | Running);

    Probe ctx = {0};
    ecs_set_context(world, &ctx);

    ecs_progress(world, 1);

    test_int(ctx.count, 3);
    test_int(ctx.invoked, 1);
    test_int(ctx.system, MatchSwitch);
    test_int(ctx.column_count, 1);
    test_null(ctx.param);

    test_int(ctx.e[0], e1);
    test_int(ctx.e[1], e2);
    test_int(ctx.e[2], e3);
    test_int(ctx.c[0][0], ECS_SWITCH | Movement);
    test_int(ctx.s[0][0], 0);

    ecs_fini(world);
}

void Switch_query_1_case_1_type() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, MatchSwitch, EcsOnUpdate, CASE | Running);

    ECS_ENTITY(world, e1, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, SWITCH | Movement, CASE | Walking);
    ECS_ENTITY(world, e3, SWITCH | Movement, CASE | Running);

    Probe ctx = {0};
    ecs_set_context(world, &ctx);

    ecs_progress(world, 1);

    test_int(ctx.count, 2);
    test_int(ctx.invoked, 2);
    test_int(ctx.system, MatchSwitch);
    test_int(ctx.column_count, 1);
    test_null(ctx.param);

    test_int(ctx.e[0], e3);
    test_int(ctx.e[1], e1);
    test_int(ctx.c[0][0], ECS_CASE | Running);
    test_int(ctx.s[0][0], 0);

    ecs_fini(world);
}

void Switch_query_1_case_2_types() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, MatchSwitch, EcsOnUpdate, CASE | Running);

    ECS_ENTITY(world, e1, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, SWITCH | Movement, CASE | Walking);
    ECS_ENTITY(world, e3, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e4, SWITCH | Movement, CASE | Walking, Position);
    ECS_ENTITY(world, e5, SWITCH | Movement, CASE | Running, Position);
    ECS_ENTITY(world, e6, SWITCH | Movement, CASE | Walking, Position);
    ECS_ENTITY(world, e7, SWITCH | Movement, CASE | Running, Position);

    Probe ctx = {0};
    ecs_set_context(world, &ctx);

    ecs_progress(world, 1);

    test_int(ctx.count, 4);
    test_int(ctx.invoked, 4);
    test_int(ctx.system, MatchSwitch);
    test_int(ctx.column_count, 1);
    test_null(ctx.param);

    test_int(ctx.e[0], e3);
    test_int(ctx.e[1], e1);
    test_int(ctx.e[2], e7);
    test_int(ctx.e[3], e5);    
    test_int(ctx.c[0][0], ECS_CASE | Running);
    test_int(ctx.s[0][0], 0);

    ecs_fini(world);
}

void Switch_query_2_cases_1_type() {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_TAG(world, Front);
    ECS_TAG(world, Back);
    ECS_TAG(world, Left);
    ECS_TAG(world, Right);
    ECS_TYPE(world, Direction, Front, Back, Left, Right);

    ECS_SYSTEM(world, MatchSwitch, EcsOnUpdate, CASE | Running, CASE | Front);

    ECS_ENTITY(world, e1, 
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e2, 
        SWITCH | Movement, CASE | Walking,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e3, 
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Back);
    ECS_ENTITY(world, e4, 
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e5, 
        SWITCH | Movement, CASE | Walking,
        SWITCH | Direction, CASE | Front);

    Probe ctx = {0};
    ecs_set_context(world, &ctx);

    ecs_progress(world, 1);

    test_int(ctx.count, 2);
    test_int(ctx.invoked, 2);
    test_int(ctx.system, MatchSwitch);
    test_int(ctx.column_count, 2);
    test_null(ctx.param);

    test_int(ctx.e[0], e4);
    test_int(ctx.e[1], e1);
    test_int(ctx.c[0][0], ECS_CASE | Running);
    test_int(ctx.c[0][1], ECS_CASE | Front);
    test_int(ctx.s[0][0], 0);

    ecs_fini(world);
}

void Switch_query_2_cases_2_types() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_TAG(world, Front);
    ECS_TAG(world, Back);
    ECS_TAG(world, Left);
    ECS_TAG(world, Right);
    ECS_TYPE(world, Direction, Front, Back, Left, Right);

    ECS_SYSTEM(world, MatchSwitch, EcsOnUpdate, CASE | Running, CASE | Front);

    ECS_ENTITY(world, e1, 
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e2, 
        SWITCH | Movement, CASE | Walking,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e3, 
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Back);
    ECS_ENTITY(world, e4, 
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e5, 
        SWITCH | Movement, CASE | Walking,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e6, 
        Position,
        SWITCH | Movement, CASE | Walking,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e7, 
        Position,
        SWITCH | Movement, CASE | Running,
        SWITCH | Direction, CASE | Front);
    ECS_ENTITY(world, e8, 
        Position,
        SWITCH | Movement, CASE | Walking,
        SWITCH | Direction, CASE | Front);

    Probe ctx = {0};
    ecs_set_context(world, &ctx);

    ecs_progress(world, 1);

    test_int(ctx.count, 3);
    test_int(ctx.invoked, 3);
    test_int(ctx.system, MatchSwitch);
    test_int(ctx.column_count, 2);
    test_null(ctx.param);

    test_int(ctx.e[0], e4);
    test_int(ctx.e[1], e1);
    test_int(ctx.e[2], e7);
    test_int(ctx.c[0][0], ECS_CASE | Running);
    test_int(ctx.c[0][1], ECS_CASE | Front);
    test_int(ctx.s[0][0], 0);

    ecs_fini(world);
}

static
void AddSwitch(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    ecs_entity_t movement = ecs_column_entity(it, 2);

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_add_entity(world, it->entities[i], ECS_SWITCH | movement);
        test_assert(ecs_has_entity(world, it->entities[i], ECS_SWITCH | movement));
    }
}

void Switch_add_switch_in_stage() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, AddSwitch, EcsOnUpdate, Position, :Movement);

    ECS_ENTITY(world, e1, Position);
    ECS_ENTITY(world, e2, Position);
    ECS_ENTITY(world, e3, Position);

    ecs_progress(world, 0);

    test_assert(ecs_has_entity(world, e1, ECS_SWITCH | Movement));
    test_assert(ecs_has_entity(world, e2, ECS_SWITCH | Movement));
    test_assert(ecs_has_entity(world, e3, ECS_SWITCH | Movement));

    ecs_fini(world);
}

static
void SetCase(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    ecs_entity_t sw_case = ecs_column_entity(it, 2);

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_add_entity(world, it->entities[i], ECS_CASE | sw_case);
        test_assert(ecs_has_entity(world, it->entities[i], ECS_CASE | sw_case));
    }
}

void Switch_add_case_in_stage() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, SetCase, EcsOnUpdate, Position, :Walking);

    ECS_ENTITY(world, e1, Position, SWITCH | Movement);
    ECS_ENTITY(world, e2, Position, SWITCH | Movement);
    ECS_ENTITY(world, e3, Position, SWITCH | Movement);

    ecs_progress(world, 0);

    test_assert(ecs_has_entity(world, e1, ECS_CASE | Walking));
    test_assert(ecs_has_entity(world, e2, ECS_CASE | Walking));
    test_assert(ecs_has_entity(world, e3, ECS_CASE | Walking));

    ecs_fini(world);
}

void Switch_change_case_in_stage() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, SetCase, EcsOnUpdate, Position, :Walking);

    ECS_ENTITY(world, e1, Position, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e2, Position, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e3, Position, SWITCH | Movement, CASE | Running);

    ecs_progress(world, 0);

    test_assert(!ecs_has_entity(world, e1, ECS_CASE | Running));
    test_assert(!ecs_has_entity(world, e2, ECS_CASE | Running));
    test_assert(!ecs_has_entity(world, e3, ECS_CASE | Running));

    test_assert(ecs_has_entity(world, e1, ECS_CASE | Walking));
    test_assert(ecs_has_entity(world, e2, ECS_CASE | Walking));
    test_assert(ecs_has_entity(world, e3, ECS_CASE | Walking));

    ecs_fini(world);
}

void Switch_change_one_case_in_stage() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, SetCase, EcsOnUpdate, Position, :Jumping, CASE | Walking);

    ECS_ENTITY(world, e0, Position, SWITCH | Movement, CASE | Jumping);
    ECS_ENTITY(world, e1, Position, SWITCH | Movement, CASE | Walking);
    ECS_ENTITY(world, e2, Position, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e3, Position, SWITCH | Movement, CASE | Walking);
    ECS_ENTITY(world, e4, Position, SWITCH | Movement, CASE | Running);
    ECS_ENTITY(world, e5, Position, SWITCH | Movement, CASE | Jumping);

    ecs_progress(world, 0);

    test_assert(ecs_has_entity(world, e0, ECS_CASE | Jumping));
    test_assert(ecs_has_entity(world, e1, ECS_CASE | Jumping));
    test_assert(ecs_has_entity(world, e2, ECS_CASE | Running));
    test_assert(ecs_has_entity(world, e3, ECS_CASE | Jumping));
    test_assert(ecs_has_entity(world, e4, ECS_CASE | Running));
    test_assert(ecs_has_entity(world, e5, ECS_CASE | Jumping));

    ecs_fini(world);
}

static
void RemoveSwitch(ecs_iter_t *it) {
    ecs_world_t *world = it->world;
    ecs_entity_t movement = ecs_column_entity(it, 1);

    int i;
    for (i = 0; i < it->count; i ++) {
        ecs_remove_entity(world, it->entities[i], ECS_SWITCH | movement);
        test_assert(!ecs_has_entity(world, it->entities[i], ECS_SWITCH | movement));
    }
}

void Switch_remove_switch_in_stage() {
    ecs_world_t *world = ecs_init();

    ECS_COMPONENT(world, Position);

    ECS_TAG(world, Walking);
    ECS_TAG(world, Running);
    ECS_TAG(world, Jumping);
    ECS_TYPE(world, Movement, Walking, Running, Jumping);

    ECS_SYSTEM(world, RemoveSwitch, EcsOnUpdate, SWITCH | Movement);

    ECS_ENTITY(world, e1, Position, SWITCH | Movement);
    ECS_ENTITY(world, e2, Position, SWITCH | Movement);
    ECS_ENTITY(world, e3, Position, SWITCH | Movement);

    ecs_progress(world, 0);

    test_assert(!ecs_has_entity(world, e1, ECS_SWITCH | Movement));
    test_assert(!ecs_has_entity(world, e2, ECS_SWITCH | Movement));
    test_assert(!ecs_has_entity(world, e3, ECS_SWITCH | Movement));

    ecs_fini(world);
}
