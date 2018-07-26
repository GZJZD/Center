#ifndef _TraderServantImp_H_
#define _TraderServantImp_H_

#include <string>
#include "servant/Application.h"
#include "TraderServant.h"
#include "Opctx.h"

class TraderServantImp : public Center::TraderServant
{
public:
	typedef tars::TC_ThreadQueue<OpctxPtr> OpctxThreadQueue;
    static string getErrorMessage(int errorcode);
    static string gbk2utf8ErrorMessage(const char* errormsg);

public:
    virtual ~TraderServantImp() {}

    virtual void initialize();
    virtual void destroy();

    void pushCustomMessage(const OpctxPtr& op);

protected:
    virtual tars::Int32 userLogin(const Center::userLoginRequest & req, Center::userLoginResponse & rsp,tars::TarsCurrentPtr current);
    virtual tars::Int32 userLogout(const Center::userLogoutRequest & req, Center::userLogoutResponse & rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 orderOpen(const Center::orderOpenRequest & req, Center::orderOpenResponse & rsp,tars::TarsCurrentPtr current);
    virtual tars::Int32 orderClose(const Center::orderCloseRequest & req, Center::orderCloseResponse & rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 instrumentCommissionQuery(const Center::instrumentCommissionQueryRequest & req,Center::instrumentCommissionQueryResponse &rsp,tars::TarsCurrentPtr current);
    virtual tars::Int32 instrumentQuery(const Center::instrumentQueryRequest & req,Center::instrumentQueryResponse &rsp,tars::TarsCurrentPtr current);

    virtual tars::Int32 marketDataQuery(const Center::marketDataQueryRequest & req,Center::marketDataQueryResponse &rsp,tars::TarsCurrentPtr current);

    virtual int doCustomMessage(bool bExpectIdle);

private:
    int fetchChannel(ChannelPtr& channel);

    void handleUserLoginNotify(const OpctxPtr& op);

    void handleUserLogoutNotify(const OpctxPtr& op);

    void handleOrderInsertNotify(const OpctxPtr& op);

    void handleOrderActionNotify(const OpctxPtr& op);

    void handleOrderRtnNotify(const OpctxPtr& op);

    void handleTradeRtnNotify(const OpctxPtr& op);

    void handleInstrumentCommissionNotify(const OpctxPtr& op);

    void handleInstrumentQueryNotify(const OpctxPtr& op);

    void handleMarketDataQueryNotify(const OpctxPtr& op);

    void handleRspErrorNotify(const OpctxPtr& op);

protected:
	OpctxThreadQueue _qOpctxQueue;
};
typedef tars::TC_AutoPtr<TraderServantImp> TraderServantImpPtr;

/////////////////////////////////////////////////////
#endif
