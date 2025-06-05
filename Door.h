//Door.h
//Defines the Door class, a special type of Passage that can be blocked
//and may require a specific item or action to open.

#ifndef ZOORK_DOOR_H //Include guard
#define ZOORK_DOOR_H

#include "Passage.h" //Door inherits from Passage
//Item.h is not strictly needed in Door.h if tryOpen only takes itemName as string
//but Player.h will be needed in Door.cpp
#include <string>   //For names, item names
#include <memory>   //For std::shared_ptr

//Forward declaration for Player, as tryOpen uses Player*
class Player;
//class Item; //Not strictly needed here if tryOpen only deals with itemName string

class Door : public Passage {
public:
    //Constructor for a Door.
    //keyItemName: the name of the item required to open/unlock this door.
    //blockedDescription: text shown when trying to pass a closed/locked door.
    Door(const std::string& name, const std::string& description,
         Room* from, Room* to,
         const std::string& keyItemName, const std::string& blockedDescription);

    //Constructor with a custom enter command (less common for doors needing keys).
    Door(const std::string& name, const std::string& description,
         std::shared_ptr<Command> enterCommand, Room* from, Room* to,
         const std::string& keyItemName, const std::string& blockedDescription);

    //isOpen: checks if the door is currently open.
    bool isOpen() const;

    //tryOpen: attempts to open the door, typically with an item from the player.
    //Returns true if successfully opened, false otherwise.
    //ConsumesKey: if true and open is successful, the key item might be removed from player.
    bool tryOpen(Player* player, const std::string& itemName, bool consumesKey = false);

    //getBlockedDescription: returns the message displayed when the door is blocked.
    std::string getBlockedDescription() const;

private:
    bool isOpen_; //Tracks whether the door is open or closed.
    std::string keyItemName_; //Name of the item required to open this door.
    std::string blockedDescription_; //Description when passage is blocked.
};

#endif //ZOORK_DOOR_H