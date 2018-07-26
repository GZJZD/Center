/*
 * Maintainer.h
 *
 *  Created on: 2018年7月3日
 *      Author: panhongfa
 */

#ifndef EFSERVER_MAINTAINER_H_
#define EFSERVER_MAINTAINER_H_

#include "Worker.h"
#include "Opctx.h"

class Maintainer : public Worker<Opctx, 1000> {
public:
	Maintainer();
	virtual ~Maintainer();

protected:
	virtual void doProcess(Opctx* op);
	virtual void doTimeout();

private:
	void onFrontConnected(const Opctx* op);
	void onFrontDisconnected(const Opctx* op);
};

#endif /* EFSERVER_MAINTAINER_H_ */
