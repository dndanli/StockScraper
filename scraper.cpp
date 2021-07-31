
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "scraper.h"
#include <vector>

Scraper::Scraper(std::string ticker) : url("https://finance.yahoo.com/quote/" + ticker + "/key-statistics?p=" + ticker) {}

// This code was partially made by consulting the libcurl docs
// and pages on stack overflow

// By user Thanatos
// https://stackoverflow.com/questions/5525613/how-do-i-fetch-a-html-page-source-with-libcurl-in-c

// A response from the libcurl author/developer Daniel Stenberg in the link above
// also provides a link for an implementation with C
// https://curl.se/libcurl/c/getinmemory.html

// By user Joachim Isaksson
//https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
size_t curlToStringCallback(void *contentBuffer, size_t size, size_t numberOfMembers, void *dataStream)
{
    //getting the number of bytes
    size_t nbytes = size * numberOfMembers;

    //cast stream into a string
    std::string *dataStreamStr = static_cast<std::string *>(dataStream);

    //cast buffer into char
    char *contents = static_cast<char *>(contentBuffer);

    // append contents to dataStream
    dataStreamStr->append(contents, nbytes);

    //returning size of each member of stream.
    return nbytes;
}

std::string Scraper::fetchHtml(std::string url)
{
    //string to hold contents
    std::string dataFromPage;

    //result from curl handle
    CURLcode result;

    curl_global_init(CURL_GLOBAL_ALL);

    //initializing curl session
    CURL *curl = curl_easy_init();

    //in case if function fail
    if (!curl)
    {
        std::cout << stderr << "init failed" << std::endl;
    }

    //set curl behavior and fetch content of stock page
    //url is converted to char * since setopt does not accept a std::string.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //send all data to callback function
    //convert the curl handle to a string
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToStringCallback);

    // write the data of curl handle into pageData.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataFromPage);

    //perform out action
    //takes set of options and performs request
    result = curl_easy_perform(curl);
    if (!result == CURLE_OK)
    {
        std::cout << stderr << " download problem " << curl_easy_strerror(result) << std::endl;
    }

    // MUST have cleanup says Stenberg....
    curl_easy_cleanup(curl);

    curl_global_cleanup();

    return dataFromPage;
}

void Scraper::formatData(std::string &dataSource)
{

    size_t startPos = dataSource.find("root.App.main");

    size_t endPos = dataSource.find("}}}};");

    // take off minus 12 characters of js code
    size_t charsToFetch = ((endPos - 12) - startPos);

    dataSource = dataSource.substr((startPos + 16), charsToFetch);
}
