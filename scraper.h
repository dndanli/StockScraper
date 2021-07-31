#pragma once
#include <string>
class Scraper
{
public:
    std::string url;
    Scraper(std::string tickerName);
    std::string fetchHtml(std::string url);
    void formatData(std::string &dataSource);
};