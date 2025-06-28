#include "json.hpp"
#include "minecraft.hpp"
#include "FileWatch.hpp"
#include <fstream>
#include <iostream>
#include <string>
using json = nlohmann::json;

std::string worldName;
json trackedAdvancementsData;
json worldAdvancementsData;
std::string trackedAdvancementsFile = "advancements.json";

void loadTrackedAdvancements() {

    try {
        trackedAdvancementsData = json::parse(std::ifstream(trackedAdvancementsFile));
        std::cout << "Tracked advancements loaded successfully." << std::endl;
    }
    catch (const json::parse_error& e) {
        std::cerr << "Error parsing tracked advancements JSON file: " << e.what() << std::endl;
        exit(1);
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred while loading tracked advancements: " << e.what() << std::endl;
        exit(1);
    }

}

void loadWorldAdvancements(const std::string& worldName) {

    std::string worldAdvancementsJSONFile = getWorldAdvancementsJSONFile(worldName);

    try {
        worldAdvancementsData = json::parse(std::ifstream(worldAdvancementsJSONFile));
        std::cout << "Loaded world advancements data successfully." << std::endl;
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
        exit(1);
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        exit(1);
    }

}

void trackAdvancements() {

    for (auto& advancement : trackedAdvancementsData.items()) {

        std::string key = advancement.key();
        if(worldAdvancementsData.contains(key)) {
            bool isCompleted = worldAdvancementsData[key]["done"];
            trackedAdvancementsData[key]["completed"] = isCompleted;
        }

    } 
}

void printCurrentStatus() {

    for(auto&advancement : trackedAdvancementsData.items()) {

        std::cout << advancement.key() << "\t" << advancement.value()["completed"] << std::endl;
    }

}

void monitorWorldAdvancementsFile(const std::string &worldAdvancementsJSONFile, const filewatch::Event change_type) {

    std::cout << "Monitoring World Advancements File of the World: " << worldName << std::endl;
    switch (change_type) {
        case filewatch::Event::removed:
            std::cout << "The file was removed from the directory" << std::endl;
            break;
        case filewatch::Event::modified:
            std::cout << "The file was modified" << std::endl;
            loadWorldAdvancements(worldName);
            trackAdvancements();
            printCurrentStatus();
            break;
        case filewatch::Event::renamed_old:
            std::cout << "The file was renamed (old name)" << std::endl;
            break;
        case filewatch::Event::renamed_new:
            std::cout << "The file was renamed (new name)" << std::endl;
            break;
    }

}


int main() {

    loadTrackedAdvancements();
    std::cout << "Enter the name of the Minecraft world: \n";
    getline(std::cin, worldName);
    std::cout<< "Entered World Name: " << worldName << std::endl;
    std::string worldAdvancementsJSONFile = getWorldAdvancementsJSONFile(worldName);
    loadWorldAdvancements(worldName);
    trackAdvancements();
    printCurrentStatus();

    auto watcher = std::make_unique<filewatch::FileWatch<std::string>>(worldAdvancementsJSONFile, monitorWorldAdvancementsFile);
    std::cin.get();
}
