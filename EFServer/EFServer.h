#ifndef _EFServer_H_
#define _EFServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class EFServer : public Application
{
public:
    /**
     *
     **/
    virtual ~EFServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern EFServer g_app;

////////////////////////////////////////////
#endif
