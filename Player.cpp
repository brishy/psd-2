//Player.cpp
//Implementation of the Player class methods.

#include "Player.h"
#include "Item.h" //Player interacts with Item objects for inventory
#include "Room.h" //Player's current location is a Room
//No need for <vector> or <string> here as they are used via Player.h members/parameters

//Initialize the static playerInstance pointer to nullptr.
//This ensures that the first call to Player::instance() will create the object.
Player* Player::playerInstance = nullptr;

//setCurrentRoom: Updates the player's current room.
void Player::setCurrentRoom(Room* room) {
    //It's good practice to check if the provided room pointer is valid,
    //though the game logic should ideally always provide a valid room or NullRoom.
    if (room) {
        currentRoom = room;
    } else {
        //Handle error or default to a NullRoom if necessary.
        //For now, assume 'room' will be valid or a NullRoom instance.
        currentRoom = room; //Allow setting to nullptr if that's intended (though NullRoom is better)
    }
}

//getCurrentRoom: Returns a pointer to the player's current room.
Room* Player::getCurrentRoom() const {
    return currentRoom;
}

//addItem: Adds an item to the player's inventory.
void Player::addItem(Item* item) {
    if (item != nullptr) { //Only add valid item pointers
        inventory.push_back(item);
    }
}

//removeItem: Finds an item by its name in the inventory, removes it, and returns it.
//Returns nullptr if the item is not found.
Item* Player::removeItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it) != nullptr && (*it)->getName() == itemName) { //Check for valid pointer and name match
            Item* foundItem = *it; //Store the item pointer
            inventory.erase(it);   //Remove the item from the inventory vector
            return foundItem;      //Return the removed item
        }
    }
    return nullptr; //Item not found in inventory
}

//getItem: Finds an item by its name in the inventory and returns it without removing.
//Returns nullptr if the item is not found.
Item* Player::getItem(const std::string& itemName) const {
    for (const auto& item : inventory) {
        if (item != nullptr && item->getName() == itemName) { //Check for valid pointer and name match
            return item; //Return the found item
        }
    }
    return nullptr; //Item not found
}

//getInventory: Returns a constant reference to the player's inventory.
const std::vector<Item*>& Player::getInventory() const {
    return inventory;
}