/*
 * Event.h
 *
 *  Created on: 2018年6月25日
 *      Author: panhongfa
 */

#ifndef EFSERVER_EVENT_H_
#define EFSERVER_EVENT_H_

#include <util/tc_autoptr.h>
#include "include/ThostFtdcUserApiStruct.h"

class Channel;
class Event : public tars::TC_HandleBase {
public:
	typedef enum EventType {
		none_event = 0,
		front_connected_event,
		front_disconnected_event,
		user_login_event,
		user_logout_event,
		order_insert_event,
		order_action_event,
		order_rtn_event,
		trade_rtn_event,
		instrument_commission_event,
		instrument_query_event,
		marketdata_query_event,
		rsp_error_event
	}EventType;

	Event(int type = none_event) {_enmType = (EventType)type; _pSrcChannel = NULL;}
	virtual ~Event() {};

	EventType getType() const {
		return _enmType;
	}

	void setType(EventType enmType) {
		_enmType = enmType;
	}

	Channel* getSrcChannel() const {
		return _pSrcChannel;
	}

	void setSrcChannel(Channel* srcChannel) {
		_pSrcChannel = srcChannel;
	}

protected:
	EventType _enmType;
	Channel* _pSrcChannel;
};
typedef tars::TC_AutoPtr<Event> EventPtr;

class FrontConnectedEvent : public Event {
public:
	FrontConnectedEvent() : Event(front_connected_event) {
	}
	virtual ~FrontConnectedEvent() {}
};
typedef tars::TC_AutoPtr<FrontConnectedEvent> FrontConnectedEventPtr;

class FrontDisconnectedEvent : public Event {
public:
	FrontDisconnectedEvent() : Event(front_disconnected_event) {
		_nReason = 0;
	}
	virtual ~FrontDisconnectedEvent() {}

	int getReason() const {
		return _nReason;
	}

	void setReason(int reason) {
		_nReason = reason;
	}

protected:
	int _nReason;
};
typedef tars::TC_AutoPtr<FrontDisconnectedEvent> FrontDisconnectedEventPtr;

class UserLoginEvent : public Event {
public:
	UserLoginEvent() : Event(user_login_event) {
		_nRequestID = 0;
		_pUserLoginField = new CThostFtdcRspUserLoginField();
		memset(_pUserLoginField,0,sizeof(CThostFtdcRspUserLoginField)/sizeof(char));

		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}
	virtual ~UserLoginEvent() {
		if (_pUserLoginField) delete _pUserLoginField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	const CThostFtdcRspUserLoginField* getUserLoginField() const {
		return _pUserLoginField;
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcRspUserLoginField* pUserLoginField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pUserLoginField) memcpy(_pUserLoginField, pUserLoginField, sizeof(CThostFtdcRspUserLoginField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcRspUserLoginField* _pUserLoginField;
	CThostFtdcRspInfoField* _pRspInfoField;
};
typedef tars::TC_AutoPtr<UserLoginEvent> UserLoginEventPtr;

class UserLogoutEvent : public Event {
public:
	UserLogoutEvent() : Event(user_logout_event) {
		_nRequestID = 0;
		_pUserLogoutField = new CThostFtdcUserLogoutField();
		memset(_pUserLogoutField,0,sizeof(CThostFtdcUserLogoutField)/sizeof(char));

		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}
	virtual ~UserLogoutEvent() {
		if (_pUserLogoutField) delete _pUserLogoutField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	const CThostFtdcUserLogoutField* getUserLogoutField() const {
		return _pUserLogoutField;
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcUserLogoutField* pUserLogoutField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pUserLogoutField) memcpy(_pUserLogoutField, pUserLogoutField, sizeof(CThostFtdcUserLogoutField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcUserLogoutField* _pUserLogoutField;
	CThostFtdcRspInfoField* _pRspInfoField;
};
typedef tars::TC_AutoPtr<UserLogoutEvent> UserLogoutEventPtr;

class OrderInsertEvent : public Event {
public:
	OrderInsertEvent() : Event(order_insert_event) {
		_nRequestID = 0;
		_pOrderInsertField = new CThostFtdcInputOrderField();
		memset(_pOrderInsertField,0,sizeof(CThostFtdcInputOrderField)/sizeof(char));

		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}
	virtual ~OrderInsertEvent() {
		if (_pOrderInsertField) delete _pOrderInsertField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcInputOrderField* getOrderInsertField() const {
		return _pOrderInsertField;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcInputOrderField* pOrderInsertField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pOrderInsertField) memcpy(_pOrderInsertField, pOrderInsertField, sizeof(CThostFtdcInputOrderField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcInputOrderField* _pOrderInsertField;
	CThostFtdcRspInfoField* _pRspInfoField;
};
typedef tars::TC_AutoPtr<OrderInsertEvent> OrderInsertEventPtr;

class OrderActionEvent : public Event {
public:
	OrderActionEvent() : Event(order_action_event) {
		_nRequestID = 0;
		_pOrderActionField = new CThostFtdcInputOrderActionField();
		memset(_pOrderActionField,0,sizeof(CThostFtdcInputOrderActionField)/sizeof(char));

		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}
	virtual ~OrderActionEvent() {
		if (_pOrderActionField) delete _pOrderActionField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcInputOrderActionField* getOrderActionField() const {
		return _pOrderActionField;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcInputOrderActionField* pOrderActionField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pOrderActionField) memcpy(_pOrderActionField, pOrderActionField, sizeof(CThostFtdcInputOrderActionField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcInputOrderActionField* _pOrderActionField;
	CThostFtdcRspInfoField* _pRspInfoField;
};
typedef tars::TC_AutoPtr<OrderActionEvent> OrderActionEventPtr;

class RtnOrderEvent : public Event {
public:
	RtnOrderEvent() : Event(order_rtn_event) {
		_nRequestID = 0;
		_pOrderField = new CThostFtdcOrderField();
		memset(_pOrderField,0,sizeof(CThostFtdcOrderField)/sizeof(char));
	}
	virtual ~RtnOrderEvent() {
		if (_pOrderField) delete _pOrderField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	void copyRspField(const CThostFtdcOrderField* pOrderField) {
		memcpy(_pOrderField, pOrderField, sizeof(CThostFtdcOrderField)/sizeof(char));
	}

	const CThostFtdcOrderField*& getOrderField() const {
		return _pOrderField;
	}

protected:
	int _nRequestID;

	CThostFtdcOrderField* _pOrderField;
};
typedef tars::TC_AutoPtr<RtnOrderEvent> RtnOrderEventPtr;

class RtnTradeEvent : public Event {
public:
	RtnTradeEvent() : Event(trade_rtn_event) {
		_pTradeField = new CThostFtdcTradeField();
		memset(_pTradeField,0,sizeof(CThostFtdcTradeField)/sizeof(char));
	}
	virtual ~RtnTradeEvent() {
		if (_pTradeField) delete _pTradeField;
	}

	const CThostFtdcTradeField* getTradeField() const {
		return _pTradeField;
	}

	void copyRspField(const CThostFtdcTradeField* pTradeField) {
		memcpy(_pTradeField, pTradeField, sizeof(CThostFtdcTradeField)/sizeof(char));
	}

protected:
	CThostFtdcTradeField* _pTradeField;
};
typedef tars::TC_AutoPtr<RtnTradeEvent> RtnTradeEventPtr;

class RspErrorEvent : public Event {
public:
	RspErrorEvent() : Event(rsp_error_event) {
		_nRequestID = 0;
		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}
	virtual ~RspErrorEvent() {
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	void copyRspField(const CThostFtdcRspInfoField* rspInfoField) {
		memcpy(_pRspInfoField, rspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}

protected:
	int _nRequestID;
	CThostFtdcRspInfoField *_pRspInfoField;
};
typedef tars::TC_AutoPtr<RspErrorEvent> RspErrorEventPtr;

class QryInstrumentCommissionEvent : public Event {
public:
	QryInstrumentCommissionEvent() : Event(instrument_commission_event) {
		_nRequestID = 0;
		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));

		_pInstrumentCommissionField = new CThostFtdcInstrumentCommissionRateField();
		memset(_pInstrumentCommissionField,0,sizeof(CThostFtdcInstrumentCommissionRateField)/sizeof(char));
	}

	virtual ~QryInstrumentCommissionEvent() {
		if (_pInstrumentCommissionField) delete _pInstrumentCommissionField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcInstrumentCommissionRateField* getInstrumentCommissionField() const {
		return _pInstrumentCommissionField;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	void copyRspField(const CThostFtdcRspInfoField* rspInfoField) {
		memcpy(_pRspInfoField, rspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcInstrumentCommissionRateField* pInstrumentCommissionField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pInstrumentCommissionField) memcpy(_pInstrumentCommissionField, pInstrumentCommissionField, sizeof(CThostFtdcInstrumentCommissionRateField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcInstrumentCommissionRateField *_pInstrumentCommissionField;
	CThostFtdcRspInfoField *_pRspInfoField;
};
typedef tars::TC_AutoPtr<QryInstrumentCommissionEvent> QryInstrumentCommissionEventPtr;

class QryInstrumentEvent : public Event {
public:
	QryInstrumentEvent() : Event(instrument_query_event) {
		_nRequestID = 0;
		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));

		_pInstrumentField = new CThostFtdcInstrumentField();
		memset(_pInstrumentField,0,sizeof(CThostFtdcInstrumentField)/sizeof(char));
	}

	virtual ~QryInstrumentEvent() {
		if (_pInstrumentField) delete _pInstrumentField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcInstrumentField* getInstrumentField() const {
		return _pInstrumentField;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcInstrumentField* pInstrumentField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pInstrumentField) memcpy(_pInstrumentField, pInstrumentField, sizeof(CThostFtdcInstrumentField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcInstrumentField *_pInstrumentField;
	CThostFtdcRspInfoField *_pRspInfoField;
};
typedef tars::TC_AutoPtr<QryInstrumentEvent> QryInstrumentEventPtr;

class QryDepthMarketDataEvent : public Event {
public:
	QryDepthMarketDataEvent() : Event(marketdata_query_event) {
		_nRequestID = 0;
		_pRspInfoField = new CThostFtdcRspInfoField();
		memset(_pRspInfoField,0,sizeof(CThostFtdcRspInfoField)/sizeof(char));

		_pDepthMarketDataField = new CThostFtdcDepthMarketDataField();
		memset(_pDepthMarketDataField,0,sizeof(CThostFtdcDepthMarketDataField)/sizeof(char));
	}

	virtual ~QryDepthMarketDataEvent() {
		if (_pDepthMarketDataField) delete _pDepthMarketDataField;
		if (_pRspInfoField) delete _pRspInfoField;
	}

	int getRequestId() const {
		return _nRequestID;
	}

	void setRequestId(int requestId) {
		_nRequestID = requestId;
	}

	const CThostFtdcDepthMarketDataField* getDepthMarketDataField() const {
		return _pDepthMarketDataField;
	}

	const CThostFtdcRspInfoField* getRspInfoField() const {
		return _pRspInfoField;
	}

	void copyRspField(const CThostFtdcRspInfoField* pRspInfoField, const CThostFtdcDepthMarketDataField* pDepthMarketDataField = 0) {
		if (pRspInfoField) memcpy(_pRspInfoField, pRspInfoField, sizeof(CThostFtdcRspInfoField)/sizeof(char));
		if (pDepthMarketDataField) memcpy(_pDepthMarketDataField, pDepthMarketDataField, sizeof(CThostFtdcDepthMarketDataField)/sizeof(char));
	}

protected:
	int _nRequestID;

	CThostFtdcDepthMarketDataField *_pDepthMarketDataField;
	CThostFtdcRspInfoField *_pRspInfoField;
};
typedef tars::TC_AutoPtr<QryDepthMarketDataEvent> QryDepthMarketDataEventPtr;

#endif /* EFSERVER_EVENT_H_ */
