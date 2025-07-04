#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

int createSave(std::string save_directory) {
    std::cerr << "No save files found" << std::endl;
    std::string save_file_name = "";

    // create new file
    std::cout << "Please create a new save file :";
    std::getline(std::cin, save_file_name);

    // if(save_file_name does not contain .csv) {
    //     save_file_name = save_file_name + ".csv"; // ensure save file is .csv file
    // }

    std::string full_directory = save_directory + "/" + save_file_name;
    std::cout << full_directory << std::endl;

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
