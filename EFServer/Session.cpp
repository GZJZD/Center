/*
 * Session.cpp
 *
 *  Created on: 2018年5月2日
 *      Author: panhongfa
 */

#include "Session.h"
#include "Common.h"
#include "servant/TarsLogger.h"
#include "include/ThostFtdcUserApiStruct.h"

Session::Session(const string& account, const string& pwd)
{
	_strAccount = account;
	_strPassword = pwd;
	_eumStatus = logouted;

	_nFrontID= 0;
	_nSessionID = 0;
}

Session::~Session() {
	// TODO Auto-generated destructor stub
}

void Session::pushActionContext(TraderCallbackPtr& cb) {
	TC_ThreadLock::Lock lock(*this);
	_vctActions.push_back(cb);
}

bool Session::getActionContext(int actionid, TraderCallbackPtr& cb) {
	TC_ThreadLock::Lock lock(*this);
	vector<TraderCallbackPtr>::iterator it = _vctActions.begin();
	while (it != _vctActions.end()) {
		cb = *it;
		if (cb->getActionID() == actionid) {
			return true;
		}
		it++;
	}
	return false;
}

void Session::delActionContext(int actionid) {
	TC_ThreadLock::Lock lock(*this);
	vector<TraderCallbackPtr>::iterator it = _vctActions.begin();
	while (it != _vctActions.end()) {
		TraderCallbackPtr& cb = *it;
		if (cb->getActionID() == actionid) {
			it = _vctActions.erase(it);
		}
		else {
			it++;
		}
	}
}

void Session::delActionContext(TraderCallbackPtr& cb) {
	TC_ThreadLock::Lock lock(*this);
	vector<TraderCallbackPtr>::iterator it = _vctActions.begin();
	while (it != _vctActions.end()) {
		if (*it == cb) {
			it = _vctActions.erase(it);
		}
		else {
			it++;
		}
	}
}

bool Session::updateProperty(const CThostFtdcRspUserLoginField* pField) {
	if (pField->UserID != _strAccount)
		return false;

	{
		_nFrontID = pField->FrontID;
		_nSessionID = pField->SessionID;
		_strBrokerID = pField->BrokerID;
		_strLoginTime = pField->LoginTime;
		_strSystemName = pField->SystemName;
		_strMaxOrderRef = pField->MaxOrderRef;
	}
	return true;
}

bool Session::tryLogoutSession() {
	return (_lstOrders.empty()) ? true : false;
}

bool Session::fetchOrder(int orderid, OrderPtr& order) {
	TC_ThreadLock::Lock lock(*this);

	list<OrderPtr>::iterator it = _lstOrders.begin();
	while (it != _lstOrders.end())
	{
		order = *(it);
		if (order->getOrderId() == orderid) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

bool Session::fetchOrder(const Order::ChannelOrderIndex& index, OrderPtr& order) {
	TC_ThreadLock::Lock lock(*this);

	list<OrderPtr>::iterator it = _lstOrders.begin();
	while (it != _lstOrders.end())
	{
		order = *(it);
		TraderCallbackPtr cb = NULL;
		if (order->getChannelIndex() == index) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

bool Session::fetchOrder(const Order::SystemOrderIndex& index, OrderPtr& order) {
	TC_ThreadLock::Lock lock(*this);

	list<OrderPtr>::iterator it = _lstOrders.begin();
	while (it != _lstOrders.end())
	{
		order = *(it);
		TraderCallbackPtr cb = NULL;
		if (order->getSystemIndex() == index) {
			return true;
		}
		else {
			it++;
		}
	}
	return false;
}

OrderPtr& Session::createOrder(int orderid) {
	TC_ThreadLock::Lock lock(*this);

	OrderPtr order = new Order(orderid, _nFrontID, _nSessionID);
	_lstOrders.insert(_lstOrders.end(), order);

	TLOGDEBUG("[Center][Channel::createOrder] create new order, " << \
			"order.orderid = " << orderid << "; " << \
			"order.channelindex = " << order->getChannelIndex().toIndex() << endl);

	return _lstOrders.back();
}

void Session::removeOrder(int orderid) {

}
