#include "TraderServantImp.h"
#include "servant/Application.h"
#include "util/tc_encoder.h"
#include "util/tc_common.h"
#include "Common.h"
#include "Channel.h"
#include "Manager.h"

using namespace std;

//////////////////////////////////////////////////////
void TraderServantImp::initialize()
{

}
//////////////////////////////////////////////////////
void TraderServantImp::destroy()
{

}

string TraderServantImp::getErrorMessage(int errorcode) {
	return "";
}

string TraderServantImp::gbk2utf8ErrorMessage(const char* errormsg) {
	return tars::TC_Encoder::gbk2utf8(errormsg);
}

void TraderServantImp::pushCustomMessage(const OpctxPtr& op) {
	_qOpctxQueue.push_back(op);
	getHandle()->notifyFilter();
}

int TraderServantImp::fetchChannel(ChannelPtr& channel) {
	int result = Center::SUCCESS;
	channel = Manager::getInstance()->getChannel(getName());
	if (!channel) {
		result = Center::UNKNOWN_ERROR;
		break;
	}

	if (!channel->isLinkOnLine()) {
		result = Center::LINK_BROKEN;
		break;
	}
	return result;
}

tars::Int32 TraderServantImp::userLogin(const Center::userLoginRequest& req,
		Center::userLoginResponse& rsp, tars::TarsCurrentPtr current) {
	int result = Center::SUCCESS;
	ChannelPtr channel = NULL;

	TLOGDEBUG("[Center][TraderServantImp::userLogin] userLogin request recved, " << \
			"requestid = " << req.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"threadid = " << getHandle()->id() << endl);

	result = fetchChannel(channel);
	if (result == Center::SUCCESS && channel) {
		TraderCallbackPtr cb = new TraderCallback("userLogin", this, current);
		result = channel->userLogin(cb, req);
	}

	//调用API成功后异步应答
	if (result == Center::SUCCESS) {
		current->setResponse(false);
		break;
	}
	else {
		current->setResponse(true);
		rsp.typeId = req.typeId;
		rsp.requestId = req.requestId;
		rsp.errcode = result;
		rsp.errmsg = getErrorMessage(rsp.errcode);

		TLOGWARN("[Center][TraderServantBase::userLogin] userLogin failed, " << \
				"request = " << req.requestId << "; " << \
				"account = " << req.userId << "; " << \
				"threadid = " << getHandle()->id() << "; " << \
				"errcode = " << rsp.errcode << "; " <<\
				"errmsg = " << rsp.errmsg <<endl);
	}

	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderServantImp::userLogout(const Center::userLogoutRequest& req,
		Center::userLogoutResponse& rsp, tars::TarsCurrentPtr current) {
	int result = Center::SUCCESS;
	ChannelPtr channel = NULL;

	TLOGDEBUG("[Center][TraderServantImp::userLogout] userLogout request recved, " << \
			"requestid = " << req.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"threadid = " << getHandle()->id() << endl);

	result = fetchChannel(channel);
	if (result == Center::SUCCESS && channel) {
		TraderCallbackPtr cb = new TraderCallback("userLogout", this, current);
		result = channel->userLogout(cb, req);
	}

	//调用API成功后异步应答
	if (result == Center::SUCCESS) {
		current->setResponse(false);
		break;
	}
	else {
		current->setResponse(true);
		rsp.typeId = req.typeId;
		rsp.requestId = req.requestId;
		rsp.errcode = result;
		rsp.errmsg = getErrorMessage(rsp.errcode);

		TLOGWARN("[Center][TraderServantBase::userLogout] userLogout failed, " << \
				"request = " << req.requestId << "; " << \
				"account = " << req.userId << "; " << \
				"threadid = " << getHandle()->id() << "; " << \
				"errcode = " << rsp.errcode << "; " <<\
				"errmsg = " << rsp.errmsg <<endl);
	}

	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderServantImp::orderOpen(const Center::orderOpenRequest& req,
		Center::orderOpenResponse& rsp, tars::TarsCurrentPtr current) {
	int result = Center::SUCCESS;
	ChannelPtr channel = NULL;

	TLOGDEBUG("[Center][TraderServantImp::orderOpen] orderOpen request recved, " << \
			"requestid = " << req.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"direction = " << req.orderDirection << ";" << \
			"volume = " << req.volumeTotalOriginal << "; " << \
			"threadid = " << getHandle()->id() << endl);

	result = fetchChannel(channel);
	if (result == Center::SUCCESS && channel) {
		TraderCallbackPtr cb = new TraderCallback("orderOpen", this, current);
		result = channel->orderOpen(cb, req);
	}

	//调用API成功后异步应答
	if (result == Center::SUCCESS) {
		current->setResponse(false);
		break;
	}
	else {
		current->setResponse(true);
		rsp.typeId = req.typeId;
		rsp.requestId = req.requestId;
		rsp.errcode = result;
		rsp.errmsg = getErrorMessage(rsp.errcode);

		TLOGWARN("[Center][TraderServantBase::orderOpen] orderOpen failed, " << \
				"request = " << req.requestId << "; " << \
				"account = " << req.userId << "; " << \
				"instrument = " << req.instrumentId << "; " << \
				"direction = " << req.orderDirection << ";" << \
				"volume = " << req.volumeTotalOriginal << "; " << \
				"threadid = " << getHandle()->id() << "; " << \
				"errcode = " << rsp.errcode << "; " <<\
				"errmsg = " << rsp.errmsg <<endl);
	}

	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderServantImp::orderClose(const Center::orderCloseRequest& req,
		Center::orderCloseResponse& rsp, tars::TarsCurrentPtr current) {
	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderServantImp::instrumentCommissionQuery(
		const Center::instrumentCommissionQueryRequest& req,
		Center::instrumentCommissionQueryResponse& rsp,
		tars::TarsCurrentPtr current) {
	int result = Center::SUCCESS;
	ChannelPtr channel = NULL;

	TLOGDEBUG("[Center][TraderServantImp::instrumentCommissionQuery] instrumentCommissionQuery request recved, " << \
			"requestid = " << req.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"threadid = " << getHandle()->id() << endl);

	result = fetchChannel(channel);
	if (result == Center::SUCCESS && channel) {
		TraderCallbackPtr cb = new TraderCallback("instrumentCommissionQuery", this, current);
		result = channel->instrumentCommission(cb, req);
	}

	//调用API成功后异步应答
	if (result == Center::SUCCESS) {
		current->setResponse(false);
		break;
	}
	else {
		current->setResponse(true);
		rsp.typeId = req.typeId;
		rsp.requestId = req.requestId;
		rsp.errcode = result;
		rsp.errmsg = getErrorMessage(rsp.errcode);

		TLOGWARN("[Center][TraderServantBase::orderOpen] orderOpen failed, " << \
				"request = " << req.requestId << "; " << \
				"account = " << req.userId << "; " << \
				"instrument = " << req.instrumentId << "; " << \
				"threadid = " << getHandle()->id() << "; " << \
				"errcode = " << rsp.errcode << "; " <<\
				"errmsg = " << rsp.errmsg <<endl);
	}

	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderServantImp::instrumentQuery(
		const Center::instrumentQueryRequest& req,
		Center::instrumentQueryResponse& rsp, tars::TarsCurrentPtr current) {
	int result = Center::SUCCESS;
	ChannelPtr channel = NULL;

	TLOGDEBUG("[Center][TraderServantImp::instrumentQuery] instrumentQuery request recved, " << \
			"requestid = " << req.requestId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"threadid = " << getHandle()->id() << endl);

	result = fetchChannel(channel);
	if (result == Center::SUCCESS && channel) {
		TraderCallbackPtr cb = new TraderCallback("instrumentQuery", this, current);
		result = channel->instrumentQuery(cb, req);
	}

	//调用API成功后异步应答
	if (result == Center::SUCCESS) {
		current->setResponse(false);
		break;
	}
	else {
		current->setResponse(true);
		rsp.typeId = req.typeId;
		rsp.requestId = req.requestId;
		rsp.errcode = result;
		rsp.errmsg = getErrorMessage(rsp.errcode);

		TLOGWARN("[Center][TraderServantBase::orderOpen] orderOpen failed, " << \
				"request = " << req.requestId << "; " << \
				"instrument = " << req.instrumentId << "; " << \
				"threadid = " << getHandle()->id() << "; " << \
				"errcode = " << rsp.errcode << "; " <<\
				"errmsg = " << rsp.errmsg <<endl);
	}

	return tars::TARSSERVERSUCCESS;
}

tars::Int32 TraderServantImp::marketDataQuery(
		const Center::marketDataQueryRequest& req,
		Center::marketDataQueryResponse& rsp, tars::TarsCurrentPtr current) {
	int result = Center::SUCCESS;
	ChannelPtr channel = NULL;

	TLOGDEBUG("[Center][TraderServantImp::marketDataQuery] marketDataQuery request recved, " << \
			"requestid = " << req.requestId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"threadid = " << getHandle()->id() << endl);

	result = fetchChannel(channel);
	if (result == Center::SUCCESS && channel) {
		TraderCallbackPtr cb = new TraderCallback("marketDataQuery", this, current);
		result = channel->marketDataQuery(cb, req);
	}

	//调用API成功后异步应答
	if (result == Center::SUCCESS) {
		current->setResponse(false);
		break;
	}
	else {
		current->setResponse(true);
		rsp.typeId = req.typeId;
		rsp.requestId = req.requestId;
		rsp.errcode = result;
		rsp.errmsg = getErrorMessage(rsp.errcode);

		TLOGWARN("[Center][TraderServantBase::orderOpen] orderOpen failed, " << \
				"request = " << req.requestId << "; " << \
				"instrument = " << req.instrumentId << "; " << \
				"threadid = " << getHandle()->id() << "; " << \
				"errcode = " << rsp.errcode << "; " <<\
				"errmsg = " << rsp.errmsg <<endl);
	}

	return tars::TARSSERVERSUCCESS;
}

int TraderServantImp::doCustomMessage(bool bExpectIdle) {

	OpctxPtr opPtr = NULL;
    while (_qOpctxQueue.pop_front(opPtr))
    {
		switch (opPtr->getEvent()->getType()) {
			case Event::user_login_event:
				handleUserLoginNotify(opPtr);
				break;
			case Event::user_logout_event:
				handleUserLogoutNotify(opPtr);
				break;
			case Event::order_insert_event:
				handleOrderInsertNotify(opPtr);
				break;
			case Event::order_action_event:
				handleOrderActionNotify(opPtr);
				break;
			case Event::order_rtn_event:
				handleOrderRtnNotify(opPtr);
				break;
			case Event::trade_rtn_event:
				handleTradeRtnNotify(opPtr);
				break;
			case Event::instrument_commission_event:
				handleInstrumentCommissionNotify(opPtr);
				break;
			case Event::instrument_query_event:
				handleInstrumentQueryNotify(opPtr);
				break;
			case Event::marketdata_query_event:
				handleMarketDataQueryNotify(opPtr);
				break;
			default:
				break;
		}
    }
    return -1;
}

void TraderServantImp::handleUserLoginNotify(const OpctxPtr& op) {
	const TarsCurrentPtr& current = op->getCallback()->getCurrent();

    //从上下文中的请求BUFF中解析出请求对象和应答对象
	Center::userLoginRequest req;
	Center::userLoginResponse rsp;
	if (current->getRequestVersion() == TUPVERSION) {
        UniAttribute<tars::BufferWriter, tars::BufferReader>  tarsAttr;
        tarsAttr.setVersion(current->getRequestVersion());
        tarsAttr.decode(current->getRequestBuffer());
        tarsAttr.get("req", req);
        tarsAttr.getByDefault("rsp", rsp, rsp);
	}

	//更新请求编号
	rsp.typeId = req.typeId;
	rsp.requestId = req.requestId;

	//填充交易API异步反馈的登录结果并更新session对象属性
	do {
		SessionPtr session = NULL;
		UserLoginEventPtr event = UserLoginEventPtr::dynamicCast(op->getEvent());
		if (!event || !op->getChannel()->fetchSession(event->getUserLoginField()->UserID, session)) {
			rsp.errcode = Center::UNKNOWN_ERROR;
			rsp.errmsg = TraderServantImp::getErrorMessage(rsp.errcode);
			break;
		}

		//更新错误码和session状态
		rsp.errcode = event->getRspInfoField()->ErrorID;
		if (rsp.errcode == Center::SUCCESS) {
			rsp.errmsg = "OK";
			session->setEumStatus(Session::logined);
			session->updateProperty(event->getUserLoginField());
		}
		else {
			rsp.errmsg = TraderServantImp::gbk2utf8ErrorMessage(event->getRspInfoField()->ErrorMsg);
			session->setEumStatus(Session::logouted);
		}

		//移除session中保存的请求上下文
		session->delActionContext(event->getRequestId());

	} while(false);

	//完成异步应答
	TraderServant::async_response_userLogin(current, Center::SUCCESS, rsp);

	TLOGDEBUG("[Center][TraderServantImp::handleUserLoginNotify] userLogin response sended, " << \
			"request = " << rsp.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"threadid = " << getHandle()->id() << "; " << \
			"errcode = " << rsp.errcode << "; " << \
			"errmsg = " << rsp.errmsg <<endl);
}

void TraderServantImp::handleUserLogoutNotify(const OpctxPtr& op) {
	const TarsCurrentPtr& current = op->getCallback()->getCurrent();

    //从上下文中的请求BUFF中解析出请求对象和应答对象
	Center::userLogoutRequest req;
	Center::userLogoutResponse rsp;
	if (current->getRequestVersion() == TUPVERSION) {
        UniAttribute<tars::BufferWriter, tars::BufferReader>  tarsAttr;
        tarsAttr.setVersion(current->getRequestVersion());
        tarsAttr.decode(current->getRequestBuffer());
        tarsAttr.get("req", req);
        tarsAttr.getByDefault("rsp", rsp, rsp);
	}

	//更新请求编号
	rsp.typeId = req.typeId;
	rsp.requestId = req.requestId;

	//填充交易API异步反馈的注销结果并删除对应的session对象
	do {
		SessionPtr session = NULL;
		UserLogoutEventPtr event = UserLogoutEventPtr::dynamicCast(op->getEvent());
		if (!event || !op->getChannel()->fetchSession(event->getUserLogoutField()->UserID, session)) {
			rsp.errcode = Center::UNKNOWN_ERROR;
			rsp.errmsg = TraderServantImp::getErrorMessage(rsp.errcode);
			break;
		}

		//更新错误码和session状态
		rsp.errcode = event->getRspInfoField()->ErrorID;
		if (rsp.errcode == Center::SUCCESS) {
			rsp.errmsg = "OK";
			session->setEumStatus(Session::logouted);
		}
		else {
			rsp.errmsg = TraderServantImp::gbk2utf8ErrorMessage(event->getRspInfoField()->ErrorMsg);
		}

		//移除session中保存的请求上下文
		session->delActionContext(event->getRequestId());

		//注销成功后删除session对象
		if (rsp.errcode == Center::SUCCESS) {
			op->getChannel()->removeSession(event->getUserLogoutField()->UserID);
		}

	} while(false);

	//完成异步应答
	TraderServant::async_response_userLogout(current, Center::SUCCESS, rsp);

	TLOGDEBUG("[Center][TraderServantImp::handleUserLogoutNotify] userLogout response sended, " << \
			"request = " << rsp.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"threadid = " << getHandle()->id() << "; " << \
			"errcode = " << rsp.errcode << "; " << \
			"errmsg = " << rsp.errmsg <<endl);
}

void TraderServantImp::handleInstrumentCommissionNotify(const OpctxPtr& op) {
	const TarsCurrentPtr& current = op->getCallback()->getCurrent();

    //从上下文中的请求BUFF中解析出请求对象和应答对象
	Center::instrumentCommissionQueryRequest req;
	Center::instrumentCommissionQueryResponse rsp;
	if (current->getRequestVersion() == TUPVERSION) {
        UniAttribute<tars::BufferWriter, tars::BufferReader>  tarsAttr;
        tarsAttr.setVersion(current->getRequestVersion());
        tarsAttr.decode(current->getRequestBuffer());
        tarsAttr.get("req", req);
        tarsAttr.getByDefault("rsp", rsp, rsp);
	}

	//更新请求编号
	rsp.typeId = req.typeId;
	rsp.requestId = req.requestId;

	//填充查询合约手续费API的返回数据
	do {
		InstrumentPtr instrument = NULL;
		QryInstrumentCommissionEventPtr event = QryInstrumentCommissionEventPtr::dynamicCast(op->getEvent());
		if (!event || !op->getChannel()->fetchInstrument(event->getInstrumentCommissionField()->InstrumentID, instrument)) {
			rsp.errcode = Center::UNKNOWN_ERROR;
			rsp.errmsg = TraderServantImp::getErrorMessage(rsp.errcode);
			break;
		}

		//更新错误码和session状态
		rsp.errcode = event->getRspInfoField()->ErrorID;
		if (rsp.errcode == Center::SUCCESS) {
			rsp.errmsg = "OK";
			instrument->updateProperty(event->getInstrumentCommissionField());
		}
		else {
			rsp.errmsg = TraderServantImp::gbk2utf8ErrorMessage(event->getRspInfoField()->ErrorMsg);
		}

		//移除session中保存的请求上下文
		instrument->delActionContext(event->getRequestId());

	} while(false);

	//完成异步应答
	TraderServant::async_response_instrumentCommissionQuery(current, Center::SUCCESS, rsp);

	TLOGDEBUG("[Center][TraderServantImp::handleInstrumentCommissionNotify] instrumentCommission response sended, " << \
			"request = " << rsp.requestId << "; " << \
			"account = " << req.userId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"threadid = " << getHandle()->id() << "; " << \
			"errcode = " << rsp.errcode << "; " << \
			"errmsg = " << rsp.errmsg <<endl);
}

void TraderServantImp::handleInstrumentQueryNotify(const OpctxPtr& op) {
	const TarsCurrentPtr& current = op->getCallback()->getCurrent();

    //从上下文中的请求BUFF中解析出请求对象和应答对象
	Center::instrumentQueryRequest req;
	Center::instrumentQueryResponse rsp;
	if (current->getRequestVersion() == TUPVERSION) {
        UniAttribute<tars::BufferWriter, tars::BufferReader>  tarsAttr;
        tarsAttr.setVersion(current->getRequestVersion());
        tarsAttr.decode(current->getRequestBuffer());
        tarsAttr.get("req", req);
        tarsAttr.getByDefault("rsp", rsp, rsp);
	}

	//更新请求编号
	rsp.typeId = req.typeId;
	rsp.requestId = req.requestId;

	//填充查询合约手续费API的返回数据
	do {
		InstrumentPtr instrument = NULL;
		QryInstrumentEventPtr event = QryInstrumentEventPtr::dynamicCast(op->getEvent());
		if (!event || !op->getChannel()->fetchInstrument(event->getInstrumentField()->InstrumentID, instrument)) {
			rsp.errcode = Center::UNKNOWN_ERROR;
			rsp.errmsg = TraderServantImp::getErrorMessage(rsp.errcode);
			break;
		}

		//更新错误码和session状态
		rsp.errcode = event->getRspInfoField()->ErrorID;
		if (rsp.errcode == Center::SUCCESS) {
			rsp.errmsg = "OK";
			instrument->updateProperty(event->getInstrumentField());
		}
		else {
			rsp.errmsg = TraderServantImp::gbk2utf8ErrorMessage(event->getRspInfoField()->ErrorMsg);
		}

		//移除session中保存的请求上下文
		instrument->delActionContext(event->getRequestId());

	} while(false);

	//完成异步应答
	TraderServant::async_response_instrumentQuery(current, Center::SUCCESS, rsp);

	TLOGDEBUG("[Center][TraderServantImp::handleInstrumentQueryNotify] instrumentQuery response sended, " << \
			"request = " << rsp.requestId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"threadid = " << getHandle()->id() << "; " << \
			"errcode = " << rsp.errcode << "; " << \
			"errmsg = " << rsp.errmsg <<endl);
}

void TraderServantImp::handleMarketDataQueryNotify(const OpctxPtr& op) {
	const TarsCurrentPtr& current = op->getCallback()->getCurrent();

    //从上下文中的请求BUFF中解析出请求对象和应答对象
	Center::marketDataQueryRequest req;
	Center::marketDataQueryResponse rsp;
	if (current->getRequestVersion() == TUPVERSION) {
        UniAttribute<tars::BufferWriter, tars::BufferReader>  tarsAttr;
        tarsAttr.setVersion(current->getRequestVersion());
        tarsAttr.decode(current->getRequestBuffer());
        tarsAttr.get("req", req);
        tarsAttr.getByDefault("rsp", rsp, rsp);
	}

	//更新请求编号
	rsp.typeId = req.typeId;
	rsp.requestId = req.requestId;

	//填充查询合约手续费API的返回数据
	do {
		InstrumentPtr instrument = NULL;
		QryDepthMarketDataEventPtr event = QryDepthMarketDataEventPtr::dynamicCast(op->getEvent());
		if (!event || !op->getChannel()->fetchInstrument(event->getDepthMarketDataField()->InstrumentID, instrument)) {
			rsp.errcode = Center::UNKNOWN_ERROR;
			rsp.errmsg = TraderServantImp::getErrorMessage(rsp.errcode);
			break;
		}

		//更新错误码和session状态
		rsp.errcode = event->getRspInfoField()->ErrorID;
		if (rsp.errcode == Center::SUCCESS) {
			rsp.errmsg = "OK";
			instrument->updateProperty(event->getDepthMarketDataField());
		}
		else {
			rsp.errmsg = TraderServantImp::gbk2utf8ErrorMessage(event->getRspInfoField()->ErrorMsg);
		}

		//移除session中保存的请求上下文
		instrument->delActionContext(event->getRequestId());

	} while(false);

	//完成异步应答
	TraderServant::async_response_marketDataQuery(current, Center::SUCCESS, rsp);

	TLOGDEBUG("[Center][TraderServantImp::handleMarketDataQueryNotify] marketDataQuery response sended, " << \
			"request = " << rsp.requestId << "; " << \
			"instrument = " << req.instrumentId << "; " << \
			"threadid = " << getHandle()->id() << "; " << \
			"errcode = " << rsp.errcode << "; " << \
			"errmsg = " << rsp.errmsg <<endl);
}

void TraderServantImp::handleOrderInsertNotify(const OpctxPtr& op) {
}

void TraderServantImp::handleOrderActionNotify(const OpctxPtr& op) {
}

void TraderServantImp::handleOrderRtnNotify(const OpctxPtr& op) {
}

void TraderServantImp::handleTradeRtnNotify(const OpctxPtr& op) {
}
