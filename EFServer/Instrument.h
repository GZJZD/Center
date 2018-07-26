/*
 * Instrument.h
 *
 *  Created on: 2018年7月25日
 *      Author: panhongfa
 */

#ifndef EFSERVER_INSTRUMENT_H_
#define EFSERVER_INSTRUMENT_H_

#include "util/tc_monitor.h"
#include "util/tc_autoptr.h"
#include "TraderCallback.h"

class Instrument : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	Instrument(const string& id);
	virtual ~Instrument();

	string getInstrumentId() const {
		return _strInstrumentID;
	}

	void setInstrumentId(const string& instrumentId) {
		_strInstrumentID = instrumentId;
	}

	void pushActionContext(TraderCallbackPtr& cb);

	bool getActionContext(int actionid, TraderCallbackPtr& cb);

	void delActionContext(int actionid);

	void delActionContext(TraderCallbackPtr& cb);

	bool updateProperty(const CThostFtdcInstrumentCommissionRateField* pField);

	bool updateProperty(const CThostFtdcInstrumentField* pField);

	bool updateProperty(const CThostFtdcDepthMarketDataField* pField);

private:
	string _strInstrumentID;
	vector<TraderCallbackPtr> _vctActions;
};
typedef tars::TC_AutoPtr<Instrument> InstrumentPtr;

#endif /* EFSERVER_INSTRUMENT_H_ */
