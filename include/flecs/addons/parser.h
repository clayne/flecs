/**
 * @file addons/parser.h
 * @brief Parser addon.
 *
 * The parser addon parses string expressions into lists of terms, and can be
 * used to construct queries.
 */

#ifdef FLECS_PARSER

/**
 * @defgroup c_addons_parser Parser
 * @ingroup c_addons
 * Query DSL parser and parsing utilities.
 *
 * @{
 */

#ifndef FLECS_PARSER_H
#define FLECS_PARSER_H

/** Maximum number of extra arguments in term expression */
#define FLECS_TERM_ARG_COUNT_MAX (16)

#ifdef __cplusplus
extern "C" {
#endif

/** Skip whitespace characters.
 * This function skips whitespace characters. Does not skip newlines.
 *
 * @param ptr Pointer to (potential) whitespaces to skip.
 * @return Pointer to the next non-whitespace character.
 */
FLECS_API
const char* ecs_parse_ws(
    const char *ptr);

/** Skip whitespace and newline characters.
 * This function skips whitespace characters.
 *
 * @param ptr Pointer to (potential) whitespaces to skip.
 * @return Pointer to the next non-whitespace character.
 */
FLECS_API
const char* ecs_parse_ws_eol(
    const char *ptr);

/** Utility function to parse an identifier */
const char* ecs_parse_identifier(
    const char *name,
    const char *expr,
    const char *ptr,
    char *token_out);

/** Parse digit.
 * This function will parse until the first non-digit character is found. The
 * provided expression must contain at least one digit character.
 *
 * @param ptr The expression to parse.
 * @param token The output buffer.
 * @return Pointer to the first non-digit character.
 */
FLECS_API
const char* ecs_parse_digit(
    const char *ptr,
    char *token);

/** Parse a single token.
 * This function can be used as simple tokenizer by other parsers.
 *
 * @param name of program (used for logging).
 * @param expr pointer to token to parse.
 * @param ptr pointer to first character to parse.
 * @param token_out Parsed token (buffer should be ECS_MAX_TOKEN_SIZE large)
 * @return Pointer to the next token, or NULL if error occurred.
 */
FLECS_API
const char* ecs_parse_token(
    const char *name,
    const char *expr,
    const char *ptr,
    char *token_out,
    char delim);

/** Parse term in expression.
 * This operation parses a single term in an expression and returns a pointer
 * to the next term expression.
 *
 * If the returned pointer points to the 0-terminator, the expression is fully
 * parsed. The function would typically be called in a while loop:
 *
 * @code
 * const char *ptr = expr;
 * while (ptr[0] && (ptr = ecs_parse_term(world, name, expr, ptr, &term))) { }
 * @endcode
 *
 * The operation does not attempt to find entity ids from the names in the
 * expression. Use the ecs_term_resolve_ids() function to resolve the identifiers
 * in the parsed term.
 *
 * The returned term will in most cases contain allocated resources, which
 * should freed (or used) by the application. To free the resources for a term,
 * use the ecs_term_free() function.
 *
 * The parser accepts expressions in the legacy string format.
 *
 * @param world The world.
 * @param stage The stage.
 * @param name The name of the expression (optional, improves error logs)
 * @param expr The expression to parse (optional, improves error logs)
 * @param ptr The pointer to the current term (must be in expr).
 * @param term_out Out parameter for the term.
 * @param extra_args Out array for extra args, must be of size FLECS_TERM_ARG_COUNT_MAX.
 * @return pointer to next term if successful, NULL if failed.
 */
FLECS_API
char* ecs_parse_term(
    const ecs_world_t *world,
    ecs_stage_t *stage,
    const char *name,
    const char *expr,
    const char *ptr,
    ecs_term_t *term_out,
    ecs_oper_kind_t *extra_oper,
    ecs_term_ref_t *extra_args,
    bool allow_newline);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FLECS_PARSER_H

/** @} */

#endif // FLECS_PARSER
