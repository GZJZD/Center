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
#include "util/tc_monitor.h"
#include "util/tc_autoptr.h"
#include "TraderCallback.h"
#include "TraderServant.h"

using namespace std;
class Order : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	Order();
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

	typedef struct {
		int nFrontID;
		int nSessionID;
		string strOrderRef;
	} ChannelOrderIndex;

	typedef struct {
		string strExchangeID;
		string strTraderID;
		string strOrderLocalID;
	} ExchangeOrderIndex;

	typedef struct {
		string strExchangeID;
		string strOrderSysID;
	} SystemOrderIndex;

private:
	int _nOrderID;
	vector<TraderCallbackPtr> _vctActions;
};
typedef tars::TC_AutoPtr<Order> OrderPtr;

#endif /* EFSERVER_ORDER_H_ */
