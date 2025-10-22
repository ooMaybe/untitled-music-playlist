#include <iostream>

using namespace std;

int main() {

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::cout << i * j << " ";
        }
        std::cout << std::endl;
    }


    std::cout << "Welcome to the Untitled Music Playlist!" << std::endl;
    return 0;
}