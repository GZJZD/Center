/*
 * TraderCallback.cpp
 *
 *  Created on: 2018年6月15日
 *      Author: panhongfa
 */

#include "TraderCallback.h"

TraderCallback::TraderCallback(const string& type, const tars::ServantPtr& servant, const tars::TarsCurrentPtr& current)
: ServantCallback(type,servant,current) {
	_nActionID = 0;
}

int TraderCallback::getActionID() const {
	return _nActionID;
}

void TraderCallback::setActionID(int id) {
	_nActionID = id;
}
