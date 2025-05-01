#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <optional>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <random>

struct Location {
    double latitude;
    double longitude;
};

struct Driver {
    std::string id;
    Location currentLocation;
    bool isAvailable;
    std::string vehicleType;
    double rating;
    double cancelRate;
    int completedTrips;
    bool hasOngoingRide;
    std::time_t availableFrom;
    int currentPassengers;
    int maxPassengers;
};

struct RideRequest {
    Location pickup;
    std::string requiredVehicleType;
    double minDriverRating;
    std::time_t requestTime;
    std::unordered_set<std::string> blacklistedDrivers;
    bool isInSurgeZone;
    int passengers;
};

double calculateDistance(const Location& a, const Location& b) {
    double dx = a.latitude - b.latitude;
    double dy = a.longitude - b.longitude;
    return std::sqrt(dx * dx + dy * dy);
}

double estimateETA(const Location& from, const Location& to, double trafficMultiplier) {
    double dist = calculateDistance(from, to);
    return dist * 2.5 * trafficMultiplier;
}

double normalizedCancelScore(double cancelRate, int completedTrips) {
    if (completedTrips == 0) return 1.0;
    return cancelRate * std::min(1.0, 100.0 / completedTrips);
}

std::optional<Driver> matchDriver(const std::vector<Driver>& drivers, const RideRequest& request, double trafficMultiplier = 1.0) {
    std::vector<std::pair<Driver, double>> scoredDrivers;

    for (const auto& driver : drivers) {
        std::cout << "\nChecking driver " << driver.id << ": ";
        
        if (!driver.isAvailable) {
            std::cout << "[REJECTED] Not available";
            continue;
        }
        if (driver.hasOngoingRide) {
            std::cout << "[REJECTED] Has ongoing ride";
            continue;
        }
        if (request.blacklistedDrivers.count(driver.id)) {
            std::cout << "[REJECTED] Blacklisted";
            continue;
        }
        if (driver.vehicleType != request.requiredVehicleType) {
            std::cout << "[REJECTED] Vehicle type mismatch (" << driver.vehicleType << " != " << request.requiredVehicleType << ")";
            continue;
        }
        if (driver.rating < request.minDriverRating) {
            std::cout << "[REJECTED] Low rating (" << driver.rating << " < " << request.minDriverRating << ")";
            continue;
        }
        if (driver.currentPassengers + request.passengers > driver.maxPassengers) {
            std::cout << "[REJECTED] Capacity full (" << driver.currentPassengers << "+" << request.passengers << " > " << driver.maxPassengers << ")";
            continue;
        }
        
        std::cout << "[ACCEPTED] Meets all requirements";
    
        double distance = calculateDistance(driver.currentLocation, request.pickup);
        if (distance > 10.0) continue;

        double eta = estimateETA(driver.currentLocation, request.pickup, trafficMultiplier);
        double cancelScore = normalizedCancelScore(driver.cancelRate, driver.completedTrips);
        double surgePenalty = request.isInSurgeZone ? 1.2 : 1.0;

        double finalScore = eta * surgePenalty + cancelScore * 15.0 - driver.rating;

        scoredDrivers.emplace_back(driver, finalScore);
    }

    if (scoredDrivers.empty()) return std::nullopt;

    std::sort(scoredDrivers.begin(), scoredDrivers.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    std::vector<Driver> topCandidates;
    double bestScore = scoredDrivers.front().second;

    for (const auto& pair : scoredDrivers) {
        if (std::abs(pair.second - bestScore) < 0.01) {
            topCandidates.push_back(pair.first);
        } else {
            break;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, topCandidates.size() - 1);
    return topCandidates[dis(gen)];
}
void printRideDetails(double pickupLat, double pickupLon,
    const std::string& vehicleType, double minRating,
    bool isSurge, int passengerCount){

std::cout << "\n=== REQUEST DETAILS ==="
<< "\nPickup Location: " << pickupLat << ", " << pickupLon
<< "\nVehicle Type: " << vehicleType
<< "\nMinimum Driver Rating: " << minRating
<< "\nSurge Pricing: " << (isSurge ? "Yes" : "No")
<< "\nPassenger Count: " << passengerCount;
std::cout << "\n====================\n";
}


int main() {
    std::vector<Driver> drivers = {
        {"D1", {41.0, 29.0}, true, "standard", 4.8, 0.05, 200, false, std::time(nullptr) - 60, 0, 4},
        {"D2", {41.005, 29.005}, true, "standard", 4.5, 0.2, 50, false, std::time(nullptr) - 120, 1, 4},
        {"D3", {41.01, 29.01}, true, "premium", 5.0, 0.02, 300, false, std::time(nullptr), 0, 4},
        {"D4", {41.015, 29.015}, true, "standard", 4.9, 0.1, 150, false, std::time(nullptr) - 30, 0, 4},
        {"D5", {40.995, 28.995}, false, "premium", 4.7, 0.15, 80, true, std::time(nullptr) - 180, 2, 6},
        {"D6", {41.02, 29.02}, true, "standard", 3.9, 0.3, 20, false, std::time(nullptr) - 10, 0, 4}
    };

    double lat, lon;
    std::string vehicleType;
    double minRating;
    int passengerCount;
    std::string isSurgeInput;
    bool isSurge;

    std::cout << "Enter pickup latitude: ";
    std::cin >> lat;
    std::cout << "Enter pickup longitude: ";
    std::cin >> lon;
    std::cout << "Enter required vehicle type (standard/premium): ";
    std::cin >> vehicleType;
    std::cout << "Enter minimum driver rating: ";
    std::cin >> minRating;
    std::cout << "Enter number of passengers: ";
    std::cin >> passengerCount;
    std::cout << "Is this a surge zone? (yes/no): ";
    std::cin >> isSurgeInput;
    isSurge = (isSurgeInput == "yes");

    RideRequest request = {
        {lat, lon},
        vehicleType,
        minRating,
        std::time(nullptr),
        {},
        isSurge,
        passengerCount
    };
    printRideDetails(lat, lon, vehicleType,minRating,isSurge,passengerCount);
    auto match = matchDriver(drivers, request);
    if (match.has_value()) {
        std::cout << "\n\nMatched driver ID: " << match->id << std::endl;
    } else {
        std::cout << "No suitable driver found." << std::endl;
    }

    return 0;
}
