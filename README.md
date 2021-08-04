# Dependencies

libcurl: https://curl.se/

jsoncpp: https://github.com/open-source-parsers/jsoncpp

# Compiling
Using g++ or your compiler

g++ -o exec scraper.cpp -L/your/libcurl/path/here/curl/ -lcurl main.cpp utilityFunctions.cpp -L/your/libjsoncpp/path/here/jsoncpp/json/ -ljsoncpp
