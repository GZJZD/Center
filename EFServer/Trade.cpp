/*
 * Trade.cpp
 *
 *  Created on: 2018年7月27日
 *      Author: panhongfa
 */

#include "Trade.h"
#include "Order.h"

Trade::Trade(const string& tradeid, Order* order) {
	_strTradeID = tradeid;
	_pOrder = order;
}

Trade::~Trade() {
	// TODO Auto-generated destructor stub
}

