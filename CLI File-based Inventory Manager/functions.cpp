#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

int createSave(std::string save_directory) {
    std::cerr << "No save files found" << std::endl;
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

int loadSave(std::string save_directory, std::string save_file_name) {
    std::string full_directory = save_directory + "/" + save_file_name;

    std::ifstream save_file(full_directory);
    if(save_file.is_open()) {
        std::cout << "File loaded successfully" << std::endl;
        // Load the file content into a vector or any other data structure as needed
        // For now, just close the file
        save_file.close();
    } else {
        std::cerr << "File error: Unable to open file" << std::endl;
    }

    return 0;
}

int scanSaveDirectory(std::string save_directory, std::string target_extension) {
    for(const auto& entry : std::filesystem::directory_iterator(save_directory)) {

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
    return 0;
}
