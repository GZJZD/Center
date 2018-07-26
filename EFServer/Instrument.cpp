/*
 * Instrument.cpp
 *
 *  Created on: 2018年7月25日
 *      Author: panhongfa
 */

#include "Instrument.h"

Instrument::Instrument(const string& id) {
	_strInstrumentID = id;
}

Instrument::~Instrument() {
	// TODO Auto-generated destructor stub
}

void Instrument::pushActionContext(TraderCallbackPtr& cb) {
	TC_ThreadLock::Lock lock(*this);
	_vctActions.push_back(cb);
}

bool Instrument::getActionContext(int actionid, TraderCallbackPtr& cb) {
	TC_ThreadLock::Lock lock(*this);
	vector<TraderCallbackPtr>::iterator it = _vctActions.begin();
	while (it != _vctActions.end()) {
		cb = *it;
		if (cb->getActionID() == actionid) {
			return true;
		}
		it++;
	}
	return false;
}

void Instrument::delActionContext(int actionid) {
	TC_ThreadLock::Lock lock(*this);
	vector<TraderCallbackPtr>::iterator it = _vctActions.begin();
	while (it != _vctActions.end()) {
		TraderCallbackPtr& cb = *it;
		if (cb->getActionID() == actionid) {
			it = _vctActions.erase(it);
		}
		else {
			it++;
		}
	}
}

void Instrument::delActionContext(TraderCallbackPtr& cb) {
	TC_ThreadLock::Lock lock(*this);
	vector<TraderCallbackPtr>::iterator it = _vctActions.begin();
	while (it != _vctActions.end()) {
		if (*it == cb) {
			it = _vctActions.erase(it);
		}
		else {
			it++;
		}
	}
}
