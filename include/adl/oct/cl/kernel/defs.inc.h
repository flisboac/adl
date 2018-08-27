adl_INCLUDE_STRFY(

/*
 * [[ VOODOOS AND COMPILE-TIME CHECKS ]]
 */

// For proper IDE support
#ifndef __OPENCL_C_VERSION__
    #define __global
    #define __local
    #define bool int
    #define adl_CONSTANT_TYPE float
    #define adl_VARID_TYPE int
#endif

#ifndef adl_CONSTANT_TYPE
    #error "adl_CONSTANT_TYPE not provided."
#endif

#ifndef adl_VARID_TYPE
    #error "adl_VARID_TYPE not provided."
#endif


/*
 * [[ MACRO DEFINITIONS ]]
 */


#define null_index ((size_t)-1)

#define null_varid ((varid_t)0)
#define null_ovar ((ovar_t){null_var_id})
#define null_odvar ((odvar_t){null_var_id})

#define dbm_CREATE(name, data, ldata, nvars)     \
    dbm_s name##__struct = {nvars, data, ldata}; \
    dbm_t name = &name##__struct

/*
 * [[ TYPE DEFINITIONS ]]
 */

typedef adl_CONSTANT_TYPE constant_t;
typedef adl_VARID_TYPE varid_t;
typedef struct { varid_t id; } ovar_t;
typedef struct { varid_t id; } odvar_t;
typedef struct ocons_s ocons_t;
typedef struct odcons_s odcons_t;
typedef struct dbm_s* dbm_t;



/*
 * [[ FUNCTIONS: OCTDIFF-VARIABLES ]]
 */

static inline odvar_t odvar(varid_t id) { odvar_t v = {id >= 0 ? id : 0}; return v; }
static inline int odvar_sign(odvar_t var) { return var.id == 0 ? 0 : var.id % 2 == 0 ? 1 : -1; }
static inline int odvar_cmp(odvar_t a, odvar_t b) { return (b.id < a.id) - (a.id < b.id); }
static inline bool odvar_isvalid(odvar_t var) { return var.id != 0; }
static inline bool odvar_isnull(odvar_t var) { return var.id == 0; }
static inline bool odvar_ispos(odvar_t var) { return var.id != 0 && var.id % 2 != 0; }
static inline bool odvar_isneg(odvar_t var) { return var.id != 0 && var.id % 2 == 0; }
static inline odvar_t odvar_normal(odvar_t var) { odvar_t cv = {var.id == 0 ? 0 : var.id % 2 == 0 ? var.id - 1: var.id}; return cv; }
static inline odvar_t odvar_neg(odvar_t var) { odvar_t cv = {var.id == 0 ? 0 : var.id % 2 == 0 ? var.id - 1: var.id + 1}; return cv; }
static inline odvar_t odvar_inc(odvar_t var, size_t offset) { odvar_t cv = {var.id + ((varid_t) offset)}; return cv; }
static inline odvar_t odvar_dec(odvar_t var, size_t offset) { odvar_t cv = {var.id - ((varid_t) offset)}; return cv; }
static inline size_t odvar_toindex(odvar_t var) { return var.id == 0 ? null_index : var.id - 1; }
#if defined(adl_VARID_ISINT)
    static inline ovar_t odvar_tocounter(odvar_t var) { ovar_t cv = {var.id == 0 ? 0 : var.id % 2 == 0 ? (var.id >> 1) + 1: -(var.id >> 1)}; return cv; }
#else
    static inline ovar_t odvar_tocounter(odvar_t var) { ovar_t cv = {var.id == 0 ? 0 : var.id % 2 == 0 ? (var.id / 2) + 1: -(var.id / 2)}; return cv; }
#endif


/*
 * [[ FUNCTIONS: OCT-VARIABLES ]]
 */

#if defined(adl_VARID_ISINT)
    static inline odvar_t ovar_tocounter(ovar_t var) { odvar_t cvar = { (var.id < 0) ? (-var.id << 1) : ((var.id << 1) - 1) }; return cvar; }
#else
    static inline odvar_t ovar_tocounter(ovar_t var) { odvar_t cvar = { (var.id < 0) ? (-var.id * 2) : (var.id * 2) - 1}; return cvar; }
#endif
static inline ovar_t ovar(varid_t id) { ovar_t v = {id}; return v; }
static inline int ovar_sign(ovar_t var) { return var.id == 0 ? 0 : var.id < 0 ? -1 : 1; }
static inline int ovar_cmp(ovar_t a, ovar_t b) { return odvar_cmp(ovar_tocounter(a), ovar_tocounter(b)); }
static inline bool ovar_isvalid(ovar_t var) { return var.id != 0; }
static inline bool ovar_isnull(ovar_t var) { return var.id == 0; }
static inline bool ovar_ispos(ovar_t var) { return var.id > 0; }
static inline bool ovar_isneg(ovar_t var) { return var.id < 0; }
static inline ovar_t ovar_normal(ovar_t var) { ovar_t cv = {var.id >= 0 ? var.id : -var.id}; return cv; }
static inline ovar_t ovar_neg(ovar_t var) { ovar_t cv = {var.id >= 0 ? var.id : -var.id}; return cv; }
static inline size_t ovar_toindex(ovar_t var) { return odvar_toindex(ovar_tocounter(var)); }
static inline ovar_t ovar_inc(ovar_t var, size_t offset) { return odvar_tocounter(odvar_inc(ovar_tocounter(var), offset)); }
static inline ovar_t ovar_dec(ovar_t var, size_t offset) { return odvar_tocounter(odvar_dec(ovar_tocounter(var), offset)); }


/*
 * [[ FUNCTIONS: DBM ]]
 */

static inline size_t oct_indexof(size_t nvars, odvar_t xi, odvar_t xj) {
    size_t i = null_index;
    if (xi.id != 0 && xj.id != 0) {
        i = odvar_toindex(xi) * nvars + odvar_toindex(xj);
    }
    return i;
}

)
