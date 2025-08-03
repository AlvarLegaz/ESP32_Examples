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
    void handleConfig();
    void handleConfigPost();
    void handleOutput(); // Gestiona /output y /output/{pin}
    void handleDInput(); // Gestiona /dinput y /dinput/{pin}
};

#endif // MYWEBSERVER_H
