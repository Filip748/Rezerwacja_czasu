# Optymalizacja Czasu Wolnego Dla Znajomych

System dedykowany grupom znajomych, mający na celu automatyzację procesu planowania wspólnych aktywności i eliminację chaosu komunikacyjnego.

---

## Wizja Projektu
Głównym celem aplikacji jest zastąpienie dziesiątek wiadomości na czatach jednym, inteligentnym systemem. Rozwiązanie analizuje dostępność użytkowników i wskazuje optymalne terminy spotkań, co pozwala zaoszczędzić czas i zwiększyć frekwencję podczas wspólnych wyjść.

### Kluczowe założenia:
* **Optymalizacja:** Algorytm sam wskazuje datę, w której może spotkać się najwięcej osób.
* **Wszechstronność:** Obsługa aktywności sportowych, towarzyskich oraz edukacyjnych.
* **Przejrzystość:** Wszystkie plany i statusy obecności dostępne w jednym widoku.

---

## Skład Zespołu Projektowego

| Członek Zespołu | Rola | Główne odpowiedzialności |
| :--- | :--- | :--- |
| **Filip Sobieraj** | Team Leader / Product Owner | Wizja produktu, koordynacja prac, zarządzanie backlogiem. |
| **Karolina Kolarz** | Fullstack Developer | Implementacja logiki biznesowej i algorytmów dopasowywania. |
| **Paweł Wdowik** | UI/UX / QA / Mobile Dev | Design interfejsu, testy jakościowe, rozwój aplikacji mobilnej. |

---

## Grupa Docelowa
Projekt realizowany jest przy współpracy z przedstawicielami **samorządów studenckich** oraz **koordynatorami amatorskich drużyn sportowych**, którzy na co dzień mierzą się z wyzwaniem synchronizacji grafików wielu osób.

---

## Historyjki Użytkownika (User Stories)

### Organizator (Inicjator)
* **Jako organizator**, chcę utworzyć wydarzenie i wybrać jego typ, aby znajomi znali charakter spotkania.
* **Jako organizator**, chcę zaproponować zakres dat, aby dać grupie pole do manewru.
* **Jako organizator**, chcę otrzymać powiadomienie o "zwycięskim terminie", aby dokonać rezerwacji.

### Uczestnik (Członek grupy)
* **Jako uczestnik**, chcę oznaczyć godziny zajętości, aby aplikacja automatycznie odrzucała nietrafione terminy.
* **Jako uczestnik**, chcę móc zaproponować alternatywną godzinę, gdy propozycje mi nie odpowiadają.
* **Jako uczestnik**, chcę widzieć listę obecności, aby wiedzieć, czy zbierze się wystarczający skład.

### System (Automatyzacja)
* **Jako system**, chcę porównać dostępność wszystkich osób i wskazać przedział z najwyższą frekwencją.
* **Jako system**, chcę wysyłać przypomnienia o nadchodzącym spotkaniu do wszystkich potwierdzonych osób.

---

## Główne Korzyści
1.  **Oszczędność czasu:** Decyzja o terminie zapada w kilka minut, a nie godzin.
2.  **Wyższa frekwencja:** Dzięki dopasowaniu do realnych grafików użytkowników.
3.  **Brak spamu:** Koniec z setkami powiadomień z czatów grupowych dotyczących ustalania godziny.