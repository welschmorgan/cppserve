// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   manual.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/23 15:38:40 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 17:13:45 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MANUAL_HPP
# define MANUAL_HPP

# include "string.hpp"
# include <stdexcept>
# include <chrono>
# include <map>

struct								ManualSection
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	String							name;
	String							title;
	StringStream					lines;
	String							author;
	StringStream					desc;
	bool							active;
	const TimePoint					created_at;

	ManualSection();
	ManualSection(const String &name_ = String(),
			   const String &title_ = String(),
			   const StringStream &lines_ = StringStream(),
			   const String &author_ = String(),
			   const StringStream &desc_ = StringStream());
	ManualSection(const ManualSection &rhs);
	virtual ~ManualSection() throw();

	ManualSection					&operator=(const ManualSection &rhs);
};

class									Manual
	: public std::map<String, ManualSection>
{
public:
	typedef std::map<String, ManualSection>		Container;
	typedef typename Container::iterator		Iterator;
	typedef typename Container::const_iterator	ConstIterator;
public:
	Manual();
	Manual(const Manual &rhs);
	virtual ~Manual() throw();

	Manual								&operator=(const Manual &rhs);
	ManualSection						*createSection(const String &name_ = String(),
													   const String &title_ = String(),
													   const StringStream &lines_ = StringStream(),
													   const String &author_ = String(),
													   const StringStream &desc_ = StringStream()) throw (std::runtime_error);

	void								activateSection(const String &name) throw (std::runtime_error);
	void								disactivateSection(const String &name) throw (std::runtime_error);

	ManualSection						*getActiveSection() throw();
	const ManualSection					*getActiveSection() const throw();
};

OStream									&operator<<(OStream &os,
													const ManualSection &scr);

#endif
