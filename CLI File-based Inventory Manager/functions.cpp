#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "functions.hpp"

int createSave(std::string save_directory) {
    // std::cerr << "No save files found" << std::endl;
    std::string save_file_name = "";

    // create new file
    std::cout << "Please create a new save file: ";
    std::getline(std::cin, save_file_name);

    // if(save_file_name does not contain .csv) {
    //     save_file_name = save_file_name + ".csv"; // ensure save file is .csv file
    // }

    std::string full_directory = save_directory + "/" + save_file_name;
    // std::cout << full_directory << std::endl;

    std::ofstream new_save(full_directory);
    if(new_save.is_open()) {

        std::cout << "File created successfully" << std::endl;
        new_save << "";
        new_save.close();
    } else {

        std::cerr << "File error: Unable to open file" << std::endl;
    }
    
    return 0;
}

// change loadSave to return a vector of Item objects

std::vector<Item> loadSave(std::string save_directory, std::string save_file_name) {
    std::string full_directory = save_directory + "/" + save_file_name;
    std::vector<Item> allItems; // Vector to hold all items

    std::ifstream save_file(full_directory);
    if(save_file.is_open()) {

        std::string line;
        while(std::getline(save_file, line)) {
            // Process each line of the save file
            Item item; // Create an Item object to hold the data

            std::stringstream ss(line);
            std::string item_id, item_name, item_price, item_quantity;
            std::getline(ss, item_id, ',');
            std::getline(ss, item_name, ',');
            std::getline(ss, item_price, ',');
            std::getline(ss, item_quantity, ',');

            item.init(std::stoi(item_id), item_name, std::stod(item_price), std::stoi(item_quantity));
            allItems.push_back(item); // Add the item to the vector
        }
        // after loading all items, close the file
        save_file.close();

        std::cout << "File loaded successfully" << std::endl;
    } else {
        std::cerr << "File error: Unable to open file" << std::endl;
    }

    return allItems; // Return the vector of items
}

bool scanSaveDirectory(std::string save_directory, std::string target_extension) {
    bool save_file_exists = false; // flag to check if save file exists
    for(const auto& entry : std::filesystem::directory_iterator(save_directory)) {

        // Check if the entry is a regular file
        if(std::filesystem::is_regular_file(entry.status())) {
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
    return save_file_exists; // return true if at least one valid save file exists
}


// flowchart: print menu options; subroutine B: add item remove item modify item search, show all
void printMenu() {
    std::cout << "Menu Options:" << std::endl;
    std::cout << "1. Add Item" << std::endl;
    std::cout << "2. Remove Item" << std::endl;
    std::cout << "3. Modify Item" << std::endl;
    std::cout << "4. Search Item" << std::endl;
    std::cout << "5. Show All Items" << std::endl;
    std::cout << "6. Exit" << std::endl;
}

void showAllItems(const std::vector<Item>& allItems) {
    std::cout << "All Items:" << std::endl;
    for(const auto& item : allItems) {
        item.display(); // Display each item
    }
}