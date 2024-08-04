#include "tcp_server.h"

int main()
{
    using namespace http;

    TCPServer Server = TCPServer("127.0.0.1", 8080);
    Server.start_listen();

    return 0;
}
