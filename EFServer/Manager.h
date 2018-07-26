/*
 * Manager.h
 *
 *  Created on: 2018年7月10日
 *      Author: panhongfa
 */

#ifndef EFSERVER_MANAGER_H_
#define EFSERVER_MANAGER_H_

#include <vector>
#include <map>
#include "util/tc_singleton.h"
#include "util/tc_autoptr.h"
#include "Handler.h"

using namespace std;

class Channel;
class Maintainer;
class Dispatcher;
class Manager : public tars::TC_Singleton<Manager>, public tars::TC_ThreadLock {
public:
	Manager();
	virtual ~Manager();

    virtual void initialize(vector<string>& names);
    virtual void destroy();

	Dispatcher* getDispatcher() const {
		return _pDispatcher;
	}

	Maintainer* getMaintainer() const {
		return _pMaintainer;
	}

	Channel* getChannel(const string& name);

private:
    Maintainer* _pMaintainer;
    Dispatcher* _pDispatcher;

    vector<HandlerPtr> _vctHandler;
    map<string, Channel*> _mapChannels;
};

#endif /* EFSERVER_MANAGER_H_ */
