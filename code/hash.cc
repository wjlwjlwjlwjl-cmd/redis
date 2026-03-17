#include <iostream>
#include <sw/redis++/redis.h>
#include "utils.hpp"

void test1(sw::redis::Redis& server){
    server.flushall();
    server.hset("hash1", "1", "zhangsan");
    //server.hmset("hash1", "1", "zhangsan");
    server.hset("hash1", {std::make_pair("2", "lisi"), std::make_pair("3", "wangwu")});
    server.hmset("hash1", {std::make_pair("2", "lisi"), std::make_pair("3", "wangwu")});
    std::vector<std::pair<std::string, std::string>> pairs = {std::make_pair("4", "zhouliu"), std::make_pair("5", "tianqi")};
    server.hset("hash1", pairs.begin(), pairs.end());
    server.hmset("hash1", pairs.begin(), pairs.end());

    auto ret = server.hget("hash1", "1");
    if(ret) std::cout << "1 " << ret.value() << std::endl;
}

void test2(sw::redis::Redis& server){
    bool exists = server.hexists("hash1", "zhouliu");
    std::cout << "zhouliu exists " << std::boolalpha << exists << std::endl;

    long long ret = server.hdel("hash1", {"2", "3"});
    std::cout << "delete: " << ret << std::endl;

    ret = server.hlen("hash1");
    std::cout << "hash1's member " << ret << std::endl;
}

void test3(sw::redis::Redis& server){
    server.flushall();
    server.hset("hash1", {std::make_pair("3", "zhangsan"), std::make_pair("2", "lisi")});
    server.hset("hash1", {std::make_pair("4", "wangwu"), std::make_pair("5", "zhouliu")});

    std::vector<std::string> keys;
    auto it = std::back_inserter(keys);
    server.hkeys("hash1", it);

    std::vector<std::string> vals;
    it = std::back_inserter(vals);
    server.hvals("hash1", it);
    PrintContainer<std::vector<std::string>>(keys);
    std::cout << std::endl;
    PrintContainer<std::vector<std::string>>(vals);
    std::cout << "---" << std::endl;

    vals.clear();
    server.hmget("hash1", {"3", "4", "5"}, it);
    PrintContainer<std::vector<std::string>>(vals);
}

int main(){
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    test3(server);
    
    return 0;
}