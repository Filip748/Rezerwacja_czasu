#pragma once
#include <string>

class User {
private:
    std::string id;
    std::string username;

public:
    User(const std::string& id, const std::string& username) 
        : id(id), username(username) {}

    std::string getId() const { return id; }
    std::string getUsername() const { return username; }
};