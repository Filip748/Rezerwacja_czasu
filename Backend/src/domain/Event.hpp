#pragma once
#include <string>
#include <vector>
#include <memory>
#include "User.hpp"
#include "TimeSlot.hpp"

class Event {
private:
    std::string id;
    std::string name;
    int minParticipants;
    std::shared_ptr<User> organizer;
    std::vector<std::shared_ptr<TimeSlot>> proposedSlots;
    std::shared_ptr<TimeSlot> finalSlot;
    std::string status;

public:
    Event(const std::string& id, const std::string& name, int minParticipants, std::shared_ptr<User> organizer)
        : id(id), name(name), minParticipants(minParticipants), organizer(organizer), status("VOTING_IN_PROGRESS"), finalSlot(nullptr) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    int getMinParticipants() const { return minParticipants; }
    std::shared_ptr<User> getOrganizer() const { return organizer; }
    std::vector<std::shared_ptr<TimeSlot>> getProposedSlots() const { return proposedSlots; }
    std::shared_ptr<TimeSlot> getFinalSlot() const { return finalSlot; }
    std::string getStatus() const { return status; }

    void setStatus(const std::string& newStatus) { status = newStatus; }
    void setFinalSlot(std::shared_ptr<TimeSlot> slot) { finalSlot = slot; }
    void addProposedSlot(std::shared_ptr<TimeSlot> slot) { proposedSlots.push_back(slot); }
};