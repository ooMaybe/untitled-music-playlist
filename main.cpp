#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

// Project defined libraries
#include "Backend/Backend.h"
#include "Backend/FileManager.h"

using namespace std;

Backend backend;
FileManager fileManager;

int main() {
    handleSetup();
    handleUI();

    return 0;
}

void handleSetup(){
    fileManager = new FileManager();
    backend = new Backend();
}

void handleUI(){

}