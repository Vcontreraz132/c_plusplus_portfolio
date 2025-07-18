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
    std::cout << "Welcome" << std::endl;
    // scan save directory if it exists
    std::string save_directory = "saves"; // expected save directory
    std::string target_extension = ".csv"; // expected save file extension

    namespace fs = std::filesystem;

    // pare down try/catch block to only catch filesystem errors

    bool save_file_exists = false; // flag to check if save file exists

    try {
        if(fs::exists(save_directory) && fs::is_directory(save_directory)) { // check if the save directory exists
            
            save_file_exists = scanSaveDirectory(save_directory, target_extension); // scan the save directory for valid save files

        } else { // no save directory found
            
            std::cerr << "No save directory found" << std::endl;
            // create new save directory
            if(std::filesystem::create_directories(save_directory)) {
                while(!save_file_exists) { // if no valid save file found, create a new save file
                    std::cout << "No valid save file found, creating new save file" << std::endl;
                    // create a new save file
                    createSave(save_directory); // create save file
                    save_file_exists = scanSaveDirectory(save_directory, target_extension); // after creating a new save file, scan the directory again
                }
            }
        }
    } catch (const fs::filesystem_error& ex) {

        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    // Collect valid save files into a vector
    std::vector<fs::directory_entry> valid_files;
    for(const auto& entry : fs::directory_iterator(save_directory)) {
        if(fs::is_regular_file(entry.status()) && entry.path().extension() == target_extension) {
            valid_files.push_back(entry);
        }
    }

    // Display valid save files with numbers
    std::cout << "Available save files:\n";
    for(size_t i = 0; i < valid_files.size(); ++i) {
        std::cout << i + 1 << ". " << valid_files[i].path().filename().string() << '\n';
    }

    // Prompt user to select a file
    std::string input;
    size_t selected_index = 0;
    bool valid_selection = false;
    std::string save_file_name;

    while(!valid_selection) {
        std::cout << "Enter the number or name of the save file to load: ";
        std::getline(std::cin, input);

        // Try to interpret input as a number
        try {
            selected_index = std::stoul(input);
            if(selected_index >= 1 && selected_index <= valid_files.size()) {
                save_file_name = valid_files[selected_index - 1].path().filename().string();
                valid_selection = true;
            } else {
                std::cout << "Invalid number. ";
            }
        } catch(...) {
            // Not a number, check if input matches a filename
            for(const auto& entry : valid_files) {
                if(entry.path().filename() == input) {
                    save_file_name = entry.path().filename().string();
                    valid_selection = true;
                    break;
                }
            }
            if(!valid_selection) {
                std::cout << "Invalid filename." << std::endl;
            }
        }
    }
    std::cout << "Selected save file: " << save_file_name << std::endl;

    std::string full_directory = save_directory + "/" + save_file_name; // full path to the selected save file

    // create a vector to hold all items
    std::vector<Item> allItems; // Vector to hold all items

    // now that we have a valid save file, we can load it
    if(!save_file_name.empty()) {
        allItems = loadSave(full_directory); // load items from the selected save file
        if(allItems.empty()) {
            std::cout << "No items found in the save file." << std::endl;
        } else {
            std::cout << "Items loaded successfully." << std::endl;
        }
    } else {
        std::cerr << "No save file selected" << std::endl;
    }

    // main loop
    bool running = true;
    while(running) {
        printMenu();
        std::string choice;
        std::cout << "Enter your choice: ";
        std::getline(std::cin, choice);
        
        switch(choice[0]) {
            case '1': // add item
                addItem(allItems, full_directory); // function to add item
                break;
            case '2': // remove item
                removeItem(allItems); // function to remove item
                break;
            case '3': // modify item
                modifyItem(allItems);
                break;
            case '4': // search item
                searchItem(allItems); // function to search item
                break;
            case '5': // show all
                showAllItems(allItems); // function to display all items
                break;
            case '6': // exit
                running = false; // exit the loop
                saveToFile(allItems, full_directory); // save to file before exiting
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
                break;
        }
    }

    return 0;
}