#ifndef LIBIO_CODEC_HPP
# define LIBIO_CODEC_HPP

# include <string>
# include <streambuf>

template<typename char_t,
		 uint32_t blk_sz = 64>
struct						basic_codec
{
	typedef char_t			char_type;

	static const int		char_size = sizeof(char_type);
	static const int		block_size = blk_sz;

	basic_codec() {}
	virtual ~basic_codec() throw() {}

	virtual size_t			compress(char_t *dst,
									 const char_t *src) = 0;
	virtual size_t			decompress(char_t *dst,
									   const char *src) = 0;
};

template<typename char_t,
		 uint32_t blk_sz = 64>
struct						basic_crypt
{
	typedef char_t			char_type;

	static const int		char_size = sizeof(char_type);
	static const int		block_size = blk_sz;

	basic_crypt() {}
	virtual ~basic_crypt() throw() {}

	virtual size_t			encrypt(char_t *dst,
									const char_t *src) = 0;
	virtual size_t			decrypt(char_t *dst,
									const char_t *src) = 0;
};

template<typename char_t,
		 uint32_t blk_sz = 64,
		 uint32_t mask = 0x14a>
struct						xor_crypt
	: public basic_crypt<char_t, blk_sz>
{
	typedef basic_crypt<char_t, blk_sz> base_type;

	xor_crypt() {}
	virtual ~xor_crypt() throw() {}

	size_t					apply(char_t *dst,
								  const char_t *src)
		{
			char_t			*pbeg(dst), *pcur;
			char_t			*pend(dst + blk_sz * base_type::char_size);
			for (pcur = pbeg; *pcur && pcur != pend; pcur++)
				*pcur = (*src) ^ mask, src++;
			return (pcur - pbeg);
		}
	size_t					decrypt(char_t *dst,
									const char_t *src)
		{
			std::cout << "unXOR" << std::endl;
			return (apply(dst, src));
		}

	size_t					encrypt(char_t *dst,
									 const char_t *src)
		{
			std::cout << "XOR: " << src << std::endl;
			return (apply(dst, src));
		}
};


template<typename char_t,
		 uint32_t blk_sz = 64>
struct						basic_scodec
	: public basic_codec<char_t, blk_sz>
	, public basic_crypt<char_t, blk_sz>
{
	typedef char_t								char_type;
	typedef basic_codec<char_t, blk_sz>			base_type;

	static const int							char_size = base_type::char_size;
	static const int							block_size = base_type::block_size;

	basic_scodec() {}
	virtual ~basic_scodec() throw() {}

	virtual size_t								compress(char_t *dst,
														 const char_t *src) = 0;
	virtual size_t								decompress(char_t *dst,
														   const char *src) = 0;

	virtual size_t								encrypt(char_t *dst,
														const char_t *src) = 0;

	virtual size_t								decrypt(char_t *dst,
														const char *src) = 0;
};

#endif
