#pragma once
#include <jsoncpp/json/json.h>
#include <fstream>

void saveDataToFile(std::vector<Json::Value> formattedDataSource);
void printLowestPrices(std::vector<std::string> tickerName, std::vector<double> tickerPrices);