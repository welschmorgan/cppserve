// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   path.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 13:12:35 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 17:58:27 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef LIBFS_PATH_HPP
# define LIBFS_PATH_HPP

# include <iostream>

# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <memory>
# include <string.h>
# include "except.hpp"

namespace fs
{
	template<typename string_t>
	class										basic_path
	{
	public:
		typedef basic_path<string_t>			self_type;
		typedef string_t						str_type;
		typedef typename str_type::value_type	char_type;

		static const str_type					sep;

	protected:
		str_type								m_str;

	public:
		explicit
		basic_path(const str_type &str = str_type())
			: m_str(str)
			{}
		basic_path(const basic_path<string_t> &rhs)
			: m_str(rhs.m_str)
			{}
		virtual ~basic_path() throw()
			{}
		basic_path								&operator=(const basic_path<string_t> &rhs)
			{
				m_str = rhs.m_str;
				return (*this);
			}


		explicit
		operator								str_type() const throw()
			{ return (m_str); }

		inline str_type								hash_str() const throw(error)
			{ return (std::to_string(hash())); }

		size_t									hash() const throw(error)
			{
				static const std::streamsize	block_size = 4096;
				typedef std::map<str_type, size_t>	cache_map;
				cache_map						cache;
				typename cache_map::iterator	it;
				std::ifstream					ifs;
				std::hash<str_type>				hasher;
				static char_type				buf[block_size] = { 0 };
				str_type						bigbuf;
				std::streamoff					readsz;
				std::streamoff					offs;
				std::streamoff					endoffs;

				if ((it = cache.find(m_str)) != cache.end())
					return (it->second);
				ifs.open(m_str);
				if (!ifs)
					throw error("cannot hash, failed to open ifle!", m_str);
				offs = 0;
				ifs.seekg(0, std::ios::end);
				endoffs = ifs.tellg();
				ifs.seekg(0, std::ios::beg);
				bigbuf.reserve(endoffs);
				while (ifs
					   && (offs < endoffs))
				{
					if ((endoffs - offs) >= block_size)
						readsz = block_size;
					else
						readsz = (endoffs - offs);
					readsz = ifs.readsome(&buf[0], readsz);
					if (readsz > 0)
					{
						bigbuf += buf;
						offs += readsz;
					}
				}
				std::pair<typename cache_map::iterator,
						  bool>		insit;
				insit = cache.insert({m_str, hasher(bigbuf)});
				if (!insit.second)
					return (size_t(0));
				return (insit.first->second);
			}

		size_t									size() const noexcept
			{
				typedef std::map<str_type, size_t>	cache_map;
				static cache_map					cache;
				typename cache_map::iterator		it;
				std::ifstream						ifs;
				size_t								sz(0);
				if ((it = cache.find(m_str)) != cache.end())
					return (it->second);
				try {
					ifs.open(m_str, std::ios::in);
					if (ifs)
					{
						ifs.seekg(0, std::ios::end);
						sz = ifs.tellg();
						ifs.close();
					}
					std::pair<typename cache_map::iterator,
							  bool>					insit;
					insit = cache.insert({m_str, sz});
					if (!insit.second)
						sz = 0;
				} catch (std::exception &e) {
					std::cerr << e.what() << std::endl;
				}
				return (sz);
			}

		bool									empty() const noexcept
			{ return (m_str.empty() || !size()); }

		bool									exists() const noexcept
			{
				if (!m_str.empty())
					return (false);
				std::ifstream ifs;
				ifs.open(m_str);
				return ((bool)ifs);
			}

		inline str_type							str() const noexcept
			{ return (m_str); }

		inline const char_type					*c_str() const noexcept
			{ return (m_str.c_str()); }

		str_type								dirname() const noexcept
			{
				size_t							pos;
				str_type						ret;

				if ((pos = m_str.find_last_of(self_type::sep))
					!= std::string::npos)
					ret = m_str.substr(0, pos);
				return (ret);
			}

		str_type								basename() const noexcept
			{
				size_t							pos;
				size_t							sepsz(self_type::sep.size());
				str_type						ret(m_str);

				if ((pos = m_str.find_last_of(self_type::sep))
					!= std::string::npos)
					ret = m_str.substr(pos + sepsz, m_str.size() - (pos + sepsz));
				return (ret);
			}

		str_type								ext() const noexcept
			{
				str_type						xt(basename());
				size_t							pos;
				if ((pos = xt.find(sep)) != std::string::npos)
					xt = xt.substr(pos + 1, xt.size() - (pos + 1));
				else
					xt.clear();
				return (xt);
			}
	};

	ostream								&operator<<(ostream &os,
													const path &p)
	{
		os << p.str();
		return (os);
	}

	template<typename str_t>
	const typename basic_path<str_t>::str_type		basic_path<str_t>::sep("/");

	template<typename str_t,
			 template<typename,
					  template<typename> class>
			 class container_t = std::vector,
			 template<typename> class alloc_t = std::allocator>
	class										path_list
		: private  container_t<str_t, alloc_t>
	{
	public:
		typedef container_t<str_t, alloc_t>		base_type;
		typedef path_list<str_t, container_t>	self_type;
		typedef str_t							str_type;
		typedef typename str_type::value_type	char_type;
		typedef alloc_t<str_t>					alloc_type;

		typedef typename base_type::iterator				iterator;
		typedef typename base_type::const_iterator			const_iterator;
		typedef typename base_type::reverse_iterator		reverse_iterator;
		typedef typename base_type::const_reverse_iterator	const_reverse_iterator;

		typedef typename base_type::reference		reference;
		typedef typename base_type::const_reference	const_reference;
		typedef typename base_type::pointer			pointer;
		typedef typename base_type::const_pointer	const_pointer;

	public:
		path_list() : base_type() {}
		virtual ~path_list() throw() {}

		iterator								begin()
			{ return (base_type::begin()); }
		iterator								end()
			{ return (base_type::end()); }

		const_iterator							begin() const
			{ return (base_type::cbegin()); }
		const_iterator							end() const
			{ return (base_type::cend()); }

		reverse_iterator						rbegin()
			{ return (base_type::begin()); }
		reverse_iterator						rend()
			{ return (base_type::end()); }

		const_reverse_iterator					rbegin() const
			{ return (base_type::cbegin()); }
		const_reverse_iterator					rend() const
			{ return (base_type::cend()); }

		const_reverse_iterator					crbegin() const
			{ return (base_type::cbegin()); }
		const_reverse_iterator					crend() const
			{ return (base_type::cend()); }

		size_t									size() const noexcept
			{ return (base_type::size()); }
		size_t									max_size() const noexcept
			{ return (base_type::max_size()); }
		size_t									capacity() const noexcept
			{ return (base_type::capacity()); }

		void									resize(size_t n,
													   const str_type &val)
			{ base_type::resize(n, val); }
		void									resize(size_t n)
			{ base_type::resize(n); }

		bool									empty() const noexcept
			{ return (base_type::empty()); }

		void									reserve(size_t n)
			{ base_type::reserve(n); }

		void									shrink_to_fit()
			{ base_type::shrink_to_fit(); }

		reference								at(size_t n)
			{ return (base_type::at(n)); }
		const_reference							at(size_t n) const
			{ return (base_type::at(n)); }

		reference								operator[](size_t n)
			{ return (base_type::operator[](n)); }
		const_reference							operator[](size_t n) const
			{ return (base_type::operator[](n)); }

		reference								front()
			{ return (base_type::front()); }
		const_reference							front() const
			{ return (base_type::front()); }

		str_type								*data() noexcept
			{ return (base_type::data()); }
		const str_type							*data() const noexcept
			{ return (base_type::data()); }

		reference								back()
			{ return (base_type::back()); }
		const_reference							back() const
			{ return (base_type::back()); }

		template<class input_iter>
		void									assign(input_iter first,
													   input_iter last)
			{ base_type::assign(first, last); }

		void									assign(size_t n,
													   const str_type &val)
			{ base_type::assign(n, val); }

		void									assign(std::initializer_list<str_type> il)
			{ base_type::assign(il); }

		void									push_back(str_type &&val)
			{ base_type::push_back(val); }
		void									push_back(const str_type &val)
			{ base_type::push_back(val); }

		void									pop_back()
			{ base_type::pop_back(); }

		iterator								insert(const_iterator position,
													   const str_type &val)
			{ return (base_type::insert(position, val)); }
		iterator								insert(const_iterator position,
													   size_t n,
													   const str_type &val)
			{ return (base_type::insert(position, n, val)); }
		template<typename input_iter>
		iterator								insert(const_iterator position,
													   input_iter first,
													   input_iter last)
			{ return (base_type::insert(position, first, last)); }
		iterator								insert(const_iterator position,
													   str_type &&val)
			{ return (base_type::insert(position, val)); }
		iterator								insert(const_iterator position,
													   std::initializer_list<str_type> il)
			{ return (base_type::insert(position, il)); }

		iterator								erase(const_iterator position)
			{ return (base_type::erase(position)); }

		iterator								erase(const_iterator first,
													  const_iterator last)
			{ return (base_type::erase(first, last)); }

		void									swap(base_type &x)
			{ base_type::swap(x); }

		void									clear() noexcept
			{ base_type::clear(); }

		template<class... args_t>
		iterator								emplace(const_iterator position,
														args_t&&... args)
			{ return (base_type::emplace(position, args...)); }

		template<class... args_t>
		void									emplace_back(args_t&&... args)
			{ base_type::emplace_back(args...); }

		alloc_type								get_allocator() const noexcept
			{ return (base_type::get_allocator()); }

		bool									operator==(const self_type &rhs)
			{ return (base_type::operator==(rhs)); }
		bool									operator!=(const self_type &rhs)
			{ return (base_type::operator!=(rhs)); }
		bool									operator<(const self_type &rhs)
			{ return (base_type::operator<(rhs)); }
		bool									operator>(const self_type &rhs)
			{ return (base_type::operator>(rhs)); }
		bool									operator<=(const self_type &rhs)
			{ return (base_type::operator<=(rhs)); }
		bool									operator>=(const self_type &rhs)
			{ return (base_type::operator>=(rhs)); }
	};

};

#endif
