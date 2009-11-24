#include <vector>
#include"statistic.h"

std::vector<int> statistic;

void StatisticInit()
{
    statistic.clear();
}

void StatisticAdd(int index, int value)
{
    while(statistic.size() <= index) statistic.push_back(0);
    statistic[index] += value;
}

int StatisticGet(int index)
{
    if(statistic.size() <= index) return 0;
    return statistic[index];
}

