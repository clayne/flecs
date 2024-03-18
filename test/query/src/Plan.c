#include <query.h>

void Plan_reordered_plan_1(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, ChildOf($this, $p, $gp, $ggp), Bar($ggp)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      "
    LINE " 1. [ 0,  2]  selfupid    $[this]           (Foo)"
    LINE " 2. [ 1,  3]  and         $[this]           (ChildOf, $p)"
    LINE " 3. [ 2,  4]  and         $p                (ChildOf, $gp)"
    LINE " 4. [ 3,  5]  and         $gp               (ChildOf, $ggp)"
    LINE " 5. [ 4,  6]  selfupid    $ggp              (Bar)"
    LINE " 6. [ 5,  7]  setvars     "
    LINE " 7. [ 6,  8]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_reordered_plan_2(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo($ggp), ChildOf($this, $p, $gp, $ggp), Bar($this)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      "
    LINE " 1. [ 0,  2]  selfupid    $[ggp]            (Foo)"
    LINE " 2. [ 1,  3]  each        $ggp              ($[ggp])"
    LINE " 3. [ 2,  4]  and         $[gp]             (ChildOf, $ggp)"
    LINE " 4. [ 3,  5]  each        $gp               ($[gp])"
    LINE " 5. [ 4,  6]  and         $[p]              (ChildOf, $gp)"
    LINE " 6. [ 5,  7]  each        $p                ($[p])"
    LINE " 7. [ 6,  8]  and         $[this]           (ChildOf, $p)"
    LINE " 8. [ 7,  9]  selfupid    $[this]           (Bar)"
    LINE " 9. [ 8, 10]  setvars     "
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_1_trivial_plan(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  andid       $[this]           (Foo)"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_2_trivial_plan(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo(self), Bar(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  triv        {0,1}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_1_trivial_plan_component(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  andid       $[this]           (Position)"
    LINE " 2. [ 1,  3]  popself     {0}"
    LINE " 3. [ 2,  4]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_2_trivial_plan_component(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(self), Velocity(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  trivpop     {0,1}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_3_trivial_plan_w_pair(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo(self), Bar(self), ChildOf(self, p)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  triv        {0,1,2}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_3_trivial_plan_w_wildcard(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo(self), Bar(self), ChildOf(self, *)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  trivwc      {0,1,2}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_3_trivial_plan_w_any(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo(self), Bar(self), ChildOf(self, _)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  triv        {0,1}"
    LINE " 2. [ 1,  3]  andany      $[this]           (ChildOf, $_'1)"
    LINE " 3. [ 2,  4]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_3_trivial_plan_w_pair_component(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(self), Velocity(self), ChildOf(self, p)"
    });
    
    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  trivpop     {0,1,2}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_3_trivial_plan_w_wildcard_component(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(self), Velocity(self), ChildOf(self, *)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  trivwc      {0,1,2}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_3_trivial_plan_w_any_component(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(self), Velocity(self), ChildOf(self, _)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  trivpop     {0,1}"
    LINE " 2. [ 1,  3]  andany      $[this]           (ChildOf, $_'1)"
    LINE " 3. [ 2,  4]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_1_trivial_component_w_none(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "[none] Position(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  andid       $[this]           (Position)"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_2_trivial_component_w_none(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "p");

    ecs_query_t *r = ecs_query(world, {
        .expr = "[none] Position(self), [none] Velocity(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  triv        {0,1}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_2_trivial_plan_w_wildcard(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo(self), ChildOf(self, *)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setids      " 
    LINE " 1. [ 0,  2]  trivwc      {0,1}"
    LINE " 2. [ 1,  3]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_this_before_fixed_src(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, Bar(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Bar)"
    LINE " 3. [ 2,  4]  selfupid    $[this]           (Foo)"
    LINE " 4. [ 3,  5]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_fixed_src_before_this(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Bar(e), Foo"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Bar)"
    LINE " 3. [ 2,  4]  selfupid    $[this]           (Foo)"
    LINE " 4. [ 3,  5]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_var_before_fixed_src(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo($var), Bar(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Bar)"
    LINE " 3. [ 2,  4]  selfupid    $[var]            (Foo)"
    LINE " 4. [ 3,  5]  each        $var              ($[var])"
    LINE " 5. [ 4,  6]  setvars     "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_fixed_src_before_var(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Bar(e), Foo($var)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Bar)"
    LINE " 3. [ 2,  4]  selfupid    $[var]            (Foo)"
    LINE " 4. [ 3,  5]  each        $var              ($[var])"
    LINE " 5. [ 4,  6]  setvars     "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_this_before_fixed_src_w_not(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, !Bar(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  4]  not         "
    LINE " 3. [ 2,  4]   selfupid   e                 (Bar)"
    LINE " 4. [ 2,  5]  end         e                 (Bar)"
    LINE " 5. [ 4,  6]  selfupid    $[this]           (Foo)"
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_this_before_fixed_src_w_first_var(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, $this(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfup      e                 ($this)"
    LINE " 3. [ 2,  4]  selfupid    $this             (Foo)"
    LINE " 4. [ 3,  5]  setthis                       ($this)"
    LINE " 5. [ 4,  6]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_this_before_fixed_src_w_first_var_w_not(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, !$this(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    $[this]           (Foo)"
    LINE " 3. [ 2,  4]  each        $this             ($[this])"
    LINE " 4. [ 3,  6]  not         "
    LINE " 5. [ 4,  6]   selfup     e                 ($this)"
    LINE " 6. [ 4,  7]  end         e                 ($this)"
    LINE " 7. [ 6,  8]  setthis                       ($this)"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_this_before_fixed_src_w_second_var(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, Bar(e, $this)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfup      e                 (Bar, $this)"
    LINE " 3. [ 2,  4]  selfupid    $this             (Foo)"
    LINE " 4. [ 3,  5]  setthis                       ($this)"
    LINE " 5. [ 4,  6]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_this_before_fixed_src_w_second_var_w_not(void) {
    ecs_world_t *world = ecs_mini();

    ECS_TAG(world, Foo);
    ECS_TAG(world, Bar);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Foo, !Bar(e, $this)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    $[this]           (Foo)"
    LINE " 3. [ 2,  4]  each        $this             ($[this])"
    LINE " 4. [ 3,  6]  not         "
    LINE " 5. [ 4,  6]   selfup     e                 (Bar, $this)"
    LINE " 6. [ 4,  7]  end         e                 (Bar, $this)"
    LINE " 7. [ 6,  8]  setthis                       ($this)"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed_1_this_self(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  andid       $[this]           (Velocity)"
    LINE " 5. [ 4,  6]  popself     {1}"
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_self(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e), Mass(self), Rotation(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  trivpop     {2,3}"
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_self_interleaved(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Mass(self), Velocity(e), Rotation(self)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,2}"
    LINE " 5. [ 4,  6]  trivpop     {1,3}"
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_this_self_2_fixed(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Mass(self), Rotation(self), Position(e), Velocity(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {2,3}"
    LINE " 5. [ 4,  6]  trivpop     {0,1}"
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed_1_this_up(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(up)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  upid        $[this]           (Velocity)"
    LINE " 5. [ 4,  6]  pop         {1}"
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_up(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e), Mass(up), Rotation(up)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  upid        $[this]           (Mass)"
    LINE " 6. [ 5,  7]  upid        $[this]           (Rotation)"
    LINE " 7. [ 6,  8]  pop         {2,3}"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_up_interleaved(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Mass(up), Velocity(e), Rotation(up)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,2}"
    LINE " 5. [ 4,  6]  upid        $[this]           (Mass)"
    LINE " 6. [ 5,  7]  upid        $[this]           (Rotation)"
    LINE " 7. [ 6,  8]  pop         {1,3}"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_this_up_2_fixed(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Mass(up), Rotation(up), Position(e), Velocity(e)"
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {2,3}"
    LINE " 5. [ 4,  6]  upid        $[this]           (Mass)"
    LINE " 6. [ 5,  7]  upid        $[this]           (Rotation)"
    LINE " 7. [ 6,  8]  pop         {0,1}"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed_1_this_self_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(self)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  cachepop    "
    LINE " 5. [ 4,  6]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_self_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e), Mass(self), Rotation(self)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  cachepop    "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_self_interleaved_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Mass(self), Velocity(e), Rotation(self)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,2}"
    LINE " 5. [ 4,  6]  cachepop    "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_this_self_2_fixed_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Mass(self), Rotation(self), Position(e), Velocity(e)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {2,3}"
    LINE " 5. [ 4,  6]  cachepop    "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed_1_this_up_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(up)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  cachepop    "
    LINE " 5. [ 4,  6]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_up_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e), Mass(up), Rotation(up)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  cachepop    "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_this_up_interleaved_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Mass(up), Velocity(e), Rotation(up)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,2}"
    LINE " 5. [ 4,  6]  cachepop    "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_this_up_2_fixed_cached(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Mass(up), Rotation(up), Position(e), Velocity(e)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {2,3}"
    LINE " 5. [ 4,  6]  cachepop    "
    LINE " 6. [ 5,  7]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed_1_var_self(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity($var:self)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  andid       $[var]            (Velocity)"
    LINE " 5. [ 4,  6]  each        $var              ($[var])"
    LINE " 6. [ 5,  7]  setvars     "
    LINE " 7. [ 6,  8]  pop         {1}"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_var_self(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e), Mass($var:self), Rotation($var:self)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  andid       $[var]            (Mass)"
    LINE " 6. [ 5,  7]  andid       $[var]            (Rotation)"
    LINE " 7. [ 6,  8]  each        $var              ($[var])"
    LINE " 8. [ 7,  9]  setvars     "
    LINE " 9. [ 8, 10]  pop         {2,3}"
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_var_self_interleaved(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Mass($var:self), Velocity(e), Rotation($var:self)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,2}"
    LINE " 5. [ 4,  6]  andid       $[var]            (Mass)"
    LINE " 6. [ 5,  7]  andid       $[var]            (Rotation)"
    LINE " 7. [ 6,  8]  each        $var              ($[var])"
    LINE " 8. [ 7,  9]  setvars     "
    LINE " 9. [ 8, 10]  pop         {1,3}"
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_var_self_2_fixed(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Mass($var:self), Rotation($var:self), Position(e), Velocity(e)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {2,3}"
    LINE " 5. [ 4,  6]  andid       $[var]            (Mass)"
    LINE " 6. [ 5,  7]  andid       $[var]            (Rotation)"
    LINE " 7. [ 6,  8]  each        $var              ($[var])"
    LINE " 8. [ 7,  9]  setvars     "
    LINE " 9. [ 8, 10]  pop         {0,1}"
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_1_fixed_1_var_up(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity($var:up)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  pop         {0}"
    LINE " 4. [ 3,  5]  upid        $[var]            (Velocity)"
    LINE " 5. [ 4,  6]  each        $var              ($[var])"
    LINE " 6. [ 5,  7]  setvars     "
    LINE " 7. [ 6,  8]  pop         {1}"
    LINE " 8. [ 7,  9]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_var_up(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Velocity(e), Mass($var:up), Rotation($var:up)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,1}"
    LINE " 5. [ 4,  6]  upid        $[var]            (Mass)"
    LINE " 6. [ 5,  7]  upid        $[var]            (Rotation)"
    LINE " 7. [ 6,  8]  each        $var              ($[var])"
    LINE " 8. [ 7,  9]  setvars     "
    LINE " 9. [ 8, 10]  pop         {2,3}"
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_fixed_2_var_up_interleaved(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position(e), Mass($var:up), Velocity(e), Rotation($var:up)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {0,2}"
    LINE " 5. [ 4,  6]  upid        $[var]            (Mass)"
    LINE " 6. [ 5,  7]  upid        $[var]            (Rotation)"
    LINE " 7. [ 6,  8]  each        $var              ($[var])"
    LINE " 8. [ 7,  9]  setvars     "
    LINE " 9. [ 8, 10]  pop         {1,3}"
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_populate_2_var_up_2_fixed(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Mass($var:up), Rotation($var:up), Position(e), Velocity(e)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  setfix      "
    LINE " 1. [ 0,  2]  setids      "
    LINE " 2. [ 1,  3]  selfupid    e                 (Position)"
    LINE " 3. [ 2,  4]  selfupid    e                 (Velocity)"
    LINE " 4. [ 3,  5]  pop         {2,3}"
    LINE " 5. [ 4,  6]  upid        $[var]            (Mass)"
    LINE " 6. [ 5,  7]  upid        $[var]            (Rotation)"
    LINE " 7. [ 6,  8]  each        $var              ($[var])"
    LINE " 8. [ 7,  9]  setvars     "
    LINE " 9. [ 8, 10]  pop         {0,1}"
    LINE "10. [ 9, 11]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_cache_2_or(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position || Velocity",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  xcache      "
    LINE " 1. [ 0,  2]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_cache_2_or_w_not(void) {
    ecs_world_t *world = ecs_mini();

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Mass);
    ECS_COMPONENT(world, Rotation);

    ecs_new_entity(world, "e");

    ecs_query_t *r = ecs_query(world, {
        .expr = "Position || Velocity, !Mass",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  xcache      "
    LINE " 1. [ 0,  2]  yield       "
    LINE "";
    char *plan = ecs_query_plan(r);

    test_str(expect, plan);
    ecs_os_free(plan);

    ecs_query_fini(r);

    ecs_fini(world);
}

void Plan_1_plan_any_src(void) {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, RelA);
    ECS_TAG(world, Tag);

    ecs_query_t *r = ecs_query(world, {
        .expr = "RelA(_)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  1]  ids         $[_'1]            (RelA)"
    LINE " 1. [ 0,  2]  yield       "
    LINE "";
    char *plan = ecs_query_str(r);

    test_str(expect, plan);
    ecs_os_free(plan);
    ecs_query_fini(r);
    ecs_fini(world);
}

void Plan_1_plan_not_any_src(void) {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, RelA);
    ECS_TAG(world, Tag);

    ecs_query_t *r = ecs_query(world, {
        .expr = "!RelA(_)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  2]  not         "
    LINE " 1. [ 0,  2]   ids        $[_'1]            (RelA)"
    LINE " 2. [ 0,  3]  end         $[_'1]            (RelA)"
    LINE " 3. [ 2,  4]  yield       "
    LINE "";
    char *plan = ecs_query_str(r);

    test_str(expect, plan);
    ecs_os_free(plan);
    ecs_query_fini(r);
    ecs_fini(world);
}

void Plan_1_plan_optional_any_src(void) {
    ecs_world_t *world = ecs_init();

    ECS_TAG(world, RelA);
    ECS_TAG(world, Tag);

    ecs_query_t *r = ecs_query(world, {
        .expr = "?RelA(_)",
        .cache_kind = EcsQueryCacheAuto
    });

    test_assert(r != NULL);

    ecs_log_enable_colors(false);

    const char *expect = 
    HEAD " 0. [-1,  2]  option      "
    LINE " 1. [ 0,  2]   ids        $[_'1]            (RelA)"
    LINE " 2. [ 0,  3]  end         $[_'1]            (RelA)"
    LINE " 3. [ 2,  4]  yield       "
    LINE "";
    char *plan = ecs_query_str(r);
    test_str(expect, plan);
    ecs_os_free(plan);
    ecs_query_fini(r);
    ecs_fini(world);
}
