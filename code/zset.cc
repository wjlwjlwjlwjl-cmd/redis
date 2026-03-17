#include <iostream>
#include <sw/redis++/redis.h>
#include "utils.hpp"

void test1(sw::redis::Redis& server){
    server.flushall();
    server.zadd("zset1", "zhangsan", 1);
    server.zadd("zset1", {std::make_pair("lisi", 2), std::make_pair("wangwu", 3)});
    std::vector<std::pair<std::string, std::string>> pairs = {
        std::make_pair("zhouliu", "4"),
        std::make_pair("tianqi", "5")
    };
    server.zadd("zset1", pairs.begin(), pairs.end());

    std::vector<std::string> withnoscore;
    auto it = std::back_inserter(withnoscore);
    server.zrange("zset1", 0, -1, it);
    std::vector<std::pair<std::string, double>> withscore;
    auto it2 = std::back_inserter(withscore);
    server.zrange("zset1", 0, -1, it2);
    std::cout << "with no score" << std::endl;
    PrintContainer<std::vector<std::string>>(withnoscore);

    std::cout << "with score" << std::endl;
    for(auto& e: withscore){
        std::cout << e.first << " " << e.second << std::endl;
    }
}

void test2(sw::redis::Redis& server){
    //zcard zrem zscore zrank
    long long ret = server.zcard("zset1");
    std::cout << "ret: " << ret << std::endl;
    
    auto score = server.zscore("zset1", "tianqi");
    if(score) std::cout << "tianqi's score: " << score.value() << std::endl;

    auto rank = server.zrank("zset1", "zhangsan");
    if(rank) std::cout << "zhangsan's rank: " << rank.value() << std::endl;

    server.zrem("zset1", "lisi");

    std::vector<std::string> mems;
    server.zrange("zset1", 0, -1, std::back_inserter(mems));
    PrintContainer<std::vector<std::string>>(mems);
}

int main(){
    sw::redis::Redis server("tcp://127.0.0.1:6379");
    test2(server);

    return 0;
}