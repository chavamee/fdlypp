# fdlypp - A C++ wrapper to the Feedly REST API

## Example Usage
```cpp
#include <fdly.hpp>
#include <iostream>

int main(void) {
  Fdly connection {Fdly::User{"<user id>", "<api token>"}};

  auto categories = connection.GetCategories();
  for (auto& category : categories) {
    std::cout << category.Label << std::endl;

    auto entries = connection.GetEntries(category);
    for (auto& entry: entries) {
      std::cout << "     " << entry.Title << " " << entry.ID  << std::endl;
    }
  }
}
```
