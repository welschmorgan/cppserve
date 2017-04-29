/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 12:52:31 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 13:53:20 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		STATUS_H
# define	STATUS_H

# include "stringmap.hpp"

typedef std::map<uint16_t, std::string> StatusMap;

class					HTTPStatus {
public:
	typedef uint16_t			T;
	typedef std::string			NameT;
	typedef std::map<T, NameT>	NameMap;

	static const NameMap		&List();

	static std::string			CodeName(T id);

	template <typename SecT>
	class Base {
		Base();
		Base(const Base &rk);
		Base				&operator=(const Base &rk);

	protected:

	public:
		virtual ~Base();

		static const NameT	Name;
		static const T		Range[2];
	};

	static void				init();

	class Info : public Base<Info> {
	public:
		static const T		Continue;
		static const T		SwitchingProtocols;
		static const T		Processing;
	};

	class Success : public Base<Info> {
	public:
		static const T		Ok;
		static const T		Created;
		static const T		Accepted;
		static const T		NonAuthInfo;
		static const T		NoContent;
		static const T		ResetContent;
		static const T		PartialContent;
		static const T		MultiStatus;
		static const T		AlreadyReported;
		static const T		IMUsed;
	};

	class Redirect : public Base<Info> {
	public:
		static const T		MultipleChoices;
		static const T		MovedPermanently;
		static const T		Found;
		static const T		SeeOther;
		static const T		NotModified;
		static const T		UseProxy;
		static const T		Unused;
		static const T		TempRedirect;
		static const T		PermRedirect;
	};

	class Client : public Base<Info> {
	public:
		static const T		BadRequest;
		static const T		Unauthorized;
		static const T		PaymentRequired;
		static const T		Forbidden;
		static const T		NotFound;
		static const T		MethodNotAllowed;
		static const T		NotAcceptable;
		static const T		ProxyAuthRequired;
		static const T		RequestTimeout;
		static const T		Conflict;
		static const T		Gone;
		static const T		LengthRequired;
		static const T		PreconditionFailed;
		static const T		RequestEntityTooLarge;
		static const T		RequestURITooLong;
		static const T		UnsupportedMediaType;
		static const T		RequestedRangeNotSatisfiable;
		static const T		ExpectationFailed;
		static const T		ImATeapot;
		static const T		EnhanceYourCalm;
		static const T		UnprocessableEntity;
		static const T		Locked;
		static const T		FailedDependency;
		static const T		Reserved;
		static const T		UpgradeRequired;
		static const T		PreconditionRequired;
		static const T		TooManyRequests;
		static const T		RequestHeaderFieldsTooLarge;
		static const T		NoResponse;
		static const T		RetryWith;
		static const T		Blocked;
		static const T		UnavailableForLegalReasons;
		static const T		ClientClosedRequest;
	};

	class Server : public Base<Info> {
	public:
		static const T		InternalError;
		static const T		NotImplemented;
		static const T		BadGateway;
		static const T		ServiceUnavailable;
		static const T		GatewayTimeout;
		static const T		VersionNotSupported;
		static const T		VariantAlsoNegotiates;
		static const T		InsufficientStorage;
		static const T		LoopDetected;
		static const T		BandwidthLimitExceeded;
		static const T		NotExtended;
		static const T		NetworkAuthenticationRequired;
		static const T		NetworkReadTimeout;
		static const T		NetworkConnectTimeout;
	};
};

#endif
