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
                createSave(save_directory); // create save file
            }
        }
    } catch (const fs::filesystem_error& ex) {

        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    // move logic out of try/catch block

    // if no valid save file found, create a new save file
    while(!save_file_exists) {
        std::cout << "No valid save file found, creating new save file" << std::endl;
        createSave(save_directory); // create save file
        save_file_exists = scanSaveDirectory(save_directory, target_extension); // after creating a new save file, scan the directory again
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

    // now that we have a valid save file, we can load it
    if(!save_file_name.empty()) {
        loadSave(save_directory, save_file_name); // load the save file, data stored in .csv format, load data into item class
    } else {
        std::cerr << "No save file selected" << std::endl;
    }

    // create test item object and save to file
    Item test_item;
    test_item.init(1, "Test Item", 9.99, 100);
    std::cout << "Test Item: ";
    test_item.display();
    std::ofstream save_file(save_directory + "/" + save_file_name, std::ios::app); // append to existing file
    if(save_file.is_open()) {
        save_file << test_item.getId() << "," << test_item.getName() << ","
                  << test_item.getPrice() << "," << test_item.getQuantity() << "\n";
        save_file.close();
        std::cout << "Test item saved to " << save_file_name << std::endl;
    } else {
        std::cerr << "Error: Unable to open save file for writing" << std::endl;
    }

    // create a new item and prompt user for information before saving to file
    Item new_item;
    int id;
    std::string name;
    double price;
    int quantity;
    std::cout << "Enter item ID: ";
    std::cin >> id;
    std::cin.ignore(); // clear the newline character from the input buffer
    std::cout << "Enter item name: ";
    std::getline(std::cin, name);
    std::cout << "Enter item price: ";
    std::cin >> price;
    std::cout << "Enter item quantity: ";
    std::cin >> quantity;
    new_item.init(id, name, price, quantity);
    std::cout << "New Item: ";
    new_item.display();
    
    // append to the same save file
    save_file.open(save_directory + "/" + save_file_name, std::ios::app);
    if(save_file.is_open()) {
        save_file << new_item.getId() << "," << new_item.getName() << ","
                  << new_item.getPrice() << "," << new_item.getQuantity() << "\n";
        save_file.close();
        std::cout << "Test item saved to " << save_file_name << std::endl;
    } else {
        std::cerr << "Error: Unable to open save file for writing" << std::endl;
    }

    return 0;
}