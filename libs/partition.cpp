//
// Created by Nathan Suh on 11/21/23.
//
#include "partition.h"
#include <cmath>
using namespace std;
Partition::Partition(set<string> &people) : people(people) {}
set<string> Partition::get_people() const noexcept{ return people; }
bool Partition::operator==(const Partition &other) const noexcept {
    return people == other.people;
}
size_t hash<Partition>::operator()(const Partition &p) const noexcept {
    int hash_value = 17;
    for (auto & n : p.get_people()){
        hash_value = (hash_value * 31 + hash<string>()(n)) % int((pow(2, 3)));
    }
    return hash_value;
}

#include "partition.h"
