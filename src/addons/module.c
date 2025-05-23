/**
 * @file addons/module.c
 * @brief Module addon.
 */

#include "flecs.h"

#ifdef FLECS_MODULE

#include "../private_api.h"

char* flecs_module_path_from_c(
    const char *c_name)
{
    ecs_strbuf_t str = ECS_STRBUF_INIT;
    const char *ptr;
    char ch;

    for (ptr = c_name; (ch = *ptr); ptr++) {
        if (isupper(ch)) {
            ch = flecs_ito(char, tolower(ch));
            if (ptr != c_name) {
                ecs_strbuf_appendstrn(&str, ".", 1);
            }
        }

        ecs_strbuf_appendstrn(&str, &ch, 1);
    }

    return ecs_strbuf_get(&str);
}

ecs_entity_t ecs_import(
    ecs_world_t *world,
    ecs_module_action_t module,
    const char *module_name)
{
    flecs_poly_assert(world, ecs_world_t);
    ecs_check(!(world->flags & EcsWorldReadonly), 
        ECS_INVALID_WHILE_READONLY, NULL);

    ecs_entity_t old_scope = ecs_set_scope(world, 0);
    const char *old_name_prefix = world->info.name_prefix;

    char *path = flecs_module_path_from_c(module_name);
    ecs_entity_t e = ecs_lookup(world, path);
    ecs_os_free(path);
    
    if (!e) {
        ecs_trace("#[magenta]import#[reset] %s", module_name);
        ecs_log_push();

        /* Load module */
        module(world);

        /* Lookup module entity (must be registered by module) */
        e = ecs_lookup(world, module_name);
        ecs_check(e != 0, ECS_MODULE_UNDEFINED, module_name);

        ecs_log_pop();
    }

    /* Restore to previous state */
    ecs_set_scope(world, old_scope);
    world->info.name_prefix = old_name_prefix;

    return e;
error:
    return 0;
}

ecs_entity_t ecs_import_c(
    ecs_world_t *world,
    ecs_module_action_t module,
    const char *c_name)
{
    char *name = flecs_module_path_from_c(c_name);
    ecs_entity_t e = ecs_import(world, module, name);
    ecs_os_free(name);
    return e;
}

ecs_entity_t ecs_import_from_library(
    ecs_world_t *world,
    const char *library_name,
    const char *module_name)
{
    ecs_check(library_name != NULL, ECS_INVALID_PARAMETER, NULL);

    char *import_func = ECS_CONST_CAST(char*, module_name);
    char *module = ECS_CONST_CAST(char*, module_name);

    if (!ecs_os_has_modules() || !ecs_os_has_dl()) {
        ecs_err(
            "library loading not supported, set module_to_dl, dlopen, dlclose "
            "and dlproc os API callbacks first");
        return 0;
    }

    /* If no module name is specified, try default naming convention for loading
     * the main module from the library */
    if (!import_func) {
        import_func = ecs_os_malloc(ecs_os_strlen(library_name) + ECS_SIZEOF("Import"));
        ecs_assert(import_func != NULL, ECS_OUT_OF_MEMORY, NULL);
        
        const char *ptr;
        char ch, *bptr = import_func;
        bool capitalize = true;
        for (ptr = library_name; (ch = *ptr); ptr ++) {
            if (ch == '.') {
                capitalize = true;
            } else {
                if (capitalize) {
                    *bptr = flecs_ito(char, toupper(ch));
                    bptr ++;
                    capitalize = false;
                } else {
                    *bptr = flecs_ito(char, tolower(ch));
                    bptr ++;
                }
            }
        }

        *bptr = '\0';

        module = ecs_os_strdup(import_func);
        ecs_assert(module != NULL, ECS_OUT_OF_MEMORY, NULL);

        ecs_os_strcat(bptr, "Import");
    }

    char *library_filename = ecs_os_module_to_dl(library_name);
    if (!library_filename) {
        ecs_err("failed to find library file for '%s'", library_name);
        if (module != module_name) {
            ecs_os_free(module);
        }
        return 0;
    } else {
        ecs_trace("found file '%s' for library '%s'", 
            library_filename, library_name);
    }

    ecs_os_dl_t dl = ecs_os_dlopen(library_filename);
    if (!dl) {
        ecs_err("failed to load library '%s' ('%s')", 
            library_name, library_filename);
        
        ecs_os_free(library_filename);

        if (module != module_name) {
            ecs_os_free(module);
        }    

        return 0;
    } else {
        ecs_trace("library '%s' ('%s') loaded", 
            library_name, library_filename);
    }

    ecs_module_action_t action = (ecs_module_action_t)
        ecs_os_dlproc(dl, import_func);
    if (!action) {
        ecs_err("failed to load import function %s from library %s",
            import_func, library_name);
        ecs_os_free(library_filename);
        ecs_os_dlclose(dl);            
        return 0;
    } else {
        ecs_trace("found import function '%s' in library '%s' for module '%s'",
            import_func, library_name, module);
    }

    /* Do not free id, as it will be stored as the component identifier */
    ecs_entity_t result = ecs_import(world, action, module);

    if (import_func != module_name) {
        ecs_os_free(import_func);
    }

    if (module != module_name) {
        ecs_os_free(module);
    }

    ecs_os_free(library_filename);

    return result;
error:
    return 0;
}

ecs_entity_t ecs_module_init(
    ecs_world_t *world,
    const char *c_name,
    const ecs_component_desc_t *desc)
{
    ecs_check(desc != NULL, ECS_INVALID_PARAMETER, NULL);
    flecs_poly_assert(world, ecs_world_t);

    ecs_entity_t old_scope = ecs_set_scope(world, 0);

    ecs_entity_t e = desc->entity;
    if (!e) {
        char *module_path = flecs_module_path_from_c(c_name);
        e = ecs_entity(world, { .name = module_path });
        ecs_set_symbol(world, e, module_path);
        ecs_os_free(module_path);
    } else if (!ecs_exists(world, e)) {
        char *module_path = flecs_module_path_from_c(c_name);
        ecs_make_alive(world, e);
        ecs_add_fullpath(world, e, module_path);
        ecs_set_symbol(world, e, module_path);
        ecs_os_free(module_path);
    }
    
    ecs_add_id(world, e, EcsModule);

    ecs_component_desc_t private_desc = *desc;
    private_desc.entity = e;

    if (desc->type.size) {
        ecs_entity_t result = ecs_component_init(world, &private_desc);
        ecs_assert(result != 0, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(result == e, ECS_INTERNAL_ERROR, NULL);
        (void)result;
    }

    ecs_set_scope(world, old_scope);

    return e;
error:
    return 0;
}

#endif
