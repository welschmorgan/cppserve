#ifndef LIBIO_CONFIG_NODE_HPP
# define LIBIO_CONFIG_NODE_HPP

# include <list>
# include <vector>
# include <string>
# include <stdexcept>
# include "serializer_traits.hpp"

namespace io
{
	template<typename char_t,
			 template<typename, typename> class list_t = std::list,
			 template<typename> class alloc_t = std::allocator>
	class															basic_config_node
	{
	public:
		typedef basic_config_node<char_t>							self_type;
		typedef basic_serializer_traits<self_type, char_t>			traits_type;

		typedef typename traits_type::char_type						char_type;
		typedef typename traits_type::string_type					string_type;

		typedef std::vector<string_type>							string_list_type;

		typedef list_t<self_type*, alloc_t<self_type*>>				list_type;
		typedef typename list_type::iterator						iterator;
		typedef typename list_type::const_iterator					const_iterator;
		typedef typename list_type::reverse_iterator				reverse_iterator;
		typedef typename list_type::const_reverse_iterator			const_reverse_iterator;

	protected:
		self_type													*m_parent;
		string_type													m_key;
		string_type													m_value;
		string_type													m_comment;
		list_type													m_children;

	public:
		basic_config_node(const string_type &key = string_type(),
						  const string_type &value = string_type(),
						  const string_type &comment = string_type(),
						  const list_type &children = list_type())
			: m_parent(NULL)
			, m_key(key)
			, m_value(value)
			, m_comment(comment)
			, m_children(children)
			{}
		basic_config_node(const self_type &rhs)
			: m_parent(rhs.m_parent)
			, m_key(rhs.m_key)
			, m_value(rhs.m_value)
			, m_comment(rhs.m_comment)
			, m_children(rhs.m_children)
			{}
		virtual ~basic_config_node() throw()
			{ reset(true); }

		self_type						&operator=(const self_type &rhs)
			{
				m_parent = rhs.m_parent;
				m_key = rhs.m_key;
				m_value = rhs.m_value;
				m_comment = rhs.m_comment;
				m_children = rhs.m_children;
				return (*this);
			}

		self_type						*parent() throw()
			{ return (m_parent); }

		const self_type					*parent() const throw()
			{ return (m_parent); }

		void							parent(self_type *parent) throw()
			{ m_parent = parent; }


		list_type						*children() throw()
			{ return (&m_children); }

		const list_type					*children() const throw()
			{ return (&m_children); }


		void							children(const list_type &nodes)
			{
				clear_children();
				m_children = nodes;
			}

		self_type						*nth_child(size_t id) throw()
			{
				typename list_type::iterator		it = m_children.begin();
				self_type				*ret = NULL;

				while (id-- && it != m_children.end())
					it++;
				if (it != m_children.end())
					ret = *it;
				return (ret);
			}

		const self_type					*nth_child(size_t id) const throw()
			{
				typename list_type::const_iterator	it = m_children.begin();
				const self_type				*ret = NULL;

				while (id-- && it != m_children.end())
					it++;
				if (it != m_children.end())
					ret = *it;
				return (ret);
			}

		self_type						*first_child() throw()
			{
				self_type				*ret= NULL;
				if (!m_children.empty())
					ret = m_children.at(0);
				return (ret);
			}
		const self_type					*first_child() const throw()
			{
				const self_type			*ret= NULL;
				if (!m_children.empty())
					ret = m_children.at(0);
				return (ret);
			}

		self_type						&operator[](const string_type &key) throw(std::runtime_error)
			{
				self_type				*node(child(key));

				if (!node)
					throw std::runtime_error(key + ": failed to create key!");
			}

		const self_type					*root() const throw() {
			const self_type				*node = this;
			const self_type				*last_node = NULL;
			while (node)
			{
				last_node = node;
				node = node->parent();
			}
			return (last_node);
		}

		self_type						*root() throw() {
			self_type					*node = this;
			self_type					*last_node = NULL;
			while (node)
			{
				last_node = node;
				node = node->parent();
			}
			return (last_node);
		}

		self_type						*create_child(const string_type &key,
													  const string_type &value = string_type(),
													  const string_type &comment = string_type(),
													  const list_type &children = list_type())
			{
				string_list_type		parts;
				typename string_list_type::iterator		it;
				self_type				*node;
				self_type				*last_node;

				parts = split_key(key);
				node = this;
				for (it = parts.begin(); it != parts.end(); it++)
				{
					last_node = node;
					node = last_node->_child(*it);
					if (!node)
					{
						if (!(node = last_node->_create_child(*it)))
							break ;
					}
				}
				if (node)
				{
					node->value(value);
					node->comment(comment);
					node->children(children);
				}
				return (node);
			}

		self_type						*_create_child(const string_type &key,
													   const string_type &value = string_type(),
													   const string_type &comment = string_type(),
													   const list_type &children = list_type())
			{
				string_list_type		parts;
				self_type				*node(_child(key));

				if (!node)
				{
					node = new self_type(key);
					if (!node)
						return (node);
					m_children.push_back(node);
				}
				node->parent(this);
				if (!comment.empty())
					node->comment(comment);
				if (!value.empty())
					node->value(value);
				if (!children.empty())
					node->children(children);
				return (node);
			}

		self_type						*child(const string_type &key,
											   const char_type sep = '.') throw()
			{
				iterator				it;
				self_type				*node;
				self_type				*last_node;
				string_list_type		parts = self_type::split_key(key, sep);
				typename string_list_type::iterator	pit;

				last_node = NULL;
				node = this;
				for (pit = parts.begin(); pit != parts.end(); pit++)
				{
					last_node = node;
					if (node->key() == *pit
						|| !(node = last_node->_child(*pit)))
						break ;
				}

				return (node);
			}

		const self_type					*child(const string_type &key,
											   const char_type sep = '.') const throw()
			{
				const_iterator						it;
				const self_type						*node;
				const self_type						*last_node;
				string_list_type					parts = self_type::split_key(key, sep);
				typename string_list_type::iterator	pit;

				last_node = NULL;
				node = this;
				for (pit = parts.begin(); pit != parts.end(); pit++)
				{
					last_node = node;
					if (node->key() == *pit
						|| !(node = last_node->_child(*pit)))
						break ;
				}

				return (node);
			}

		bool							has_child(const string_type &key,
												  const char_type sep = '.') const throw()
			{
				return (child(key, sep) != NULL);
			}

		bool							has_children() const throw()
			{
				return (!m_children.empty());
			}

		void							clear_children() throw()
			{
				iterator it;
				for (it = m_children.begin(); it != m_children.end(); it++)
				{
					if (*it)
						delete *it, *it = NULL;
				}
				m_children.clear();
			}
		size_t							num_children() const throw()
			{
				return (m_children.size());
			}
		void							reset(bool clear_parent = false)
			{
				m_key.clear();
				m_value.clear();
				m_comment.clear();
				if (clear_parent)
					m_parent = NULL;
				clear_children();
			}

		string_type						full_key(bool skip_empty = false) const throw()
			{
				string_list_type		keys;
				typename string_list_type::const_reverse_iterator		it;
				const self_type			*node = this;

				while (node)
				{
					if (!skip_empty || !node->key().empty())
						keys.push_back(node->key());
					node = node->parent();
				}
				string_type				ret;
				for (it = keys.rbegin(); it != keys.rend(); it++)
				{
					if (!ret.empty())
						ret += ".";
					ret += *it;
				}
				return (ret);
			}

		void							key(const string_type &k) throw()
			{ m_key = k; }

		void							value(const string_type &v) throw()
			{ m_value = v; }

		const string_type				&comment() const throw()
			{ return (m_comment); }

		void							comment(const string_type &comment) throw()
			{ m_comment = comment; }


		const string_type				&key() const throw()
			{ return (m_key); }

		const string_type				&value() const throw()
			{ return (m_value); }

		virtual void					validate()
			{}

	protected:
		static string_list_type			split_key(const string_type &k,
												  const char_type sep = '.')
			{
				typename string_type::const_iterator		it;
				string_list_type							ret;
				string_type									accu;
				for (it = k.begin(); it != k.end(); it++)
				{
					if (*it == sep)
					{
						ret.push_back(accu);
						accu.clear();
					}
					else
						accu.push_back(*it);
				}
				if (!accu.empty())
					ret.push_back(accu);
				return (ret);
			}


		const self_type					*_child(const string_type &key) const throw()
			{
				const_iterator			it;
				const self_type			*found;

				found = NULL;
				for (it = m_children.begin(); !found && it != m_children.end(); it++)
				{
					if (*it && (*it)->key() == key)
						found = *it;
				}
				return (found);
			}

		self_type						*_child(const string_type &key) throw()
			{
				iterator				it;
				self_type				*found;

				found = NULL;
				for (it = m_children.begin(); !found && it != m_children.end(); it++)
				{
					if (*it && (*it)->key() == key)
						found = *it;
				}
				return (found);
			}


	};
}

#endif
