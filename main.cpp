#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

struct HealthRecord {
    string date;
    string condition;
    string symptoms;
    string actions;
};

struct Plant {
    string name;
    string species;
    string location;
    string wateringFrequency;
    string lastWatered;
    vector<HealthRecord> healthHistory;
    string lastFertilized;
    string soilType;
    string potSize;
    bool needsRepotting;
    string nextWateringDate;
};

const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string BRIGHT_GREEN = "\033[92m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";
const string DIM = "\033[2m";

const string PLANT_HEADER = R"(
    /======================================\
    |           Plant Care System          |
    \======================================/
)";

const string PLANT_FOOTER = R"(
    /======================================\
    | Thank you for caring for your plants!|
    \======================================/
)";

const string SMALL_PLANT = R"(
      _\_
     \\ \\
     |// /
      \_/
)";


vector<Plant> plants;

// Plant-related functions
void addNewPlant();
void viewPlantHistory();
void updatePlant();
void deletePlant();
void recordHealthCheck();
void waterPlant();
void getCareInstructions();
void displayPlant(const Plant& plant);

// File I/O functions
void saveToFile();
void loadFromFile();

// Helper functions
void returnToMainMenu();
string getCurrentDate();
string calculateNextWateringDate(const string& frequency, const string& lastWatered);
void pauseProgram(int time);
void mainMenu();
void printBoxedText(const string& text, const string& color);
void printDivider();
void clearScreen();

int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << "Welcome to Plant Care System!\n\n";
    mainMenu();
    return 0;
}

void addNewPlant() {
    Plant newPlant;

    cout << "\n=== Add New Plant ===\n";
    cout << "Enter plant name: ";
    getline(cin, newPlant.name);

    cout << "Enter species (Succulent/Fern/Other): ";
    getline(cin, newPlant.species);

    cout << "Enter location in home: ";
    getline(cin, newPlant.location);

    cout << "Enter watering frequency (Daily/Weekly/Bi-weekly): ";
    getline(cin, newPlant.wateringFrequency);

    cout << "Enter soil type: ";
    getline(cin, newPlant.soilType);

    cout << "Enter pot size (inches): ";
    getline(cin, newPlant.potSize);

    newPlant.lastWatered = getCurrentDate();
    newPlant.nextWateringDate = calculateNextWateringDate(newPlant.wateringFrequency, newPlant.lastWatered);
    newPlant.lastFertilized = "Not yet fertilized";
    newPlant.needsRepotting = false;

    plants.push_back(newPlant);
    saveToFile();
    cout << "\nPlant added successfully!\n";
    returnToMainMenu();
}

void viewPlantHistory() {
    if (plants.empty()) {
        cout << "\nNo plants registered yet!\n";
        return;
    }

    cout << "\n=== View Plant History ===\n";
    cout << "Select plant:\n";
    for (int i = 0; i < plants.size(); i++) {
        cout << i + 1 << ". " << plants[i].name << endl;
    }

    int choice;
    cout << "Enter number: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > plants.size()) {
        cout << "Invalid choice!\n";
        return;
    }

    Plant& plant = plants[choice-1];
    displayPlant(plant);

    cout << "\nHealth History:\n";
    if (plant.healthHistory.empty()) {
        cout << "No health records yet.\n";
    } else {
        for (const HealthRecord& record : plant.healthHistory) {
            cout << "\nDate: " << record.date
                 << "\nCondition: " << record.condition
                 << "\nSymptoms: " << record.symptoms
                 << "\nActions: " << record.actions
                 << "\n-----------------" << endl;
        }
    }

    returnToMainMenu();
}


void updatePlant() {
    if (plants.empty()) {
        cout << "\nNo plants registered yet!\n";
        return;
    }

    cout << "\n=== Update Plant ===\n";
    cout << "Select plant to update:\n";
    for (int i = 0; i < plants.size(); i++) {
        cout << i + 1 << ". " << plants[i].name << endl;
    }

    int choice;
    cout << "Enter number: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > plants.size()) {
        cout << "Invalid choice!\n";
        return;
    }

    Plant& plant = plants[choice-1];
    displayPlant(plant);

    cout << "\nWhat would you like to update?\n";
    cout << "1. Name\n";
    cout << "2. Species\n";
    cout << "3. Location\n";
    cout << "4. Watering Frequency\n";
    cout << "5. Soil Type\n";
    cout << "6. Pot Size\n";
    cout << "7. Repotting Status\n";
    cout << "Choice: ";

    int updateChoice;
    cin >> updateChoice;
    cin.ignore();

    switch(updateChoice) {
        case 1:
            cout << "Enter new name: ";
            getline(cin, plant.name);
            break;
        case 2:
            cout << "Enter new species: ";
            getline(cin, plant.species);
            break;
        case 3:
            cout << "Enter new location: ";
            getline(cin, plant.location);
            break;
        case 4:
            cout << "Enter new watering frequency (Daily/Weekly/Bi-weekly): ";
            getline(cin, plant.wateringFrequency);
            plant.nextWateringDate = calculateNextWateringDate(plant.wateringFrequency, plant.lastWatered);
            break;
        case 5:
            cout << "Enter new soil type: ";
            getline(cin, plant.soilType);
            break;
        case 6:
            cout << "Enter new pot size: ";
            getline(cin, plant.potSize);
            break;
        case 7:
            cout << "Does plant need repotting? (y/n): ";
            char repot;
            cin >> repot;
            plant.needsRepotting = (repot == 'y' || repot == 'Y');
            break;
        default:
            cout << "Invalid choice!\n";
            return;
    }

    saveToFile();
    cout << "\nPlant updated successfully!\n";
    returnToMainMenu();
}

void deletePlant() {
    if (plants.empty()) {
        cout << "\nNo plants registered yet!\n";
        return;
    }

    cout << "\n=== Delete Plant ===\n";
    cout << "Select plant to delete:\n";
    for (int i = 0; i < plants.size(); i++) {
        cout << i + 1 << ". " << plants[i].name << endl;
    }

    int choice;
    cout << "Enter number: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > plants.size()) {
        cout << "Invalid choice!\n";
        return;
    }

    string plantName = plants[choice-1].name;
    plants.erase(plants.begin() + choice - 1);
    saveToFile();
    cout << "\n" << plantName << " has been deleted.\n";

    returnToMainMenu();
}

void recordHealthCheck() {
    if (plants.empty()) {
        cout << "\nNo plants registered yet!\n";
        return;
    }

    cout << "\n=== Record Health Check ===\n";
    cout << "Select plant:\n";
    for (int i = 0; i < plants.size(); i++) {
        cout << i + 1 << ". " << plants[i].name << endl;
    }

    int choice;
    cout << "Enter number: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > plants.size()) {
        cout << "Invalid choice!\n";
        return;
    }

    HealthRecord record;
    record.date = getCurrentDate();

    cout << "Enter condition (Healthy/Needs Attention/Critical): ";
    getline(cin, record.condition);

    cout << "Enter symptoms (or 'none'): ";
    getline(cin, record.symptoms);

    cout << "Enter actions taken: ";
    getline(cin, record.actions);

    cout << "Does the plant need repotting? (y/n): ";
    char repot;
    cin >> repot;
    plants[choice-1].needsRepotting = (repot == 'y' || repot == 'Y');

    plants[choice-1].healthHistory.push_back(record);
    saveToFile();
    cout << "\nHealth record added!\n";

    returnToMainMenu();

}

void waterPlant() {
    if (plants.empty()) {
        cout << "\nNo plants registered yet!\n";
        return;
    }

    cout << "\n=== Water Plant ===\n";
    cout << "Plants due for watering:\n";
    bool plantsNeedWatering = false;

    for (int i = 0; i < plants.size(); i++) {
        if (plants[i].nextWateringDate <= getCurrentDate()) {
            cout << i + 1 << ". " << plants[i].name << " (Last watered: " << plants[i].lastWatered << ")\n";
            plantsNeedWatering = true;
        }
    }

    if (!plantsNeedWatering) {
        cout << "No plants need watering at this time!\n";
        return;
    }

    int choice;
    cout << "Enter number (0 to cancel): ";
    cin >> choice;
    cin.ignore();

    if (choice == 0) return;
    if (choice < 1 || choice > plants.size()) {
        cout << "Invalid choice!\n";
        return;
    }

    plants[choice-1].lastWatered = getCurrentDate();
    plants[choice-1].nextWateringDate = calculateNextWateringDate(
        plants[choice-1].wateringFrequency,
        plants[choice-1].lastWatered
    );

    saveToFile();
    cout << "\nWatering recorded!\n";

    returnToMainMenu();
}

void getCareInstructions() {
    if (plants.empty()) {
        printBoxedText("No plants registered yet!", YELLOW);
        cout << "\nNo plants registered yet!\n";
        return;
    }

    cout << CYAN << "\n    Select plant:\n" << RESET;
    for (int i = 0; i < plants.size(); i++) {
        cout << GREEN << "    " << (i + 1) << ". " << RESET << plants[i].name << "\n";
    }

    cout << CYAN << "\n    Enter number: " << RESET;
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > plants.size()) {
        printBoxedText("Invalid choice!", RED);
        return;
    }

    string species = plants[choice-1].species;
    clearScreen();

    cout << BRIGHT_GREEN << SMALL_PLANT << RESET;
    printBoxedText("Care Guide for " + plants[choice-1].name + " (" + species + ")", CYAN + BOLD);

    if (species == "Succulent") {
        cout << YELLOW << "\n    🌞 LIGHT & TEMPERATURE\n" << RESET;
        cout << "    • Bright, direct sunlight (6+ hours daily)\n";
        cout << "    • Rotate pot for even growth\n";
        cout << "    • Prefer warm, dry conditions\n";

        cout << BLUE << "\n    💧 WATERING\n" << RESET;
        cout << "    • Water sparingly when soil is dry\n";
        cout << "    • Avoid getting leaves wet\n";
        cout << "    • Reduce watering in winter\n";

        cout << GREEN << "\n    🌱 SOIL & DRAINAGE\n" << RESET;
        cout << "    • Well-draining cactus/succulent mix\n";
        cout << "    • Add perlite or sand for drainage\n";
        cout << "    • Pot must have drainage holes\n";

        cout << RED << "\n    ⚠️ COMMON ISSUES\n" << RESET;
        cout << "    • Overwatering (yellowing, soft leaves)\n";
        cout << "    • Stretching (not enough light)\n";
        cout << "    • Root rot (black stems, mushy base)\n";
    } else if (species == "Fern") {
        cout << YELLOW << "\n    🌞 LIGHT & TEMPERATURE\n" << RESET;
        cout << "    • Indirect, filtered light\n";
        cout << "    • Avoid direct sunlight\n";
        cout << "    • Prefer cool, humid conditions\n";

        cout << BLUE << "\n    💧 WATERING\n" << RESET;
        cout << "    • Keep soil consistently moist\n";
        cout << "    • Mist leaves regularly\n";
        cout << "    • Never let soil dry completely\n";

        cout << GREEN << "\n    🌱 SOIL & DRAINAGE\n" << RESET;
        cout << "    • Rich, organic potting mix\n";
        cout << "    • Add peat moss for moisture\n";
        cout << "    • Good drainage essential\n";

        cout << RED << "\n    ⚠️ COMMON ISSUES\n" << RESET;
        cout << "    • Brown fronds (low humidity)\n";
        cout << "    • Yellowing (overwatering)\n";
        cout << "    • Crispy tips (dry air)\n";
    } else {
        cout << YELLOW << "\n    🌞 GENERAL PLANT CARE GUIDE\n" << RESET;
        cout << "    • Check light requirements for your specific plant\n";
        cout << "    • Most plants prefer indirect light\n";
        cout << "    • Maintain consistent temperature\n";

        cout << BLUE << "\n    💧 WATERING BASICS\n" << RESET;
        cout << "    • Check soil moisture before watering\n";
        cout << "    • Water thoroughly, then allow to drain\n";
        cout << "    • Adjust watering based on season\n";

        cout << GREEN << "\n    🌱 SOIL & POTTING\n" << RESET;
        cout << "    • Use appropriate potting mix\n";
        cout << "    • Ensure pot has drainage holes\n";
        cout << "    • Repot when roots outgrow container\n";

        cout << RED << "\n    ⚠️ GENERAL TROUBLESHOOTING\n" << RESET;
        cout << "    • Yellow leaves often indicate overwatering\n";
        cout << "    • Brown edges usually mean too dry\n";
        cout << "    • Check for pests regularly\n";
    }

    printDivider();
    cout << CYAN << "\n    Current Status:\n" << RESET;
    cout << "    • Next watering due: " << (plants[choice-1].nextWateringDate < getCurrentDate() ? RED : GREEN)
         << plants[choice-1].nextWateringDate << RESET << "\n";
    cout << "    • Current pot size: " << plants[choice-1].potSize << "\n";
    cout << "    • Soil type: " << plants[choice-1].soilType << "\n";

    if (plants[choice-1].needsRepotting) {
        printBoxedText("⚠️  This plant needs repotting!", YELLOW + BOLD);
    }
   returnToMainMenu();
}

void displayPlant(const Plant& plant) {
    cout << GREEN << SMALL_PLANT << RESET;
    printBoxedText("Plant Details", CYAN + BOLD);

    cout << CYAN << "\n    📋 Basic Information:\n" << RESET;
    cout << "    • Name: " << BOLD << plant.name << RESET << "\n";
    cout << "    • Species: " << BOLD << plant.species << RESET << "\n";
    cout << "    • Location: " << plant.location << "\n";

    cout << CYAN << "\n    💧 Watering Schedule:\n" << RESET;
    cout << "    • Frequency: " << plant.wateringFrequency << "\n";
    cout << "    • Last Watered: " << plant.lastWatered << "\n";
    cout << "    • Next Watering: " << BOLD;

    if (plant.nextWateringDate < getCurrentDate()) {
        cout << RED << plant.nextWateringDate << " (OVERDUE)" << RESET;
    } else {
        cout << GREEN << plant.nextWateringDate << RESET;
    }
    cout << "\n";

    cout << CYAN << "\n    🌱 Care Status:\n" << RESET;
    cout << "    • Soil Type: " << plant.soilType << "\n";
    cout << "    • Pot Size: " << plant.potSize << "\n";
    cout << "    • Needs Repotting: " << (plant.needsRepotting ? RED + BOLD + "Yes!" : GREEN + "No") << RESET << "\n";
    cout << "    • Last Fertilized: " << plant.lastFertilized << "\n";

    if (plant.nextWateringDate < getCurrentDate()) {
        printBoxedText("⚠️  WATERING ALERT: This plant needs watering!", RED + BOLD);
    }
}


// File I/O

void saveToFile() {
    ofstream file("plants.txt");
    if (file.is_open()) {
        for (const Plant& plant : plants) {
            file << "PLANT\n";
            file << plant.name << "\n";
            file << plant.species << "\n";
            file << plant.location << "\n";
            file << plant.wateringFrequency << "\n";
            file << plant.lastWatered << "\n";
            file << plant.lastFertilized << "\n";
            file << plant.soilType << "\n";
            file << plant.potSize << "\n";
            file << plant.needsRepotting << "\n";
            file << plant.nextWateringDate << "\n";

            file << "HEALTH_RECORDS\n";
            for (const HealthRecord& record : plant.healthHistory) {
                file << record.date << "\n";
                file << record.condition << "\n";
                file << record.symptoms << "\n";
                file << record.actions << "\n";
            }
            file << "END_HEALTH_RECORDS\n";
        }
        file.close();
    }
}

void loadFromFile() {
    ifstream file("plants.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line == "PLANT") {
                Plant plant;
                getline(file, plant.name);
                getline(file, plant.species);
                getline(file, plant.location);
                getline(file, plant.wateringFrequency);
                getline(file, plant.lastWatered);
                getline(file, plant.lastFertilized);
                getline(file, plant.soilType);
                getline(file, plant.potSize);
                string repotting;
                getline(file, repotting);
                plant.needsRepotting = (repotting == "1");
                getline(file, plant.nextWateringDate);

                getline(file, line); // HEALTH_RECORDS
                while (getline(file, line) && line != "END_HEALTH_RECORDS") {
                    HealthRecord record;
                    record.date = line;
                    getline(file, record.condition);
                    getline(file, record.symptoms);
                    getline(file, record.actions);
                    plant.healthHistory.push_back(record);
                }
                plants.push_back(plant);
            }
        }
        file.close();
    }
}

// Helper

void returnToMainMenu() {
    cout << "\nPress any key to exit...";
    cin.get();
    system("CLS");
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    stringstream ss;
    ss << setfill('0')
       << setw(4) << 1900 + ltm->tm_year << "-"
       << setw(2) << 1 + ltm->tm_mon << "-"
       << setw(2) << ltm->tm_mday;

    return ss.str();
}

string calculateNextWateringDate(const string& frequency, const string& lastWatered) {
    tm date = {};
    stringstream ss(lastWatered);
    string token;
    getline(ss, token, '-');
    date.tm_year = stoi(token) - 1900;
    getline(ss, token, '-');
    date.tm_mon = stoi(token) - 1;
    getline(ss, token, '-');
    date.tm_mday = stoi(token);

    if (frequency == "Daily") {
        date.tm_mday += 1;
    } else if (frequency == "Weekly") {
        date.tm_mday += 7;
    } else if (frequency == "Bi-weekly") {
        date.tm_mday += 14;
    }

    time_t next = mktime(&date);
    tm* nextDate = localtime(&next);

    stringstream result;
    result << setfill('0')
           << setw(4) << 1900 + nextDate->tm_year << "-"
           << setw(2) << 1 + nextDate->tm_mon << "-"
           << setw(2) << nextDate->tm_mday;

    return result.str();
}

void pauseProgram(int time) {
    Sleep(time);
    system("cls");
}

void mainMenu() {
    loadFromFile();

    while (true) {
        clearScreen();
        cout << BRIGHT_GREEN << PLANT_HEADER << RESET;

        // Check for alerts
        bool hasAlerts = false;
        for (const Plant& plant : plants) {
            if (plant.nextWateringDate < getCurrentDate() || plant.needsRepotting) {
                if (!hasAlerts) {
                    cout << RED << BOLD << "\n    🚨 ALERTS:\n" << RESET;
                    hasAlerts = true;
                }
                if (plant.nextWateringDate < getCurrentDate()) {
                    cout << RED << "    ▶ " << plant.name << " needs watering!\n" << RESET;
                }
                if (plant.needsRepotting) {
                    cout << YELLOW << "    ▶ " << plant.name << " needs repotting!\n" << RESET;
                }
            }
        }
        if (hasAlerts) printDivider();

        cout << CYAN << "\n    🌿 Main Menu:\n" << RESET;
        cout << GREEN << "    1. " << RESET << "Add New Plant\n";
        cout << GREEN << "    2. " << RESET << "View Plant History\n";
        cout << GREEN << "    3. " << RESET << "Update Plant\n";
        cout << GREEN << "    4. " << RESET << "Delete Plant\n";
        cout << GREEN << "    5. " << RESET << "Record Health Check\n";
        cout << GREEN << "    6. " << RESET << "Record Watering\n";
        cout << GREEN << "    7. " << RESET << "Get Care Instructions\n";
        cout << GREEN << "    8. " << RESET << "Exit\n";

        printDivider();
        cout << CYAN << "    Choice: " << RESET;

        int choice;
        cin >> choice;
        cin.ignore();

        clearScreen();

        try {
            switch (choice) {
                case 1:
                    printBoxedText("Add New Plant", MAGENTA + BOLD);
                    addNewPlant();
                    break;
                case 2:
                    printBoxedText("View Plant History", MAGENTA + BOLD);
                    viewPlantHistory();
                    break;
                case 3:
                    printBoxedText("Update Plant", MAGENTA + BOLD);
                    updatePlant();
                    break;
                case 4:
                    printBoxedText("Delete Plant", MAGENTA + BOLD);
                    deletePlant();
                    break;
                case 5:
                    printBoxedText("Record Health Check", MAGENTA + BOLD);
                    recordHealthCheck();
                    break;
                case 6:
                    printBoxedText("Record Watering", MAGENTA + BOLD);
                    waterPlant();
                    break;
                case 7:
                    printBoxedText("Care Instructions", MAGENTA + BOLD);
                    getCareInstructions();
                    break;
                case 8:
                    saveToFile();
                    cout << BRIGHT_GREEN << PLANT_FOOTER << RESET;
                    return;
                default:
                    printBoxedText("Invalid choice! Please try again.", RED);
            }
        } catch (const exception& e) {
            printBoxedText(string("Error: ") + e.what(), RED + BOLD);
            cout << "\n    Press Enter to continue...";
            cin.get();
        }
    }
}


void printBoxedText(const string& text, const string& color) {
    int width = text.length() + 4;
    cout << color;
    cout << "    +" << string(width - 2, '-') << "+\n";
    cout << "    | " << text << " |\n";
    cout << "    +" << string(width - 2, '-') << "+\n";
    cout << RESET;
}


void printDivider() {
    cout << DIM << "    ────────────────────────────────────────\n" << RESET;
}

void clearScreen() {
    system("cls");
}
