/*
 * HandlerImp.h
 *
 *  Created on: 2018年6月25日
 *      Author: panhongfa
 */

#ifndef EFSERVER_HANDLERIMP_H_
#define EFSERVER_HANDLERIMP_H_

#include "Handler.h"
#include "TraderServantImp.h"

class HandlerImp : public Handler {
public:
	HandlerImp(Channel* channel);
	virtual ~HandlerImp();

protected:
	void OnFrontConnected();
	void OnFrontDisconnected(int nReason);

	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction,
			CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspError(CThostFtdcRspInfoField *pRspInfo,
			int nRequestID, bool bIsLast);

	void OnRtnOrder(CThostFtdcOrderField *pOrder);

	void OnRtnTrade(CThostFtdcTradeField *pTrade);

};

#endif /* EFSERVER_HANDLERIMP_H_ */
