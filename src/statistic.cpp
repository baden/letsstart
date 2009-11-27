#include <vector>
#include"statistic.h"

std::vector<int> statistic;

void StatisticInit()
{
    statistic.clear();
}

void StatisticAdd(int index, int value)
{
    // pushing in loop is too slow
    // while(statistic.size() <= index) statistic.push_back(0);

    if(statistic.size() <= index)
    {
	// fastest way
	// int values is zeroed already
        statistic.resize(index+1);
    }
    statistic[index] += value;
}

int StatisticGet(int index)
{
    if(statistic.size() <= index) return 0;
    return statistic[index];
}

