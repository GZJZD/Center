#include "EFServer.h"
#include "TraderServantImp.h"
#include "Manager.h"

using namespace std;

EFServer g_app;

/////////////////////////////////////////////////////////////////
void
EFServer::initialize()
{
    //initialize application here:
    addServant<TraderServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".TraderServantObj");
    addServant<TraderServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".TraderServantObjA");

    vector<string> names;
    names.push_back("TraderServantObj");
    names.push_back("TraderServantObjA");
    Manager::getInstance()->initialize(names);
}
/////////////////////////////////////////////////////////////////
void
EFServer::destroyApp()
{
    //destroy application here:
	Manager::getInstance()->destroy();
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
