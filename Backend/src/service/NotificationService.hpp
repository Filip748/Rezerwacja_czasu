#pragma once
#include <vector>
#include <memory>
#include "../domain/Event.hpp"
#include "../domain/User.hpp"
#include "../domain/TimeSlot.hpp"

class NotificationService {
public:
    virtual void sendPushNotificationAboutFinalDate(const Event& event) = 0;
    virtual void alertOrganizerAboutTie(const User& organizer, const std::vector<std::shared_ptr<TimeSlot>>& tiedSlots) = 0;
    virtual void askOrganizerToProceedOrCancel(const Event& event) = 0;
    
    virtual ~NotificationService() = default;
};