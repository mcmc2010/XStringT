/*
 * XStringT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __XSTRINGT_H__
#define __XSTRINGT_H__

#include <cstddef>
#include <stdexcept>
#include <iostream>

#include "XStringBaseT.h"

//
namespace XStringT
{

/*-----------------------------------------------------
StringAU8 Class
-------------------------------------------------------*/
class StringAU8
{
public:
	typedef	StringBase::size_type									size_type;					//!< Unsigned type used for size values and indices
	typedef	StringBaseT<StringAU8, utf8>::value_type				value_type;					//!< Basic 'code point' type used for String (utf32)
	typedef	StringBaseT<StringAU8, utf8>::reference&				reference;					//!< Type used for utf32 code point references
	typedef	StringBaseT<StringAU8, utf8>::const_reference&			const_reference;			//!< Type used for constant utf32 code point references
	typedef	StringBaseT<StringAU8, utf8>::pointer*					pointer;					//!< Type used for utf32 code point pointers
	typedef	StringBaseT<StringAU8, utf8>::const_pointer*			const_pointer;				//!< Type used for constant utf32 code point pointers

public:
	StringAU8(void);
	virtual ~StringAU8(void);

	//////////////////////////////////////////////////////////////////////////
	// Construction via XStringT::StringU8
	//////////////////////////////////////////////////////////////////////////
	StringAU8(const StringAU8& str)
	{
		init();
		assign(str);
	}

	StringAU8(const StringAU8& str, size_type str_idx, size_type str_num = StringBase::npos)
	{
		init();
		assign(str, str_idx, str_num);
	}

	StringAU8(const utf8* utf8_str)
	{
		init();
		assign(utf8_str);
	}

	StringAU8(const utf8* utf8_str, size_type chars_len)
	{
		init();
		assign(utf8_str, chars_len);
	}

public:
	StringAU8&	operator=(const StringAU8& str)
	{
		return assign(str);
	}

	StringAU8&	operator=(const utf8* utf8_str)
	{
		return assign(utf8_str, total_length(utf8_str));
	}

	StringAU8&	operator+=(const StringAU8& str)
	{
		return append(str);
	}

	StringAU8&	operator+=(const utf8* utf8_str)
	{
		return append(utf8_str, total_length(utf8_str));
	}

	reference	operator[](size_type idx)
	{
		return (d_stringU8.ptr()[idx]);
	}

	value_type	operator[](size_type idx) const
	{
		return d_stringU8.ptr()[idx];
	}

	reference	at(size_type idx)
	{
		if (d_stringU8.length() <= idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::at().")); }

		return d_stringU8.ptr()[idx];
	}

	const_reference	at(size_type idx) const
	{
		if (d_stringU8.length() <= idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::at().")); }

		return d_stringU8.ptr()[idx];
	}

	//////////////////////////////////////////////////////////////////////////
	// C-Strings and arrays
	//////////////////////////////////////////////////////////////////////////
	virtual const char* c_str(void) const
	{
		return (const char*)d_stringU8.ptr();
	}

	const utf32* data(void) const
	{
		return d_stringU32.ptr();
	}

	//////////////////////////////////////////////////////////////////////////
	// Size operations
	//////////////////////////////////////////////////////////////////////////
	size_type	size(void) const
	{
		return d_stringU8.size();
	}

	size_type	length(void) const
	{
		return d_stringU8.size();
	}

	size_type	max_size(void) const
	{
		return d_stringU8.max_size();
	}
	
	size_type	max_length(void) const
	{
		return d_stringU8.max_length();
	}

	bool		empty(void) const
	{
		return	d_stringU8.empty();
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// Erasing characters
	//////////////////////////////////////////////////////////////////////////
	void		clear(void)
	{
		d_stringU8.clear();
		d_stringU32.clear();
	}

	StringAU8&	erase(void)
	{
		clear();
		return *this;
	}

	StringAU8&	erase(size_type idx)
	{
		return erase(idx, 1);
	}

	StringAU8&	erase(size_type idx, size_type len)
	{
        // cover the no-op case.
        if (len == 0)
		{ return *this; }

		d_stringU8.erase(idx, len);

		size_type enc_sze = encoded_size(d_stringU8.ptr(), d_stringU8.length());

		d_stringU32.reserve(enc_sze);
		encode(d_stringU8.ptr(), d_stringU32.ptr(), enc_sze, d_stringU8.size());
		d_stringU32.setlen(enc_sze);
		return	*this;
	}
		
	//////////////////////////////////////////////////////////////////////////
	// Assignment Functions
	//////////////////////////////////////////////////////////////////////////
	StringAU8&	assign(const StringAU8& str, size_type str_idx = 0, size_type str_num = StringBase::npos)
	{
		d_stringU8.assign(str.d_stringU8, str_idx, str_num);

		size_type enc_sze = encoded_size(d_stringU8.ptr(), d_stringU8.length());

		d_stringU32.reserve(enc_sze);
		encode(d_stringU8.ptr(), d_stringU32.ptr(), enc_sze, d_stringU8.size());
		d_stringU32.setlen(enc_sze);
		return *this;
	}

	StringAU8&	assign(const utf8* utf8_str)
	{
		return assign(utf8_str, total_length(utf8_str));
	}

	StringAU8&	assign(const utf8* utf8_str, size_type str_num)
	{
		d_stringU8.assign(utf8_str, str_num);

		size_type enc_sze = encoded_size(d_stringU8.ptr(), d_stringU8.length());

		d_stringU32.reserve(enc_sze);
		encode(d_stringU8.ptr(), d_stringU32.ptr(), enc_sze, d_stringU8.size());
		d_stringU32.setlen(enc_sze);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Appending Functions
	//////////////////////////////////////////////////////////////////////////
	StringAU8& append(const StringAU8& str, size_type str_idx = 0, size_type str_num = StringBase::npos)
	{
		d_stringU8.append(str.d_stringU8, str_idx, str_num);

		size_type enc_sze = encoded_size(d_stringU8.ptr(), d_stringU8.length());

		d_stringU32.reserve(enc_sze);
		encode(d_stringU8.ptr(), d_stringU32.ptr(), enc_sze, d_stringU8.size());
		d_stringU32.setlen(enc_sze);
		return *this;
	}

	StringAU8& append(const utf8* utf8_str)
	{
		return append(utf8_str, total_length(utf8_str));
	}

	StringAU8& append(const utf8* utf8_str, size_type len)
	{
		d_stringU8.append(utf8_str, len);

		size_type enc_sze = encoded_size(d_stringU8.ptr(), d_stringU8.length());

		d_stringU32.reserve(enc_sze);
		encode(d_stringU8.ptr(), d_stringU32.ptr(), enc_sze, d_stringU8.size());
		d_stringU32.setlen(enc_sze);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Substring
	//////////////////////////////////////////////////////////////////////////
	StringAU8 substr(size_type idx = 0, size_type len = StringBase::npos) const
	{
		if (d_stringU8.length() < idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for this XStringT::StringAU8::substr().")); }

		return StringAU8(*this, idx, len);
	}

	size_type	copy(utf8* buf, size_type len = StringBase::npos, size_type idx = 0) const
	{
		if (d_stringU8.length() < idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::StringAU8::copy().")); }

		if (len == StringBase::npos)
		{ len = d_stringU32.length(); }

		return encode(&data()[idx], buf, StringBase::npos, len);
	}

protected:
	virtual void	init(void)
	{
		d_stringU8.init();
		d_stringU32.init();
	}

	// return number of utf32 code units required to re-encode given utf8 data as utf32.  len is number of code units in 'buf'.
	size_type encoded_size(utf32 code_point) const;
	size_type encoded_size(const utf8* buf, size_type len) const;
	// encoding functions
	// for all:
	//	src_len is in code units, or 0 for null terminated string.
	//	dest_len is in code units.
	//	returns number of code units put into dest buffer.
	size_type encode(const utf32* src, utf8* dest, size_type dest_len, size_type src_len = 0) const;
	size_type encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len = 0) const;

	// return number of code units in a null terminated string
	__inline size_type total_length(const utf8* utf8_str) const
	{
		size_type cnt = 0; while (*utf8_str++){ cnt++; }
		return cnt;
	}
	__inline size_type total_length(const utf32* utf32_str) const
	{
		size_type cnt = 0; while (*utf32_str++){ cnt++; }
		return cnt;
	}

protected:
	StringBaseT<StringAU8, utf32>		d_stringU32;
	StringBaseT<StringAU8, utf8>		d_stringU8;
};

//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
StringAU8	operator+(const StringAU8& str1, const StringAU8& str2);
StringAU8	operator+(const StringAU8& str, const utf8* utf8_str);
StringAU8	operator+(const utf8* utf8_str, const StringAU8& str);

};//namespace XStringT

#endif /* __XSTRINGT_H__ */
