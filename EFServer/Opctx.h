/*
 * Opctx.h
 *
 *  Created on: 2018年6月25日
 *      Author: panhongfa
 */

#ifndef EFSERVER_OPCTX_H_
#define EFSERVER_OPCTX_H_

#include <util/tc_autoptr.h>
#include "TraderCallback.h"
#include "util/tc_loop_queue.h"
#include "util/tc_thread_queue.h"
#include "Event.h"
#include "Channel.h"

class Opctx : public tars::TC_ThreadLock, public tars::TC_HandleBase {
public:
	Opctx();
	virtual ~Opctx();

	const TraderCallbackPtr& getCallback() const {
		return _pCallback;
	}

	void setCallback(const TraderCallbackPtr& callback) {
		_pCallback = callback;
	}

	const EventPtr& getEvent() const {
		return _pEvent;
	}

	void setEvent(const EventPtr& event) {
		_pEvent = event;
	}

	const ChannelPtr& getChannel() const {
		return _pChannel;
	}

	void setChannel(const ChannelPtr& channel) {
		_pChannel = channel;
	}

protected:
	TraderCallbackPtr _pCallback;
	EventPtr _pEvent;
	ChannelPtr _pChannel;
};
typedef tars::TC_AutoPtr<Opctx> OpctxPtr;

#endif /* EFSERVER_OPCTX_H_ */
