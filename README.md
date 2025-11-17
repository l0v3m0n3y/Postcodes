# Postcodes
api for postcodes.io site postcodes data base
# main
```cpp
#include "Postcodes.h"
#include <iostream>

int main() {
   Postcodes api;

    auto postcodes = api.random_postcodes().then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    postcodes.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
