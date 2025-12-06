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

void handleSetup();
void handleUI();

int main() {
    handleSetup();
    handleUI();

    return 0;
}

void handleSetup(){
    cout << "[Main] Starting setup...\n";
    
    // Initialize Backend and FileManager instances
    fileManager = FileManager();
    fileManager.createFolder("data");

    backend = Backend();
    backend.initialize();

    cout << "[Main] Setup complete.\n";
}

void handleUI(){

}