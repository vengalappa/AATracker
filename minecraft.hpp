#include "os.hpp"
#include <windows.h>
#include <filesystem>
#include <iostream>
// #define DEBUG

std::string getWindowsDotMinecraftPath() {

    const std::string userHomePath = getenv("USERPROFILE");
    const std::string userAppDataPath = getenv("APPDATA");
    /*
    APPDATA C:\Users\<username>\AppData\Roaming.
    USERPROFILE C:\Users\<username>.
    LOCALAPPDATA  C:\Users\<username>\AppData\Local.
    */
    if (userAppDataPath.empty()) {
        std::cerr << "Couldn't find APPDATA environment variable!" << std::endl;
        exit(1);
    }
    #ifdef DEBUG
    std::cout << "Detected operating system: Windows" << std::endl;
    std::cout << "User Home Path: " << userHomePath << std::endl;
    std::cout << "APPDATA Path: " << userAppDataPath << std::endl;
    std::cout << ".minecraft path: " << userAppDataPath + "\\.minecraft" << std::endl;
    #endif

    return userAppDataPath + "\\.minecraft";
}

std::string getMacOSDotMinecraftPath() {

    const std::string userHomePath = getenv("HOME");
    if (userHomePath.empty()) {
        std::cerr << "Couldn't find HOME environment variable!" << std::endl;
        exit(1);
    }
    #ifdef DEBUG
    std::cout << "Detected operating system: MacOS" << std::endl;
    std::cout << "User Home Path: " << userHomePath << std::endl;
    std::cout << ".minecraft path: " << userHomePath + "/Library/Application Support/minecraft" << std::endl;
    #endif

    return userHomePath + "/Library/Application Support/minecraft";
}

std::string getLinuxDotMinecraftPath() {

    const std::string userHomePath = getenv("HOME");
    if (userHomePath.empty()) {
        std::cerr << "Couldn't find HOME environment variable!" << std::endl;
        exit(1);
    }
    #ifdef DEBUG
    std::cout << "Detected operating system: Linux" << std::endl;
    std::cout << "User Home Path: " << userHomePath << std::endl;
    std::cout << ".minecraft path: " << userHomePath + "/.minecraft" << std::endl;
    #endif

    return userHomePath + "/.minecraft";
}

std::filesystem::path getDotMinecraftPath() {

    const std::string operatingSystem = getPlatformName();
    std::string minecraftPath;
    if (operatingSystem == "Windows") {
        minecraftPath = getWindowsDotMinecraftPath();
    }
    else if (operatingSystem == "MacOS") {
        minecraftPath = getMacOSDotMinecraftPath();
    }
    else if (operatingSystem == "Linux") {
        minecraftPath = getLinuxDotMinecraftPath();
    }
    else {
        std::cerr << "Unsupported operating system: " << operatingSystem << std::endl;
        exit(1);
    }

    std::filesystem::path dotMinecraftPath(minecraftPath);

    if (!std::filesystem::exists(dotMinecraftPath)) {
        std::cerr << "Minecraft directory does not exist at: " << dotMinecraftPath << std::endl;
        exit(1);
    }
    if (!std::filesystem::is_directory(dotMinecraftPath)) {
        std::cerr << "Path is not a directory: " << dotMinecraftPath << std::endl;
        exit(1);
    }

    return dotMinecraftPath;
}

std::filesystem::path getSavesPath() {
    const std::filesystem::path dotMinecraftPath(getDotMinecraftPath());
    const std::filesystem::path savesPath = dotMinecraftPath / "saves";

    #ifdef DEBUG
    std::cout << "Saves path: " << savesPath << std::endl;
    #endif

    if (!std::filesystem::exists(savesPath)) {
        std::cerr << "Saves directory does not exist at: " << savesPath << std::endl;
        exit(1);
    }
    if (!std::filesystem::is_directory(savesPath)) {
        std::cerr << "Path to saves is not a directory: " << savesPath << std::endl;
        exit(1);
    }

    return savesPath;
}

std::filesystem::path getWorldPath(const std::string& worldName) {
    const std::filesystem::path savesPath = getSavesPath();
    const std::filesystem::path worldPath = savesPath / worldName;

    #ifdef DEBUG
    std::cout << "World path: " << worldPath << std::endl;
    #endif

    if (!std::filesystem::exists(worldPath)) {
        std::cerr << "World directory does not exist at: " << worldPath << std::endl;
        exit(1);
    }
    if (!std::filesystem::is_directory(worldPath)) {
        std::cerr << "Path to world is not a directory: " << worldPath << std::endl;
        exit(1);
    }

    return worldPath;
}

std::filesystem::path getWorldAdvancementsJSONFile(const std::string& worldName) {

    std::filesystem::path worldPath = getWorldPath(worldName);
    std::filesystem::path advancementsPath = worldPath / "advancements";

    for(auto const& entry : std::filesystem::directory_iterator(advancementsPath)) {
        if (entry.path().extension() == ".json") {
            #ifdef DEBUG
            std::cout << "Found advancements JSON file at: " << entry.path() << std::endl;
            #endif
            return entry.path();
        }
    }
    std::cerr << "No advancements JSON file found in world: " << worldName << std::endl;
    exit(1);
}