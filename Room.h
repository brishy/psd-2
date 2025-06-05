//Room.h
//(Your existing file comments)
#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Location.h"
#include "Item.h"
//#include "Passage.h" //REMOVED to break cycle
#include "Command.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

//Forward declaration for Passage
class Passage;

class Room : public Location {
public:
    //getExitDirections: returns a vector of strings representing available exit directions.
    std::vector<std::string> getExitDirections() const;
    //constructors
    Room(const std::string& name, const std::string& description); //basic constructor
    Room(const std::string& name, const std::string& description, std::shared_ptr<Command> enterCommand); //constructor with a specific enter command

    //item management methods
    void addItem(Item* item); //adds an item to the room's inventory
    Item* removeItem(const std::string& itemName); //removes an item by name from the room and returns it
    Item* getItem(const std::string& itemName) const; //retrieves an item by name from the room without removing it
    const std::vector<Item*>& getItems() const; //returns a const reference to the vector of items in the room

    //passage management methods
    void addPassage(const std::string& direction, std::shared_ptr<Passage> passage); //adds a passage leading from this room
    void removePassage(const std::string& direction); //removes a passage in a given direction
    std::shared_ptr<Passage> getPassage(const std::string& direction); //retrieves a passage in a given direction

protected:
    std::vector<Item*> items; //list of items currently in this room
    std::map<std::string, std::shared_ptr<Passage>> passageMap; //map of passages, keyed by direction string (e.g., "north")
    // std::vector<Character*> characters; //list of characters currently in this room (future expansion)
};

#endif //ZOORK_ROOM_H