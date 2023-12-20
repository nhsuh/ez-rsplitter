//
// Created by Nathan Suh on 11/20/23.
//

#include "receipt.h"
#include <iostream>
Receipt::Receipt()= default;
int Receipt::set_item(string& item, set<string>& people, float& price) {
    float new_price = receipt_map[item].second + price;
    receipt_map[item] = make_pair(people, new_price);
    return 0;
}
