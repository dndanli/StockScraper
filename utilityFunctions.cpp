#include "utilityFunctions.h"
#include <iostream>
#include <vector>
#include <algorithm>

//allows user to save stock information to a csv file
void saveDataToFile(std::vector<Json::Value> formattedDataSource, std::string rowName)
{
    std::fstream csvFile;
    csvFile.open("stockData.csv", std::ios::app);
    if (csvFile.is_open())
    {
        csvFile << rowName << ", ";
        for (size_t i = 0; i < formattedDataSource.size(); i++)
        {
            csvFile << formattedDataSource[i] << ", ";
        }
        csvFile << std::endl;
        csvFile.close();
    }
}

//this function compares the second element of two pairs
bool compare(const std::pair<std::string, double> &a, const std::pair<std::string, double> &b)
{
    return (a.second < b.second);
}

//this function prints the tickers with the lowest price.
void printLowestPrices(std::vector<std::string> tickerName, std::vector<double> tickerPrices)
{
    std::vector<std::pair<std::string, double>> pairInfo;
    for (size_t i = 0; i < tickerName.size(); i++)
    {
        pairInfo.push_back(std::make_pair(tickerName[i], tickerPrices[i]));
    }

    std::sort(pairInfo.begin(), pairInfo.end(), compare);

    for (size_t i = 0; i < pairInfo.size(); i++)
    {
        std::cout << pairInfo[i].first << ": " << pairInfo[i].second << std::endl;
    }
}
