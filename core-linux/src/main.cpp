#include <cstdlib>
#include <string>
#include "lib/json.hpp"
#include "settings.h"
#include "resources.h"
#include "auth/authbasic.h"
#include "ping/ping.h"
#include "permission.h"
#include "api/app/app.h"
#include "server/serverlistener.h"

using namespace std;
using json = nlohmann::json;

int main(int argc, char ** argv) {
    json args;
    bool enableHTTPServer = false;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
    settings::setGlobalArgs(args);
    if (!loadResFromDir)
        resources::makeFileTree();
    json options = settings::getConfig();
    authbasic::generateToken();
    ping::startPingReceiver();
    permission::registerBlockList();
    ServerListener serverListener;
    if(!options["enableHTTPServer"].is_null())
        enableHTTPServer = options["enableHTTPServer"];

    string navigationUrl = options["url"];
    if(enableHTTPServer)
        navigationUrl = serverListener.init();

    string mode = settings::getMode();
    if(mode == "browser") {
        json browserOptions = options["modes"]["browser"];
        browserOptions["url"] = navigationUrl;
        app::open(browserOptions);
    }
    else if(mode == "window") {
        json windowOptions = options["modes"]["window"];
        windowOptions["url"] = navigationUrl;
        app::showWindow(windowOptions);
    }
    if(enableHTTPServer)
        serverListener.run();
    else
        while(1);
    return 0;
}
