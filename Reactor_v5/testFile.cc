#include "EchoServer.h"

int main()
{
    EchoServer es(4, 10, "127.0.0.1", 8888);
    es.start();
    es.stop();
    return 0;
}