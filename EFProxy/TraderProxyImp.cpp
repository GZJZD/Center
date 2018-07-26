/*
 * TraderProxyImp.cpp
 *
 *  Created on: 2018年7月18日
 *      Author: panhongfa
 */

#include "TraderProxyImp.h"
#include "servant/Application.h"
#include "promise/when_all.h"
#include "Common.h"
#include "servant/TarsLogger.h"

//////////////////////////////////////////////////////
void TraderProxyImp::initialize()
{
	_pTraderAPrx = Application::getCommunicator()->stringToProxy<Center::TraderServantPrx>("Center.EFServer.TraderServantAObj");
	_pTraderBPrx = Application::getCommunicator()->stringToProxy<Center::TraderServantPrx>("Center.EFServer.TraderServantBObj");
	_pTraderCPrx = Application::getCommunicator()->stringToProxy<Center::TraderServantPrx>("Center.EFServer.TraderServantCObj");
}

//////////////////////////////////////////////////////
void TraderProxyImp::destroy()
{

}

void TraderProxyImp::handleUserLogin(TarsCurrentPtr current,
		const LoginResults& results) {
	int ret = tars::TARSSERVERSUCCESS;
	Center::userLoginResponse rsp;

    try
    {
    	promise::Tuple<LoginResult,LoginResult,LoginResult> datas = results.get();
    	do {
        	//有一个channel登陆失败则返回登录失败
        	TraderServantPrxCallbackPromise::PromiseuserLoginPtr loginPtrA = datas.get<0>().get();
   			ret = loginPtrA->_ret;
    		rsp = loginPtrA->rsp;

    		if (loginPtrA->_ret != tars::TARSSERVERSUCCESS || loginPtrA->rsp.errcode != Center::SUCCESS) {
    			TLOGDEBUG("[Center][TraderProxyImp::handleUserLogin] channel A userlogin failed," <<\
    					"res = " << ret <<\
    					"errcode = " << rsp.errcode << endl);
    			break;
    		}
    		else {
    			continue;
    		}

        	//有一个channel登陆失败则返回登录失败
    		TraderServantPrxCallbackPromise::PromiseuserLoginPtr loginPtrB = datas.get<1>().get();
   			ret = loginPtrB->_ret;
    		rsp = loginPtrB->rsp;

    		if (loginPtrB->_ret != tars::TARSSERVERSUCCESS || loginPtrB->rsp.errcode != Center::SUCCESS) {
    			TLOGDEBUG("[Center][TraderProxyImp::handleUserLogin] channel B userlogin failed," <<\
    					"res = " << ret <<\
    					"errcode = " << rsp.errcode << endl);
    			break;
    		}
    		else {
    			continue;
    		}

        	//有一个channel登陆失败则返回登录失败
    		TraderServantPrxCallbackPromise::PromiseuserLoginPtr loginPtrC = datas.get<2>().get();
   			ret = loginPtrC->_ret;
    		rsp = loginPtrC->rsp;

    		if (loginPtrC->_ret != tars::TARSSERVERSUCCESS || loginPtrC->rsp.errcode != Center::SUCCESS) {
    			TLOGDEBUG("[Center][TraderProxyImp::handleUserLogin] channel C userlogin failed," <<\
    					"res = " << ret <<\
    					"errcode = " << rsp.errcode << endl);
    			break;
    		}
    		else {
    			continue;
    		}

    	} while(false);
    }
    catch (exception& e)
    {
        ret = -1;
        rsp.errmsg = e.what();

    	TLOGDEBUG("[Center][TraderProxyImp::handleUserLogin] Exception," <<\
    			e.what() << endl);
    }

    TraderServant::async_response_userLogin(current, ret, rsp);
}

tars::Int32 TraderProxyImp::userLogin(const Center::userLoginRequest& req,
		Center::userLoginResponse& rsp, tars::TarsCurrentPtr current) {
	map<string, string> ctx;
	LoginResult resultA = _pTraderAPrx->promise_async_userLogin(req, ctx);
	LoginResult resultB = _pTraderBPrx->promise_async_userLogin(req, ctx);
	LoginResult resultC = _pTraderCPrx->promise_async_userLogin(req, ctx);

	LoginResults results = promise::whenAll(resultA, resultB, resultC);
	results.then(tars::TC_Bind(&handleUserLogin, current));
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderProxyImp::userLogout(const Center::userLogoutRequest& req,
		Center::userLogoutResponse& rsp, tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderProxyImp::orderOpen(const Center::orderOpenRequest& req,
		Center::orderOpenResponse& rsp, tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderProxyImp::orderClose(const Center::orderCloseRequest& req,
		Center::orderCloseResponse& rsp, tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderProxyImp::instrumentCommissionQuery(
		const Center::instrumentCommissionQueryRequest& req,
		Center::instrumentCommissionQueryResponse& rsp,
		tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderProxyImp::instrumentQuery(
		const Center::instrumentQueryRequest& req,
		Center::instrumentQueryResponse& rsp, tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderProxyImp::marketDataQuery(
		const Center::marketDataQueryRequest& req,
		Center::marketDataQueryResponse& rsp, tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}
