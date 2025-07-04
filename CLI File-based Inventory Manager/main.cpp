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

        bool save_file_exists = false; // flag to check if save file exists

        if(fs::exists(save_directory) && fs::is_directory(save_directory)) { // check if the save directory exists
            std::cout << "Please choose a file to load: " << std::endl;

            for(const auto& entry : fs::directory_iterator(save_directory)) {

                // Check if the entry is a regular file
                if(fs::is_regular_file(entry.status())) {
                    // Check if the file has the target extension
                    if(entry.path().extension() == target_extension) {
                        std::cout << entry.path().filename() << std::endl; // print valid save file
                        save_file_exists = true; // set flag to true
                        // continue to scan directory for more valid files
                    } else {
                        std::cout << entry.path().filename() << std::endl; // print invalid file
                        // continue to scan directory for more files
                    }
                }
            }

            // if no valid save file found, create a new save file
                if(!save_file_exists) {
                    std::cout << "No valid save file found, creating new save file" << std::endl;
                    createSave(save_directory); // create save file
                }
        
        } else { // no save directory found
            
            std::cerr << "No save directory found" << std::endl;
            // create new save directory
            if(std::filesystem::create_directories(save_directory)) {
                createSave(save_directory); // create save file
            }
        }
    } catch (const fs::filesystem_error& ex) {

        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    return 0;
}