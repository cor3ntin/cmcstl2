// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ALGORITHM_SET_INTERSECTION_HPP
#define STL2_DETAIL_ALGORITHM_SET_INTERSECTION_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/tuple.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/algorithm/copy.hpp>
#include <stl2/detail/concepts/algorithm.hpp>

///////////////////////////////////////////////////////////////////////////
// set_intersection [set.intersection]
//
STL2_OPEN_NAMESPACE {
  template <InputIterator I1, Sentinel<I1> S1, InputIterator I2, Sentinel<I2> S2,
            WeaklyIncrementable O, class Comp = less<>, class Proj1 = identity, class Proj2 = identity>
    requires Mergeable<I1, I2, O, Comp, Proj1, Proj2>()
  O set_intersection(I1 first1, S1 last1, I2 first2, S2 last2, O result,
                     Comp comp_ = Comp{}, Proj1 proj1_ = Proj1{}, Proj2 proj2_ = Proj2{}) {
    auto&& comp = __stl2::as_function(comp_);
    auto&& proj1 = __stl2::as_function(proj1_);
    auto&& proj2 = __stl2::as_function(proj2_);

    while (first1 != last1 && first2 != last2) {
      if (comp(proj1(*first1), proj2(*first2))) {
        ++first1;
      } else if (comp(proj2(*first2), proj1(*first1))) {
        ++first2;
      } else {
        *result = *first1;
        ++result;
        ++first1;
        ++first2;
      }
    }
    return result;
  }

  template <InputRange Rng1, InputRange Rng2, WeaklyIncrementable O,
            class Comp = less<>, class Proj1 = identity, class Proj2 = identity>
    requires Mergeable<IteratorType<Rng1>, IteratorType<Rng2>, O, Comp, Proj1, Proj2>()
  O set_intersection(Rng1&& rng1, Rng2&& rng2, O result,
                     Comp&& comp = Comp{}, Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{}) {
    return __stl2::set_intersection(__stl2::begin(rng1), __stl2::end(rng1),
                                    __stl2::begin(rng2), __stl2::end(rng2),
                                    __stl2::move(result),
                                    __stl2::forward<Comp>(comp),
                                    __stl2::forward<Proj1>(proj1),
                                    __stl2::forward<Proj2>(proj2));
  }
} STL2_CLOSE_NAMESPACE

#endif