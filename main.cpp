// main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
//#include "StateClimate.h"

class StateClimate {
    private:
        int fips;
        int year;
        double temp;
        double tempc;
    
    public:
        // Constructor
        StateClimate(int f, int y, double t, double tc);
    
        // Getters
        int getFips() const;
        int getYear() const;
        double getTemp() const;
        double getTempC() const;
    
        // Setters
        void setFips(int f);
        void setYear(int y);
        void setTemp(double t);
        void setTempC(double tc);
    
        // Display function
        void display() const;
};

StateClimate::StateClimate(int f, int y, double t, double tc) : fips(f), year(y), temp(t), tempc(tc) {}

// Getters
int StateClimate::getFips() const { return fips; }
int StateClimate::getYear() const { return year; }
double StateClimate::getTemp() const { return temp; }
double StateClimate::getTempC() const { return tempc; }

// Setters
void StateClimate::setFips(int f) { fips = f; }
void StateClimate::setYear(int y) { year = y; }
void StateClimate::setTemp(double t) { temp = t; }
void StateClimate::setTempC(double tc) { tempc = tc; }

// Display function
void StateClimate::display() const {
    std::cout << "FIPS: " << fips << ", Year: " << year 
              << ", Temp (F): " << temp << ", Temp (C): " << tempc << std::endl;
}

int main() {
    std::vector<StateClimate> climateData;
    std::ifstream file("climdiv_state_year.csv");
    
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int fips, year;
        double temp, tempc;
        char comma;

        ss >> fips >> comma >> year >> comma >> temp >> comma >> tempc;
        climateData.emplace_back(fips, year, temp, tempc);
    }

    file.close();

    std::sort(climateData.begin(), climateData.end(),[](const StateClimate &a, const StateClimate &b){
        return a.getFips() < b.getFips();
    });

    // Display data
    //for (const auto &entry : climateData) {
    //    entry.display();
    //}

    // ===== ADD: prompt user =====
    std::cout << "Enter a state name: ";
    std::string state;
    std::getline(std::cin, state);

    std::map<std::string,int> stateToFips = {
        {"Alabama", 1},
        {"Alaska", 2},
        {"Arizona", 4},
        {"Arkansas", 5},
        {"California", 6},
        {"Colorado", 8},
        {"Connecticut", 9},
        {"Delaware", 10},
        {"Florida", 12},
        {"Georgia", 13},
        {"Hawaii", 15},
        {"Idaho", 16},
        {"Illinois", 17},
        {"Indiana", 18},
        {"Iowa", 19},
        {"Kansas", 20},
        {"Kentucky", 21},
        {"Louisiana", 22},
        {"Maine", 23},
        {"Maryland", 24},
        {"Massachusetts", 25},
        {"Michigan", 26},
        {"Minnesota", 27},
        {"Mississippi", 28},
        {"Missouri", 29},
        {"Montana", 30},
        {"Nebraska", 31},
        {"Nevada", 32},
        {"New Hampshire", 33},
        {"New Jersey", 34},
        {"New Mexico", 35},
        {"New York", 36},
        {"North Carolina", 37},
        {"North Dakota", 38},
        {"Ohio", 39},
        {"Oklahoma", 40},
        {"Oregon", 41},
        {"Pennsylvania", 42},
        {"Rhode Island", 44},
        {"South Carolina", 45},
        {"South Dakota", 46},
        {"Tennessee", 47},
        {"Texas", 48},
        {"Utah", 49},
        {"Vermont", 50},
        {"Virginia", 51},
        {"Washington", 53},
        {"West Virginia", 54},
        {"Wisconsin", 55},
        {"Wyoming", 56}
    };
    

    auto itMap = stateToFips.find(state);
    if (itMap == stateToFips.end()) {
        std::cout << "Unknown state: " << state << std::endl;
        return 0;
    }
    int targetFips = itMap->second;

    // ===== ADD: binary search for range of matching FIPS =====
    auto lower = std::lower_bound(
        climateData.begin(), climateData.end(), targetFips,
        [](const StateClimate &sc, int value) {
            return sc.getFips() < value;
        });
    if (lower == climateData.end() || lower->getFips() != targetFips) {
        std::cout << "No climate data for " << state << std::endl;
        return 0;
    }
    auto upper = std::upper_bound(
        climateData.begin(), climateData.end(), targetFips,
        [](int value, const StateClimate &sc) {
            return value < sc.getFips();
        });

    // ===== ADD: display the found records =====
    std::cout << "Climate data for " << state << " (FIPS " << targetFips << "):" << std::endl;
    for (auto it = lower; it != upper; ++it) {
        it->display();
    }

    return 0;
}