// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
//  Copyright 2005 - 2007 Adobe Systems Incorporated
//  Distributed under the MIT License(see accompanying file LICENSE_1_0_0.txt
//  or a copy at http://stlab.adobe.com/licenses.html)

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#include <stl2/detail/algorithm/unique.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace stl2 = __stl2;

/// Calls the iterator interface of the algorithm
template <class Iter>
struct iter_call
{
	using begin_t = Iter;
	using sentinel_t = typename sentinel_type<Iter>::type;

	template <class B, class E, class... Args>
	auto operator()(B &&It, E &&e, Args &&... args) const
	{
		return stl2::unique(begin_t{It}, sentinel_t{e}, std::forward<Args>(args)...);
	}
};

/// Calls the range interface of the algorithm
template <class Iter>
struct range_call
{
	using begin_t = Iter;
	using sentinel_t = typename sentinel_type<Iter>::type;

	template <class B, class E, class... Args>
	auto operator()(B &&It, E &&e, Args &&... args) const
	{
		auto rng = stl2::subrange(begin_t{It}, sentinel_t{e});
		return stl2::unique(rng, std::forward<Args>(args)...);
	}
};

template <class T> using identity_t = T;

template <class It, template <class> class FunT>
void test()
{
	using Fun = FunT<It>;

	{
		int a[] = {0};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + sa));
		CHECK(a[0] == 0);
	}
	{
		int a[] = {0, 1};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + sa));
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
	}
	{
		int a[] = {0, 0};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + 1));
		CHECK(a[0] == 0);
	}
	{
		int a[] = {0, 0, 1};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + 2));
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
	}
	{
		int a[] = {0, 0, 1, 0};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + 3));
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
		CHECK(a[2] == 0);
	}
	{
		int a[] = {0, 0, 1, 1};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + 2));
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
	}
	{
		int a[] = {0, 1, 1};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + 2));
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
	}
	{
		int a[] = {0, 1, 1, 1, 2, 2, 2};
		const unsigned sa = sizeof(a) / sizeof(a[0]);
		auto r = Fun{}(a, a + sa);
		CHECK(r == It(a + 3));
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
		CHECK(a[2] == 2);
	}
}

int main()
{
	test<forward_iterator<int*>, iter_call>();
	test<bidirectional_iterator<int*>, iter_call>();
	test<random_access_iterator<int*>, iter_call>();
	test<int*, iter_call>();

	test<forward_iterator<int*>, range_call>();
	test<bidirectional_iterator<int*>, range_call>();
	test<random_access_iterator<int*>, range_call>();
	test<int*, range_call>();

	// Test rvalue range
	{
		int a[] = {0, 1, 1, 1, 2, 2, 2};
		auto r = stl2::unique(stl2::move(a));
		CHECK(r.get_unsafe() == a + 3);
		CHECK(a[0] == 0);
		CHECK(a[1] == 1);
		CHECK(a[2] == 2);
	}

	return ::test_result();
}
