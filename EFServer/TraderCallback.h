/*
 * TraderCallback.h
 *
 *  Created on: 2018年6月15日
 *      Author: panhongfa
 */

#ifndef EFSERVER_TRADERCALLBACK_H_
#define EFSERVER_TRADERCALLBACK_H_

#include <vector>
#include "servant/Servant.h"

using namespace std;

class TraderCallback : public ServantCallback {
public:
	TraderCallback(const string& type, const tars::ServantPtr& servant, const tars::TarsCurrentPtr& current);
	virtual ~TraderCallback() {}

	int getActionID() const;
	void setActionID(int id);

private:
	int _nActionID;
};
typedef tars::TC_AutoPtr<TraderCallback> TraderCallbackPtr;

#endif /* EFSERVER_TRADERCALLBACK_H_ */
