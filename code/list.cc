#include <iostream>
#include <sw/redis++/redis.h>
#include "utils.hpp"

void test1(sw::redis::Redis& server)
{
    server.flushall();
    server.lpush("list1", "1");
    server.lpush("list1", {"2", "3", "4"});
    std::vector<std::string> vals = {"5", "6"};
    server.lpush("list1", vals.begin(), vals.end());

    std::vector<std::string> rets;
    auto bit = std::back_inserter(rets);
    server.lrange("list1", 0, 3, bit);
    PrintContainer<std::vector<std::string>>(rets);
    for(auto e: rets)
    {
        std::cout << e << std::endl;
    }
}

void test2(sw::redis::Redis& server)
{
    server.flushall();
    server.rpush("list1", {"1", "2", "3"});
    auto val1 = server.lpop("list1");
    auto val2 = server.rpop("list1");
    std::cout << val1.value() << " " << val2.value() << std::endl;
}

void test3(sw::redis::Redis& server){
    using namespace std::chrono_literals;
    server.flushall();
    server.lpush("list1", {"1", "2", "3"});
    auto ret = server.blpop("list1");
    if(ret)
    {
        std::cout << "key " << ret.value().first << std::endl;
        std::cout << "val " << ret.value().second << std::endl;
    }
    auto ret2 = server.brpop("list2", 20s);
    if(ret2)
    {
        std::cout << "key " << ret2.value().first << std::endl;
        std::cout << "val " << ret2.value().second << std::endl;
    }
}

void test4(sw::redis::Redis& server){
    auto len = server.llen("list1");
    std::cout << len << std::endl;
}

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");

    test4(server);

    return 0;
}
