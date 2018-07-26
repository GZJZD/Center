/*
 * Maintainer.cpp
 *
 *  Created on: 2018年7月3日
 *      Author: panhongfa
 */

#include "Maintainer.h"
#include "servant/TarsLogger.h"

Maintainer::Maintainer() {
}

Maintainer::~Maintainer() {
}

void Maintainer::doProcess(Opctx* op) {
	switch (op->getEvent()->getType()) {
		case Event::front_connected_event:
			onFrontConnected(op);
			break;
		case Event::front_disconnected_event:
			onFrontDisconnected(op);
			break;
		default:
			break;
	}
}

void Maintainer::doTimeout() {
}

void Maintainer::onFrontConnected(const Opctx* op) {
	op->getChannel()->setLinkStatus(Channel::link_on);

	//上线之后处理积压的消息
	//TODO
}

void Maintainer::onFrontDisconnected(const Opctx* op) {
	op->getChannel()->setLinkStatus(Channel::link_off);

	//掉线后的session清理动作
	//TODO
}
