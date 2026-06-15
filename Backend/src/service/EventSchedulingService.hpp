#pragma once
#include <memory>
#include <vector>
#include "../domain/Event.hpp"
#include "NotificationService.hpp"

class EventSchedulingService {
private:
    std::shared_ptr<NotificationService> notificationService;

public:
    EventSchedulingService(std::shared_ptr<NotificationService> notificationService)
        : notificationService(notificationService) {}

    void processVotingResults(Event& event) {
        auto slots = event.getProposedSlots();
        if (slots.empty()) return;

        int maxVotes = -1;
        for (const auto& slot : slots) {
            if (slot->getVotes() > maxVotes) {
                maxVotes = slot->getVotes();
            }
        }

        // Scenariusz 3: Brak kworum
        if (maxVotes < event.getMinParticipants()) {
            event.setStatus("WYMAGA_UWAGI");
            notificationService->askOrganizerToProceedOrCancel(event);
            return;
        }

        std::vector<std::shared_ptr<TimeSlot>> topSlots;
        for (const auto& slot : slots) {
            if (slot->getVotes() == maxVotes) {
                topSlots.push_back(slot);
            }
        }

        // Scenariusz 2: Remis
        if (topSlots.size() > 1) {
            event.setStatus("OCZEKUJE_NA_ORGANIZATORA");
            notificationService->alertOrganizerAboutTie(*event.getOrganizer(), topSlots);
            return;
        }

        // Scenariusz 1: Idealne dopasowanie
        event.setFinalSlot(topSlots[0]);
        event.setStatus("ZATWIERDZONE");
        notificationService->sendPushNotificationAboutFinalDate(event);
    }
};