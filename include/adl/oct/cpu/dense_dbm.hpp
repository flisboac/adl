// $flisboac 2017-08-28
/**
 * @file dense_dbm.hpp
 */
#ifndef adl__oct__cpu__dense_dbm__hpp__
#define adl__oct__cpu__dense_dbm__hpp__

#include "adl.cfg.hpp"

#include "adl/oct/dbm/traits.hpp"
#include "adl/oct/dbm/dense_dbm.hpp"

adl_BEGIN_ROOT_MODULE
namespace oct {

template <typename ContextType, typename ValueType, typename ValueLimits>
struct dbm_traits<cpu::dense_dbm<ContextType, ValueType, ValueLimits>> : public dbm_types_<ValueType, ValueLimits> {
    using dbm_type = cpu::dense_dbm<ContextType, ValueType, ValueLimits>;
    constexpr const static bool valid = true;

    constexpr const static dbm_major default_major = dbm_major::row;
    constexpr const static bool is_always_dense = true;
    constexpr const static bool is_always_autocoherent = false;
    constexpr const static bool is_always_default_major = false;
};

namespace cpu {

template <typename ContextType, typename ValueType, typename ValueLimits>
class dense_dbm : public dense_dbm_base_<adl::oct::cpu::dense_dbm<ContextType, ValueType, ValueLimits>, ValueType, ValueLimits> {
private:
    using superclass_ = dense_dbm_base_<dense_dbm, ValueType, ValueLimits>;
    using container_type_ = std::vector<ValueType>;

public:
    using typename superclass_::superclass_;
    using typename superclass_::traits_;
    using typename superclass_::identity_var_type;
    using typename superclass_::constant_type;
    using typename superclass_::value_limits;

    using context_type = ContextType;

    constexpr static const dbm_major default_major = traits_::defaut_major;

    dense_dbm();
    dense_dbm(dense_dbm const&) = default;
    dense_dbm(dense_dbm &&) noexcept = default;
    dense_dbm& operator=(dense_dbm const&) = default;
    dense_dbm& operator=(dense_dbm &&) noexcept = default;

    template <typename ValueType_,
            typename ValueLimits_,
            typename = std::enable_if_t<std::is_convertible<ValueType, ValueType_>::value> >
        dense_dbm(octdiff_system<ValueType_, ValueLimits_> const& rhs, constant_type default_value, dbm_major major = default_major);

    template <typename VarType_, typename = common_var_t<VarType_>>
        explicit dense_dbm(VarType_ last_var, constant_type value, dbm_major major = default_major);

    std::size_t size() const noexcept;

    void initialize(constant_type value);
    template <typename VarType_, typename = common_var_t<VarType_>>
        void resize(VarType_ new_last_var, constant_type value = value_limits::top());

private:
    constant_type& constant_(std::size_t index);
    constant_type const& constant_(std::size_t index) const;

private:
    container_type_ data_;
};

template <typename ValueType, typename ValueLimits>
class dense_dbm<async_context, ValueType, ValueLimits>
    : public dense_dbm<seq_context, ValueType, ValueLimits>,
      public std::enable_shared_from_this<dense_dbm<async_context, ValueType, ValueLimits>>
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
template <typename ContextType, typename ValueType, typename ValueLimits>
inline dense_dbm<ContextType, ValueType, ValueLimits>::dense_dbm()
    : superclass_(default_major) {};

template <typename ContextType, typename ValueType, typename ValueLimits>
template <typename ValueType_, typename ValueLimits_, typename>
inline dense_dbm<ContextType, ValueType, ValueLimits>::dense_dbm(
    octdiff_system<ValueType_, ValueLimits_> const& rhs,
    constant_type default_value,
    dbm_major major
) : superclass_(major) {
    resize(rhs.vars().last_var(), default_value);
    for (auto cons : rhs) superclass_::at(cons) = cons.c();
};

template <typename ContextType, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline dense_dbm<ContextType, ValueType, ValueLimits>::dense_dbm(
    VarType_ last_var,
    constant_type value,
    dbm_major major
) : superclass_(major) {
    resize(to_end_index_(last_var), value);
};

template <typename ContextType, typename ValueType, typename ValueLimits>
inline std::size_t dense_dbm<ContextType, ValueType, ValueLimits>::size() const noexcept {
    return data_.size() >> 1;
};

template <typename ContextType, typename ValueType, typename ValueLimits>
inline void dense_dbm<ContextType, ValueType, ValueLimits>::initialize(constant_type value) {
    data_.assign(data_.size(), value);
};

template <typename ContextType, typename ValueType, typename ValueLimits>
template <typename VarType_, typename>
inline void dense_dbm<ContextType, ValueType, ValueLimits>::resize(VarType_ new_last_var, constant_type value) {
    data_.resize(to_end_index_(new_last_var), value);
};

template <typename ContextType, typename ValueType, typename ValueLimits>
inline typename dense_dbm<ContextType, ValueType, ValueLimits>::constant_type&
dense_dbm<ContextType, ValueType, ValueLimits>::constant_(std::size_t index) {
    return data_.at(index);
};

template <typename ContextType, typename ValueType, typename ValueLimits>
inline typename dense_dbm<ContextType, ValueType, ValueLimits>::constant_type const&
dense_dbm<ContextType, ValueType, ValueLimits>::constant_(std::size_t index) const {
    return data_.at(index);
};


} // namespace cpu
} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__cpu__dense_dbm__hpp__
