/*
 * Channel.h
 *
 *  Created on: 2018年6月27日
 *      Author: panhongfa
 */

#ifndef EFSERVER_CHANNEL_H_
#define EFSERVER_CHANNEL_H_

#include <map>
#include "Session.h"
#include "Handler.h"
#include "Instrument.h"
#include "servant/Servant.h"
#include "TraderCallback.h"

class CThostFtdcTraderApi;
class Channel : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	typedef enum LinkStatus {
		link_off = 0,
		link_on
	}LinkStatus;

	Channel(string& ip, string& path);
	virtual ~Channel();

	virtual void initialize(HandlerPtr& handler);
	virtual void release();

	bool fetchSession(const string& account, SessionPtr& session);

	bool fetchSession(int actionid, SessionPtr& session);

	SessionPtr& createSession(const string& account, const string& pwd);

	void removeSession(const string& account);

	bool fetchInstrument(const string& code, InstrumentPtr& instrument);

	bool fetchInstrument(int actionid, InstrumentPtr& instrument);

	InstrumentPtr& createInstrument(const string& code);

	void removeInstrument(const string& code);

	int curActionId() const {
		return _nActionID;
	}

	int incActionId() {
		_nActionID++;
		return curActionId();
	}

	bool isLinkOnLine() {
		return true;
	}

	int getLinkStatus() const {
		return _nLinkStatus;
	}

	void setLinkStatus(int linkStatus) {
		_nLinkStatus = linkStatus;
	}

	virtual int userLogin(TraderCallbackPtr& cb, const Center::userLoginRequest& req);
	virtual int userLogout(TraderCallbackPtr& cb, const Center::userLogoutRequest& req);

	virtual int orderOpen(TraderCallbackPtr& cb, const Center::orderOpenRequest& req);
	virtual int orderClose(TraderCallbackPtr& cb, const Center::orderCloseRequest& req);

	virtual int instrumentCommission(TraderCallbackPtr& cb, const Center::instrumentCommissionQueryRequest& req);
	virtual int instrumentQuery(TraderCallbackPtr& cb, const Center::instrumentQueryRequest& req);
	virtual int marketDataQuery(TraderCallbackPtr& cb, const Center::marketDataQueryRequest& req);

protected:
	bool transformUserLoginParam(const Center::userLoginRequest& req_src,
			CThostFtdcReqUserLoginField *req_dst);

	bool transformUserLogoutParam(const Center::userLogoutRequest& req_src,
			CThostFtdcUserLogoutField *req_dst);

	bool transfromInstrumentCommissionParam(const Center::instrumentCommissionQueryRequest& req_src,
			CThostFtdcQryInstrumentCommissionRateField *req_dst);

	bool transformInstrumentQueryParam(const Center::instrumentQueryRequest& req_src,
			CThostFtdcQryInstrumentField *req_dst);

	bool transformMarketDataQueryParam(const Center::marketDataQueryRequest& req_src,
			CThostFtdcQryDepthMarketDataField *req_dst);

	bool transformOrderInsertParam(const Center::orderOpenRequest& req_src,
			CThostFtdcInputOrderField *req_dst);

	bool transformOrderInsertParam(const Center::orderCloseRequest& req_src,
			CThostFtdcInputOrderField *req_dst);

private:
	int _nActionID;
	int _nLinkStatus;

	string _strFrontIp;
	string _strTempPath;

	CThostFtdcTraderApi* _pTraderApi;
	list<SessionPtr> _lstSessions;
	list<InstrumentPtr> _lstInstruments;
};
typedef tars::TC_AutoPtr<Channel> ChannelPtr;

#endif /* EFSERVER_CHANNEL_H_ */
