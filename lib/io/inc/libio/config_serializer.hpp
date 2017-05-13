// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ini_serializer.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/05/12 20:43:08 by mwelsch           #+#    #+#             //
//   Updated: 2017/05/13 15:39:25 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef LIBIO_CONFIG_SERIALIZER_HPP
# define LIBIO_CONFIG_SERIALIZER_HPP

# include "serializer.hpp"
# include "config_node.hpp"
# include <stack>

namespace io
{
	template<typename char_t>
	class basic_config_serializer
		: public basic_serializer<basic_config_node<char_t>, char_t>
	{
	public:
		typedef basic_config_node<char_t>							node_type;
		typedef basic_serializer_traits<node_type,
										char_t>						traits_type;
		typedef basic_serializer<node_type, char_t>					base_type;
		typedef basic_config_serializer<char_t>						self_type;

		typedef typename traits_type::item_type						item_type;
		typedef typename traits_type::char_type						char_type;
		typedef typename traits_type::string_type					string_type;
		typedef typename traits_type::string_list_type				string_list_type;
		typedef typename traits_type::istream_type					istream_type;
		typedef typename traits_type::ostream_type					ostream_type;
		typedef std::stack<node_type*>								node_stack_type;

		string_list_type											m_errors;
		node_type													*m_item;
		string_type													m_key;
		string_type													m_val;
		string_type													m_com;
		node_stack_type												m_nodes;

		basic_config_serializer(const string_type &name = string_type("config"),
								const string_type version = string_type("1.0.0"))
			: base_type(name, version)
			, m_errors()
			, m_item(NULL)
			, m_key()
			, m_val()
			, m_com()
			, m_nodes()
			{}
		basic_config_serializer(const self_type &rhs)
			: base_type(rhs)
			, m_errors(rhs.m_errors)
			, m_item(rhs.m_item)
			, m_key(rhs.m_key)
			, m_val(rhs.m_val)
			, m_com(rhs.m_com)
			, m_nodes(rhs.m_nodes)
			{}
		virtual ~basic_config_serializer() throw()
			{}

		self_type						&operator=(const self_type &rhs)
			{
				base_type::operator=(rhs);
				m_errors = rhs.m_errors;
				m_item = rhs.m_item;
				m_key = rhs.m_key;
				m_val = rhs.m_val;
				m_com = rhs.m_com;
				m_nodes = rhs.m_nodes;
				return (*this);
			}

		virtual ostream_type			&stringify(ostream_type &os,
												   const item_type &item)
			{
				(void)item;
				typename item_type::const_iterator it;
				if (!item.children()->empty())
				{
					os << "[" << item.key() << "]";
					if (!item.value().empty() || !item.comment().empty())
					{
						os << " # ";
						if (!item.value().empty())
							os << item.value();
						if (!item.comment().empty())
						{
							if (!item.value().empty())
								os << " ";
							os << item.comment();
						}
					}
					os << std::endl;
					size_t								nb_printed = 0;
					for (it = item.children()->begin(); it != item.children()->end(); it++)
					{
						if (*it)
						{
							if (nb_printed)
								os << std::endl;
							stringify(os, **it);
							nb_printed++;
						}
					}
				}
				else
				{
					os << item.key() << " = " << item.value();
					if (!item.comment().empty())
						os << " # " << item.comment();
				}
				return (os);
			}
		string_type						&trim(string_type &str)
			{
				string_type				charset(" \r\t\v\f\n");

				while (!str.empty()
					   && charset.find(str.at(str.size() - 1)) != std::string::npos)
					str.erase(str.begin() + (str.size() - 1));
				while (!str.empty()
					   && charset.find(str.at(0)) != std::string::npos)
					str.erase(str.begin());
				return (str);
			}

		const string_list_type			&errors() const throw() {
			return (m_errors);
		}

		istream_type					&fail(istream_type &is,
											 const string_type &msg = string_type())
			{
				is.setstate(std::ios::failbit);
				m_errors.push_back(msg);
				return (is);
			}

		virtual istream_type			&parse(istream_type &is,
											   item_type &item)
			{
				(void)item;
				string_type				line;
				size_t					pos;

				m_item = &item;
				m_item->reset();
				while (std::getline(is, line))
				{
					m_com.clear();
					m_key.clear();
					m_val.clear();
					if ((pos = line.find('#')) != std::string::npos)
					{
						m_com = line.substr(pos + 1, std::string::npos);
						line = line.substr(0, pos);
					}
					trim(line);
					if (!parse_section(is, line))
						parse_key(is, line);
				}
				m_item->validate();
				return (is);
			}

	protected:
		bool				parse_section(istream_type &is,
										  string_type &line)
			{
				bool		ret = false;
				size_t		posr = line.find(']');
				size_t		posl = line.find('[');

				if (posr != std::string::npos
					|| posl != std::string::npos)
				{
					if (posl == std::string::npos)
						return (fail(is,
									 "missing left bracket for section delimiter"));
					if (posr == std::string::npos)
						return (fail(is,
									 "missing right bracket for section delimiter"));
					if (posl != 0)
						return (fail(is,
									 "left bracket is not at start of line"));
					if (posr != (line.size() - 1))
						return (fail(is,
									 "right bracket is not at end of line"));
					if (!line.empty())
						line.erase(line.begin());
					if (!line.empty())
						line.erase(line.begin()
								   + (line.size() - 1));
					m_key = line;
					trim(m_com);
					trim(m_key);
					trim(m_val);
					if (m_nodes.empty())
					{
						m_item->key(m_key);
						m_item->value(m_val);
						m_item->comment(m_com);
						m_nodes.push(m_item);
					}
					else
						m_nodes.push(m_item->create_child(m_key,
														  m_val,
														  m_com));
					ret = true;
				}
				return (ret);
			}

		bool					parse_key(istream_type &is,
										  string_type &line)
			{
				size_t			pos;
				bool			ret = false;
				node_type		*node;
				(void)is;
				if ((pos = line.find('=')) != std::string::npos)
				{
					m_val = line.substr(pos + 1, std::string::npos);
					m_key = line.substr(0, pos);
				}
				else
				{
					m_key = line;
					m_val.clear();
				}
				trim(m_com);
				trim(m_key);
				trim(m_val);
				if (m_nodes.empty())
					m_nodes.push(m_item);
				node = m_nodes.top();
				node->create_child(m_key, m_val, m_com);
				return (ret);
			}
	};
}

#endif
