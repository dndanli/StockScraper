#pragma once
#include <jsoncpp/json/json.h>
#include <fstream>

void saveDataToFile(std::vector<Json::Value> formattedDataSource);
