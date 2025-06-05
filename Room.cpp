//Room.cpp
//Implementation of the Room class methods.

#include "Room.h"
#include "Item.h" //Though Room.h includes Item.h, direct use of Item type might warrant it here.
#include "Passage.h" //Similar to Item.h.
#include "NullPassage.h" //Used in getPassage for non-existent exits.
#include "RoomDefaultEnterCommand.h" //For the default constructor.
#include <memory> //For std::shared_ptr and std::make_shared.
#include <iostream> //For the message in getPassage (can be removed if logging is handled elsewhere).
//No need for <algorithm> if using manual loops for find/remove.

//Constructor: basic version, sets a default enter command.
Room::Room(const std::string& name, const std::string& description)
    : Location(name, description) { //Initialize base class
    //Set the default command to be executed when entering this room.
    this->setEnterCommand(std::make_shared<RoomDefaultEnterCommand>(this));
    //items vector and passageMap are default-initialized (empty).
}

//Constructor: allows specifying a custom enter command.
Room::Room(const std::string& name, const std::string& description, std::shared_ptr<Command> enterCommand)
    : Location(name, description, enterCommand) { //Initialize base class with custom command
    //items vector and passageMap are default-initialized (empty).
}

//addItem: adds an item pointer to the room's list of items.
void Room::addItem(Item* item) {
    if (item != nullptr) { //Ensure item pointer is valid before adding.
        items.push_back(item);
    }
}

//removeItem: finds an item by name, removes it from the room, and returns it.
//Returns nullptr if the item is not found.
Item* Room::removeItem(const std::string& itemName) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        //Check if current item is not null and its name matches.
        if ((*it) != nullptr && (*it)->getName() == itemName) {
            Item* foundItem = *it; //Store pointer to the item.
            items.erase(it); //Remove item from the vector.
            return foundItem; //Return the removed item.
        }
    }
    return nullptr; //Item not found in the room.
}

//getItem: finds an item by name in the room and returns it without removing.
//Returns nullptr if the item is not found.
Item* Room::getItem(const std::string& itemName) const {
    for (const auto& item : items) {
        //Check if current item is not null and its name matches.
        if (item != nullptr && item->getName() == itemName) {
            return item; //Return the found item.
        }
    }
    return nullptr; //Item not found.
}

//getItems: returns a constant reference to the room's item list.
const std::vector<Item*>& Room::getItems() const {
    return items;
}

//addPassage: adds a passage to the room in a specific direction.
void Room::addPassage(const std::string& direction, std::shared_ptr<Passage> passage) {
    if (passage != nullptr) { //Ensure passage pointer is valid.
        passageMap[direction] = passage; //Add or update passage for the direction.
    }
}

//removePassage: removes a passage from the room for a specific direction.
void Room::removePassage(const std::string& direction) {
    //std::map::contains is C++20.
    //If not C++20, use: if (passageMap.find(direction) != passageMap.end())
    if (passageMap.contains(direction)) {
        passageMap.erase(direction); //Remove the passage if it exists.
    }
}

//getPassage: retrieves the passage in a given direction.
//Returns a NullPassage if no passage exists for that direction.
std::shared_ptr<Passage> Room::getPassage(const std::string& direction) {
    //std::map::contains is C++20.
    if (passageMap.contains(direction)) {
        return passageMap[direction]; //Return the existing passage.
    } else {
        //This message could be moved to ZOOrkEngine for cleaner class design,
        //but kept here to match original behavior if any.
        //std::cout << "It is impossible to go " << direction << "!\n";
        return std::make_shared<NullPassage>(this); //Return a safe, non-functional passage.
    }
}

//getExitDirections: returns a vector of strings representing available exit directions.
std::vector<std::string> Room::getExitDirections() const {
    std::vector<std::string> directions;
    for (auto const& [directionKey, passageVal] : passageMap) { //C++17 structured binding
        if (passageVal) { //Ensure the passage pointer is valid
             directions.push_back(directionKey);
        }
    }
    return directions;
}