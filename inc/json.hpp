// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   json.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/03/25 20:52:06 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 22:03:24 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef JSON_HPP
# define JSON_HPP

# include <stdexcept>
# include "string.hpp"
# include "serializer.hpp"
# include <list>
# include <map>
# include <stack>
# include <memory>
# include <vector>
# include <sstream>
# include <functional>

namespace json
{
# if defined(UNICODE)
	typedef std::wstring							string;
	typedef std::wstringstream						string_stream;
# else
	typedef std::string								string;
	typedef std::string								string_stream;
#endif
	typedef std::vector<string>						string_list;

	class					node;
	typedef std::stack<node*> node_stack;

	class					error
		: public std::runtime_error
	{
	protected:
		String				m_file;
		size_t				m_line_id;
		size_t				m_col_id;
		int					m_code;

	public:
		error(const String &file,
				  size_t line_id, size_t col_id,
				  const String &message,
				  int code = 1);
		error(const error &);
		virtual ~error() throw();

		error			&operator=(const error &rk);

		static String		make_message(const String &file,
										size_t line_id,
										size_t col_id,
										const String &message);

		int					code() const throw();

		String				file() const throw();
		size_t				line() const throw();
		size_t				col() const throw();
	};

	class					parse_error
		: public error
	{
	public:
		parse_error(const String &file,
					size_t line_id, size_t col_id,
					const String &message,
					int code = 1);
		parse_error(const parse_error &);
		virtual ~parse_error() throw();

		parse_error			&operator=(const parse_error &rk);
	};

	class												parser;
	class												token {
	public:
		typedef	std::function<bool() throw(error)>	Handler;

		token();
		token(
			const String &trigger,
			const String &name,
			const Handler &handler,
			const size_t &line_id = 0,
			const size_t &ch_id = 0);
		token(const token &);
		~token() throw();

		token			&operator=(const token &);
		bool				operator==(String::value_type ch) const throw();
		bool				operator()() throw(error);

		const String		&trigger() const throw();
		const String		&name() const throw();
		const Handler		&handler() const throw();
		size_t				line() const throw();
		size_t				column() const throw();

		token				&trigger(const String &);
		token				&name(const String &);
		token				&handler(const Handler &);
		token				&line(size_t);
		token				&column(size_t);

		bool				triggered(String::value_type ch) const throw(error);
		bool				handle() throw(error);

	protected:
		String											m_trigger;
		String											m_name;
		Handler											m_handler;
		size_t											m_line_id;
		size_t											m_ch_id;
		friend class									parser;
	};

	typedef	std::map<String, token>					token_map;
	typedef	std::reference_wrapper<token>			token_ref;

	class							parser
	{
	public:
		parser();
		virtual						~parser() throw();

		node						*parse(std::istream &is) throw(parse_error);

		void						clear();

		node						*data() throw();
		const node					*data() const throw();

	protected:
		bool						parse_comma() throw(error);
		bool						parse_quote() throw(error);
		bool						parse_key() throw(error);
		bool						parse_node_open() throw(error);
		bool						parse_node_close() throw(error);

		void						_eat_ch();
		inline void					_except(const String &msg, int code = 1) throw(parse_error);
		token						&_declare_token(const String &name,
													const String &trigger,
													token::Handler handler);

	public:
		static String				&trim_quotes(String &str);
		static inline std::string	&ltrim(std::string &s);
		static inline std::string	&rtrim(std::string &s);
		static inline std::string	&trim(std::string &s);

	protected:
		node				*m_root;
		node_stack			m_nodes;
		token_map			m_tokens;
		size_t				m_line_id;
		size_t				m_ch_id;
		String				m_line;
		String				m_accu;
		String::iterator	m_ch;
		bool				m_root_found;
		bool				m_key_found;
		String				m_key;
		bool				m_token_found;
		token_ref			m_token;
		token_ref			m_last_token;
	};

	enum									type_id
	{
		JDT_STRING,
		JDT_NUMBER,
		JDT_BOOLEAN,
		JDT_FUNCTION
	};

	typedef std::map<type_id, string>		type_name_map;
	extern const type_name_map				*get_type_names();

	IStream									&operator>>(IStream &is,
														type_id &t);
	OStream									&operator<<(OStream &os,
														type_id t);
	struct									var
	{
		type_id								type;
		String								key;
		String								value;
	};

	typedef std::list<var>					var_list;
	typedef std::list<node*>				node_list;

	enum									node_type
	{
		JNT_NONE = 0,
		JNT_ROOT_OBJECT,
		JNT_OBJECT,
		JNT_ARRAY,
		JNT_COUNT
	};

	template<typename IStreamT = std::istream,
			 typename OStreamT = std::ostream>
	class					serializer
	{
		serializer(const serializer &) = delete;
		serializer &operator=(const serializer &) = delete;

	public:
		serializer() {}
		virtual ~serializer() throw() {}

		virtual void						serialize(const node &from,
													  OStreamT &to,
													  unsigned short indent = 0) {
			(void)from;
			(void)to;
			(void)indent;
		};
		virtual void						deserialize(IStreamT &from,
														node &to) {
			(void)from;
			(void)to;
		}
	};

	class					text_serializer
		: public serializer<>
	{
		parser			m_parser;

	public:
		text_serializer();
		~text_serializer() throw();

		void						serialize(const node &from,
											  std::ostream &to,
											  unsigned short indent = 0);
		void						deserialize(std::istream &from,
												node &to);
	};

	class					debug_serializer
		: public serializer<>
	{
	public:
		debug_serializer();
		~debug_serializer() throw();

		void						serialize(const node &from,
											  std::ostream &to,
											  unsigned short indent = 0);
		void						deserialize(std::istream &from,
												node &to);
	};

# include <typeinfo>

	class					node
	{
	protected:
		node				*m_parent;
		node_type			m_type;
		String				m_name;
		var_list			m_data;
		node_list			m_children;

	public:
		node(node_type t = JNT_NONE, const String &name = String(), node *parent = NULL);
		node(const node &rhs);
		virtual				~node();

		node			&operator=(const node &rhs);

		node_type		type() const throw();
		node			&type(node_type t) throw();

		void				clear() throw();

		node_list		*children() throw();
		const node_list	*children() const throw();

		node			*parent() throw();
		node			&parent(node *p) throw();
		const node		*parent() const throw();

		node			*create_child(const String &name,
										 node_type type) throw();
		node			*child(const String &name,
								   bool recurs = false) throw();
		const node		*child(const String &name,
								   bool recurs = false) const throw();

		operator			String() const throw();

		bool				empty() const throw();

		const String		&name() const throw();
		node				&name(const String &name) throw();

		json::var			*data(const String &name) throw();
		const json::var		*data(const String &name) const throw();
		node				&data(const String &name,
								  const String &val) throw ();

		var_list			*datas() throw();
		const var_list		*datas() const throw();

		String				stringify(unsigned short indent = 0) const throw();

		String				type_name() const throw();
		static String		type_name(node_type t) throw();

		template<class T>
		inline T				data(const String &name,
									 bool required = true,
									 T	def_val = T())
			{
				T					ret;
				std::stringstream	ss;
				var			*data(this->data(name));
				node			*child(this->child(name));

				if (child != NULL)
				{
					for (auto data : *child->datas())
					{
						if (child->type() == JNT_ARRAY)
							ss.str(data.value + ";");
						else
							ss.str(data.key + "=" + data.value + ";");
					}
				}
				else if (data)
				{
					ss.str(data->value);
				}
				if (!(ss >> ret))
				{
					if (required)
					{
						if (ss.str().empty())
							throw std::runtime_error("Required data '" + name + "' is empty!");
						throw std::runtime_error("Cannot extract data '" + name + "' as " + typeid(def_val).name() + ": " + ss.str() + "!");
					}
					ret = def_val;
				}
				return (ret);
			}



	private:
		String				_stringify(const node *node,
									   unsigned short indent) const throw();
	};

	template<>
	inline bool			node::data<bool>(const String &name,
											 bool required,
											 bool def_val)
	{
		bool		ret;
		std::stringstream ss;
		var	*data(this->data(name));
		node	*child(this->child(name));

		if (child != NULL)
		{
			for (auto data : *child->datas())
			{
				if (child->type() == JNT_ARRAY)
					ss << data.value;
				else
					ss << data.key + "=" + data.value;
			}
		} else {
			if (data)
				ss << data->value;
		}
		if (!(ss >> std::boolalpha >> ret))
		{
			if (required)
			{
				if (ss.str().empty())
					throw std::runtime_error("Required data '" + name + "' is empty!");
				throw std::runtime_error("Cannot extract data '" + name + "' as " + typeid(def_val).name() + ": " + ss.str() + "!");
			}
			ret = def_val;
		}
		return (ret);
	}

	template<>
	inline String			node::data<String>(const String &name,
												   bool required,
												   String def_val)
	{
		String		ret;
		var	*data(this->data(name));
		node	*child(this->child(name));

		if (child != NULL)
		{
			for (auto data : *child->datas())
			{
				if (child->type() == JNT_ARRAY)
					ret += data.value + ";";
				else
					ret += data.key + "=" + data.value + ";";
			}
		} else {
			if (data)
				ret += data->value;
		}
		if (!ret.empty() && ret.at(0) == '"')
			ret.erase(ret.begin());
		if (!ret.empty() && ret.at(ret.size() - 1) == '"')
			ret.erase(ret.begin() + ret.size() - 1);
		if (ret.empty())
		{
			if (required)
			{
				throw std::runtime_error("Required data '" + name + "' is empty!");
			}
			ret = def_val;
		}
		return (ret);
	}

	template<>
	inline string_list		node::data<string_list>(const String &name,
													   bool required,
													   string_list def_val)
	{
		string_list	ret;
		var	*data(this->data(name));
		node	*child(this->child(name));

		if (child != NULL)
		{
			for (auto data : *child->datas())
			{
				if (child->type() == JNT_ARRAY)
					ret.push_back(data.value);
				else
					ret.push_back(data.key + "=" + data.value + ";");
			}
		} else {
			if (data)
				ret.push_back(data->value);
		}
		string_list::iterator it;
		for(it = ret.begin(); it != ret.end(); it++)
		{
			if (!it->empty() && it->at(0) == '"')
				it->erase(it->begin());
			if (!it->empty() && it->at(it->size() - 1) == '"')
				it->erase(it->begin() + it->size() - 1);
		}
		if (ret.empty())
		{
			if (required)
			{
				throw std::runtime_error("Required data '" + name + "' is empty!");
			}
			ret = def_val;
		}
		return (ret);
	}

};

#endif
