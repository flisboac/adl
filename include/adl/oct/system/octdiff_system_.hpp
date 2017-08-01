// $flisboac 2017-07-10
/**
 * @file octdiff_system_.hpp
 */
#ifndef adl__oct__system__octdiff_system___hpp__
#define adl__oct__system__octdiff_system___hpp__


#include "adl.cfg.hpp"

#include "adl/oct.fwd.hpp"
#include "adl/oct/traits.hpp"
#include "adl/oct/var.hpp"
#include "adl/oct/vexpr.hpp"
#include "adl/oct/system.hpp"

#include "adl/oct/system/system_base_.hpp"

//
// [[ API ]]
//
adl_BEGIN_ROOT_MODULE

namespace oct {

template <typename ValueType, typename ValueLimits>
class octdiff_system : public system_base_<domain_space::octdiff, ValueType, ValueLimits> {
private:
    using superclass_ = system_base_<domain_space::octdiff, ValueType, ValueLimits>;

public:
    using superclass_::constraints_;
    using superclass_::variables_;
    using superclass_::space;

    using typename superclass_::counterpart_system_type;
    using typename superclass_::iterator;
    using typename superclass_::const_iterator;
    using typename superclass_::value_type;

    octdiff_system() = default;
    octdiff_system(octdiff_system const&) = default;
    octdiff_system(octdiff_system &&) noexcept = default;
    octdiff_system& operator=(octdiff_system const&) = default;
    octdiff_system& operator=(octdiff_system &&) noexcept = default;

    octdiff_system(counterpart_system_type const&);

    template <typename VarType_, typename = std::enable_if<
        common_var<VarType_>::space == space>>
        std::size_t count(octdiff_vexpr<common_var_t<VarType_>> vexpr) const;
    template <typename VarType_, typename = common_octdiff_conjunction_t<ValueType, VarType_>>
        std::size_t count(common_octdiff_conjunction_t<ValueType, VarType_> conjunction) const;

    template <typename VarType_, typename = std::enable_if<
        common_var<VarType_>::space == space>>
        const_iterator find(octdiff_vexpr<common_var_t<VarType_>> vexpr) const;
    template <typename VarType_, typename = common_octdiff_conjunction_t<ValueType, VarType_>>
        const_iterator find(common_octdiff_conjunction_t<ValueType, VarType_> conjunction) const;

    template <typename VarType_, typename = std::enable_if<
        common_var<VarType_>::space == space>>
        value_type const& at(octdiff_vexpr<common_var_t<VarType_>> vexpr) const;
    template <typename VarType_, typename = common_octdiff_conjunction_t<ValueType, VarType_>>
        value_type const& at(common_octdiff_conjunction_t<ValueType, VarType_> conjunction) const;

    template <typename VarType_, typename = std::enable_if<
        common_var<VarType_>::space == space>>
        value_type const& operator[](octdiff_vexpr<common_var_t<VarType_>> vexpr) const;
    template <typename VarType_, typename = common_octdiff_conjunction_t<ValueType, VarType_>>
        value_type const& operator[](common_octdiff_conjunction_t<ValueType, VarType_> conjunction) const;

};

} // namespace oct

adl_END_ROOT_MODULE


//
// [[ TEMPLATE IMPLEMENTATION ]]
//
adl_BEGIN_ROOT_MODULE
namespace oct {

//
// octdiff_system
//


} // namespace oct
adl_END_ROOT_MODULE

#endif //adl__oct__system__octdiff_system___hpp__
