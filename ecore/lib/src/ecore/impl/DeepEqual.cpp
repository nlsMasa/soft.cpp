#include "ecore/impl/DeepEqual.hpp"

using namespace ecore;
using namespace ecore::impl;

bool DeepEqual::equals( const std::shared_ptr<EObject>& lhs, const std::shared_ptr<EObject>& rhs )
{
    return false;
}

bool DeepEqual::equals( const std::shared_ptr<EList<std::shared_ptr<EObject>>>& lhs,
                        const std::shared_ptr<EList<std::shared_ptr<EObject>>>& rhs )
{
    return false;
}
