#include <iostream>
#include <sw/redis++/redis.h>
#include <thread>
#include "utils.hpp"

using namespace std::chrono_literals;

void test1(sw::redis::Redis& server){
    server.set("key1", "111");
    auto ret = server.get("key1");
    if(ret)
        std::cout << ret.value() << std::endl;
}

void test2(sw::redis::Redis& server){
    auto ret = server.expire("key1", 5s);
    std::this_thread::sleep_for(2s);
    auto time = server.ttl("key1");
    std::cout << time << std::endl;
}

void test3(sw::redis::Redis& server){
    server.set("key2", "val2", 0s, sw::redis::UpdateType::NOT_EXIST);
    auto ret = server.get("key2");
    std::cout << ret.value() << std::endl;
    server.set("key2", "valllll", 0s, sw::redis::UpdateType::EXIST);
    ret = server.get("key2");
    std::cout << ret.value() << std::endl;
    server.set("key2", "val2222", 0s, sw::redis::UpdateType::NOT_EXIST);
    ret = server.get("key2");
    std::cout << ret.value() << std::endl;
}

void test4(sw::redis::Redis& server){
    server.flushall();
    server.mset({std::make_pair("key1", "val1"), std::make_pair("key2", "val2"), std::make_pair("key3", "val3")});
    std::vector<std::pair<std::string, std::string>> keys = {{"key4", "val4"}, {"key5", "val5"}};
    server.mset(keys.begin(), keys.end());

    std::vector<sw::redis::OptionalString> vals;
    auto bit = std::back_inserter(vals);
    server.mget({"key1", "key3", "key6"}, bit);
    PrintContainer<std::vector<sw::redis::OptionalString>>(vals);
}

void test5(sw::redis::Redis& server){
    server.flushall();
    server.set("key", "my name is wangjial");
    std::string ret = server.getrange("key", 5, 10);
    std::cout << ret << std::endl;

    server.setrange("key", 5, "hello world");
    auto val = server.get("key");
    if(val){
        std::cout << val.value() << std::endl;
    }
}

void test6(sw::redis::Redis& server){
    server.flushall();
    server.set("key", "1");
    long long ret1 = server.incr("key");
    long long ret2 = server.decr("key");
    std::cout << ret1 << " " << ret2 << std::endl;
}

int main()
{
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    //test1(server);
    //test2(server);
    test6(server);
    
    return 0;
}
