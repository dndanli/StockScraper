#include <iostream>
#include <vector>
#include <fstream>
#include "scraper.h"
#include <jsoncpp/json/json.h>

std::vector<Json::Value> stockPrice;
std::vector<Json::Value> companyName;
std::vector<Json::Value> marketCap;
std::vector<Json::Value> trailingPE;
std::vector<Json::Value> enterpriseValue;
std::vector<Json::Value> totalDebt;
std::vector<Json::Value> totalCash;
std::vector<Json::Value> forwardPE;
std::vector<Json::Value> beta;

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

int main()
{
  Json::CharReaderBuilder builder;
  Json::CharReader *reader = builder.newCharReader();
  Json::Value jsonData;
  std::string error;

  std::vector<std::string> stockVec;

  std::string ticker;
  int stockQuantity;
  int count = 0;

  std::cout << "How any stocks would you like to fetch?" << std::endl;
  std::cin >> stockQuantity;

  std::cout << "Stock name" << std::endl;

  while (count < stockQuantity)
  {
    std::cin >> ticker;
    stockVec.push_back(ticker);
    count++;
  }

  std::cout << std::endl;
  for (int i = 0; i < stockVec.size(); i++)
  {

    Scraper s(stockVec[i]);
    std::string data = s.fetchHtml(s.url);
    s.formatData(data);

    bool isParsingSuccessful = reader->parse(data.c_str(), data.c_str() + data.size(), &jsonData, &error);

    if (!isParsingSuccessful)
    {
      std::cout << data << std::endl;
      std::cout << error << std::endl;
    }

    stockPrice.push_back((jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["price"]["regularMarketPrice"]["fmt"]));
    companyName.push_back(jsonData["context"]["dispatcher"]["stores"]["StreamDataStore"]["quoteData"][stockVec[i]]["longName"]);
    marketCap.push_back(jsonData["context"]["dispatcher"]["stores"]["StreamDataStore"]["quoteData"][stockVec[i]]["marketCap"]["fmt"]);
    trailingPE.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["summaryDetail"]["trailingPE"]["fmt"]);

    enterpriseValue.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["enterpriseValue"]["fmt"]);
    totalCash.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["financialData"]["totalCash"]["fmt"]);
    totalDebt.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["financialData"]["totalDebt"]["fmt"]);

    forwardPE.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["forwardPE"]["fmt"]);
    beta.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["beta"]["fmt"]);

    std::cout << "+----------------------------------------------+" << std::endl;
    std::cout << "Company Metadata" << std::endl;
    std::cout << std::endl;

    std::cout << "Company: " << companyName[i] << std::endl;
    std::cout << "Stock price: $" << stockPrice[i] << std::endl;
    std::cout << "Market Cap: $" << marketCap[i] << std::endl;
    std::cout << "Trailing PE: " << trailingPE[i] << std::endl;
    std::cout << std::endl;

    std::cout << "Enterprise Value Per share" << std::endl;
    std::cout << std::endl;

    std::cout << "Enterprise Value: " << enterpriseValue[i] << std::endl;
    std::cout << "Total Cash: $" << totalCash[i] << std::endl;
    std::cout << "Total Debt: $" << totalDebt[i] << std::endl;
    std::cout << std::endl;

    std::cout << "Assumptions Metadata" << std::endl;
    std::cout << std::endl;

    std::cout << "Beta: " << beta[i] << std::endl;
    std::cout << "Forward P/E: " << forwardPE[i] << std::endl;
    std::cout << "+----------------------------------------------+" << std::endl;
  }

  saveDataToFile(companyName);
  saveDataToFile(stockPrice);
  saveDataToFile(marketCap);
  saveDataToFile(trailingPE);
  saveDataToFile(enterpriseValue);
  saveDataToFile(totalCash);
  saveDataToFile(totalDebt);
  saveDataToFile(beta);
  saveDataToFile(forwardPE);

  return 0;
}