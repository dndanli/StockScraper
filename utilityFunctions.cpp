#include "utilityFunctions.h"

//allows user to save stock information to a csv file
void saveDataToFile(std::vector<Json::Value> formattedDataSource)
{
    std::fstream csvFile;
    csvFile.open("stockData.csv", std::ios::app);
    if (csvFile.is_open())
    {
        for (size_t i = 0; i < formattedDataSource.size(); i++)
        {
            csvFile << formattedDataSource[i] << ", ";
        }
        csvFile << std::endl;
        csvFile.close();
    }
}