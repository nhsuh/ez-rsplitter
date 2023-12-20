//
// Created by Nathan Suh on 11/21/23.
//

#ifndef EZ_SPLITTER_PARTITION_H
#define EZ_SPLITTER_PARTITION_H
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
using namespace std;
class Partition {
    set<string> people;
public:
    Partition(set<string> &people);
    set<string> get_people() const noexcept;
    bool operator==(const Partition& other) const noexcept;
};
template<>
struct hash<Partition>{
    size_t operator()(const Partition& p) const noexcept;
};

#endif //EZ_SPLITTER_PARTITION_H
