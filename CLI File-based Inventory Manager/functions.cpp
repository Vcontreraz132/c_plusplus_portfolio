#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "functions.hpp"

int createSave(std::string save_directory) {
    // std::cerr << "No save files found" << std::endl;
    std::string save_file_name = "";

    // create new file
    std::cout << "Please create a new save file: ";
    std::getline(std::cin, save_file_name);

    // if save file does not end with .csv, append it
    if(save_file_name.find(".csv") == std::string::npos) {
        save_file_name += ".csv"; // append .csv to the save file name
    }
    // check if save file name is empty
    if(save_file_name.empty()) {
        std::cerr << "Save file name cannot be empty" << std::endl;
        return -1; // return error code
    }
    // check if save file name is valid
    if(save_file_name.find_first_of("/\\") != std::string::npos) {
        std::cerr << "Invalid save file name: " << save_file_name << std::endl;
        return -1; // return error code
    }
    // check if save file name is too long
    if(save_file_name.length() > 255) {
        std::cerr << "Save file name is too long: " << save_file_name << std::endl;
        return -1; // return error code
    }

    std::string full_directory = save_directory + "/" + save_file_name;

    std::ofstream new_save(full_directory);
    if(new_save.is_open()) {
        std::cout << "File created successfully" << std::endl;
        // Write header to the new save file
        new_save << "ID,Name,Price,Quantity\n"; // CSV header
        new_save.close();
    } else {

        std::cerr << "File error: Unable to open file" << std::endl;
    }
    
    return 0;
}

// change loadSave to return a vector of Item objects

std::vector<Item> loadSave(std::string full_directory) {
    std::vector<Item> allItems; // Vector to hold all items

    std::ifstream save_file(full_directory);
    if(save_file.is_open()) {

        std::string line;
        while(std::getline(save_file, line)) {
            // Process each line of the save file
            Item item; // Create an Item object to hold the data

            // Skip the header line
            if(line.find("ID,Name,Price,Quantity") != std::string::npos) {
                continue; // Skip the header line
            }

            //  validate csv line format
            if(line.empty() || line.find(',') == std::string::npos) {
                std::cerr << "Invalid line format: " << line << std::endl;
                continue; // Skip to the next line if the format is invalid
            } else if(std::count(line.begin(), line.end(), ',') < 3) { // count tokens
                std::cerr << "Insufficient data in line: " << line << std::endl;
                continue; // Skip to the next line if there are not enough commas
            }

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
    std::cin.get(); // Wait for user input before returning to the menu 
}

void addItem(std::vector<Item>& allItems, std::string full_directory) {
    Item newItem; // Create a new Item object
    int id;
    std::string name;
    double price;
    int quantity;

    // wrap user input in try / catch block to handle invalid input
    try {
        std::cout << "Enter Item ID: ";
        std::cin >> id;
        if(std::cin.fail()) {
            throw std::invalid_argument("Invalid ID input. Please enter a valid number.");
        }
        std::cin.ignore(); // Ignore the newline character left in the input buffer
        std::cout << "Enter Item Name: ";
        std::getline(std::cin, name);
        if(name.empty()) {
            throw std::invalid_argument("Item name cannot be empty.");
        }
        std::cout << "Enter Item Price: ";
        std::cin >> price;
        if(std::cin.fail() || price < 0) {
            throw std::invalid_argument("Invalid price input. Please enter a valid positive number.");
        }
        std::cin.ignore(); // Ignore the newline character left in the input buffer
        std::cout << "Enter Item Quantity: ";
        std::cin >> quantity;
        if(std::cin.fail() || quantity < 0) {
            throw std::invalid_argument("Invalid quantity input. Please enter a valid non-negative number.");
        }
    }
    catch(const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
        return; // Exit the function if input is invalid
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer after reading all inputs

    // Initialize the new item with the provided details
    newItem.init(id, name, price, quantity);
    allItems.push_back(newItem); // Add the new item to the vector
    std::cout << "Item added successfully!" << std::endl;
    newItem.display(); // Display the newly added item

    // add new item to save file
    std::ofstream save_file(full_directory, std::ios::app); // Open the file in append mode
    if(save_file.is_open()) {
        save_file << newItem.getId() << "," 
                  << newItem.getName() << ","
                  << newItem.getPrice() << ","
                  << newItem.getQuantity() << "\n"; // Write the item details to the file
        save_file.close(); // Close the file after writing
        std::cout << "Item saved to file successfully!" << std::endl;
    } else {
        std::cerr << "Error: Unable to open save file for writing." << std::endl;
    }
}

void removeItem(std::vector<Item>& allItems) {
    if(allItems.empty()) {
        std::cout << "No items to remove." << std::endl;
        return; // Exit if there are no items
    }

    std::string input;
    std::cout << "Enter the ID or name of the item to remove: ";
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer after reading the input

    // Find the item by ID or name
    auto it = std::remove_if(allItems.begin(), allItems.end(), [input](const Item& item) {
        return std::to_string(item.getId()) == input || item.getName() == input;
    });
    if(it != allItems.end()) {
        // Item found, remove it
        allItems.erase(it, allItems.end());
        std::cout << "Item removed successfully." << std::endl;
    } else {
        std::cout << "Item not found." << std::endl;
        return; // Exit if item not found
    }
}

void printModifyMenu() {
    std::cout << "Modify Item Options:" << std::endl;
    std::cout << "1. Change Name" << std::endl;
    std::cout << "2. Change Price" << std::endl;
    std::cout << "3. Change Quantity" << std::endl;
    std::cout << "4. Exit" << std::endl;
}

void modifyItem(std::vector<Item>& allItems) {
    // take in vector and prompt user for name or id of item to modify

    std::string input;
    std::cout << "Enter the ID or name of the item to modify: ";
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer to prevent newline issues
    auto it = std::find_if(allItems.begin(), allItems.end(), [input](const Item& item) { // mistakenly used std::remove_if
        return std::to_string(item.getId()) == input || item.getName() == input;
    });

    if(it != allItems.end()) {
        bool modifying = true;
        while(modifying) {
            printModifyMenu();
            std::string choice;
            std::cout << "Enter your choice: ";
            std::getline(std::cin, choice);

            switch(choice[0]) {
                case '1': { // change item name
                    std::string newName;
                    std::cout << "Enter new name: ";
                    std::getline(std::cin, newName);
                    it->setName(newName); // call setName of selected item
                    it->display(); // confirm that name has changed
                    std::cout << "Name changed successfully" << std::endl;
                    break;
                }

                case '2': { // change item price
                    double newPrice;
                    std::cout << "Enter new item price: ";
                    std::cin >> newPrice;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    it->setPrice(newPrice);
                    it->display(); // confirm that price has changed
                    std::cout << "Price changed successfully" << std::endl;
                    break;
                }

                case '3': { // change item quantity
                    int newQuantity;
                    std::cout << "Enter new item quantity: ";
                    std::cin >> newQuantity;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    it->setQuantity(newQuantity);
                    it->display();
                    std::cout << "Quantity changed successfully" << std::endl;
                    break;
                }

                case '4': {
                    std::cout << "Modifications complete" << std::endl;
                    modifying = false;
                    break;
                }

                default : {
                    std::cout << "Invalid entry" << std::endl;
                    return;
                }
            }
        }
    }
}