# Dependencies

libcurl: https://curl.se/

jsoncpp: https://github.com/open-source-parsers/jsoncpp

# Compiling
### Using g++ or your compiler

g++ -o exec scraper.cpp -L/your/libcurl/path/here/curl/ -lcurl main.cpp utilityFunctions.cpp -L/your/libjsoncpp/path/here/jsoncpp/json/ -ljsoncpp

### Or

g++ -o exec scraper.cpp -lcurl main.cpp utilityFunctions.cpp -ljsoncpp

# How to use it

1. In your terminal type ./exec
2. Type how many stocks to check
3. Type the name of each stock ticker
4. Wait for results
5. Follow output in case you want to check the lowest prices.

# Output
![sc1](https://user-images.githubusercontent.com/73452073/129433357-f35fc6ca-e657-451e-8fcc-10f90c1cd9e1.png)
![sc2](https://user-images.githubusercontent.com/73452073/129433361-fb8b0475-0b66-4249-8628-84d59727d50b.png)



