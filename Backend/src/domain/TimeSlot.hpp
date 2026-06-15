#pragma once
#include <string>

class TimeSlot {
private:
    std::string id;
    std::string startTime;
    std::string endTime;
    int votes;

public:
    TimeSlot(const std::string& id, const std::string& startTime, const std::string& endTime) 
        : id(id), startTime(startTime), endTime(endTime), votes(0) {}

    std::string getId() const { return id; }
    std::string getStartTime() const { return startTime; }
    std::string getEndTime() const { return endTime; }
    int getVotes() const { return votes; }

    void addVote() { votes++; }
};