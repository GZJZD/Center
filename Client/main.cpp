/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "TraderServant.h"
#include "servant/Communicator.h"
#include "util/tc_thread_pool.h"
#include <iostream>
#include "TraderServant.h"
#include "util/tc_common.h"
#include "util/tc_option.h"

using namespace std;
using namespace Center;
using namespace tars;

class Test1
{
public:
    Test1(const string &sStr);
    ~Test1();

    void testUserLogin(int requestID, int iExecuteNum);
    void testUserLogout(int requestID, int iExecuteNum);

private:
    Communicator    _comm;
    TraderServantPrx _pPrx;
};

Test1::Test1(const string &sStr)
{
    _comm.setProperty("locator", "tars.tarsregistry.QueryObj@tcp -h 192.168.3.189 -p 17890 -t 10000 ");
    _comm.stringToProxy(sStr, _pPrx);
}

Test1::~Test1()
{
    
}

void Test1::testUserLogin(int requestID, int iExecuteNum)
{
    time_t _iTime=TC_TimeProvider::getInstance()->getNowMs();

    for(int i=0; i<iExecuteNum; i++)
    {
        try
        {
            int ret = -1;

            Center::userLoginRequest req;
			Center::userLoginResponse rsp;

			req.requestId = requestID;
			req.brokerId = 8001;
			req.userId = "6612126";
			req.password = "888888";

            ret = _pPrx->userLogin(req, rsp);
            cout << "ret: " << ret << ", " << "rsp.requestId: " << rsp.requestId << ", " <<\
            		"rsp.errcode: " << rsp.errcode << ", " << "rsp.errmsg: " << rsp.errmsg << endl;

            if(i == iExecuteNum)
            {
                cout << "pthread id: " << pthread_self() << " | " << TC_TimeProvider::getInstance()->getNowMs() - _iTime << endl;
                _iTime=TC_TimeProvider::getInstance()->getNowMs();
            }
        }
        catch(TC_Exception &e)
        {
            cout << "pthread id: " << pthread_self() << "id: " << i << "exception: " << e.what() << endl;
        }
        catch(...)
        {
            cout << "pthread id: " << pthread_self() << "id: " << i << "unknown exception." << endl;
        }
    }
}

void Test1::testUserLogout(int requestID, int iExecuteNum) {
    time_t _iTime=TC_TimeProvider::getInstance()->getNowMs();

    for(int i=0; i<iExecuteNum; i++)
    {
        try
        {
            int ret = -1;

            Center::userLogoutRequest req;
			Center::userLogoutResponse rsp;

			req.requestId = requestID;
			req.brokerId = 8001;
			req.userId = "6612125";

            ret = _pPrx->userLogout(req, rsp);
            cout << "ret: " << ret << ", " << "rsp.requestId: " << rsp.requestId << ", " <<\
            		"rsp.errcode: " << rsp.errcode << ", " << "rsp.errmsg: " << rsp.errmsg << endl;

            if(i == iExecuteNum)
            {
                cout << "pthread id: " << pthread_self() << " | " << TC_TimeProvider::getInstance()->getNowMs() - _iTime << endl;
                _iTime=TC_TimeProvider::getInstance()->getNowMs();
            }
        }
        catch(TC_Exception &e)
        {
            cout << "pthread id: " << pthread_self() << "id: " << i << "exception: " << e.what() << endl;
        }
        catch(...)
        {
            cout << "pthread id: " << pthread_self() << "id: " << i << "unknown exception." << endl;
        }
    }
}

void usage()
{
    cout << "Usage : ./TestClient --c=connecter --t=testType [--s=1] [--n=1]" << endl;

    cout << "  TestClient         工具名" << endl;
    cout << "  --c                 必须指定，连接到tars服务的连接字符串，包含了服务名，IP和端口信息" << endl;
    cout << "  --t                 必须指定，运行测试用例的编号" << endl;

    cout << "  --s                 运行测试用例的线程个数，默认为1" << endl;
    cout << "  --n                 测试用例每次运行需执行的次数，默认为1" << endl;
    exit(0);
}

int main(int argc,char ** argv)
{
    if(argc < 2)
    {
        usage();
    }

    tars::TC_Option option;
    option.decode(argc, argv);
    vector<string> vTarsFiles = option.getSingle();

    if(option.hasParam("help"))
    {
        usage();
    }

    string strConnect;
    if(option.hasParam("c"))
    {
    	strConnect = option.getValue("c");
    }
    else
    {
        usage();
    }

    int nType = 0;
    if(option.hasParam("t"))
    {
        string type = option.getValue("t");
        if(tars::TC_Common::strto<int>(type) > 0)
        {
        	nType = tars::TC_Common::strto<int>(type);
        }
        else
        {
            cout<<"type should be >0 "<<endl;
            exit(1);

        }
    }
    else
    {
        usage();
    }

    int threads = 1;
    if(option.hasParam("s"))
    {
        string strthreads = option.getValue("s");
        if(tars::TC_Common::strto<int>(strthreads) > 0)
        {
        	threads = tars::TC_Common::strto<int>(strthreads);
        }
        else
        {
            cout<<"threads should be >0 "<<endl;
            exit(1);

        }
    }

    int times = 1;
    if(option.hasParam("n"))
    {
        string strtimes = option.getValue("n");
        if(tars::TC_Common::strto<int>(strtimes) > 0)
        {
        	times = tars::TC_Common::strto<int>(strtimes);
        }
        else
        {
            cout<<"times should be >0 "<<endl;
            exit(1);

        }
    }

    Test1 test1(strConnect);

    try
    {
        TC_ThreadPool tp;
        tp.init(threads);
        tp.start();
        
        TC_Functor<void, TL::TLMaker<int, int>::Result> func;
        switch (nType) {
			case 1:
			{
				TC_Functor<void, TL::TLMaker<int, int>::Result> cmd(&test1, &Test1::testUserLogin);
				func = cmd;
			}
				break;
			case 2:
			{
				TC_Functor<void, TL::TLMaker<int, int>::Result> cmd(&test1, &Test1::testUserLogout);
				func = cmd;
			}
				break;
			default:
				break;
		}

        int rqst_id = 1;
        for(int i = 0; i<threads; i++, rqst_id++)
        {
            TC_Functor<void, TL::TLMaker<int, int>::Result>::wrapper_type fw(func,rqst_id,times);
            tp.exec(fw);
            cout << "********************" <<endl;
        }
        tp.waitForAllDone(); 
    }catch(exception &e)
    {
        cout<<e.what()<<endl;
    }
    catch(...)
    {
        
    }
    
    return 0;
}
