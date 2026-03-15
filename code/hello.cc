#include <iostream>
#include <sw/redis++/redis.h>

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    std::string ret = server.ping();
    std::cout << ret << std::endl;
    return 0;
}