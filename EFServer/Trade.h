/*
 * Trade.h
 *
 *  Created on: 2018年7月27日
 *      Author: panhongfa
 */

#ifndef EFSERVER_TRADE_H_
#define EFSERVER_TRADE_H_

#include <string>
#include <list>
#include <sstream>
#include "util/tc_monitor.h"
#include "util/tc_autoptr.h"

using namespace std;

class Order;
class Trade : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	typedef struct TradeVolume {
		double fPrice;
		int nVolume;
	} TradeVolume;

	typedef struct TradeDateTime {
		string strTradeDate;
		string strTradeTime;
	} TradeDateTime;

	typedef struct TradeProperty {
		char bTradeType;       //成交类型
		char bPriceSource;     //成交价来源
	} TradeProperty;

public:
	Trade(const string& tradeid, Order* order);
	virtual ~Trade();

	const string& getStrTradeId() const {
		return _strTradeID;
	}

	const TradeDateTime& getTradeDateTime() const {
		return _sTradeDateTime;
	}

	void setTradeDateTime(const TradeDateTime& tradeDateTime) {
		_sTradeDateTime = tradeDateTime;
	}

	TradeProperty getTradeProperty() const {
		return _sTradeProperty;
	}

	void setTradeProperty(TradeProperty tradeProperty) {
		_sTradeProperty = tradeProperty;
	}

	const TradeVolume& getTradeVolume() const {
		return _sTradeVolume;
	}

	void setTradeVolume(const TradeVolume& tradeVolume) {
		_sTradeVolume = tradeVolume;
	}

private:
	string _strTradeID;

	TradeVolume _sTradeVolume;
	TradeDateTime _sTradeDateTime;
	TradeProperty _sTradeProperty;

	Order* _pOrder;
};
typedef tars::TC_AutoPtr<Trade> TradePtr;

#endif /* EFSERVER_TRADE_H_ */
