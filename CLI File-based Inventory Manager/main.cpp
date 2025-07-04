/*2. File-Based Inventory Manager
Goals:

Practice file I/O (<fstream>) and basic data persistence

Start using STL containers (std::vector, std::map)

Features to implement:

Load/save “items” (ID, name, quantity, price) to a CSV or JSON-style text file

Add/remove/update items via a menu

Simple search/filter by name or price range*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "functions.hpp"


int main() {
    // prompt user to load file
    std::cout << "Welcome" << std::endl;
    // scan save directory if it exists
    std::string save_directory = "saves";
    std::string target_extension = ".csv";

    namespace fs = std::filesystem;

    try {

        if(fs::exists(save_directory) && fs::is_directory(save_directory)) {
            std::cout << "Please choose a file to load" << std::endl;

            for(const auto& entry : fs::directory_iterator(save_directory)) {
                std::cout << "scanning save directory" << std::endl;

                if(fs::is_regular_file(entry.status())) {
                    std::cout << ""

                    if(entry.path().extension() == target_extension) {

                        std::cout << "vaild save file found" << std::endl;
                        std::cout << entry.path().filename() << std::endl;
                    } else { // no valid .csv files found
                        std::cout << "Directory found; no valid files" << std::endl;
                        createSave(save_directory);
                    }
                }
            }
        } else { // no save directory found
            
            std::cerr << "No save directory found" << std::endl;
            // create new save directory
            if(std::filesystem::create_directories(save_directory)) {

                std::cout << "directory created\n";
                createSave(save_directory); // create save file
            }
        }
    } catch (const fs::filesystem_error& ex) {

        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    return 0;
}