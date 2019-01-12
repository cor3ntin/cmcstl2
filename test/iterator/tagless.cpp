#include <stl2/detail/iterator/basic_iterator.hpp>
#include <stl2/algorithm.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/raw_ptr.hpp>
#include <stl2/view/repeat.hpp>
#include <initializer_list>
#include <iostream>
#include <memory>
#include "../simple_test.hpp"

namespace ranges = __stl2;

template <class it>
struct base_iterator {
protected:
	char* _p = nullptr;
public:
	using base = it;
	using difference_type = std::ptrdiff_t;
	using value_type        = char;
	using pointer           = char*;
	using reference         = char&;

	char operator*() const {return *_p; };

	bool operator==(const base & o) const {
		return _p == o._p;
	};
	bool operator !=(const base & o) const {
		return _p != o._p;
	};
};

struct move_only_iterator : base_iterator<move_only_iterator> {
	move_only_iterator() = default;
	move_only_iterator(const move_only_iterator &) = delete;
	move_only_iterator(move_only_iterator &&) = default;
	move_only_iterator & operator=(move_only_iterator && other) = default;
	base & operator++() noexcept {
		++_p;
		return *this;
	}
	void operator++(int) noexcept {
		++*this;
	}
};

struct copyable_iterator :  base_iterator<copyable_iterator> {
	using base_iterator<copyable_iterator>::base_iterator;
	base & operator++() noexcept {
		++_p;
		return *this;
	}
	base operator++(int) noexcept {
		base o = *this;
		++*this;
		return o;
	}
};

struct copyable_iterator_with_input_tag :  base_iterator<copyable_iterator_with_input_tag> {
	using base_iterator<copyable_iterator_with_input_tag>::base_iterator;

	using iterator_category = ranges::input_iterator_tag;

	base & operator++() noexcept {
		++_p;
		return *this;
	}
	base operator++(int) noexcept {
		base o = *this;
		++*this;
		return o;
	}
};

struct bidi_iterator :  base_iterator<bidi_iterator> {
	using base_iterator<bidi_iterator>::base_iterator;
	base & operator++() noexcept {
		++_p;
		return *this;
	}
	base operator++(int) noexcept {
		base o = *this;
		++*this;
		return o;
	}

	base & operator--() noexcept {
		--_p;
		return *this;
	}
	base operator--(int) noexcept {
		base o = *this;
		--*this;
		return o;
	}
};

struct bidi_iterator_with_fw_tag :  base_iterator<bidi_iterator_with_fw_tag> {
	using base_iterator<bidi_iterator_with_fw_tag>::base_iterator;

	using iterator_category = ranges::forward_iterator_tag;

	base & operator++() noexcept {
		++_p;
		return *this;
	}
	base operator++(int) noexcept {
		base o = *this;
		++*this;
		return o;
	}

	base & operator--() noexcept {
		--_p;
		return *this;
	}
	base operator--(int) noexcept {
		base o = *this;
		--*this;
		return o;
	}
};




// Move only iterators satisfy the requirement of InputIterator
static_assert(ranges::Iterator<move_only_iterator>);
static_assert(ranges::InputIterator<move_only_iterator>);
static_assert(!ranges::ForwardIterator<move_only_iterator>);


static_assert(ranges::InputIterator<copyable_iterator>);
static_assert(ranges::ForwardIterator<copyable_iterator>);


static_assert(ranges::InputIterator<copyable_iterator_with_input_tag>);
static_assert(!ranges::ForwardIterator<copyable_iterator_with_input_tag>);

static_assert(ranges::ForwardIterator<bidi_iterator>);
static_assert(ranges::BidirectionalIterator<bidi_iterator>);
static_assert(!ranges::RandomAccessIterator<bidi_iterator>);

static_assert(ranges::ForwardIterator<bidi_iterator>);
static_assert(!ranges::BidirectionalIterator<bidi_iterator_with_fw_tag>);
static_assert(!ranges::RandomAccessIterator<bidi_iterator_with_fw_tag>);



//template <ranges::ForwardIterator T>
//requires !ranges::detail::TagDerivedFrom<T, ranges::forward_iterator_tag>
void test() {

}
int main() {
	//test<copyable_iterator_with_input_tag>();
}