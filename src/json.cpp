// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   json.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/03/25 20:55:06 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 22:04:04 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "json.hpp"
#include "path.hpp"
#include <iostream>
#include <sstream>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace json
{
	const type_name_map						*get_type_names()
	{
		static const type_name_map				names
		{
			{ JDT_STRING, "string" },
			{ JDT_NUMBER, "number" },
			{ JDT_BOOLEAN, "boolean" },
			{ JDT_FUNCTION, "function" }
		};
		return (&names);
	}

	IStream										&operator>>(IStream &is,
															type_id &t)
	{
		const type_name_map						*names;
		type_name_map::const_iterator			name;
		String									word;

		names = get_type_names();
		if (!(is >> word))
			return (is);
		for (name = names->begin(); name != names->end(); name++)
		{
			if (name->second == word)
				break ;
		}
		if (name == names->end())
			is.setstate(std::ios::failbit);
		else
			t = name->first;
		return (is);
	}

	OStream										&operator<<(OStream &os,
															type_id t)
	{
		const type_name_map						*names;
		type_name_map::const_iterator			name;

		names = get_type_names();
		name = names->find(t);
		if (name == names->end())
			os.setstate(std::ios::failbit);
		else
			os << name->second;
		return (os);
	}


	debug_serializer::debug_serializer()
		: serializer()
	{}
	debug_serializer::~debug_serializer() throw()
	{}

	void						debug_serializer::serialize(const node &from,
															std::ostream &to,
															unsigned short indent)
	{
		node_list::const_iterator	child;
		var_list::const_iterator	data;
		String							indent_str(indent, '\t');
		String							next_indentStr(indent + 1, '\t');

		if (indent > 0)
			indent_str += "| ";
		to << indent_str << from.type_name() << "(" << from.name() << ")" << std::endl;
		to << indent_str << "+s(data):" << std::endl;
		for (data = from.datas()->begin(); data != from.datas()->end(); data++)
			to << indent_str << "\t-" << data->key << " = " << data->value << "," << std::endl;
		to << indent_str << "+s(" << from.children()->size() << " children):" << std::endl;
		for (child = from.children()->begin(); child != from.children()->end(); child++)
		{
			serialize(**child, to, indent + 1);
			to << std::endl;
		}
	}

	void					debug_serializer::deserialize(std::istream &from,
														  node &to)
	{
		(void)from;
		(void)to;
		throw error("stream", 0, 0, "cannot deserialize json in human mode!");
	}

	text_serializer::text_serializer()
		: m_parser()
	{}
	text_serializer::~text_serializer() throw()
	{}

	void						text_serializer::serialize(const node &from,
														   std::ostream &to,
														   unsigned short indent)
	{
		node_list::const_iterator	child;
		var_list::const_iterator	data;
		String							indent_str(indent, '\t');
		String							next_indentStr(indent + 1, '\t');
		bool							is_array(from.type() == JNT_ARRAY);
		bool							is_object(from.type() == JNT_OBJECT);
		bool							is_inArray(from.parent() && from.parent()->type() == JNT_ARRAY);
		bool							is_inObject(from.parent() && from.parent()->type() == JNT_OBJECT);

		(void)is_inObject;
		to << indent_str;
		if (!is_inArray)
			to << "\"" << from.name() << "\": ";
		if (is_object)
			to << "{";
		else if (is_array)
			to << "[";
		to << std::endl;
		if (is_array)
		{
			for (data = from.datas()->begin(); data != from.datas()->end(); data++)
				to << next_indentStr << data->value << "," << std::endl;
		}
		else if (is_object)
		{
			for (data = from.datas()->begin(); data != from.datas()->end(); data++)
				to << next_indentStr << "\"" << data->key << "\": " << data->value << "," << std::endl;
		}
		for (child = from.children()->begin(); child != from.children()->end(); child++)
		{
			serialize(**child, to, indent + 1);
			to << "," << std::endl;
		}
		to << indent_str;
		if (is_object)
			to << "}";
		else if (is_array)
			to << "]";
	}

	void					text_serializer::deserialize(std::istream &from,
														 node &to)
	{
		m_parser.parse(from);
		if (m_parser.data())
			to = *m_parser.data();
	}

	token::token()
		: m_trigger()
		, m_name()
		, m_handler()
		, m_line_id(0)
		, m_ch_id(0)

	{
	}

	token::token(
		const String &trigger,
		const String &name,
		const Handler &handler,
		const size_t &line_id,
		const size_t &ch_id)
		: m_trigger(trigger)
		, m_name(name)
		, m_handler(handler)
		, m_line_id(line_id)
		, m_ch_id(ch_id)
	{
	}

	token::token(const token &rhs)
		: m_trigger(rhs.m_trigger)
		, m_name(rhs.m_name)
		, m_handler(rhs.m_handler)
		, m_line_id(rhs.m_line_id)
		, m_ch_id(rhs.m_ch_id)
	{
	}
	token::~token() throw()
	{
	}

	token &token::operator=(const token &rhs)
	{
		m_trigger = rhs.m_trigger;
		m_name = rhs.m_name;
		m_handler = rhs.m_handler;
		m_line_id = rhs.m_line_id;
		m_ch_id = rhs.m_ch_id;
		return (*this);
	}
	bool				token::operator==(String::value_type ch) const throw()
	{
		return (triggered(ch));
	}

	bool				token::operator()() throw(error)
	{
		return (handle());
	}


	const String		&token::trigger() const throw()
	{
		return (m_trigger);
	}
	const String		&token::name() const throw()
	{
		return (m_name);
	}
	const token::Handler	&token::handler() const throw()
	{
		return (m_handler);
	}
	size_t				token::line() const throw()
	{
		return (m_line_id);
	}
	size_t				token::column() const throw()
	{
		return (m_ch_id);
	}

	token			&token::trigger(const String &t)
	{
		m_trigger = t;
		return (*this);
	}
	token			&token::name(const String &n)
	{
		m_name = n;
		return (*this);
	}
	token			&token::handler(const Handler &h)
	{
		m_handler = h;
		return (*this);
	}
	token			&token::line(size_t l)
	{
		m_line_id = l;
		return (*this);
	}
	token			&token::column(size_t c)
	{
		m_ch_id = c;
		return (*this);
	}

	bool				token::handle() throw(error)
	{
		return (m_handler());
	}

	bool				token::triggered(String::value_type ch) const throw(error)
	{
		String::const_iterator it;
		for (it = m_trigger.begin(); it != m_trigger.end(); it++)
		{
			if (*it == ch)
				return (true);
		}
		return (false);
	}

	error::error(const String &file,
				 size_t line_id, size_t col_id,
				 const String &message,
				 int code)
		: std::runtime_error(error::make_message(file, line_id, col_id, message))
		, m_file(file)
		, m_line_id(line_id)
		, m_col_id(col_id)
		, m_code(code)
	{
	}
	error::error(const error &rhs)
		: std::runtime_error(rhs)
		, m_file(rhs.m_file)
		, m_line_id(rhs.m_line_id)
		, m_col_id(rhs.m_col_id)
		, m_code(rhs.m_code)
	{
	}
	error::~error() throw()
	{}

	error			&error::operator=(const error &rhs)
	{
		std::runtime_error::operator=(rhs);
		m_file = rhs.m_file;
		m_line_id = rhs.m_line_id;
		m_col_id = rhs.m_col_id;
		m_code = rhs.m_code;
		return (*this);
	}


	String				error::make_message(const String &file,
											size_t line_id,
											size_t col_id,
											const String &message)
	{
		std::stringstream	ss;
		String				path(file);
		size_t				pos(path.find_last_of('/'));
		if (pos != std::string::npos)
			path = path.substr(pos + 1, path.size() - (pos + 1));
		ss << "[" << path << "@l" << line_id << ":c" << col_id << "] " << message;
		return (ss.str());
	}

	int					error::code() const throw()
	{
		return (m_code);
	}
	String				error::file() const throw()
	{
		return (m_file);
	}
	size_t				error::line() const throw()
	{
		return (m_line_id);
	}
	size_t				error::col() const throw()
	{
		return (m_col_id);
	}
	parse_error::parse_error(const String &file,
							 size_t line_id, size_t col_id,
							 const String &message,
							 int code)
		: error(file, line_id, col_id, "parse_error: " + message, code)
	{}
	parse_error::parse_error(const parse_error &rhs)
		: error(rhs)
	{}
	parse_error::~parse_error() throw()
	{}

	parse_error			&parse_error::operator=(const parse_error &rk)
	{
		error::operator=(rk);
		return (*this);
	}

	static token	dummy_ref = token();

	parser::parser()
		: m_root(NULL)
		, m_nodes()
		, m_tokens()
		, m_line_id(0)
		, m_ch_id(0)
		, m_line()
		, m_accu()
		, m_ch()
		, m_root_found(false)
		, m_key_found(false)
		, m_key()
		, m_token_found(false)
		, m_token(std::ref(dummy_ref))
		, m_last_token(std::ref(dummy_ref))
	{
		_declare_token("delimiter.key", ":", std::bind(&parser::parse_key, this));
		_declare_token("delimiter.comma", ",", std::bind(&parser::parse_comma, this));
		_declare_token("node.open", "{[", std::bind(&parser::parse_node_open, this));
		_declare_token("node.close", "}]", std::bind(&parser::parse_node_close, this));
	}

	token			&parser::_declare_token(const String &name,
											const String &trigger,
											token::Handler handler)
	{
		m_tokens[name] = token(trigger, name, handler);
		return (m_tokens[name]);
	}

	parser::~parser() throw()
	{
		try {
			clear();
		} catch (...) {
			std::cerr << "json_parser: failed to clear nodes!" << std::endl;
		}
	}

	void				parser::clear() {
		while(!m_nodes.empty())
			m_nodes.pop();
		if (m_root)
			delete m_root, m_root = NULL;
		m_accu = "";
		m_line_id = 0;
		m_ch_id = 0;
		m_line = "";
		m_root_found = false;
		m_key_found = false;
		m_key = "";
		m_token_found = false;
	}


	node			*parser::parse(std::istream &is) throw(parse_error)
	{
		String				accu;
		token_map::iterator token;
		clear();
		m_nodes.push((m_root = new node(node_type::JNT_ROOT_OBJECT, "root")));
		while (std::getline(is, m_line))
		{
			m_key_found = false;
			m_key = "";
			m_ch_id = 0;
			trim(m_line);
			if (m_line.empty())
				continue;
			for (m_ch = m_line.begin(); m_ch != m_line.end(); _eat_ch())
			{
				if (parse_quote())
					_eat_ch();
				m_token_found = false;
				for (token = m_tokens.begin(); token != m_tokens.end() && !m_token_found; token++)
				{
					if (token->second == *m_ch)
					{
						m_last_token = m_token;
						m_token = token_ref(token->second);
						m_token_found = true;
						m_token_found = token->second();
					}
				}
				if (!m_token_found)
					m_accu += String(1, *m_ch);
				if (m_nodes.empty())
					break ;
			}
			if (m_nodes.empty()) {
				break ; // Root closed
			}
			m_line_id++;
		}
		return (m_root);
	}

	bool				parser::parse_quote() throw(error)
	{
		String			quote_ch(1, '"');
		if ((*m_ch) != quote_ch.at(0))
			quote_ch.at(0) = '\'';
		if (*m_ch == quote_ch.at(0))
		{
			trim(m_accu);
			_eat_ch();
			while (m_ch != m_line.end() && (*m_ch) != quote_ch.at(0))
			{
				m_accu += String(1, *m_ch);
				_eat_ch();
			}
			m_accu = quote_ch + m_accu + quote_ch;
			return (true);
		}
		return (false);

	}

	void				parser::_eat_ch()
	{
		if (m_ch != m_line.end())
		{
			m_ch++;
			m_ch_id++;
		}
	}

	bool				parser::parse_key() throw(error)
	{
		if (*m_ch == ':')
		{
//		if (m_key_found) {
//			throw Exception(error_base + "unexpected character ':', key already defined!");
//		}
			if (m_accu.empty())
				_except("unexpected character ':', key is empty!");
			m_key = trim_quotes(trim(m_accu));
			m_key_found = true;
			m_accu = "";
			return (true);
		}
		return (false);
	}

	bool				parser::parse_node_open() throw(error)
	{
		node_type		type(JNT_NONE);

		type = (*m_ch == '{') ? JNT_OBJECT : type;
		type = (*m_ch == '[') ? JNT_ARRAY : type;
		if (type == JNT_OBJECT || type == JNT_ARRAY) {
			if (m_root_found)
				m_nodes.push(m_nodes.top()->create_child(m_key, type));
			else
				m_root_found = true;
			m_nodes.top()->type(type);
			m_key_found = false;
			m_key = "";
			m_accu = "";
			return (true);
		}
		return (false);
	}

	bool				parser::parse_node_close() throw(error)
	{
		node_type	type(JNT_NONE);
		type = (*m_ch == '}') ? JNT_OBJECT : type;
		type = (*m_ch == ']') ? JNT_ARRAY : type;
		if (type == JNT_OBJECT || type == JNT_ARRAY) {
			if (!m_nodes.top()->parent()
				&& m_nodes.top() != m_root)
				_except("unexpected character '" + String(1, *m_ch) + "'");
			if (!trim(m_accu).empty())
				_except("unexpected character '" + String(1, *m_ch) + "', expected ',' before " + m_accu);
			m_nodes.pop();
			m_key_found = false;
			m_key = "";
			m_accu = "";
			return (true);
		}
		return (false);
	}

	bool				parser::parse_comma() throw(error)
	{
		if (*m_ch == ',')
		{
			trim(m_accu);
			trim(m_key);
			trim_quotes(m_accu);
			if (m_nodes.top()->type() == JNT_ARRAY)
			{
				if (!m_key.empty())
					_except("keys only supported on objects!");
				m_key = std::to_string(m_nodes.top()->datas()->size());
				m_nodes.top()->data(m_key, trim_quotes(m_accu));
			}
			else if (m_nodes.top()->type() == JNT_OBJECT)
			{
				if (m_key.empty() && m_last_token.get().name() != "node.close") {
					std::cout << m_nodes.top()->type_name() << "-" << m_line.substr(0, m_ch_id) << std::endl;
					_except("missing key for object declaration!");
				}
				if (!m_accu.empty())
				{
					m_nodes.top()->data(m_key, m_accu);
				}
			}
			else
				return (false);
			m_key = "";
			m_accu = "";
			m_key_found = false;
			return (true);
		}
		return (false);
	}


	void				parser::_except(const String &msg,
										int code) throw(parse_error)
	{
		throw parse_error("stream", m_line_id, m_ch_id,
						  msg, code);

	}

	node			*parser::data() throw()
	{ return (m_root); }
	const node		*parser::data() const throw()
	{ return (m_root); }

	String				&parser::trim_quotes(String &str) {
		trim(str);
		if (!str.empty() && str.at(0) == '"')
			str.erase(str.begin());
		if (!str.empty() && str.at(str.size() - 1) == '"')
			str.erase(str.begin() + str.size() - 1);
		return (str);
	}
	String				 &parser::ltrim(String &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
										std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	String				&parser::rtrim(String &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
							 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	String				&parser::trim(String &s) {
		return ltrim(rtrim(s));
	}


	node::node(node_type t, const String &name, node *node)
		: m_parent(node)
		, m_type(t)
		, m_name(name)
		, m_data()
		, m_children()
	{}

	node::node(const node &rhs)
		: m_parent(rhs.m_parent)
		, m_type(rhs.m_type)
		, m_name(rhs.m_name)
		, m_data(rhs.m_data)
		, m_children()
	{
		for (auto child : rhs.m_children)
			m_children.push_back(new node(*child));
	}

	node::~node()
	{
		clear();
	}


	node			&node::type(node_type t) throw()
	{
		m_type = t;
		return (*this);
	}
	node_type		node::type() const throw()
	{
		return (m_type);
	}

	String				node::type_name() const throw()
	{
		return (type_name(m_type));
	}

	String				node::type_name(node_type t) throw()
	{
		static const std::map<node_type, String> names
		{
			{JNT_NONE, "undefined"},
			{JNT_ROOT_OBJECT, "root_object"},
			{JNT_OBJECT, "object"},
			{JNT_ARRAY, "array"},
			{JNT_COUNT, "unknown"}
		};
		if (names.find(t) == names.end()) {
			t = JNT_COUNT;
		}
		return (names.at(t));
	}

	void				node::clear() throw()
	{
		m_parent = NULL;
		m_name = "";
		m_type = JNT_NONE;
		m_data.clear();
		node_list::iterator it;
		for (it = m_children.begin(); it != m_children.end(); it++)
		{
			if (*it)
				delete *it, *it = NULL;
		}
		m_children.clear();
	}

	node			&node::operator=(const node &rhs)
	{
		clear();
		m_parent = rhs.m_parent;
		m_type = rhs.m_type;
		m_name = rhs.m_name;
		m_data = rhs.m_data;
		node_list::const_iterator it;
		for (it = rhs.m_children.begin(); it != rhs.m_children.end(); it++)
		{
			if (*it)
			{
				m_children.push_back(new node(**it));
				m_children.back()->parent(this);
			}
		}
		return (*this);
	}

	node_list		*node::children() throw()
	{
		return (&m_children);
	}
	const node_list	*node::children() const throw()
	{
		return (&m_children);
	}

	node			&node::parent(node *p) throw()
	{
		m_parent = p;
		return (*this);
	}

	node			*node::parent() throw()
	{
		return (m_parent);
	}
	const node		*node::parent() const throw()
	{
		return (m_parent);
	}

	node			&node::name(const String &name) throw()
	{
		m_name = name;
		return (*this);
	}

	const String		&node::name() const throw()
	{
		return (m_name);
	}

	node			*node::create_child(const String &name,
										node_type type) throw()
	{
		node		*nod(child(name, false));
		if (!nod) {
			nod = new node(type, name, this);
			m_children.push_back(nod);
		}
		return (nod);
	}

	node			*node::child(const String &name,
								 bool recurs) throw()
	{
		node	*node;
		for (auto child : m_children) {
			if (child->name() == name) {
				return (child);
			}
			if (recurs && (node = child->child(name, recurs)) != NULL)
				return (node);
		}
		return (NULL);
	}

	const node		*node::child(const String &name,
								 bool recurs) const throw()
	{
		const node	*node;
		for (auto child : m_children) {
			if (child->name() == name) {
				return (child);
			}
			if (recurs && (node = child->child(name, recurs)) != NULL)
				return (node);
		}
		return (NULL);
	}

	bool				node::empty() const throw()
	{
		return (m_children.empty());
	}

	const var			*node::data(const String &name) const throw()
	{
		var_list::const_iterator it;
		for (it = m_data.begin(); it != m_data.end(); it++) {
			if (it->key == name) {
				return (&(*it));
			}
		}
		return (NULL);
	}

	var					*node::data(const String &name) throw()
	{
		var_list::iterator it;
		for (it = m_data.begin(); it != m_data.end(); it++) {
			if (it->key == name) {
				return (&(*it));
			}
		}
		return (NULL);
	}

	node			&node::data(const String &name,
								const String &value) throw ()
	{
		var			*p_data;
		if ((p_data = this->data(name)) == NULL) {
			var data;
			data.key = name;
			data.value = value;
			m_data.push_back(data);
			p_data = &m_data.back();
		}
		p_data->value = value;
		return (*this);
	}

	var_list			*node::datas() throw()
	{
		return (&m_data);
	}
	const var_list		*node::datas() const throw()
	{
		return (&m_data);
	}
};
