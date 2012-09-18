// Copyright (c) 2010-2012 niXman (i dot nixman dog gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__binary__buffer_serializer_hpp_included_
#define _yas__binary__buffer_serializer_hpp_included_

#include <cassert>

#include <yas/detail/config/config.hpp>
#include <yas/detail/tools/buffers.hpp>
#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/type_traits/selector.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

template<>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::out,
	intrusive_buffer
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, const intrusive_buffer& buf) {
		ar.write(reinterpret_cast<const char*>(&buf.size), sizeof(buf.size));
		ar.write(reinterpret_cast<const char*>(buf.data), buf.size);
		return ar;
	}
};

/***************************************************************************/

#if defined(YAS_SHARED_BUFFER_USE_STD_SHARED_PTR) || \
	defined(YAS_SHARED_BUFFER_USE_BOOST_SHARED_PTR)

template<>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::out,
	shared_buffer
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, const shared_buffer& buf) {
		ar.write(reinterpret_cast<const char*>(&buf.size), sizeof(yas::uint32_t));
		ar.write(reinterpret_cast<const char*>(buf.data.get()), buf.size);
		return ar;
	}
};

template<>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::in,
	shared_buffer
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, shared_buffer& buf) {
		yas::uint32_t size = 0;
		ar.read(reinterpret_cast<char*>(&size), sizeof(size));
		buf.data.reset(new char[size+1], &shared_buffer::deleter);
		ar.read(reinterpret_cast<char*>(buf.data.get()), size);
		buf.size = size;
		buf.data.get()[size] = 0;
		return ar;
	}
};

#endif

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // _yas__binary__buffer_serializer_hpp_included_
