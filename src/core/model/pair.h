/*
 * Copyright (c) 2018 Caliola Engineering, LLC.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Jared Dulmage <jared.dulmage@caliola.com>
 */

#ifndef PAIR_H
#define PAIR_H

#include "attribute-helper.h"
#include "string.h"

#include <sstream>
#include <type_traits>
#include <typeinfo> // typeid
#include <utility>

namespace ns3
{

/**
 * Output streamer for a std::pair.
 * @tparam A \deduced Type of the `pair.first`.
 * @tparam B \deduced Type of the `pair.second`.
 * @param [in,out] os The output stream.
 * @param [in] p The pair.
 * @returns The output stream.
 */
template <class A, class B>
std::ostream&
operator<<(std::ostream& os, const std::pair<A, B>& p)
{
    os << "(" << p.first << "," << p.second << ")";
    return os;
}

/**
 * @ingroup attributes
 * @defgroup attribute_Pair Pair Attribute
 * AttributeValue implementation for Pair
 */

/**
 * @ingroup attribute_Pair
 * AttributeValue implementation for Pair.
 * Hold objects of type std::pair<A, B>.
 *
 * @see AttributeValue
 */
template <class A, class B>
class PairValue : public AttributeValue
{
  public:
    /** Type of value stored in the PairValue. */
    typedef std::pair<Ptr<A>, Ptr<B>> value_type;
    /** Type of abscissa (first entry of pair). */
    typedef typename std::invoke_result_t<decltype(&A::Get), A> first_type;
    /** Type of ordinal (second entry of pair). */
    typedef typename std::invoke_result_t<decltype(&B::Get), B> second_type;
    /** Type returned by Get or passed in Set. */
    typedef typename std::pair<first_type, second_type> result_type;

    PairValue()
        : m_value(std::make_pair(Create<A>(), Create<B>()))
    {
    }

    /**
     * Construct this PairValue from a std::pair
     *
     * @param [in] value Value with which to construct.
     */
    PairValue(const result_type& value)
    {
        Set(value);
    } // "import" constructor

    // Inherited
    Ptr<AttributeValue> Copy() const override;
    bool DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker) override;
    std::string SerializeToString(Ptr<const AttributeChecker> checker) const override;

    /**
     * Get the stored value as a std::pair.
     *
     * This differs from the actual value stored in the object which is
     * a pair of Ptr<AV> where AV is a class derived from AttributeValue.
     * @return stored value as std::pair<A, B>.
     */
    result_type Get() const;
    /**
     * Set the value.
     * @param [in] value The value to adopt.
     */
    void Set(const result_type& value);

    /**
     * Access the Pair value as type \p T.
     * @tparam T \explicit The type to cast to.
     * @param [out] value The Pair value, as type \p T.
     * @returns true.
     */
    template <typename T>
    bool GetAccessor(T& value) const;

  private:
    value_type m_value; //!< The stored Pair instance.
};

/**
 * @ingroup attribute_Pair
 * AttributeChecker implementation for PairValue.
 * @see AttributeChecker
 */
class PairChecker : public AttributeChecker
{
  public:
    /** Type holding an AttributeChecker for each member of a pair. */
    typedef std::pair<Ptr<const AttributeChecker>, Ptr<const AttributeChecker>> checker_pair_type;

    /**
     * Set the individual AttributeChecker for each pair entry.
     *
     * \param[in] firstchecker AttributeChecker for abscissa.
     * \param[in] secondchecker AttributeChecker for ordinate.
     */
    virtual void SetCheckers(Ptr<const AttributeChecker> firstchecker,
                             Ptr<const AttributeChecker> secondchecker) = 0;

    /**
     * Get the pair of checkers for each pair entry.
     *
     * @return std::pair with AttributeChecker for each of abscissa and ordinate.
     */
    virtual checker_pair_type GetCheckers() const = 0;
};

/**
 * @ingroup attribute_Pair
 *
 * Make a PairChecker from a PairValue.
 *
 * This function returns a Pointer to a non-const instance to
 * allow subsequent setting of the underlying AttributeCheckers.
 * \param[in] value PairValue from which to derive abscissa and ordinate types.
 * @return Pointer to PairChecker instance.
 */
template <class A, class B>
Ptr<AttributeChecker> MakePairChecker(const PairValue<A, B>& value);

/**
 * @ingroup attribute_Pair
 *
 * Make a PairChecker from abscissa and ordinate AttributeCheckers.
 *
 * This function returns a Pointer to a const instance since both
 * underlying AttributeCheckers are set.
 *
 * \param[in] firstchecker AttributeChecker for abscissa.
 * \param[in] secondchecker AttributeChecker for ordinate.
 * @return Pointer to PairChecker instance.
 */
template <class A, class B>
Ptr<const AttributeChecker> MakePairChecker(Ptr<const AttributeChecker> firstchecker,
                                            Ptr<const AttributeChecker> secondchecker);

/**
 * @ingroup attribute_Pair
 *
 * Make a PairChecker without abscissa and ordinate AttributeCheckers.
 *
 * @return Pointer to PairChecker instance.
 */
template <class A, class B>
Ptr<AttributeChecker> MakePairChecker();

/**
 * @ingroup attribute_Pair
 *
 * Create an AttributeAccessor for std::pair<>.
 * @tparam A \explicit The type of pair.first.
 * @tparam B \explicit The type of pair.second.
 * @tparam T1 \deduced The argument pair type.
 * @param [in] a1 The std::pair to be accessed.
 * @returns The AttributeAccessor.
 */
template <typename A, typename B, typename T1>
Ptr<const AttributeAccessor> MakePairAccessor(T1 a1);

} // namespace ns3

/*****************************************************************************
 * Implementation below
 *****************************************************************************/

namespace ns3
{

// This internal class defines templated PairChecker class that is instantiated
// in MakePairChecker. The non-templated base ns3::PairChecker is returned in that
// function. This is the same pattern as ObjectPtrContainer.
namespace internal
{

/**
 * @ingroup attribute_Pair
 *
 * Internal checker class templated to each AttributeChecker
 * for each entry in the pair.
 */
template <class A, class B>
class PairChecker : public ns3::PairChecker
{
  public:
    /** Default c'tor. */
    PairChecker();
    /**
     * Construct from a pair of AttributeChecker's.
     * @param firstchecker The AttributeChecker for first.
     * @param secondchecker The AttributeChecker for second.
     */
    PairChecker(Ptr<const AttributeChecker> firstchecker,
                Ptr<const AttributeChecker> secondchecker);
    void SetCheckers(Ptr<const AttributeChecker> firstchecker,
                     Ptr<const AttributeChecker> secondchecker) override;
    typename ns3::PairChecker::checker_pair_type GetCheckers() const override;

  private:
    /** The first checker. */
    Ptr<const AttributeChecker> m_firstchecker;
    /** The second checker. */
    Ptr<const AttributeChecker> m_secondchecker;
};

template <class A, class B>
PairChecker<A, B>::PairChecker()
    : m_firstchecker(nullptr),
      m_secondchecker(nullptr)
{
}

template <class A, class B>
PairChecker<A, B>::PairChecker(Ptr<const AttributeChecker> firstchecker,
                               Ptr<const AttributeChecker> secondchecker)
    : m_firstchecker(firstchecker),
      m_secondchecker(secondchecker)
{
}

template <class A, class B>
void
PairChecker<A, B>::SetCheckers(Ptr<const AttributeChecker> firstchecker,
                               Ptr<const AttributeChecker> secondchecker)
{
    m_firstchecker = firstchecker;
    m_secondchecker = secondchecker;
}

template <class A, class B>
typename ns3::PairChecker::checker_pair_type
PairChecker<A, B>::GetCheckers() const
{
    return std::make_pair(m_firstchecker, m_secondchecker);
}

} // namespace internal

template <class A, class B>
Ptr<AttributeChecker>
MakePairChecker(const PairValue<A, B>& value)
{
    return MakePairChecker<A, B>();
}

template <class A, class B>
Ptr<const AttributeChecker>
MakePairChecker(Ptr<const AttributeChecker> firstchecker, Ptr<const AttributeChecker> secondchecker)
{
    auto checker = MakePairChecker<A, B>();
    auto acchecker = DynamicCast<PairChecker>(checker);
    acchecker->SetCheckers(firstchecker, secondchecker);
    return checker;
}

template <class A, class B>
Ptr<AttributeChecker>
MakePairChecker()
{
    std::string pairName;
    std::string underlyingType;
    typedef PairValue<A, B> T;
    std::string first_type_name = typeid(typename T::value_type::first_type).name();
    std::string second_type_name = typeid(typename T::value_type::second_type).name();
    {
        std::ostringstream oss;
        oss << "ns3::PairValue<" << first_type_name << ", " << second_type_name << ">";
        pairName = oss.str();
    }

    {
        std::ostringstream oss;
        oss << typeid(typename T::value_type).name();
        underlyingType = oss.str();
    }

    return MakeSimpleAttributeChecker<T, internal::PairChecker<A, B>>(pairName, underlyingType);
}

template <class A, class B>
Ptr<AttributeValue>
PairValue<A, B>::Copy() const
{
    auto p = Create<PairValue<A, B>>();
    // deep copy if non-null
    if (m_value.first)
    {
        p->m_value = std::make_pair(DynamicCast<A>(m_value.first->Copy()),
                                    DynamicCast<B>(m_value.second->Copy()));
    }
    return p;
}

template <class A, class B>
bool
PairValue<A, B>::DeserializeFromString(std::string value, Ptr<const AttributeChecker> checker)
{
    auto pchecker = DynamicCast<const PairChecker>(checker);
    if (!pchecker)
    {
        return false;
    }

    std::istringstream iss(value); // copies value
    iss >> value;
    auto first = pchecker->GetCheckers().first->CreateValidValue(StringValue(value));
    if (!first)
    {
        return false;
    }

    auto firstattr = DynamicCast<A>(first);
    if (!firstattr)
    {
        return false;
    }

    iss >> value;
    auto second = pchecker->GetCheckers().second->CreateValidValue(StringValue(value));
    if (!second)
    {
        return false;
    }

    auto secondattr = DynamicCast<B>(second);
    if (!secondattr)
    {
        return false;
    }

    m_value = std::make_pair(firstattr, secondattr);
    return true;
}

template <class A, class B>
std::string
PairValue<A, B>::SerializeToString(Ptr<const AttributeChecker> checker) const
{
    std::ostringstream oss;
    oss << m_value.first->SerializeToString(checker);
    oss << " ";
    oss << m_value.second->SerializeToString(checker);

    return oss.str();
}

template <class A, class B>
typename PairValue<A, B>::result_type
PairValue<A, B>::Get() const
{
    return std::make_pair(m_value.first->Get(), m_value.second->Get());
}

template <class A, class B>
void
PairValue<A, B>::Set(const typename PairValue<A, B>::result_type& value)
{
    m_value = std::make_pair(Create<A>(value.first), Create<B>(value.second));
}

template <class A, class B>
template <typename T>
bool
PairValue<A, B>::GetAccessor(T& value) const
{
    value = T(Get());
    return true;
}

template <typename A, typename B, typename T1>
Ptr<const AttributeAccessor>
MakePairAccessor(T1 a1)
{
    return MakeAccessorHelper<PairValue<A, B>>(a1);
}

} // namespace ns3

#endif // PAIR_H
