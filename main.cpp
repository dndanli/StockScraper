#include <iostream>
#include <jsoncpp/json/json.h>
#include "scraper.h"
#include <vector>

struct MetaData
{
  Json::Value price;
  Json::Value companyName;
  Json::Value marketCap;
};

struct EnterpriseValPerShare
{
  Json::Value enterpriseValue;
  Json::Value totalDebt;
  Json::Value totalCash;
};

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

    //s.saveDataToFile(data);
    bool isParsingSuccessful = reader->parse(data.c_str(), data.c_str() + data.size(), &jsonData, &error);

    if (!isParsingSuccessful)
    {
      std::cout << data << std::endl;
      std::cout << error << std::endl;
    }

    MetaData metaData;
    metaData.marketCap = jsonData["context"]["dispatcher"]["stores"]["StreamDataStore"]["quoteData"][stockVec[i]]["marketCap"]["fmt"];
    metaData.price = jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["price"]["regularMarketPrice"]["fmt"];
    metaData.companyName = jsonData["context"]["dispatcher"]["stores"]["StreamDataStore"]["quoteData"][stockVec[i]]["longName"];

    EnterpriseValPerShare eValPerShare;
    eValPerShare.enterpriseValue = jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["enterpriseValue"]["fmt"];
    eValPerShare.totalCash = jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["financialData"]["totalCash"]["fmt"];
    eValPerShare.totalDebt = jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["financialData"]["totalDebt"]["fmt"];

    std::cout << "+----------------------------------------------+" << std::endl;
    std::cout << "Company Metadata" << std::endl;
    std::cout << "Company: " << metaData.companyName << std::endl;
    std::cout << "Stock price: $" << metaData.price << std::endl;
    std::cout << "Market Cap: $" << metaData.marketCap << std::endl;
    std::cout << std::endl;

    std::cout << "Enterprise Value Per share" << std::endl;
    std::cout << "Enterprise Value: " << eValPerShare.enterpriseValue << std::endl;
    std::cout << "Total Cash: $" << eValPerShare.totalCash << std::endl;
    std::cout << "Total Debr: $" << eValPerShare.totalDebt << std::endl;
    std::cout << "+----------------------------------------------+" << std::endl;

    std::cout << std::endl;
  }

  return 0;
}