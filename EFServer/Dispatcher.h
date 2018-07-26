/*
 * Dispatcher.h
 *
 *  Created on: 2018年7月10日
 *      Author: panhongfa
 */

#ifndef EFSERVER_DISPATCHER_H_
#define EFSERVER_DISPATCHER_H_

#include "Worker.h"
#include "Event.h"

class Maintainer;
class Dispatcher : public Worker<Event, 50> {
public:
	Dispatcher(Maintainer* pwork);
	virtual ~Dispatcher();

protected:
	virtual void doProcess(Event* op);
	virtual void doTimeout();

protected:
	Maintainer *_pMaintainer;
};

#endif /* EFSERVER_DISPATCHER_H_ */
