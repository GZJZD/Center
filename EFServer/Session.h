/*
 * Session.h
 *
 *  Created on: 2018年5月2日
 *      Author: panhongfa
 */

#ifndef EFSERVER_SESSION_H_
#define EFSERVER_SESSION_H_

#include "util/tc_monitor.h"
#include "util/tc_autoptr.h"
#include "Order.h"
#include "TraderCallback.h"

class CThostFtdcRspUserLoginField;
class Session : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	typedef enum SessionStatus {
		logouted = 0,
		logouting,
		logining,
		logined
	}SessionStatus;

public:
	Session(const string& account, const string& pwd);
	virtual ~Session();

	SessionStatus getEumStatus() const {
		return _eumStatus;
	}

	void setEumStatus(SessionStatus eumStatus) {
		_eumStatus = eumStatus;
	}

	const string& getAccount() const {
		return _strAccount;
	}

	const string& getPassword() const {
		return _strPassword;
	}

public:
	bool updateProperty(const CThostFtdcRspUserLoginField *pField);

	void pushActionContext(TraderCallbackPtr& cb);

	bool getActionContext(int actionid, TraderCallbackPtr& cb);

	void delActionContext(int actionid);

	void delActionContext(TraderCallbackPtr& cb);

	bool tryLogoutSession();

	bool fetchOrder(int orderid, OrderPtr& order);

	bool fetchOrder(const Order::ChannelOrderIndex& index, OrderPtr& order);

	bool fetchOrder(const Order::SystemOrderIndex& index, OrderPtr& order);

	OrderPtr& createOrder(int orderid);

	void removeOrder(int orderid);

public:
	int _nFrontID;
	int _nSessionID;

	string _strLoginTime;
	string _strBrokerID;
	string _strSystemName;
	string _strMaxOrderRef;

private:
	string _strAccount;
	string _strPassword;
	SessionStatus _eumStatus;

	vector<TraderCallbackPtr> _vctActions;
	list<OrderPtr> _lstOrders;
};
typedef tars::TC_AutoPtr<Session> SessionPtr;
#endif /* EFSERVER_SESSION_H_ */
