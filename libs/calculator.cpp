//
// Created by Nathan Suh on 11/21/23.
//

#include "calculator.h"
#include <iostream>
#include <utility>
Calculator::Calculator(unordered_map<std::string, float> people_involved): people_involved(std::move(people_involved)) {}
int Calculator::parse_receipt(Receipt receipt){
    for (auto & it : receipt.receipt_map){
        Partition curr(it.second.first);
        partitions[curr][it.first] = it.second.second;
    }
    return 0;
}
int Calculator::print_partitions() {
    for (auto & kt : partitions){
        set<string> tester = kt.first.get_people();
        cout << "People: ";
        for (auto & p : tester) cout << p << " ";
        cout << endl;
        for (auto & jt : kt.second){
            cout << jt.first << " ";
            cout << jt.second << endl;
        }
    }
    return 0;
}
int Calculator::calculate_partitions() {
    for(auto& it : partitions){
        for(auto& jt : it.second){
            total_prices[it.first] += jt.second;
        }
    }
    return 0;
}
int Calculator::calculate_prices() {
    calculate_partitions();
    for (auto &p: people_involved) {
        for (auto &it: partitions) {
            set<string> party_people = it.first.get_people();
            if (party_people.find(p.first) != party_people.end()) p.second += (total_prices[it.first] / float(party_people.size()));
        }
    }
    return 0;
}
int Calculator::print_prices() {
    cout << "Partition prices:" << endl;
    for (auto &it : total_prices){
        for (auto & jt : it.first.get_people()) cout << jt << " ";
        cout << "=> " << it.second;
        cout << endl;
    }
    for (auto & it : people_involved) cout << it.first << ": " << it.second << endl;


    return 0;
}
