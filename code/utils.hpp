#include <sw/redis++/redis.h>
#include <iostream>

template <class T>
inline void PrintContainer(const T& container){
    for(const auto& elem: container){
        std::cout << elem << std::endl;
    }
}

template <>
inline void PrintContainer(const std::vector<sw::redis::OptionalString>& container){
    for(const auto& elem: container){
        if(elem){
            std::cout << elem.value() << std::endl;
        }
        else{
            std::cout << "[nil]" << std::endl;
        }
    }
}
