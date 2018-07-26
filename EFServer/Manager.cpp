/*
 * Manager.cpp
 *
 *  Created on: 2018年7月10日
 *      Author: panhongfa
 */

#include "Manager.h"
#include "Dispatcher.h"
#include "Maintainer.h"
#include "Channel.h"
#include "HandlerImp.h"
#include "EFServer.h"

Manager::Manager() {
	// TODO Auto-generated constructor stub
	_pMaintainer = NULL;
	_pDispatcher = NULL;
}

Manager::~Manager() {
	// TODO Auto-generated destructor stub
}

void Manager::initialize(vector<string>& names) {
	_pMaintainer = new Maintainer();
	_pMaintainer->startService();

	_pDispatcher = new Dispatcher(_pMaintainer);
	_pDispatcher->startService();

	int no = 0;
	vector<string>::iterator it = names.begin();
	while (it != names.end()) {
    	string serverip = g_app.getConfig().get("/tars/application/customize/servant<remoteip>");
    	string datapath = g_app.getConfig().get("/tars/application/customize/servant<datapath>");
    	datapath.append("/channel");

        stringstream stream;
        stream << ++no;
        datapath.append(stream.str());

		Channel* channel = new Channel(serverip, datapath);
		HandlerPtr handler = new HandlerImp(channel);
		channel->initialize(handler);

		_mapChannels[*it] = channel;
		_vctHandler.push_back(handler);

		++it;
	}
}

void Manager::destroy() {
	if (_pDispatcher) {
		_pDispatcher->stopService();
		delete _pDispatcher;
		_pDispatcher = NULL;
	}

	if (_pMaintainer) {
		_pMaintainer->stopService();
		delete _pMaintainer;
		_pMaintainer = NULL;
	}

	map<string, Channel*>::iterator it = _mapChannels.begin();
	while (it != _mapChannels.end()) {
		Channel* channel = it->second;
		delete channel;
		++it;
	}

	_mapChannels.clear();
	_vctHandler.clear();
}

Channel* Manager::getChannel(const string& name) {
	map<string, Channel*>::iterator it = _mapChannels.begin();
	while (it != _mapChannels.end()) {
		string key = it->first;
		if (string::npos != name.find(key)) {
			return it->second;
		}
	}
	return NULL;
}
