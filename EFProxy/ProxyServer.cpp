#include "ProxyServer.h"
#include "TraderProxyImp.h"

using namespace std;

ProxyServer g_app;

/////////////////////////////////////////////////////////////////
void
ProxyServer::initialize()
{
    //initialize application here:
    //...

    addServant<TraderProxyImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".TraderProxyObj");
}
/////////////////////////////////////////////////////////////////
void
ProxyServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
