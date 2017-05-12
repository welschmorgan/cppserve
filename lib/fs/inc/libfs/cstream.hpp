// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   cstream.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 17:55:25 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 19:28:08 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef LIBFS_CSTREAM_HPP
# define LIBFS_CSTREAM_HPP

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
	template<typename char_t>
	class										basic_cstream_base
	{
	private:
		basic_cstream_base(const basic_cstream_base &) = delete;
		basic_cstream_base &operator=(const basic_cstream_base &) = delete;

	public:
		typedef basic_cstream_base<char_t>		base_type;
		typedef char_t							char_type;
		typedef std::basic_string<char_t>		str_type;
		typedef std::streamsize					size_type;

	protected:
		const size_type							block_size;

	public:
		basic_cstream_base(const size_type blocksz)
			: block_size(blocksz)
			{}

		virtual ~basic_cstream_base() throw()
			{}

		virtual void							decompress(char_type *dest,
														   const char_type *source) = 0;
		virtual void							compress(char_type *dest,
														 const char_type *source) = 0;
	};

	template<typename char_t>
	class										basic_cbuf
		: public std::basic_streambuf<char_t>
	{
	private:
		basic_cbuf(const basic_cbuf &rhs) = delete;
		basic_cbuf								&operator=(const basic_cbuf &rhs) = delete;

	public:
		typedef basic_cbuf<char_t>				self_type;
		typedef std::basic_streambuf<char_t>	base_type;
		typedef basic_cstream_base<char_t>		parent_type;
		typedef typename base_type::int_type	int_type;
		typedef char_t							char_type;
		typedef std::basic_string<char_t>		str_type;
		typedef std::streamsize					size_type;

		static const size_type					default_block_size;


	protected:
		basic_cstream_base<char_t>				*m_parent;
		base_type								*m_sbuf;
		char									*m_buf;
		const size_type							m_block_size;

	public:
		basic_cbuf(base_type *sbuf = NULL,
				   const size_t blocksz = basic_cstream_base<char_t>::default_block_size,
				   parent_type *parent = NULL)
			: base_type()
			, m_parent(parent)
			, m_sbuf(sbuf)
			, m_buf(new char_type[blocksz])
			, m_block_size(blocksz)
			{
				memset((void*)m_buf, char_type(), sizeof(char_type) * blocksz);
				base_type::setg(m_buf, m_buf, m_buf + m_block_size);
				base_type::setp(m_buf, m_buf + m_block_size);
			}
		virtual ~basic_cbuf() throw()
			{
				sync();
				if (m_buf)
					delete m_buf, m_buf = NULL;
			}
		int_type								overflow(int_type c)
			{
				std::streamsize n = base_type::pptr () - base_type::pbase ();
				if (n && sync())
					return (EOF);
				if (c != EOF)
				{
					m_buf[0] = c;
					m_buf[1] = 0;
				}
				base_type::pbump (-n);  // Reset pptr().
				return (0);
			}
		int_type								underflow()
			{
				m_buf[0] = m_sbuf->sbumpc();
				m_buf[1] = 0;
				std::cerr << "uflow" << m_buf << std::endl;
				base_type::setg(m_buf, m_buf, m_buf + 1);
				return (m_buf[0]);
			}
		int_type								sync()
			{
				std::cerr << "sync" << m_buf << std::endl;
				return (base_type::sync());
			}

	}
;
	template<typename char_t>
	const std::streamsize basic_cbuf<char_t>::default_block_size = 64;


	template<typename char_t>
	class										basic_ocstream
		: public std::basic_ostream<char_t>
		, public basic_cstream_base<char_t>
	{
	private:
		basic_ocstream(const basic_ocstream &rhs) = delete;
		basic_ocstream							&operator=(const basic_ocstream &rhs) = delete;

	public:
		typedef basic_ocstream<char_t>			self_type;
		typedef std::basic_ostream<char_t>		base_type;
		typedef basic_cbuf<char_t>				buf_type;
		typedef std::basic_string<char_t>		str_type;
		typedef char_t							char_type;
		typedef typename buf_type::size_type	size_type;

	public:
		explicit basic_ocstream(buf_type *sbuf,
								size_type block_size = buf_type::default_block_size)
			: base_type(new buf_type(sbuf, block_size, this))
			, basic_cstream_base<char_t>(block_size)
			{}
		basic_ocstream(base_type &os,
					   size_type block_size = buf_type::default_block_size)
			: base_type(new buf_type(os.rdbuf(), block_size, this))
			, basic_cstream_base<char_t>(block_size)
			{}
		virtual ~basic_ocstream() throw()
			{ delete base_type::rdbuf(); }

		void								decompress(char_type *dest,
													   const char_type *source)
			{
				(void)dest;
				(void)source;
			}
		void							compress(char_type *dest,
												 const char_type *source)
			{
				(void)dest;
				(void)source;
			}
	};

};

#endif
