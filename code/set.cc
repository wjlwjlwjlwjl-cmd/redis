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

void test2(sw::redis::Redis& server){
    server.flushall();
    server.sadd("set", {"1", "2", "3"});
    bool exist = server.sismember("set", "1");
    std::cout << "1 exists: " << std::boolalpha << exist << std::endl;

    long long num = server.scard("set");
    std::cout << "set has " << num << " members" << std::endl;

    auto ret = server.spop("set");
    if(ret){
        std::cout << ret.value() << std::endl;
    }
}

void test3(sw::redis::Redis& server){
    server.flushall();
    server.sadd("set1", {"1", "2", "3"});
    server.sadd("set2", {"5", "2", "4"});
    std::set<std::string> inset;
    auto it = std::inserter(inset, inset.end());
    server.sinter({"set1", "set2"}, it);
    PrintContainer<std::set<std::string>>(inset); 

    server.sinterstore("set3", {"set1", "set2"});
    inset.clear();
    server.smembers("set3", it);
    PrintContainer<std::set<std::string>>(inset); 
}

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    test3(server);

    return 0;
}