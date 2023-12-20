//
// Created by Nathan Suh on 11/21/23.
//

#ifndef EZ_SPLITTER_CALCULATOR_H
#define EZ_SPLITTER_CALCULATOR_H

#include "receipt.h"
#include "partition.h"
#include <unordered_map>
using namespace std;
class Calculator {
    unordered_map<string, float> people_involved;
    unordered_map<Partition, unordered_map<string, float>> partitions;
    unordered_map<Partition, float> total_prices;
    int calculate_partitions();

public:
    Calculator(unordered_map<string, float> people_involved);
    int parse_receipt(Receipt receipt);
    int print_partitions();
    int calculate_prices();
    int print_prices();
};


#endif //EZ_SPLITTER_CALCULATOR_H
