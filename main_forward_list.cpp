#include "Forward_list.hpp"

int main(){
    // 1. creating a container object to store objects of type int
    // 2. adding ten elements to the container (0, 1 ... 9)
    cntr::Forward_list<int> simple_list {0,1,2,3,4,5,6,7,8,9};

    // // 3. displaying the contents of the container on the screen
    //     // expected result: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    std::cout << simple_list<< std::endl;

    // 4. display the container size on the screen
        // expected result: 10
    std::cout << simple_list.size() << std::endl; 
    
    // 5. removal of the third (in a row), fifth and seventh elements
    for (int index = 1, i = 0; i < 3; ++i, ++index){ 
            simple_list.erase_after(index);
    }

    // 6. displaying the contents of the container on the
        // screen expected result: 0, 1, 3, 5, 7, 8, 9
    std::cout << simple_list << std::endl;

    // 7. adding element 10 to the beginning of the container
    simple_list.push_front(10);

    // 8. displaying the contents of the container on the screen
        // expected result: 10, 0, 1, 3, 5, 7, 8, 9
    std::cout << simple_list << std::endl;

    // 9. adding element 20 to the middle of the container
    simple_list.push_mid(20);

    // 10. displaying the contents of the container on the screen
        // expected result: 10, 0, 1, 3, 20, 5, 7, 8, 9
    std::cout << simple_list << std::endl;
    

    // 11. adding element 30 to the end of the container
    simple_list.push_back(30);

    // 12. displaying the contents of the container on the screen
        // expected result: 10, 0, 1, 3, 20, 5, 7, 8, 9, 30
    std::cout << simple_list << std::endl;

    // Additional task 4. Implement an additional structure - an iterator, 
    // encapsulating the logic for traversing the container
    // Example code that should work once implemented:
    for (auto iter = simple_list.begin(); iter != simple_list.end(); ++iter) {
        std::cout << *iter << std::endl;
    }

    //auto new_list = std::move(simple_list);
    auto new_list = simple_list;
    new_list.print("new_list_forward");



    return 0;
}
