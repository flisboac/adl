// $flisboac 2017-08-28
/**
 * @file dense_dbm.hpp
 */
#ifndef adl__oct__cpu__dense_dbm__hpp__
#define adl__oct__cpu__dense_dbm__hpp__

#include <type_traits>

#include "adl.cfg.hpp"

#include "adl/oct/dbm/traits.hpp"
#include "adl/oct/dbm/dense_dbm_base_.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename ContextType, typename ConstantType, typename ValueLimits>
struct dbm_traits<cpu::dense_dbm<ContextType, ConstantType, ValueLimits>> : public dbm_types_<ConstantType, ValueLimits> {
    using dbm_type = cpu::dense_dbm<ContextType, ConstantType, ValueLimits>;
    constexpr const static bool valid = true;

    constexpr const static dbm_major default_major = dbm_major::row;
    constexpr const static bool is_always_dense = true;
    constexpr const static bool is_always_autocoherent = false;
    constexpr const static bool is_always_default_major = false;
};

namespace cpu {

template <typename ContextType, typename ConstantType, typename ValueLimits>
class dense_dbm : public dense_dbm_base_<dense_dbm<ContextType, ConstantType, ValueLimits>, ConstantType, ValueLimits> {
private:
    using superclass_ = dense_dbm_base_<dense_dbm, ConstantType, ValueLimits>;
    using container_type_ = std::vector<ConstantType>;

public:
    friend class crtp_base<dense_dbm>;

    using typename superclass_::traits_;
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;
    using typename superclass_::constant_limits;
    using typename superclass_::identity_cons_type;
    using typename superclass_::counterpart_identity_cons_type;
    using typename superclass_::identity_vexpr_type;
    using typename superclass_::counterpart_identity_vexpr_type;

    using superclass_::to_end_index_;

    using context_type = ContextType;

    constexpr static const dbm_major default_major = dbm_major::row;
    constexpr static constant_type default_constant() noexcept;

    dense_dbm();
    dense_dbm(dense_dbm const&) = default;
    dense_dbm(dense_dbm &&) noexcept = default;
    dense_dbm& operator=(dense_dbm const&) = default;
    dense_dbm& operator=(dense_dbm &&) noexcept = default;

    template <typename ConstantType_,
            typename ValueLimits_,
            typename = std::enable_if_t<std::is_convertible<ConstantType_, constant_type>::value> >
        dense_dbm(
            dbm_tags::create_from_octdiff_system_tag tag,
            context_type& context,
            octdiff_system<ConstantType_, ValueLimits_> const& rhs,
            ConstantType_ default_value = default_constant(),
            dbm_major major = default_major);

    template <typename ConstantType_,
            typename VarType_,
            typename = std::enable_if_t<
                common_var<VarType_>::valid
                && std::is_convertible<constant_type, ConstantType_>::value>>
        dense_dbm(
            dbm_tags::create_from_last_var_tag tag,
            context_type& context,
            VarType_ last_var,
            ConstantType_ value = default_constant(),
            dbm_major major = default_major);

    std::size_t size() const noexcept;
    context_type const& context() const;
    context_type & context();

    void initialize(constant_type value);
    void resize(octdiff_var new_last_var, constant_type value = constant_limits::top());
    template <typename CharType, typename CharTraits = std::char_traits<CharType>>
        void print(std::basic_ostream<CharType, CharTraits> &os) const;
    std::string to_string() const;

    // NOTE These are NOT a substitute for the `add_cons` operations!
    // They do NOT verify other constraints or implement any specific incremental algorithm.
    // They just assign values to the constraints' indexes in the matrix. They're supposed to be used
    // as means to prepare a DBM without an octdiff_system (primarily for tests, for "poking holes"). Use
    // at your own risk!
    void assign(counterpart_identity_cons_type oct_cons);
    void assign(std::initializer_list<counterpart_identity_cons_type> list);
    void assign(identity_cons_type cons);
    void assign(std::initializer_list<identity_cons_type> list);

    // Consider `clear` as an `assign` to `this->top()` (infinity).
    void clear(counterpart_identity_vexpr_type oct_vexpr);
    void clear(std::initializer_list<counterpart_identity_vexpr_type> list);
    void clear(identity_vexpr_type vexpr);
    void clear(std::initializer_list<identity_vexpr_type> list);

public:
    constant_type& constant_(std::size_t index);
    constant_type const& constant_(std::size_t index) const;

private:
    container_type_ data_;
    context_type * context_;
};

// TODO Async-enabled dense_dbm
template <typename ConstantType, typename ValueLimits>
class dense_dbm<async_context, ConstantType, ValueLimits>
    : public dense_dbm<seq_context, ConstantType, ValueLimits>,
      public std::enable_shared_from_this<dense_dbm<async_context, ConstantType, ValueLimits>>
{
    // ...
};

} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {
namespace cpu {

//
// dense_dbm
//
template <typename ContextType, typename ConstantType, typename ValueLimits>
constexpr typename dense_dbm<ContextType, ConstantType, ValueLimits>::constant_type
dense_dbm<ContextType, ConstantType, ValueLimits>::default_constant() noexcept {
    return constant_limits::top();
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline dense_dbm<ContextType, ConstantType, ValueLimits>::dense_dbm()
    : superclass_(default_major), context_(nullptr) {};

template <typename ContextType, typename ConstantType, typename ValueLimits>
template <typename ConstantType_, typename ValueLimits_, typename>
inline dense_dbm<ContextType, ConstantType, ValueLimits>::dense_dbm(
    dbm_tags::create_from_octdiff_system_tag tag,
    context_type& context,
    octdiff_system<ConstantType_, ValueLimits_> const& rhs,
    ConstantType_ default_value,
    dbm_major major
) : superclass_(major), context_(&context) {
    resize(rhs.vars().last_var(), default_value);
    for (auto& cons : rhs) {
        auto vexpr = cons.to_identity_vexpr();
        this->at(vexpr) = cons.c();
    }
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
template <typename ConstantType_, typename VarType_, typename>
inline dense_dbm<ContextType, ConstantType, ValueLimits>::dense_dbm(
    dbm_tags::create_from_last_var_tag tag,
    context_type& context,
    VarType_ last_var,
    ConstantType_ value,
    dbm_major major
) : superclass_(major), context_(&context) {
    resize(last_var, value);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline std::size_t dense_dbm<ContextType, ConstantType, ValueLimits>::size() const noexcept {
    return static_cast<std::size_t>( std::sqrt(data_.size()) );
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::initialize(constant_type value) {
    data_.assign(data_.size(), value);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::resize(octdiff_var new_last_var, constant_type value) {
    auto new_size = to_end_index_(new_last_var);
    data_.resize(new_size * new_size, value);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline typename dense_dbm<ContextType, ConstantType, ValueLimits>::constant_type&
dense_dbm<ContextType, ConstantType, ValueLimits>::constant_(std::size_t index) {
    return data_.at(index);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline typename dense_dbm<ContextType, ConstantType, ValueLimits>::constant_type const&
dense_dbm<ContextType, ConstantType, ValueLimits>::constant_(std::size_t index) const {
    return data_.at(index);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline typename dense_dbm<ContextType, ConstantType, ValueLimits>::context_type const&
dense_dbm<ContextType, ConstantType, ValueLimits>::context() const {
    return *this->context_;
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline typename dense_dbm<ContextType, ConstantType, ValueLimits>::context_type&
dense_dbm<ContextType, ConstantType, ValueLimits>::context() {
    return *this->context_;
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
template <typename CharType, typename CharTraits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::print(std::basic_ostream<CharType, CharTraits> &os) const {
    char const* sep = "";
    os << "{";
    for (auto x = 0; x < data_.size(); x++) {
        os << sep << this->data_[x];
        sep = ", ";
    }
    os << "}";
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline std::string dense_dbm<ContextType, ConstantType, ValueLimits>::to_string() const {
    std::stringstream ss;
    this->print(ss);
    return ss.str();
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::assign(counterpart_identity_cons_type oct_cons) {
    auto pair = oct_cons.split();
    this->assign(pair.di());
    if (!pair.unit()) this->assign(pair.dj());
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::assign(
    std::initializer_list<counterpart_identity_cons_type> list
) {
    for (auto& cons : list) assign(cons);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::assign(identity_cons_type cons) {
    if (cons.last_var() > this->last_var()) throw std::logic_error("Variable overflow.");
    this->at(cons) = cons.c();
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::assign(
    std::initializer_list<identity_cons_type> list
) {
    for (auto& cons : list) assign(cons);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::clear(counterpart_identity_vexpr_type oct_vexpr) {
    auto pair = oct_cons<ConstantType>(oct_vexpr, 0).split();
    this->clear(pair.di());
    if (!pair.unit()) this->clear(pair.dj());
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::clear(
    std::initializer_list<counterpart_identity_vexpr_type> list
) {
    for (auto& cons : list) clear(cons);
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::clear(identity_vexpr_type vexpr) {
    if (vexpr.last_var() > this->last_var()) throw std::logic_error("Variable overflow.");
    this->at(vexpr) = this->top();
};

template <typename ContextType, typename ConstantType, typename ValueLimits>
inline void dense_dbm<ContextType, ConstantType, ValueLimits>::clear(
    std::initializer_list<identity_vexpr_type> list
) {
    for (auto& cons : list) clear(cons);
};

} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__cpu__dense_dbm__hpp__
