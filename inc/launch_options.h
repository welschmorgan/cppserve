/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_options.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 14:16:23 by mwelsch           #+#    #+#             */
/*   Updated: 2017/02/05 13:11:11 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef				LAUNCH_OPTIONS_H
# define			LAUNCH_OPTIONS_H

# include			<unistd.h>
# include			<string>
# include			<vector>
# include			<functional>
# include			<map>

class									LaunchOption {
public:
	typedef std::function<void(void)>	Handler;

	static void							NoOp(void);
protected:
	std::vector<std::string>			mTriggers;
	std::string							mName;
	std::string							mValue;
	Handler								mHandler;
	bool								mActive;

public:
	LaunchOption(const std::string &name = std::string(),
				 const std::vector<std::string> &triggers = std::vector<std::string>(),
				 LaunchOption::Handler handler = LaunchOption::NoOp,
				 const std::string &value = std::string());
	LaunchOption(const LaunchOption &rk);
	~LaunchOption();


	bool								isActive() const;
	Handler								getHandler() const;
	const std::string					&getName() const;
	const std::vector<std::string>		&getTriggers() const;
	const std::string					&getValue() const;

	void								setActive(bool state);
	void								setHandler(Handler n);
	void								setName(const std::string &n);
	void								setTriggers(const std::vector<std::string> &n);
	void								setValue(const std::string &v);

	LaunchOption						&operator=(const LaunchOption &rk);
};

class									LaunchOptions {
public:
	typedef std::vector<LaunchOption>	ArgList;

public:
	LaunchOptions(int argc = 0,
				  char *const argv[] = NULL);
	LaunchOptions(const LaunchOptions &rk);
	~LaunchOptions();

	bool								contains(const std::string &name) const;
	const LaunchOption					*get(const std::string &name) const;
	LaunchOption						*get(const std::string &name);
	bool								set(const std::string &name,
											const std::vector<std::string> &triggers,
											LaunchOption::Handler handler,
											const std::string &value);
	LaunchOptions						&operator=(const LaunchOptions &rk);
	void								parse();
protected:
	ArgList								mArgs; // original args
	ArgList								mRegs; // registered args
};

#endif
