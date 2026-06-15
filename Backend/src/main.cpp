#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <vector>
#include "domain/User.hpp"
#include "domain/TimeSlot.hpp"
#include "domain/Event.hpp"
#include "service/NotificationService.hpp"
#include "service/EventSchedulingService.hpp"

class ConsoleNotificationService : public NotificationService {
public:
    void sendPushNotificationAboutFinalDate(const Event& event) override {
        std::cout << "\n========================================\n";
        std::cout << "[SUKCES - PUSH NOTIFICATION]\n";
        std::cout << "Termin wydarzenia '" << event.getName() << "' zostal ustalony na: " 
                  << event.getFinalSlot()->getStartTime() << "!\n";
        std::cout << "Wysylam powiadomienia do uczestnikow...\n";
        std::cout << "========================================\n";
    }

    void alertOrganizerAboutTie(const User& organizer, const std::vector<std::shared_ptr<TimeSlot>>& tiedSlots) override {
        std::cout << "\n========================================\n";
        std::cout << "[REMIS - WYMAGANA AKCJA]\n";
        std::cout << organizer.getUsername() << ", powstal remis! Wybierz ostateczny termin z ponizszych:\n";
        for (const auto& slot : tiedSlots) {
            std::cout << " - " << slot->getStartTime() << " (glosow: " << slot->getVotes() << ")\n";
        }
        std::cout << "========================================\n";
    }

    void askOrganizerToProceedOrCancel(const Event& event) override {
        std::cout << "\n========================================\n";
        std::cout << "[BRAK KWORUM - WYMAGANA AKCJA]\n";
        std::cout << "Dla wydarzenia '" << event.getName() << "' nie osiagnieto minimalnej liczby uczestnikow.\n";
        std::cout << "Wymagane: " << event.getMinParticipants() << "\n";
        std::cout << "========================================\n";
    }
};

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    auto notificationService = std::make_shared<ConsoleNotificationService>();
    EventSchedulingService schedulingService(notificationService);

    std::vector<std::shared_ptr<User>> usersDatabase = {
        std::make_shared<User>("u1", "Filip Sobieraj"),
        std::make_shared<User>("u2", "Pawel Wdowik"),
        std::make_shared<User>("u3", "Karolina Kolarz")
    };

    std::shared_ptr<User> loggedUser = nullptr;
    std::shared_ptr<Event> currentEvent = nullptr;

    int choice = -1;

    while (true) {
        if (!loggedUser) {
            std::cout << "\n=== EKRAN LOGOWANIA ===\n";
            std::cout << "Wybierz uzytkownika, na ktorego chcesz sie zalogowac:\n";
            for (size_t i = 0; i < usersDatabase.size(); ++i) {
                std::cout << i + 1 << ". " << usersDatabase[i]->getUsername() << "\n";
            }
            std::cout << "0. Wyjdz z aplikacji\n";
            std::cout << "Wybor: ";
            
            std::cin >> choice;
            if (std::cin.fail()) { clearInputBuffer(); continue; }

            if (choice == 0) {
                std::cout << "Zamykanie aplikacji...\n";
                return 0;
            } else if (choice > 0 && static_cast<size_t>(choice) <= usersDatabase.size()) {
                loggedUser = usersDatabase[choice - 1];
                std::cout << "\n-> Zalogowano pomyslnie jako: " << loggedUser->getUsername() << "\n";
            } else {
                std::cout << "-> Nieprawidlowy wybor. Sprobuj ponownie.\n";
            }
        } else {
            std::cout << "\n=== MENU: APLIKACJA SPOTKAJMNIE ===\n";
            std::cout << "Zalogowany uczestnik: " << loggedUser->getUsername() << "\n";
            std::cout << "-----------------------------------\n";
            std::cout << "1. Utworz nowe wydarzenie\n";
            std::cout << "2. Zaproponuj nowy termin\n";
            std::cout << "3. Zaglosuj na termin\n";
            std::cout << "4. Zamknij glosowanie (Uruchom algorytm systemu)\n";
            std::cout << "5. Wyswietl szczegoly wydarzenia\n";
            std::cout << "8. Wyloguj sie\n";
            std::cout << "0. Wyjdz z aplikacji\n";
            std::cout << "Wybor: ";
            
            std::cin >> choice;
            if (std::cin.fail()) { clearInputBuffer(); continue; }

            switch (choice) {
                case 1: {
                    std::string eventName;
                    int minParticipants;
                    std::cout << "\nPodaj nazwe wydarzenia (np. Pilka nozna): ";
                    clearInputBuffer();
                    std::getline(std::cin, eventName);
                    
                    std::cout << "Podaj minimalna liczbe uczestnikow do startu: ";
                    std::cin >> minParticipants;

                    // Tworca wydarzenia automatycznie staje sie zalogowany uzytkownik
                    currentEvent = std::make_shared<Event>("e1", eventName, minParticipants, loggedUser);
                    std::cout << "-> Utworzono wydarzenie: " << currentEvent->getName() << "\n";
                    break;
                }
                case 2: {
                    if (!currentEvent) {
                        std::cout << "-> Najpierw utworz wydarzenie (opcja 1)!\n";
                        break;
                    }
                    std::string startTime, endTime;
                    std::cout << "\nPodaj poczatek (np. Piatek 18:00): ";
                    clearInputBuffer();
                    std::getline(std::cin, startTime);
                    
                    std::cout << "Podaj koniec (np. Piatek 20:00): ";
                    std::getline(std::cin, endTime);

                    std::string slotId = "t" + std::to_string(currentEvent->getProposedSlots().size() + 1);
                    auto slot = std::make_shared<TimeSlot>(slotId, startTime, endTime);
                    currentEvent->addProposedSlot(slot);
                    std::cout << "-> Dodano termin do glosowania!\n";
                    break;
                }
                case 3: {
                    if (!currentEvent || currentEvent->getProposedSlots().empty()) {
                        std::cout << "-> Brak terminow! Dodaj je najpierw (opcja 2).\n";
                        break;
                    }
                    std::cout << "\nDostepne terminy do glosowania:\n";
                    auto slots = currentEvent->getProposedSlots();
                    for (size_t i = 0; i < slots.size(); ++i) {
                        std::cout << i + 1 << ". " << slots[i]->getStartTime() << " (obecne glosy: " << slots[i]->getVotes() << ")\n";
                    }
                    
                    int voteChoice;
                    std::cout << "Wybierz numer terminu, na ktory " << loggedUser->getUsername() << " chce oddac glos: ";
                    std::cin >> voteChoice;

                    if (voteChoice > 0 && static_cast<size_t>(voteChoice) <= slots.size()) {
                        slots[voteChoice - 1]->addVote();
                        std::cout << "-> Zarejestrowano glos!\n";
                    } else {
                        std::cout << "-> Nieprawidlowy wybor.\n";
                    }
                    break;
                }
                case 4: {
                    if (!currentEvent) {
                        std::cout << "-> Brak wydarzenia w pamieci!\n";
                        break;
                    }
                    std::cout << "\nZamykanie glosowania i przeliczanie wynikow...\n";
                    schedulingService.processVotingResults(*currentEvent);
                    break;
                }
                case 5: {
                    if (!currentEvent) {
                        std::cout << "-> Brak wydarzenia w pamieci!\n";
                        break;
                    }
                    std::cout << "\n--- SZCZEGOLY WYDARZENIA ---\n";
                    std::cout << "Nazwa: " << currentEvent->getName() << "\n";
                    std::cout << "Organizator: " << currentEvent->getOrganizer()->getUsername() << "\n";
                    std::cout << "Status: " << currentEvent->getStatus() << "\n";
                    std::cout << "Min. uczestnikow: " << currentEvent->getMinParticipants() << "\n";
                    
                    if (currentEvent->getFinalSlot()) {
                        std::cout << "Zwycieski termin: " << currentEvent->getFinalSlot()->getStartTime() << "\n";
                    } else {
                        std::cout << "Zwycieski termin: Jeszcze nie ustalony\n";
                    }

                    std::cout << "Proponowane terminy:\n";
                    for (const auto& slot : currentEvent->getProposedSlots()) {
                        std::cout << " - " << slot->getStartTime() << " | Glosow: " << slot->getVotes() << "\n";
                    }
                    break;
                }
                case 8: {
                    std::cout << "\n-> Wylogowano uzytkownika: " << loggedUser->getUsername() << "\n";
                    loggedUser = nullptr;
                    // Resetujemy wskaznik na wydarzenie, zeby nowy uzytkownik zaczal z czysta karta
                     
                    break;
                }
                case 0:
                    std::cout << "Zamykanie aplikacji...\n";
                    return 0;
                default:
                    std::cout << "Nieznana opcja. Sprobuj ponownie.\n";
                    break;
            }
        }
    }
}