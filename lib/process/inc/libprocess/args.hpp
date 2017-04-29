// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   args.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/23 17:48:41 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 18:10:21 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef LIBPROCESS_ARGS_HPP
# define LIBPROCESS_ARGS_HPP


# include			<unistd.h>
# include			<string>
# include			<vector>
# include			<functional>
# include			<map>

namespace proc
{
	typedef std::function<void(void*)>		arg_handler;
	class									arg
	{
	public:
		typedef arg_handler					callback;

		static void							NoOp(void *);
	protected:
		std::string							m_name;
		std::vector<std::string>			m_triggers;
		callback							m_handler;
		std::string							m_value;
		std::string							m_desc;
		bool								m_active;

	public:
		arg(const std::string &name = std::string(),
					 const std::vector<std::string> &triggers = std::vector<std::string>(),
					 arg::callback handler = arg::NoOp,
					 const std::string &value = std::string(),
					 const std::string &desc = std::string());
		arg(const arg &rk);
		~arg();


		bool								active() const throw();
		callback							handler() const throw();
		const std::string					&name() const throw();
		const std::vector<std::string>		&triggers() const throw();
		const std::string					&desc() const throw();
		const std::string					&value() const throw();

		void								active(bool state);
		void								handler(callback n);
		void								name(const std::string &n);
		void								desc(const std::string &d);
		void								triggers(const std::vector<std::string> &n);
		void								value(const std::string &v);

		arg									&operator=(const arg &rk);
	};

	class									arg_list {
	public:
		typedef std::vector<arg>			container;

	public:
		arg_list(int argc = 0,
					  char *const argv[] = NULL);
		arg_list(const arg_list &rk);
		~arg_list();

		bool								contains(const std::string &name) const;
		container							*supplied() throw();
		const container						*supplied() const throw();

		container							*registered() throw();
		const container						*registered() const throw();

		const arg							*get(const std::string &name) const;
		arg									*get(const std::string &name);
		bool								set(const std::string &name,
												const std::vector<std::string> &triggers,
												arg::callback handler,
												const std::string &value,
												const std::string &desc);
		arg_list							&operator=(const arg_list &rk);
		void								parse(void *data);
	protected:
		container							m_args; // original args
		container							m_regs; // registered args
	};

};

#endif
