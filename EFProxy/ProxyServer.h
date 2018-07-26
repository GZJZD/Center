#ifndef _ProxyServer_H_
#define _ProxyServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class ProxyServer : public Application
{
public:
    /**
     *
     **/
    virtual ~ProxyServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern ProxyServer g_app;

////////////////////////////////////////////
#endif
