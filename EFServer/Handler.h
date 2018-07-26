/*
 * Handler.h
 *
 *  Created on: 2018年6月1日
 *      Author: panhongfa
 */

#ifndef EFSERVER_HANDLER_H_
#define EFSERVER_HANDLER_H_

#include "util/tc_monitor.h"
#include <util/tc_autoptr.h>
#include "include/ThostFtdcTraderApi.h"

class Channel;
class Handler : public CThostFtdcTraderSpi, public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	Handler(Channel* chl) { _pChannel = chl; }
	virtual ~Handler() {}

protected:
	Channel* _pChannel;
};
typedef tars::TC_AutoPtr<Handler> HandlerPtr;

#endif /* EFSERVER_HANDLER_H_ */
