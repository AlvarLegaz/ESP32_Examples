#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WebServer.h>

class MyWebServer {
public:
    // Constructor
    MyWebServer(int port);

    // Inicializa las rutas y arranca el servidor
    void initEndpoints();

    // Procesa las peticiones entrantes
    void handleClient();

    // Detiene el servidor
    void stop();

private:
    WebServer server;

    // Handlers para endpoints
    void handleIndex();
    void handleHealth();
    void handleInfo();
    void handleConfig();
    void handleConfigPost();
    void handleOutput(); 
    void handleSetOutput();
    void handleDInput();
};

#endif // MYWEBSERVER_H
