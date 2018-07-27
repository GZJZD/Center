/*
 * Order.h
 *
 *  Created on: 2018年6月28日
 *      Author: panhongfa
 */

#ifndef EFSERVER_ORDER_H_
#define EFSERVER_ORDER_H_

#include <string>
#include <list>
#include <sstream>
#include "util/tc_monitor.h"
#include "util/tc_autoptr.h"
#include "util/tc_timeprovider.h"
#include "TraderCallback.h"
#include "TraderServant.h"
#include "Trade.h"

using namespace std;
class Order : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	typedef struct OrderStatus {
		char bSubmitStatus;
		char bOrderStatus;

		string toStatus() const {
			stringstream ss;
			ss << bSubmitStatus << ".";
			ss << bOrderStatus;
			return ss.str();
		}

	} OrderStatus;

	typedef struct ChannelOrderIndex {
		int nFrontID;
		int nSessionID;
		string strOrderRef;

		string toIndex() const {
			stringstream ss;
			ss << nFrontID << ".";
			ss << nSessionID << ".";
			ss << strOrderRef;
			return ss.str();
		}

		bool operator==(const ChannelOrderIndex& ps) {
			if (this->nFrontID == ps.nFrontID
					&& this->nSessionID == ps.nSessionID
					&& this->strOrderRef == ps.strOrderRef) {
				return true;
			}
			return false;
		}

	} ChannelOrderIndex;

	typedef struct ExchangeOrderIndex {
		string strExchangeID;
		string strTraderID;
		string strOrderLocalID;

		string toIndex() const {
			stringstream ss;
			ss << strExchangeID << ".";
			ss << strTraderID << ".";
			ss << strOrderLocalID;
			return ss.str();
		}

		bool operator==(const ExchangeOrderIndex& ps) {
			if (this->strExchangeID == ps.strExchangeID
					&& this->strTraderID == ps.strTraderID
					&& this->strOrderLocalID == ps.strOrderLocalID) {
				return true;
			}
			return false;
		}

	} ExchangeOrderIndex;

	typedef struct SystemOrderIndex {
		string strExchangeID;
		string strOrderSysID;

		string toIndex() const {
			stringstream ss;
			ss << strExchangeID << ".";
			ss << strOrderSysID;
			return ss.str();
		}

		bool operator==(const SystemOrderIndex& ps) {
			if (this->strExchangeID == ps.strExchangeID
					&& this->strOrderSysID == ps.strOrderSysID) {
				return true;
			}
			return false;
		}

	} SystemOrderIndex;

	typedef struct OrderDateTime {
		string sInsertDate;
		string sInsertTime;
		string sActiveTime;
		string sSuspendTime;
		string sUpdateTime;
		string sCancelTime;
	} OrderDateTime;

	typedef struct OrderActionTime {
		time_t tLocalTimeCreated;
		int tMaxWaitTimeout;

		OrderActionTime() {
			tLocalTimeCreated = TC_TimeProvider::getInstance()->getNow();
			tMaxWaitTimeout = 30 * 1000;
		}

		bool hasTimeout() {
			time_t cur_time = TC_TimeProvider::getInstance()->getNow();
			if (cur_time - tLocalTimeCreated >= tMaxWaitTimeout) {
				return true;
			}
			return false;
		}
	} OrderActionTime;

	typedef struct OrderVolume {
		int nVolumeTotalOriginal; //总数量
		int	nVolumeTotal;         //剩余数量
	} OrderVolume;

public:
	Order(int orderid, int frontid, int sessionid);
	virtual ~Order();

	void pushActionContext(TraderCallbackPtr& cb);

	bool getActionContext(int actionid, TraderCallbackPtr& cb);

	bool getActionContext(TraderCallbackPtr& cb);

	void delActionContext(int actionid);

	void delActionContext(TraderCallbackPtr& cb);

	int getOrderId() const {
		return _nOrderID;
	}

	void setOrderId(int orderId) {
		_nOrderID = orderId;
	}

	const ChannelOrderIndex& getChannelIndex() const {
		return _sChannelIndex;
	}

	void setChannelIndex(const ChannelOrderIndex& channelIndex) {
		_sChannelIndex = channelIndex;
	}

	char getOpenFlag() const {
		return _bOpenFlag;
	}

	void setOpenFlag(char openFlag) {
		_bOpenFlag = openFlag;
	}

	char getBuyFlag() const {
		return _bBuyFlag;
	}

	void setBuyFlag(char buyFlag) {
		_bBuyFlag = buyFlag;
	}

	time_t getLocalTimeCreated() const {
		return _sOrderActionTime.tLocalTimeCreated;
	}

	const OrderVolume& getOrderVolume() const {
		return _sOrderVolume;
	}

	void setOrderVolume(const OrderVolume& orderVolume) {
		_sOrderVolume = orderVolume;
	}

	const ExchangeOrderIndex& getExchangeIndex() const {
		return _sExchangeIndex;
	}

	void setExchangeIndex(const ExchangeOrderIndex& exchangeIndex) {
		_sExchangeIndex = exchangeIndex;
	}

	const SystemOrderIndex& getSystemIndex() const {
		return _sSystemIndex;
	}

	void setSystemIndex(const SystemOrderIndex& systemIndex) {
		_sSystemIndex = systemIndex;
	}

	OrderStatus getOrderStat() const {
		return _sOrderStat;
	}

	void setOrderStat(OrderStatus orderStat) {
		_sOrderStat = orderStat;
	}

	void setMaxWaitTimeout(int timeout) {
		_sOrderActionTime.tMaxWaitTimeout = timeout;
	}

private:
	int _nOrderID;
	ChannelOrderIndex _sChannelIndex;
	ExchangeOrderIndex _sExchangeIndex;
	SystemOrderIndex _sSystemIndex;

	OrderActionTime _sOrderActionTime;
	OrderDateTime _sOrderDateTime;
	OrderVolume _sOrderVolume;

	char _bOpenFlag;
	char _bBuyFlag;
	OrderStatus _sOrderStat;

	//原则上报单是唯一的，但有可能出现同一编号的报单被客户端重发
	vector<TraderCallbackPtr> _vctActions;
	list<TradePtr> _lstTrades;
};
typedef tars::TC_AutoPtr<Order> OrderPtr;

#endif /* EFSERVER_ORDER_H_ */
