#include "TreeMap.h"
void print_char_vector(std::vector<KeyValuePair<int, char>> myvec){
    int length=myvec.size();
    for(int i=0;i<length;++i){
        std::cout<<myvec.at(i)<<" ";
    }
}