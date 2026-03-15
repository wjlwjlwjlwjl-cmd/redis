#include <iostream>
#include <sw/redis++/redis.h>

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    server.set("key1", "val1");
    server.set("key2", "val2");
    auto ret1 = server.get("key1");
    auto ret2 = server.get("key2");
    auto ret3 = server.get("key3");
    if(ret1) std::cout << ret1.value() << std::endl;
    if(ret2) std::cout << ret2.value() << std::endl;
    if(ret3) std::cout << ret3.value() << std::endl;
    return 0;
}