/*
 * Channel.cpp
 *
 *  Created on: 2018年6月27日
 *      Author: panhongfa
 */

#include "Channel.h"
#include "Common.h"
#include "servant/TarsLogger.h"
#include "include/ThostFtdcTraderApi.h"

Channel::Channel(string& ip, string& path) :
_strFrontIp(ip), _strTempPath(path) {
	_nActionID = 0;
	_nLinkStatus = 0;
	_pTraderApi = NULL;
}

Channel::~Channel() {
	// TODO Auto-generated destructor stub
}

void Channel::initialize(HandlerPtr& handler) {
	//初始化交易代理商接口
	_pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(_strTempPath.c_str());

	//注册Handler
	_pTraderApi->RegisterSpi(handler.get());

	//订阅私有流和公有流
	_pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	_pTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);

	//建立与交易前置的TCP链路
	_pTraderApi->RegisterFront((char *)_strFrontIp.c_str());
	_pTraderApi->Init();

	TLOGDEBUG("[Center][Channel::initialize] trader channel initialize, " << \
			" pszFlowPath = " << _strTempPath << ";" << \
			" pszFrontAddress = " << _strFrontIp << endl);
}

void Channel::release() {
	//释放UserApi
	if (_pTraderApi) {
		_pTraderApi->RegisterSpi(NULL);
		_pTraderApi->Release();
		_pTraderApi = NULL;
	}

	TLOGDEBUG("[Center][Channel::release] trader channel release, " << \
			"pszFlowPath = " << _strTempPath << "; " << \
			"pszFrontAddress = " << _strFrontIp << endl);
}

bool Channel::fetchSession(const string& account, SessionPtr& session) {
	TC_ThreadLock::Lock lock(*this);

	list<SessionPtr>::iterator it = _lstSessions.begin();
	while (it != _lstSessions.end())
	{
		session = *(it);
		if (session->getAccount() == account) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

bool Channel::fetchSession(int actionid, SessionPtr& session) {
	TC_ThreadLock::Lock lock(*this);

	list<SessionPtr>::iterator it = _lstSessions.begin();
	while (it != _lstSessions.end())
	{
		session = *(it);
		TraderCallbackPtr cb = NULL;
		if (session->getActionContext(actionid, cb)) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

SessionPtr& Channel::createSession(const string& account, const string& pwd) {
	TC_ThreadLock::Lock lock(*this);

	SessionPtr session = new Session(account, pwd);
	_lstSessions.insert(_lstSessions.end(), session);

	TLOGDEBUG("[Center][Channel::createSession] create new session, " << \
			"session.account = " << session->getAccount() << "; " << \
			"session.status = " << session->getEumStatus() << endl);

	return _lstSessions.back();
}

void Channel::removeSession(const string& account) {
	TC_ThreadLock::Lock lock(*this);

	list<SessionPtr>::iterator it = _lstSessions.begin();
	while (it != _lstSessions.end())
	{
		SessionPtr& session = *it;
		if (session->getAccount() == account) {
			it = _lstSessions.erase(it);
		}
		else {
			it++;
		}
	}
}

bool Channel::fetchInstrument(const string& code, InstrumentPtr& instrument) {
	TC_ThreadLock::Lock lock(*this);

	list<InstrumentPtr>::iterator it = _lstInstruments.begin();
	while (it != _lstInstruments.end())
	{
		instrument = *(it);
		if (instrument->getInstrumentId() == code) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

bool Channel::fetchInstrument(int actionid, InstrumentPtr& instrument) {
	TC_ThreadLock::Lock lock(*this);

	list<InstrumentPtr>::iterator it = _lstInstruments.begin();
	while (it != _lstInstruments.end())
	{
		instrument = *(it);
		TraderCallbackPtr cb = NULL;
		if (instrument->getActionContext(actionid, cb)) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

InstrumentPtr& Channel::createInstrument(const string& code) {
	TC_ThreadLock::Lock lock(*this);

	InstrumentPtr instrument = new Instrument(code);
	_lstInstruments.insert(_lstInstruments.end(), instrument);

	TLOGDEBUG("[Center][Channel::createInstrument] create new instrument, " << \
			"instrument.code = " << instrument->getInstrumentId() << endl);

	return _lstInstruments.back();
}

void Channel::removeInstrument(const string& code) {
	TC_ThreadLock::Lock lock(*this);

	list<InstrumentPtr>::iterator it = _lstInstruments.begin();
	while (it != _lstInstruments.end())
	{
		InstrumentPtr& instrument = *it;
		if (instrument->getInstrumentId() == code) {
			it = _lstInstruments.erase(it);
		}
		else {
			it++;
		}
	}
}

int Channel::userLogin(TraderCallbackPtr& cb, const Center::userLoginRequest& req) {
	SessionPtr session = NULL;
	if (!fetchSession(req.userId, session)) {
		//session不存在时，则创建该session
		session = createSession(req.userId, req.password);
	}

	//处理请求参数非法
	CThostFtdcReqUserLoginField reqUserLogin;
	if (!transformUserLoginParam(req, &reqUserLogin)) {
		TLOGERROR("[Center][Channel::userLogin] param transform error, " << \
				"account = " + req.userId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqUserLogin(&reqUserLogin, incActionId());
	if (0 == res) {
		//更新session状态
		session->setEumStatus(Session::logining);

		//保存调用交易API的登录请求后产生的上下文
		cb->setActionID(curActionId());
		session->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::userLogin] ReqUserLogin api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

int Channel::userLogout(TraderCallbackPtr& cb, const Center::userLogoutRequest& req) {
	SessionPtr session = NULL;
	if (!fetchSession(req.userId, session)) {
		TLOGERROR("[Center][Channel::userLogout] fetch session error, " << \
				"account = " + req.userId << endl);
		return Center::UNKNOWN_ERROR;
	}

	//处理会话还有未处理的报单
	if (!session->tryLogoutSession()) {
		TLOGERROR("[Center][Channel::userLogout] unfinished order error, " << \
				"account = " + req.userId << endl);
		return Center::ORDER_UNFINISHED;
	}

	//处理请求参数非法
	CThostFtdcUserLogoutField reqUserLogout;
	if (!transformUserLogoutParam(req, &reqUserLogout)) {
		TLOGERROR("[Center][Channel::userLogout] param transform error, " << \
				"account = " + req.userId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqUserLogout(&reqUserLogout, incActionId());
	if (0 == res) {
		//更新session状态
		session->setEumStatus(Session::logouting);

		//保存调用交易API的注销请求后产生的上下文
		cb->setActionID(curActionId());
		session->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::userLogin] ReqUserLogin api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

int Channel::orderOpen(TraderCallbackPtr& cb, const Center::orderOpenRequest& req) {
	SessionPtr session = NULL;
	if (!fetchSession(req.userId, session)) {
		TLOGERROR("[Center][Channel::orderOpen] fetch session error, " << \
				"account = " + req.userId << endl);
		return Center::UNKNOWN_ERROR;
	}

	OrderPtr order = NULL;
	if (!session->fetchOrder(req.requestId, order)) {
		//order不存在时，则创建该order
		order = session->createOrder(req.requestId);
	}

	//处理请求参数非法
	CThostFtdcInputOrderField reqInputOrder;
	if (!transformOrderInsertParam(req, &reqInputOrder)) {
		TLOGERROR("[Center][Channel::orderOpen] param transform error, " << \
				"order = " + req.requestId << "; " << \
				"account = " + req.userId << "; " << \
				"instrument = " + req.instrumentId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//设置市价报单限定参数
	{
		//成交量类型设定为任何数量
		reqInputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		//最小成交量设定为1
		reqInputOrder.MinVolume = 1;
		//强平原因设定为非强平
		reqInputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		//自定挂起标志设定为否
		reqInputOrder.IsAutoSuspend = 0;
		//用户强平标志设定为否
		reqInputOrder.UserForceClose = 0;

		//报单价格条件类型设定为任意价
		reqInputOrder.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		//市价单价格设定为0
		reqInputOrder.LimitPrice = 0;
		//有效期类型设定为立即完成，否则撤销
		reqInputOrder.TimeCondition = THOST_FTDC_TC_IOC;

		//开平标志，固定设定为开仓标志
		reqInputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqOrderInsert(&reqInputOrder, incActionId());
	if (0 == res) {
		//更新报单标记字段
		order->setOpenFlag(THOST_FTDC_OF_Open);
		order->setBuyFlag(reqInputOrder.Direction);

		//保存调用交易API的登录请求后产生的上下文
		cb->setActionID(curActionId());
		order->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::orderOpen] ReqOrderInsert api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

int Channel::orderClose(TraderCallbackPtr& cb, const Center::orderCloseRequest& req) {
	SessionPtr session = NULL;
	if (!fetchSession(req.userId, session)) {
		TLOGERROR("[Center][Channel::orderClose] fetch session error, " << \
				"account = " + req.userId << endl);
		return Center::UNKNOWN_ERROR;
	}

	OrderPtr order = NULL;
	if (!session->fetchOrder(req.requestId, order)) {
		//order不存在时，则创建该order
		order = session->createOrder(req.requestId);
	}

	//处理请求参数非法
	CThostFtdcInputOrderField reqInputOrder;
	if (!transformOrderInsertParam(req, &reqInputOrder)) {
		TLOGERROR("[Center][Channel::orderClose] param transform error, " << \
				"order = " + req.requestId << "; " << \
				"account = " + req.userId << "; " << \
				"instrument = " + req.instrumentId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//设置市价报单限定参数
	{
		//成交量类型设定为任何数量
		reqInputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		//最小成交量设定为1
		reqInputOrder.MinVolume = 1;
		//强平原因设定为非强平
		reqInputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		//自定挂起标志设定为否
		reqInputOrder.IsAutoSuspend = 0;
		//用户强平标志设定为否
		reqInputOrder.UserForceClose = 0;

		//报单价格条件类型设定为任意价
		reqInputOrder.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		//市价单价格设定为0
		reqInputOrder.LimitPrice = 0;
		//有效期类型设定为立即完成，否则撤销
		reqInputOrder.TimeCondition = THOST_FTDC_TC_IOC;

		//开平标志，固定设定为开仓标志
		reqInputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqOrderInsert(&reqInputOrder, incActionId());
	if (0 == res) {
		//保存调用交易API的登录请求后产生的上下文
		cb->setActionID(curActionId());
		order->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::orderClose] ReqOrderInsert api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

int Channel::instrumentCommission(TraderCallbackPtr& cb,
		const Center::instrumentCommissionQueryRequest& req) {
	InstrumentPtr instrument = NULL;
	if (!fetchInstrument(req.instrumentId, instrument)) {
		TLOGERROR("[Center][Channel::instrumentCommissionQuery] fetch instrument error, " << \
				"code = " + req.instrumentId << endl);
		return Center::UNKNOWN_ERROR;
	}

	//处理请求参数非法
	CThostFtdcQryInstrumentCommissionRateField reqInstrumentCommissionRate;
	if (!transfromInstrumentCommissionParam(req, &reqInstrumentCommissionRate)) {
		TLOGERROR("[Center][Channel::instrumentCommissionQuery] param transform error, " << \
				"account = " + req.userId << "; " << \
				"code = " + req.instrumentId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqQryInstrumentCommissionRate(&reqInstrumentCommissionRate, incActionId());
	if (0 == res) {
		//保存调用交易API的注销请求后产生的上下文
		cb->setActionID(curActionId());
		instrument->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::instrumentCommissionQuery] ReqQryInstrumentCommissionRate api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

int Channel::instrumentQuery(TraderCallbackPtr& cb, const Center::instrumentQueryRequest& req) {
	InstrumentPtr instrument = NULL;
	if (!fetchInstrument(req.instrumentId, instrument)) {
		TLOGERROR("[Center][Channel::instrumentQuery] fetch instrument error, " << \
				"code = " + req.instrumentId << endl);
		return Center::UNKNOWN_ERROR;
	}

	//处理请求参数非法
	CThostFtdcQryInstrumentField reqInstrument;
	if (!transformInstrumentQueryParam(req, &reqInstrument)) {
		TLOGERROR("[Center][Channel::instrumentQuery] param transform error, " << \
				"code = " + req.instrumentId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqQryInstrument(&reqInstrument, incActionId());
	if (0 == res) {
		//保存调用交易API的注销请求后产生的上下文
		cb->setActionID(curActionId());
		instrument->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::instrumentQuery] ReqQryInstrument api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

int Channel::marketDataQuery(TraderCallbackPtr& cb, const Center::marketDataQueryRequest& req) {
	InstrumentPtr instrument = NULL;
	if (!fetchInstrument(req.instrumentId, instrument)) {
		TLOGERROR("[Center][Channel::marketDataQuery] fetch instrument error, " << \
				"code = " + req.instrumentId << endl);
		return Center::UNKNOWN_ERROR;
	}

	//处理请求参数非法
	CThostFtdcQryDepthMarketDataField reqMarketData;
	if (!transformMarketDataQueryParam(req, &reqMarketData)) {
		TLOGERROR("[Center][Channel::marketDataQuery] param transform error, " << \
				"code = " + req.instrumentId << endl);
		return Center::PARAM_LENGTH_ERROR;
	}

	//调用API登录接口
	int res = _pTraderApi->ReqQryDepthMarketData(&reqMarketData, incActionId());
	if (0 == res) {
		//保存调用交易API的注销请求后产生的上下文
		cb->setActionID(curActionId());
		instrument->pushActionContext(cb);
	}
	else if (-1 == res) {
		res = Center::REQT_SEND_FAILED;
	}
	else if (-2 == res) {
		res = Center::REQT_QUEUESIZE_OVERLIMIT;
	}
	else if (-3 == res) {
		res = Center::REQT_RATEPERSEC_OVERLIMIT;
	}

	TLOGDEBUG("[Center][Channel::marketDataQuery] ReqQryDepthMarketData api called, " << \
			"actionid = " << curActionId() << "; " << \
			"result = " << res << endl);

	return res;
}

bool Channel::transformUserLoginParam(const Center::userLoginRequest& req_src,
		CThostFtdcReqUserLoginField* req_dst) {
	do {
		memset(req_dst, 0, sizeof(CThostFtdcReqUserLoginField));

		unsigned int brokeridSize = sizeof(req_dst->BrokerID)/sizeof(char);
		if (req_src.brokerId.length() >= brokeridSize) {
			break;
		}
		strncpy(req_dst->BrokerID, req_src.brokerId.c_str(), brokeridSize);

		unsigned int useridSize = sizeof(req_dst->UserID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->UserID, req_src.userId.c_str(), useridSize);

		unsigned int passwdSize = sizeof(req_dst->Password)/sizeof(char);
		if (req_src.password.length() >= passwdSize) {
			break;
		}
		strncpy(req_dst->Password, req_src.password.c_str(), passwdSize);

		unsigned int productSize = sizeof(req_dst->ProtocolInfo)/sizeof(char);
		if (req_src.userProductInfo.length() >= productSize) {
			break;
		}
		strncpy(req_dst->ProtocolInfo, req_src.userProductInfo.c_str(), productSize);

		return true;
	} while(false);
	return false;
}

bool Channel::transformUserLogoutParam(const Center::userLogoutRequest& req_src,
		CThostFtdcUserLogoutField* req_dst) {
	do {
		memset(req_dst, 0, sizeof(CThostFtdcUserLogoutField));

		unsigned int brokeridSize = sizeof(req_dst->BrokerID)/sizeof(char);
		if (req_src.brokerId.length() >= brokeridSize) {
			break;
		}
		strncpy(req_dst->BrokerID, req_src.brokerId.c_str(), brokeridSize);

		unsigned int useridSize = sizeof(req_dst->UserID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->UserID, req_src.userId.c_str(), useridSize);

		return true;
	} while(false);
	return false;
}

bool Channel::transfromInstrumentCommissionParam(
		const Center::instrumentCommissionQueryRequest& req_src,
		CThostFtdcQryInstrumentCommissionRateField* req_dst) {
	do {
		memset(req_dst, 0, sizeof(CThostFtdcQryInstrumentCommissionRateField));

		unsigned int brokeridSize = sizeof(req_dst->BrokerID)/sizeof(char);
		if (req_src.brokerId.length() >= brokeridSize) {
			break;
		}
		strncpy(req_dst->BrokerID, req_src.brokerId.c_str(), brokeridSize);

		//注意：目前UserID、AccountID、InvestorID，如果要填，都填一样的
		unsigned int useridSize = sizeof(req_dst->InvestorID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->InvestorID, req_src.userId.c_str(), useridSize);

		unsigned int instrumentSize = sizeof(req_dst->InstrumentID)/sizeof(char);
		if (req_src.instrumentId.length() >= instrumentSize) {
			break;
		}
		strncpy(req_dst->InstrumentID, req_src.instrumentId.c_str(), instrumentSize);

		return true;
	} while(false);
	return false;
}

bool Channel::transformInstrumentQueryParam(
		const Center::instrumentQueryRequest& req_src,
		CThostFtdcQryInstrumentField* req_dst) {
}

bool Channel::transformMarketDataQueryParam(
		const Center::marketDataQueryRequest& req_src,
		CThostFtdcQryDepthMarketDataField* req_dst) {
}

bool Channel::transformOrderInsertParam(const Center::orderOpenRequest& req_src,
		CThostFtdcInputOrderField *req_dst) {
	do {
		memset(req_dst, 0, sizeof(CThostFtdcInputOrderField));

		//设定买卖方向, 0代表买，1代表卖
		req_dst->Direction = (req_src.orderDirection == 0) ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;

		//数量
		req_dst->VolumeTotalOriginal = req_src.volumeTotalOriginal;

		//传入的请求参数赋值
		unsigned int brokeridSize = sizeof(req_dst->BrokerID)/sizeof(char);
		if (req_src.brokerId.length() >= brokeridSize) {
			break;
		}
		strncpy(req_dst->BrokerID, req_src.brokerId.c_str(), brokeridSize);

		//注意：目前UserID、AccountID、InvestorID，如果要填，都填一样的
		unsigned int useridSize = sizeof(req_dst->UserID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->UserID, req_src.userId.c_str(), useridSize);

		useridSize = sizeof(req_dst->InvestorID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->InvestorID, req_src.userId.c_str(), useridSize);

		unsigned int instrumentSize = sizeof(req_dst->InstrumentID)/sizeof(char);
		if (req_src.instrumentId.length() >= instrumentSize) {
			break;
		}
		strncpy(req_dst->InstrumentID, req_src.instrumentId.c_str(), instrumentSize);

		return true;
	} while(false);
	return false;
}

bool Channel::transformOrderInsertParam(const Center::orderCloseRequest& req_src,
		CThostFtdcInputOrderField *req_dst) {
	do {
		memset(req_dst, 0, sizeof(CThostFtdcInputOrderField));

		//设定买卖方向, 0代表买，1代表卖
		req_dst->Direction = (req_src.orderDirection == 0) ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;

		//数量
		req_dst->VolumeTotalOriginal = req_src.orderVolume;

		//传入的请求参数赋值
		unsigned int brokeridSize = sizeof(req_dst->BrokerID)/sizeof(char);
		if (req_src.brokerId.length() >= brokeridSize) {
			break;
		}
		strncpy(req_dst->BrokerID, req_src.brokerId.c_str(), brokeridSize);

		//注意：目前UserID、AccountID、InvestorID，如果要填，都填一样的
		unsigned int useridSize = sizeof(req_dst->UserID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->UserID, req_src.userId.c_str(), useridSize);

		useridSize = sizeof(req_dst->InvestorID)/sizeof(char);
		if (req_src.userId.length() >= useridSize) {
			break;
		}
		strncpy(req_dst->InvestorID, req_src.userId.c_str(), useridSize);

		unsigned int instrumentSize = sizeof(req_dst->InstrumentID)/sizeof(char);
		if (req_src.instrumentId.length() >= instrumentSize) {
			break;
		}
		strncpy(req_dst->InstrumentID, req_src.instrumentId.c_str(), instrumentSize);

		return true;
	} while(false);
	return false;
}
