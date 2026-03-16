#include <iostream>
#include <sw/redis++/redis.h>
#include <set>
#include "utils.hpp"

void test1(sw::redis::Redis& server){
    server.flushall();
    server.sadd("set1", {"zhangsan", "lisi", "wangwu"});

    std::set<std::string> mems;
    auto in = std::inserter(mems, mems.end());
    server.smembers("set1", in);
    PrintContainer<std::set<std::string>>(mems);
}

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    test1(server);

    return 0;
}