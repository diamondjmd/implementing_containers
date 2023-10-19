#include "Vector.hpp"

int main(){
    // 1. creating a container object to store objects of type int
    // 2. adding ten elements to the container (0, 1 ... 9)
    cntr::Vector<int> vec {0,1,2,3,4,5,6,7,8,9};

    // 3. displaying the contents of the container on the screen
        // expected result: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    std::cout << vec << std::endl;

    // 4. display the container size on the screen
        // expected result: 10
    std::cout << vec.size() << std::endl;

    // 5. removal of the third (in a row), fifth and seventh elements
    for (int index = 2, i = 0; i < 3; ++i, ++index){
            vec.erase(index);
    }

    // 6. displaying the contents of the container on the
        // screen expected result: 0, 1, 3, 5, 7, 8, 9
    std::cout << vec << std::endl;

    // 7. adding element 10 to the beginning of the container
    vec.insert(vec.begin(), 10);

    // 8. displaying the contents of the container on the screen
        // expected result: 10, 0, 1, 3, 5, 7, 8, 9
    std::cout << vec << std::endl;

    // 9. adding element 20 to the middle of the container
    vec.insert(vec.size() / 2, 20);

    // 10. displaying the contents of the container on the screen
        // expected result: 10, 0, 1, 3, 20, 5, 7, 8, 9
   std::cout << vec << std::endl;

    // 11. adding element 30 to the end of the container
    vec.insert(vec.end(), 30);
    
    // 12. displaying the contents of the container on the screen
        // expected result: 10, 0, 1, 3, 20, 5, 7, 8, 9, 30
    std::cout << vec << std::endl;

    // Additional task 4. Implement an additional structure - an iterator,
    // encapsulating the logic for traversing the container
    // Example code that should work once implemented:
    for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
        std::cout << *iter << std::endl;
    }

    auto vec2 = std::move(vec);
    vec2.print("vec2");

    return 0;
}
