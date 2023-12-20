//
// Created by Nathan Suh on 11/20/23.
//

#ifndef EZ_SPLITTER_RECEIPT_H
#define EZ_SPLITTER_RECEIPT_H
#include<unordered_map>
#include <string>
#include <vector>
#include<set>
using namespace std;
struct Receipt{
    unordered_map<string, pair<set<string>, float>>  receipt_map;
    Receipt();
    int set_item(string& item, set<string>& people, float& price);
    friend ostream& operator<<(ostream& os, const Receipt& r);
};
#endif //EZ_SPLITTER_RECEIPT_H
