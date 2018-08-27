adl_INCLUDE_STRFY(

/*
 * [[ DATA STRUCTURES ]]
 */

struct dbm_s {
    size_t nvars;
    __global constant_t* data;
};

static inline size_t dbm_nvars(dbm_t dbm) { return dbm->nvars; }
static inline odvar_t dbm_firstvar(dbm_t dbm) { odvar_t var = dbm->nvars > 0 ? odvar(1) : null_odvar; return var; }
static inline odvar_t dbm_lastvar(dbm_t dbm) { return odvar(dbm->nvars); }
static inline odvar_t dbm_endvar(dbm_t dbm) { return odvar(dbm->nvars + 1); }
static inline size_t dbm_indexof(dbm_t dbm, odvar_t xi, odvar_t xj) { return oct_indexof(dbm->nvars, xi, xj); }
static inline constant_t* dbm_at(dbm_t dbm, odvar_t xi, odvar_t xj) { return &dbm->data[dbm_indexof(dbm, xi, xj)]; }
static inline constant_t dbm_get(dbm_t dbm, odvar_t xi, odvar_t xj) { return *dbm_at(dbm, xi, xj); }
static inline constant_t dbm_set(dbm_t dbm, odvar_t xi, odvar_t xj, constant_t value) { return (*dbm_at(dbm, xi, xj) = value); }
static inline constant_t dbm_xchg(dbm_t dbm, odvar_t xi, odvar_t xj, constant_t value) { size_t i = dbm_indexof(dbm, xi, xj); constant_t old = dbm->data[i]; dbm->data[i] = value; return old; }

)
