#include <iostream>
#include <sw/redis++/redis.h>
#include <chrono>
#include <thread>

void test1(sw::redis::Redis& server)
{
    server.set("key1", "val1");
    server.set("key2", "val2");
    auto ret1 = server.get("key1");
    auto ret2 = server.get("key2");
    auto ret3 = server.get("key3");
    if(ret1) std::cout << ret1.value() << std::endl;
    if(ret2) std::cout << ret2.value() << std::endl;
    if(ret3) std::cout << ret3.value() << std::endl;
    auto e = server.exists("key1");
    std::cout << e << std::endl;
    e = server.exists("key4");
    std::cout << e << std::endl;
    auto ret = server.del({"key1", "key2", "key3"});
    if(!ret)
    {
        std::cout << "delete key fail" << std::endl;
    }
}

void test2(sw::redis::Redis& server){
    server.flushall();
    server.set("key1", "val1");
    server.set("key2", "val2");
    server.set("key3", "val3");
    std::vector<std::string> keys;
    auto bit = std::back_inserter(keys);
    server.keys("*", bit);
    for(auto& e: keys){
        std::cout << e << std::endl;
    }
}

void test3(sw::redis::Redis& server)
{
    using namespace std::chrono_literals;
    server.set("key", "val");
    server.expire("key", std::chrono::seconds(10));
    std::this_thread::sleep_for(3s);
    auto ret = server.ttl("key");
    std::cout << ret << std::endl;
}

void test4(sw::redis::Redis& server)
{
    std::string ret;
    server.flushall();
    server.set("key", "val");
    ret = server.type("key");
    std::cout << ret << " ";

    server.lpush("list", "1");
    ret = server.type("list");
    std::cout << ret << " ";

    server.hset("hash", {"1", "zhangsan"});
    ret = server.type("hash");
    std::cout << ret << " ";

    server.sadd("set", "zhangsan");
    ret = server.type("set");
    std::cout << ret << " ";

    server.zadd("zset", "zhangsan", 1);
    ret = server.type("zset");
    std::cout << ret << " ";
}

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    //test1(server);
    test4(server);
    return 0;
}