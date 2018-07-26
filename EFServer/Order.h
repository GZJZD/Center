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
#include "TraderCallback.h"
#include "TraderServant.h"

using namespace std;
class Order : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	typedef enum {
		none = 0,
		submitting,
		submitted,
		piecetrading,
		fulltrading,
		canceling,
		canceled
	}OrderStatus;

	typedef struct {
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
	} ChannelOrderIndex;

	typedef struct {
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
	} ExchangeOrderIndex;

	typedef struct {
		string strExchangeID;
		string strOrderSysID;

		string toIndex() const {
			stringstream ss;
			ss << strExchangeID << ".";
			ss << strOrderSysID;
			return ss.str();
		}
	} SystemOrderIndex;

	typedef struct {

	};

	typedef struct {

	} OrderVolume;

public:
	Order(int orderid, int frontid, int sessionid);
	virtual ~Order();

	void pushActionContext(TraderCallbackPtr& cb);

	bool getActionContext(int actionid, TraderCallbackPtr& cb);

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

	OrderStatus getOrderStat() const {
		return _nOrderStat;
	}

	void setOrderStat(OrderStatus orderStat) {
		_nOrderStat = orderStat;
	}

	char getBuyFlag() const {
		return _bBuyFlag;
	}

	void setBuyFlag(char buyFlag) {
		_bBuyFlag = buyFlag;
	}

private:
	int _nOrderID;
	ChannelOrderIndex _sChannelIndex;
	ExchangeOrderIndex _sExchangeIndex;
	SystemOrderIndex _sSystemIndex;

	char _bOpenFlag;
	char _bBuyFlag;
	OrderStatus _nOrderStat;

	vector<TraderCallbackPtr> _vctActions;
};
typedef tars::TC_AutoPtr<Order> OrderPtr;

#endif /* EFSERVER_ORDER_H_ */
