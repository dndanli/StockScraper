#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>
#include "utilityFunctions.h"
#include "scraper.h"

//each vector stores a piece of information of a stock
std::vector<Json::Value> stockPrice;
std::vector<Json::Value> companyName;
std::vector<Json::Value> marketCap;
std::vector<Json::Value> trailingPE;
std::vector<Json::Value> enterpriseValue;
std::vector<Json::Value> totalDebt;
std::vector<Json::Value> totalCash;
std::vector<Json::Value> forwardPE;
std::vector<Json::Value> beta;

int main()
{
  Json::CharReaderBuilder builder;
  Json::CharReader *reader = builder.newCharReader();
  Json::Value jsonData;
  std::string error;

  std::string userChoice;
  //stores stock tickers
  std::vector<std::string> stockTickers;

  //stores ticker prices as strings, used to remove commas from json value before converting to a double.
  std::vector<std::string> strTickerPrices;

  std::string ticker, line;
  int tickerCount;
  int count;

  //prompting user
  std::cout << "How many tickers would you like to fetch?" << std::endl;
  std::cin >> tickerCount;

  if (tickerCount < 1)
  {
    std::cout << "Invalid number, specify more than " << tickerCount << " ticker(s)" << std::endl;
    goto finish;
  }

  while (std::getline(std::cin, line))
  {
    std::stringstream ss(line);
    if (ss >> tickerCount)
    {
      if (ss.eof())
      {
        break;
      }
    }
    std::cout << "Please enter again: " << std::endl;
  }

  std::cout << "Enter a ticker" << std::endl;

  //prompting for ticker names
  for (count = 0; count < tickerCount; count++)
  {
    std::cin >> ticker;

    stockTickers.push_back(ticker);
  }

  std::cout << std::endl;

  //scraping all the data
  for (int i = 0; i < stockTickers.size(); i++)
  {
    //initializing scraper instance for current ticker
    Scraper s(stockTickers[i]);

    //fetching html
    std::string htmlData = s.fetchHtml(s.url);

    //format to json
    s.formatData(htmlData);

    //parsing to  valid json
    bool isParsingSuccessful = reader->parse(htmlData.c_str(), htmlData.c_str() + htmlData.size(), &jsonData, &error);

    if (!isParsingSuccessful)
    {
      std::cout << error << std::endl;
    }

    //querying data from json

    // --- Company Metadata -------------
    strTickerPrices.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["price"]["regularMarketPrice"]["fmt"].asCString());
    stockPrice.push_back((jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["price"]["regularMarketPrice"]["fmt"]));
    companyName.push_back(jsonData["context"]["dispatcher"]["stores"]["StreamDataStore"]["quoteData"][stockTickers[i]]["longName"]);
    marketCap.push_back(jsonData["context"]["dispatcher"]["stores"]["StreamDataStore"]["quoteData"][stockTickers[i]]["marketCap"]["fmt"]);
    trailingPE.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["summaryDetail"]["trailingPE"]["fmt"]);

    // --- Enterprice Value per share ---
    enterpriseValue.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["enterpriseValue"]["fmt"]);
    totalCash.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["financialData"]["totalCash"]["fmt"]);
    totalDebt.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["financialData"]["totalDebt"]["fmt"]);

    // --- Assumptions Metadata ---------
    forwardPE.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["forwardPE"]["fmt"]);
    beta.push_back(jsonData["context"]["dispatcher"]["stores"]["QuoteSummaryStore"]["defaultKeyStatistics"]["beta"]["fmt"]);

    //displaying info
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

  // saving data to csv file (optional)
  saveDataToFile(companyName);
  saveDataToFile(stockPrice);
  saveDataToFile(marketCap);
  saveDataToFile(trailingPE);
  saveDataToFile(enterpriseValue);
  saveDataToFile(totalCash);
  saveDataToFile(totalDebt);
  saveDataToFile(beta);
  saveDataToFile(forwardPE);

  std::cout << "Would you like to see stocks with the lowest prices?(y/n)" << std::endl;
  std::cin >> userChoice;
  if (userChoice == "y")
  {
    //remove comma to not affect sorting the price values
    for (size_t i = 0; i < strTickerPrices.size(); i++)
    {
      strTickerPrices[i].erase(std::remove(strTickerPrices[i].begin(), strTickerPrices[i].end(), ','), strTickerPrices[i].end());
    }

    //converting vector of strings into vector of double
    std::vector<double> tickerPrices(strTickerPrices.size());
    transform(strTickerPrices.begin(), strTickerPrices.end(), tickerPrices.begin(),
              [](std::string const &val)
              { return stod(val); });

    printLowestPrices(stockTickers, tickerPrices);
  }

finish:
  std::cout << "Goodbye!" << std::endl;

  return 0;
}
