/*
 * TraderProxyImp.h
 *
 *  Created on: 2018年7月18日
 *      Author: panhongfa
 */

#ifndef EFPROXY_TRADERPROXYIMP_H_
#define EFPROXY_TRADERPROXYIMP_H_

#include "servant/Application.h"
#include "TraderServant.h"
#include "promise/tuple.h"

using namespace std;
using namespace tars;
using namespace Center;

class TraderProxyImp : public Center::TraderServant {
public:
	~TraderProxyImp() {}

    virtual void initialize();
    virtual void destroy();

    virtual tars::Int32 userLogin(const Center::userLoginRequest & req,
    		Center::userLoginResponse & rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 userLogout(const Center::userLogoutRequest & req,
    		Center::userLogoutResponse & rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 orderOpen(const Center::orderOpenRequest & req,
    		Center::orderOpenResponse & rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 orderClose(const Center::orderCloseRequest & req,
    		Center::orderCloseResponse & rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 instrumentCommissionQuery(const Center::instrumentCommissionQueryRequest & req,
    		Center::instrumentCommissionQueryResponse &rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 instrumentQuery(const Center::instrumentQueryRequest & req,
    		Center::instrumentQueryResponse &rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 marketDataQuery(const Center::marketDataQueryRequest & req,
    		Center::marketDataQueryResponse &rsp,tars::TarsCurrentPtr current);

public:
    typedef promise::Future<TraderServantPrxCallbackPromise::PromiseuserLoginPtr> LoginResult;
    typedef promise::Future< promise::Tuple<LoginResult,LoginResult,LoginResult> > LoginResults;

    static void handleUserLogin(TarsCurrentPtr current, const LoginResults &results);

private:
    Center::TraderServantPrx _pTraderAPrx;
    Center::TraderServantPrx _pTraderBPrx;
    Center::TraderServantPrx _pTraderCPrx;
};

#endif /* EFPROXY_TRADERPROXYIMP_H_ */
