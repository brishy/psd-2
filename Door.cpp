//Door.cpp
//Implementation of the Door class methods.

#include "Door.h"
#include "Player.h" //Needed for tryOpen to check player's inventory
#include "Item.h"   //Needed for tryOpen to interact with items
#include "Room.h"   //Included via Passage.h or Door.h, but good to be explicit if directly using Room types
#include <iostream> //For potential debug messages, though not strictly used in this impl

//Constructor: basic version for a door requiring a key.
Door::Door(const std::string& name, const std::string& description,
           Room* from, Room* to,
           const std::string& keyItemName, const std::string& blockedDescription)
    : Passage(name, description, from, to), //Initialize base Passage class
      isOpen_(false), //Doors start closed by default
      keyItemName_(keyItemName),
      blockedDescription_(blockedDescription) {
    //Specific Door initialization can go here if needed
}

//Constructor: with a custom enter command (less common for key-based doors).
Door::Door(const std::string& name, const std::string& description,
           std::shared_ptr<Command> enterCommand, Room* from, Room* to,
           const std::string& keyItemName, const std::string& blockedDescription)
    : Passage(name, description, enterCommand, from, to), //Initialize base Passage class
      isOpen_(false), //Doors start closed
      keyItemName_(keyItemName),
      blockedDescription_(blockedDescription) {
    //Specific Door initialization
}

//isOpen: returns the current state of the door.
bool Door::isOpen() const {
    return isOpen_;
}

//tryOpen: attempts to open the door using an item from the player's inventory.
//Returns true if the door was successfully opened, false otherwise.
bool Door::tryOpen(Player* player, const std::string& itemName, bool consumesKey) {
    if (isOpen_) { //If door is already open
        return true; //No action needed, already open
    }

    if (!player) { //Safety check for player pointer
        return false;
    }

    Item* key = player->getItem(itemName); //Check if player has the item specified by itemName

    //Check if the item player is trying to use is the correct key for *this* door
    if (key != nullptr && (keyItemName_.empty() || key->getName() == keyItemName_)) {
        //keyItemName_.empty() means any item can open it if itemName matches,
        //OR if keyItemName_ is set, the item's name must match keyItemName_
        //AND the item used (itemName) must also match the keyItemName_ for this logic.
        //A simpler check if only one specific key works:
        //if (key != nullptr && key->getName() == keyItemName_ && itemName == keyItemName_)
        //For our "Sunstone Amulet" and "melt ice with amulet", itemName will be "amulet".
        //So we need to ensure keyItemName_ for the Frozen Pass is "amulet".

        isOpen_ = true; //Open the door
        if (consumesKey) {
            player->removeItem(itemName); //Remove the key from player's inventory if it's consumed
        }
        return true; //Successfully opened
    }
    return false; //Failed to open (wrong item, or player doesn't have it)
}

//getBlockedDescription: returns the message for when the door is blocked.
std::string Door::getBlockedDescription() const {
    return blockedDescription_;
}

//If Door needed to override Passage's or Location's enter() method:
//void Door::enter() {
//    if (isOpen_) {
//        Passage::enter(); //Call base class enter if open
//    } else {
//        std::cout << getBlockedDescription() << std::endl;
//        //Player doesn't move, effectively. ZOOrkEngine will handle not moving the player.
//    }
//}