/*
 * Worker.h
 *
 *  Created on: 2018年7月10日
 *      Author: panhongfa
 */

#ifndef EFSERVER_WORKER_H_
#define EFSERVER_WORKER_H_

#include "util/tc_autoptr.h"
#include "util/tc_thread.h"
#include "util/tc_loop_queue.h"
#include "servant/TarsLogger.h"

template<typename T, int TimeOut=1000>
class Worker : public tars::TC_Thread, public tars::TC_HandleBase, public tars::TC_ThreadLock {
public:
	typedef tars::TC_LoopQueue<T*,10000>  LoopQueue;

public:
	Worker(size_t iQueueCap = 10000) {
		_terminate = false;
		_loopQueue = new LoopQueue(iQueueCap);
	}

	virtual ~Worker() {
	    terminate();

	    if(_loopQueue) {
	        delete _loopQueue;
	        _loopQueue = NULL;
	    }
	}

	void startService() {
		start();
	}

	void stopService() {
		terminate();
	}

	void postMessage(T* op) {
	    bool bFlag = _loopQueue->push_back(op);

	    {
	    	tars::TC_ThreadLock::Lock lock(*this);
	        notify();
	    }

	    if(!bFlag) {
	        TLOGERROR("[Center][Worker::postMessage] loop_queue full." << endl);
	        delete op;
	        op = NULL;
	    }
	}

protected:
	void terminate() {
		TC_ThreadLock::Lock lock(*this);
	    _terminate = true;

	    notifyAll();
	}

	virtual void run() {
		TLOGDEBUG("[Center][Worker::run] worker thread started." << endl);
	    while (!_terminate)
	    {
	        //异步请求回来的响应包处理
	        if(_loopQueue->empty()) {
	            TC_ThreadLock::Lock lock(*this);
	            timedWait(TimeOut);
	        }

	        T* op = NULL;
	        if (_loopQueue->pop_front(op)) {
	        	doProcess(op);
	        }
	        else {
	        	doTimeout();
	        }
	    }
	    TLOGDEBUG("[Center][Worker::run] worker thread stop." << endl);
	}

	virtual void doProcess(T* op) = 0;
	virtual void doTimeout() = 0;

private:
    bool _terminate;
    LoopQueue* _loopQueue;
};

#endif /* EFSERVER_WORKER_H_ */
