// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   status.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/11 13:28:15 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/12 16:23:15 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "status.hpp"

//#if 0
# define defSection(secT, name, min, max)								\
	template<>															\
	const HTTPStatus::NameT		HTTPStatus::Base<secT>::Name(name);		\
	template<>															\
	const HTTPStatus::T			HTTPStatus::Base<secT>::Range[2] = { min, max};

# define defCode(map, secT, part)										\
	map[HTTPStatus::secT::part] = HTTPStatus::NameT(#part);


const HTTPStatus::T		HTTPStatus::Info::Continue = 100;
const HTTPStatus::T		HTTPStatus::Info::SwitchingProtocols = 101;
const HTTPStatus::T		HTTPStatus::Info::Processing = 102;

const HTTPStatus::T		HTTPStatus::Success::Ok = 200;
const HTTPStatus::T		HTTPStatus::Success::Created = 201;
const HTTPStatus::T		HTTPStatus::Success::Accepted = 202;
const HTTPStatus::T		HTTPStatus::Success::NonAuthInfo = 203;
const HTTPStatus::T		HTTPStatus::Success::NoContent = 204;
const HTTPStatus::T		HTTPStatus::Success::ResetContent = 205;
const HTTPStatus::T		HTTPStatus::Success::PartialContent = 206;
const HTTPStatus::T		HTTPStatus::Success::MultiStatus = 207;
const HTTPStatus::T		HTTPStatus::Success::AlreadyReported = 208;
const HTTPStatus::T		HTTPStatus::Success::IMUsed = 209;

const HTTPStatus::T		HTTPStatus::Redirect::MultipleChoices = 300;
const HTTPStatus::T		HTTPStatus::Redirect::MovedPermanently = 301;
const HTTPStatus::T		HTTPStatus::Redirect::Found = 302;
const HTTPStatus::T		HTTPStatus::Redirect::SeeOther = 303;
const HTTPStatus::T		HTTPStatus::Redirect::NotModified = 304;
const HTTPStatus::T		HTTPStatus::Redirect::UseProxy = 305;
const HTTPStatus::T		HTTPStatus::Redirect::Unused = 306;
const HTTPStatus::T		HTTPStatus::Redirect::TempRedirect = 307;
const HTTPStatus::T		HTTPStatus::Redirect::PermRedirect = 308;

const HTTPStatus::T		HTTPStatus::Client::BadRequest = 400;
const HTTPStatus::T		HTTPStatus::Client::Unauthorized = 401;
const HTTPStatus::T		HTTPStatus::Client::PaymentRequired = 402;
const HTTPStatus::T		HTTPStatus::Client::Forbidden = 403;
const HTTPStatus::T		HTTPStatus::Client::NotFound = 404;
const HTTPStatus::T		HTTPStatus::Client::MethodNotAllowed = 405;
const HTTPStatus::T		HTTPStatus::Client::NotAcceptable = 406;
const HTTPStatus::T		HTTPStatus::Client::ProxyAuthRequired = 407;
const HTTPStatus::T		HTTPStatus::Client::RequestTimeout = 408;
const HTTPStatus::T		HTTPStatus::Client::Conflict = 409;
const HTTPStatus::T		HTTPStatus::Client::Gone = 410;
const HTTPStatus::T		HTTPStatus::Client::LengthRequired = 411;
const HTTPStatus::T		HTTPStatus::Client::PreconditionFailed = 412;
const HTTPStatus::T		HTTPStatus::Client::RequestEntityTooLarge = 413;
const HTTPStatus::T		HTTPStatus::Client::RequestURITooLong = 414;
const HTTPStatus::T		HTTPStatus::Client::UnsupportedMediaType = 415;
const HTTPStatus::T		HTTPStatus::Client::RequestedRangeNotSatisfiable = 416;
const HTTPStatus::T		HTTPStatus::Client::ExpectationFailed = 417;
const HTTPStatus::T		HTTPStatus::Client::ImATeapot = 418;
const HTTPStatus::T		HTTPStatus::Client::EnhanceYourCalm = 420;
const HTTPStatus::T		HTTPStatus::Client::UnprocessableEntity = 422;
const HTTPStatus::T		HTTPStatus::Client::Locked = 423;
const HTTPStatus::T		HTTPStatus::Client::FailedDependency = 424;
const HTTPStatus::T		HTTPStatus::Client::Reserved = 425;
const HTTPStatus::T		HTTPStatus::Client::UpgradeRequired = 426;
const HTTPStatus::T		HTTPStatus::Client::PreconditionRequired = 427;
const HTTPStatus::T		HTTPStatus::Client::TooManyRequests = 429;
const HTTPStatus::T		HTTPStatus::Client::RequestHeaderFieldsTooLarge = 431;
const HTTPStatus::T		HTTPStatus::Client::NoResponse = 444;
const HTTPStatus::T		HTTPStatus::Client::RetryWith = 449;
const HTTPStatus::T		HTTPStatus::Client::Blocked = 450;
const HTTPStatus::T		HTTPStatus::Client::UnavailableForLegalReasons = 451;
const HTTPStatus::T		HTTPStatus::Client::ClientClosedRequest = 499;

const HTTPStatus::T		HTTPStatus::Server::InternalError = 500;
const HTTPStatus::T		HTTPStatus::Server::NotImplemented = 501;
const HTTPStatus::T		HTTPStatus::Server::BadGateway = 502;
const HTTPStatus::T		HTTPStatus::Server::ServiceUnavailable = 503;
const HTTPStatus::T		HTTPStatus::Server::GatewayTimeout = 504;
const HTTPStatus::T		HTTPStatus::Server::VersionNotSupported = 505;
const HTTPStatus::T		HTTPStatus::Server::VariantAlsoNegotiates = 506;
const HTTPStatus::T		HTTPStatus::Server::InsufficientStorage = 507;
const HTTPStatus::T		HTTPStatus::Server::LoopDetected = 508;
const HTTPStatus::T		HTTPStatus::Server::BandwidthLimitExceeded = 509;
const HTTPStatus::T		HTTPStatus::Server::NotExtended = 510;
const HTTPStatus::T		HTTPStatus::Server::NetworkAuthenticationRequired = 511;
const HTTPStatus::T		HTTPStatus::Server::NetworkReadTimeout = 512;
const HTTPStatus::T		HTTPStatus::Server::NetworkConnectTimeout = 513;

defSection(HTTPStatus::Info, "Information", 100, 199);
defSection(HTTPStatus::Success, "Success", 200, 299);
defSection(HTTPStatus::Redirect, "Redirection", 300, 399);
defSection(HTTPStatus::Client, "Client", 400, 499);
defSection(HTTPStatus::Server, "Server", 500, 599);

void							HTTPStatus::init() {

}

const HTTPStatus::NameMap		&HTTPStatus::List() {
	static NameMap				names;
	if (names.empty()) {
		defCode(names, Info, Continue);
		defCode(names, Info, SwitchingProtocols);
		defCode(names, Info, Processing);

		defCode(names, Success, Ok);
		defCode(names, Success, Created);
		defCode(names, Success, Accepted);
		defCode(names, Success, NonAuthInfo);
		defCode(names, Success, NoContent);
		defCode(names, Success, ResetContent);
		defCode(names, Success, PartialContent);
		defCode(names, Success, MultiStatus);
		defCode(names, Success, AlreadyReported);
		defCode(names, Success, IMUsed);

		defCode(names, Redirect, MultipleChoices);
		defCode(names, Redirect, MovedPermanently);
		defCode(names, Redirect, Found);
		defCode(names, Redirect, SeeOther);
		defCode(names, Redirect, NotModified);
		defCode(names, Redirect, UseProxy);
		defCode(names, Redirect, Unused);
		defCode(names, Redirect, TempRedirect);
		defCode(names, Redirect, PermRedirect);

		defCode(names, Client, BadRequest);
		defCode(names, Client, Unauthorized);
		defCode(names, Client, PaymentRequired);
		defCode(names, Client, Forbidden);
		defCode(names, Client, NotFound);
		defCode(names, Client, MethodNotAllowed);
		defCode(names, Client, NotAcceptable);
		defCode(names, Client, ProxyAuthRequired);
		defCode(names, Client, RequestTimeout);
		defCode(names, Client, Conflict);
		defCode(names, Client, Gone);
		defCode(names, Client, LengthRequired);
		defCode(names, Client, PreconditionFailed);
		defCode(names, Client, RequestEntityTooLarge);
		defCode(names, Client, RequestURITooLong);
		defCode(names, Client, UnsupportedMediaType);
		defCode(names, Client, RequestedRangeNotSatisfiable);
		defCode(names, Client, ExpectationFailed);
		defCode(names, Client, ImATeapot);
		defCode(names, Client, EnhanceYourCalm);
		defCode(names, Client, UnprocessableEntity);
		defCode(names, Client, Locked);
		defCode(names, Client, FailedDependency);
		defCode(names, Client, Reserved);
		defCode(names, Client, UpgradeRequired);
		defCode(names, Client, PreconditionRequired);
		defCode(names, Client, TooManyRequests);
		defCode(names, Client, RequestHeaderFieldsTooLarge);
		defCode(names, Client, NoResponse);
		defCode(names, Client, RetryWith);
		defCode(names, Client, Blocked);
		defCode(names, Client, UnavailableForLegalReasons);
		defCode(names, Client, ClientClosedRequest);

		defCode(names, Server, InternalError);
		defCode(names, Server, NotImplemented);
		defCode(names, Server, BadGateway);
		defCode(names, Server, ServiceUnavailable);
		defCode(names, Server, GatewayTimeout);
		defCode(names, Server, VersionNotSupported);
		defCode(names, Server, VariantAlsoNegotiates);
		defCode(names, Server, InsufficientStorage);
		defCode(names, Server, LoopDetected);
		defCode(names, Server, BandwidthLimitExceeded);
		defCode(names, Server, NotExtended);
		defCode(names, Server, NetworkAuthenticationRequired);
		defCode(names, Server, NetworkReadTimeout);
		defCode(names, Server, NetworkConnectTimeout);
	}
	return (names);
}
std::string						HTTPStatus::CodeName(T id) {
	NameMap::const_iterator it(List().find(id));
	std::string				str;
	str = "Unknown";
	if (it != List().end()) {
		str = it->second;
	}
	return (str);
}

void __attribute__ ((constructor)) init_statuses()
{
	HTTPStatus::init();
}

//#endif
