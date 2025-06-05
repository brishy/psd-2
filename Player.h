#ifndef ZOORK_PLAYER_H //Include guard
#define ZOORK_PLAYER_H

#include "Character.h" //Player is a Character
#include "Room.h"      //Player is in a Room (uses Room*)
#include "Item.h"      //Player has an inventory of Items (uses Item*)
#include "NullRoom.h"  //For default room initialization

#include <vector>   //For the inventory
#include <string>   //For item names
#include <memory>   //Not directly used here, but often associated with game objects

//Forward declaration of Room, as Player holds a Room* and Room might refer to Player.
//However, Room.h is included above, so this might be redundant if Room.h doesn't include Player.h.
//class Room; //Already included Room.h
//class Item; //Already included Item.h

class Player : public Character {
public:
    //instance: Provides access to the singleton Player object.
    //Uses lazy instantiation: the Player object is created only when first requested.
    static Player* instance() {
        if (!playerInstance) { //If no instance exists yet
            playerInstance = new Player(); //Create it
        }
        return playerInstance; //Return the single instance
    }

    //setCurrentRoom: Sets the player's current location.
    void setCurrentRoom(Room* room);

    //getCurrentRoom: Returns a pointer to the player's current room.
    Room* getCurrentRoom() const;

    // --- Inventory Management Methods (Part 1 Requirement) ---

    //addItem: Adds an item pointer to the player's inventory.
    void addItem(Item* item);

    //removeItem: Finds an item by name in the inventory, removes it, and returns a pointer to it.
    //Returns nullptr if the item is not found.
    Item* removeItem(const std::string& itemName);

    //getItem: Finds an item by name in the inventory and returns a pointer to it without removing.
    //Returns nullptr if the item is not found. Useful for checking if player has an item.
    Item* getItem(const std::string& itemName) const;

    //getInventory: Returns a constant reference to the player's inventory vector.
    const std::vector<Item*>& getInventory() const;


    //Deleted copy constructor and assignment operator to enforce Singleton pattern.
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

private:
    static Player* playerInstance; //Static pointer to the single instance of Player
    Room* currentRoom;             //Pointer to the room the player is currently in
    std::vector<Item*> inventory;  //List of items the player is carrying

    //Private constructor to enforce Singleton pattern.
    //Initializes player with a default name, description, and starting in a NullRoom.
    Player() : Character("You", "You are a person, alike in dignity to any other, but uniquely you."),
               currentRoom(new NullRoom()) { //Start in a safe, non-functional room
        //inventory is default-initialized (empty vector)
    }
};

#endif //ZOORK_PLAYER_H