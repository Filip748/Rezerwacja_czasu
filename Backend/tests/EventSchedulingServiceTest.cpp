#include <gtest/gtest.h>
#include "domain/Event.hpp"
#include "domain/User.hpp"
#include "domain/TimeSlot.hpp"
#include "service/EventSchedulingService.hpp"
#include "service/NotificationService.hpp"

// Mock serwisu powiadomień do testowania
class MockNotificationService : public NotificationService {
public:
    bool pushSent = false;
    bool tieAlertSent = false;
    bool proceedOrCancelSent = false;

    void sendPushNotificationAboutFinalDate(const Event& event) override { pushSent = true; }
    void alertOrganizerAboutTie(const User& organizer, const std::vector<std::shared_ptr<TimeSlot>>& tiedSlots) override { tieAlertSent = true; }
    void askOrganizerToProceedOrCancel(const Event& event) override { proceedOrCancelSent = true; }
};

// TEST 1: Idealne dopasowanie (Happy Path)
TEST(EventSchedulingTest, WybieraNajlepszyTerminGdyJestKworum) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Test Event", 3, organizer); 

    auto slot1 = std::make_shared<TimeSlot>("t1", "01/01 10:00", "01/01 12:00");
    auto slot2 = std::make_shared<TimeSlot>("t2", "02/01 10:00", "02/01 12:00");
    myEvent.addProposedSlot(slot1);
    myEvent.addProposedSlot(slot2);

    for(int i = 0; i < 4; i++) slot1->addVote(); 
    for(int i = 0; i < 1; i++) slot2->addVote();

    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    
    EXPECT_EQ(myEvent.getStatus(), "ZATWIERDZONE");
    EXPECT_EQ(myEvent.getFinalSlot()->getId(), "t1");
    EXPECT_TRUE(mockNotifier->pushSent); 
}

// TEST 2: Brak kworum
TEST(EventSchedulingTest, OznaczaJakoWymagaUwagiPrzyBrakuKworum) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Test Event", 10, organizer); 

    auto slot1 = std::make_shared<TimeSlot>("t1", "01/01 10:00", "01/01 12:00");
    myEvent.addProposedSlot(slot1);

    for(int i = 0; i < 5; i++) slot1->addVote();

    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    EXPECT_EQ(myEvent.getStatus(), "WYMAGA_UWAGI");
    EXPECT_TRUE(mockNotifier->proceedOrCancelSent); 
}

// TEST 3: Remis (Scenariusz 2 z kryteriów akceptacji)
TEST(EventSchedulingTest, ZglaszaRemisGdyDwaTerminyMajaTyleSamoGlosow) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Wydarzenie Remisowe", 2, organizer);

    auto slot1 = std::make_shared<TimeSlot>("t1", "01/01 10:00", "01/01 12:00");
    auto slot2 = std::make_shared<TimeSlot>("t2", "02/01 10:00", "02/01 12:00");
    myEvent.addProposedSlot(slot1);
    myEvent.addProposedSlot(slot2);

    for(int i = 0; i < 3; i++) {
        slot1->addVote();
        slot2->addVote();
    }

    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    EXPECT_EQ(myEvent.getStatus(), "OCZEKUJE_NA_ORGANIZATORA");
    EXPECT_TRUE(mockNotifier->tieAlertSent);
    EXPECT_FALSE(mockNotifier->pushSent); 
}

// TEST 4: Przypadek brzegowy - Brak jakichkolwiek terminów w wydarzeniu
TEST(EventSchedulingTest, IgnorujeZamykanieGdyBrakProponowanychTerminow) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Puste Wydarzenie", 2, organizer);
    

    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    EXPECT_EQ(myEvent.getStatus(), "VOTING_IN_PROGRESS");
    EXPECT_FALSE(mockNotifier->pushSent);
    EXPECT_FALSE(mockNotifier->tieAlertSent);
    EXPECT_FALSE(mockNotifier->proceedOrCancelSent);
}

// TEST 5: Przypadek brzegowy - Kworum spełnione "na styk"
TEST(EventSchedulingTest, AkceptujeTerminGdyKworumJestDokladnieSpelnione) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Wydarzenie na styk", 3, organizer);

    auto slot1 = std::make_shared<TimeSlot>("t1", "01/01 10:00", "01/01 12:00");
    myEvent.addProposedSlot(slot1);

    for(int i = 0; i < 3; i++) slot1->addVote();

    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    EXPECT_EQ(myEvent.getStatus(), "ZATWIERDZONE");
    EXPECT_TRUE(mockNotifier->pushSent);
    EXPECT_FALSE(mockNotifier->proceedOrCancelSent);
}

// TEST 6: Nikt nie oddał głosu
TEST(EventSchedulingTest, ZglaszaBrakKworumGdyNiktNieOddalGlosu) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Wydarzenie bez glosow", 2, organizer);

    auto slot1 = std::make_shared<TimeSlot>("t1", "01/01 10:00", "01/01 12:00");
    auto slot2 = std::make_shared<TimeSlot>("t2", "02/01 10:00", "02/01 12:00");
    myEvent.addProposedSlot(slot1);
    myEvent.addProposedSlot(slot2);


    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    EXPECT_EQ(myEvent.getStatus(), "WYMAGA_UWAGI");
    EXPECT_TRUE(mockNotifier->proceedOrCancelSent);
    EXPECT_FALSE(mockNotifier->pushSent);
}

// TEST 7: Niezależność od kolejności dodawania
TEST(EventSchedulingTest, WybieraPoprawnyTerminNiezaleznieOdKolejnosciDodania) {
    auto organizer = std::make_shared<User>("1", "Organizator");
    Event myEvent("e1", "Test Kolejnosci", 1, organizer);

    auto slot1 = std::make_shared<TimeSlot>("t1", "01/01", "01/01");
    auto slot2 = std::make_shared<TimeSlot>("t2", "02/01", "02/01");
    auto slot3 = std::make_shared<TimeSlot>("t3", "03/01", "03/01");

    myEvent.addProposedSlot(slot1);
    myEvent.addProposedSlot(slot2);
    myEvent.addProposedSlot(slot3);

    slot1->addVote();
    slot2->addVote();
    slot3->addVote();
    slot3->addVote();

    auto mockNotifier = std::make_shared<MockNotificationService>();
    EventSchedulingService service(mockNotifier);

    service.processVotingResults(myEvent);

    EXPECT_EQ(myEvent.getStatus(), "ZATWIERDZONE");
    EXPECT_EQ(myEvent.getFinalSlot()->getId(), "t3");
}