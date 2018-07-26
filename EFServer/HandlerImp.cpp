/*
 * HandlerImp.cpp
 *
 *  Created on: 2018年6月25日
 *      Author: panhongfa
 */

#include "HandlerImp.h"
#include "Manager.h"
#include "Dispatcher.h"
#include "Session.h"
#include "servant/TarsLogger.h"
#include "util/tc_encoder.h"

HandlerImp::HandlerImp(Channel* channel)
: Handler(channel) {
}

HandlerImp::~HandlerImp() {
}

void HandlerImp::OnFrontConnected() {
	FrontConnectedEvent* event = new FrontConnectedEvent();
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnFrontConnected] channel online callback event" << endl);
}

void HandlerImp::OnFrontDisconnected(int nReason) {
	FrontDisconnectedEvent* event = new FrontDisconnectedEvent();
	event->setReason(nReason);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnFrontDisconnected] channel offline callback event, " << \
			"reason = " << nReason << endl);
}

void HandlerImp::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	UserLoginEvent* event = new UserLoginEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo,pRspUserLogin);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspUserLogin] userLogin callback event, " << \
			"actionid = " << nRequestID << endl);
}

void HandlerImp::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	UserLogoutEvent* event = new UserLogoutEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo,pUserLogout);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspUserLogout] userLogout callback event, " << \
			"actionid = " << nRequestID << endl);
}

void HandlerImp::OnRspQryInstrumentCommissionRate(
		CThostFtdcInstrumentCommissionRateField* pInstrumentCommissionRate,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	QryInstrumentCommissionEvent* event = new QryInstrumentCommissionEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo,pInstrumentCommissionRate);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspQryInstrumentCommissionRate] QryInstrumentCommissionRate callback event, " << \
			"actionid = " << nRequestID << endl);
}

void HandlerImp::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	QryInstrumentEvent* event = new QryInstrumentEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo,pInstrument);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspQryInstrument] QryInstrument callback event, " << \
			"actionid = " << nRequestID << endl);
}

void HandlerImp::OnRspQryDepthMarketData(
		CThostFtdcDepthMarketDataField* pDepthMarketData,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	QryDepthMarketDataEvent* event = new QryDepthMarketDataEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo,pDepthMarketData);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspQryDepthMarketData] QryDepthMarketData callback event, " << \
			"actionid = " << nRequestID << endl);
}

void HandlerImp::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID,
		bool bIsLast) {
	RspErrorEvent* event = new RspErrorEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspError] request param error callback event, " << \
			"actionid = " << nRequestID << "; " <<\
			"errcode = " << pRspInfo->ErrorID << "; " <<\
			"errmsg = " << TC_Encoder::gbk2utf8(pRspInfo->ErrorMsg) << endl);
}

void HandlerImp::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	OrderInsertEvent* event = new OrderInsertEvent();
	event->setRequestId(nRequestID);
	event->copyRspField(pRspInfo,pInputOrder);
	event->setSrcChannel(_pChannel);
	Manager::getInstance()->getDispatcher()->postMessage(event);

	TLOGDEBUG("[Center][HandlerImp::OnRspOrderInsert] OrderInsert callback event, " << \
			"actionid = " << nRequestID << endl);
}

void HandlerImp::OnRspOrderAction(
		CThostFtdcInputOrderActionField* pInputOrderAction,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
}

void HandlerImp::OnRtnOrder(CThostFtdcOrderField* pOrder) {
}

void HandlerImp::OnRtnTrade(CThostFtdcTradeField* pTrade) {
}
