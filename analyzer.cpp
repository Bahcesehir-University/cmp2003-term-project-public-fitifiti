#include "analyzer.h"
#include <fstream>
#include <sstream>
#include <algorithm>


// Students may use ANY data structure internally

void TripAnalyzer::ingestFile(const std::string& csvPath) {
    // TODO:
    // - open file
    // - skip header
    // - skip malformed rows
    // - extract PickupZoneID and pickup hour
    // - aggregate counts


    std::ifstream file(csvPath);
    if (!file.is_open()) {
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;

        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // malformed row
        if (fields.size() < 6)
            continue;

        std::string zone = fields[1];
        std::string datetime = fields[3];

        if (zone.empty() || datetime.size() < 13)
            continue;

        int hour;
        try {
            hour = std::stoi(datetime.substr(11, 2));
        } catch (...) {
            continue;
        }

        if (hour < 0 || hour > 23)
            continue;

        zoneTripCounts[zone]++;
        zoneHourCounts[zone][hour]++;
    }

    file.close();
}

   




std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    // TODO:
    // - sort by count desc, zone asc
    // - return first k

    std::vector<ZoneCount> result;

    
    for (const auto& pair : zoneTripCounts) {
        ZoneCount z;
        z.zone = pair.first;
        z.count = pair.second;
        result.push_back(z);
    }

   
    std::sort(result.begin(), result.end(),
        [](const ZoneCount& a, const ZoneCount& b) {
            if (a.count != b.count)
                return a.count > b.count;
            return a.zone < b.zone;
        }
    );

    
    if ((int)result.size() > k)
        result.resize(k);

    return result;
}
 


std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    // TODO:
    // - sort by count desc, zone asc, hour asc
    // - return first k

     std::vector<SlotCount> result;

    
    for (const auto& zonePair : zoneHourCounts) {
        const std::string& zone = zonePair.first;
        const auto& hours = zonePair.second;

        for (int h = 0; h < 24; h++) {
            if (hours[h] > 0) {
                SlotCount s;
                s.zone = zone;
                s.hour = h;
                s.count = hours[h];
                result.push_back(s);
            }
        }
    }

    
    std::sort(result.begin(), result.end(),
        [](const SlotCount& a, const SlotCount& b) {
            if (a.count != b.count)
                return a.count > b.count;
            if (a.zone != b.zone)
                return a.zone < b.zone;
            return a.hour < b.hour;
        }
    );

    
    if ((int)result.size() > k)
        result.resize(k);

    return result;
}
