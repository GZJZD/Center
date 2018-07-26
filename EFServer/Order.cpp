/*
 * Order.cpp
 *
 *  Created on: 2018年6月28日
 *      Author: panhongfa
 */

#include "Order.h"
#include "Common.h"
#include "servant/TarsLogger.h"
#include "include/ThostFtdcUserApiStruct.h"

Order::Order(int orderid, int frontid, int sessionid) {
	_nOrderID = orderid;
	_sChannelIndex.nFrontID = frontid;
	_sChannelIndex.nSessionID = sessionid;
}

Order::~Order() {

}
