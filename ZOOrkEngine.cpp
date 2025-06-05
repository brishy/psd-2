//ZOOrkEngine.cpp
//Implementation of the ZOOrkEngine class methods.

#include "ZOOrkEngine.h"
#include "Player.h"
#include "Room.h"
#include "Item.h"
#include "Passage.h"
#include "Door.h" //<<< INCLUDE DOOR.H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> //For std::transform
#include <memory>

//Constructor
ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> startRoom) {
    player = Player::instance();
    if (startRoom) {
        player->setCurrentRoom(startRoom.get());
        if (player->getCurrentRoom() && player->getCurrentRoom()->getName() != "Nowhere") {
             player->getCurrentRoom()->enter();
        } else if (!player->getCurrentRoom()) {
            std::cerr << "Error: Player's current room is null after initialization." << std::endl;
            gameOver = true;
        }
    } else {
        std::cerr << "Error: ZOOrkEngine initialized with a null start room." << std::endl;
        //gameOver = true;
    }
}

//run: main game loop
void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";
        std::string inputLine;
        std::getline(std::cin, inputLine);

        if (std::cin.eof()) {
            std::cout << "Exiting ZOOrk." << std::endl;
            gameOver = true;
            continue;
        }
        if (inputLine.empty()) {
            continue;
        }

        std::vector<std::string> words = tokenizeString(inputLine);
        if (words.empty()) {
            continue;
        }
        std::string commandWord = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (commandWord == "go") {
            handleGoCommand(arguments);
        } else if (commandWord == "look" || commandWord == "l" || commandWord == "inspect") {
            handleLookCommand(arguments);
        } else if (commandWord == "take" || commandWord == "get") {
            handleTakeCommand(arguments);
        } else if (commandWord == "drop") {
            handleDropCommand(arguments);
        } else if (commandWord == "inventory" || commandWord == "i") {
            handleInventoryCommand(arguments);
        } else if (commandWord == "use") { //<<< ADDED "use" COMMAND
            handleUseCommand(arguments);
        } else if (commandWord == "quit" || commandWord == "q") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command: \"" << commandWord << "\"\n";
        }
    }
}

//handleGoCommand: processes the "go" command.
void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Go where?\n";
        return;
    }
    std::string direction = arguments[0];
    Room* currentRoom = player->getCurrentRoom();
    if (!currentRoom) { //Should not happen
        std::cout << "Error: You are nowhere!\n";
        return;
    }

    std::shared_ptr<Passage> passage = currentRoom->getPassage(direction);

    //Check if the passage is valid and not a NullPassage (which would mean the direction is impossible)
    //NullPassage's name is "null" and its fromRoom and toRoom are the same as the room it was created for.
    //Room::getPassage already prints "It is impossible to go..." for invalid directions.
    if (passage && passage->getName() != "null") { //Proceed if it's a real passage
        Door* door = dynamic_cast<Door*>(passage.get()); //Check if it's a Door
        if (door && !door->isOpen()) { //If it's a Door and it's not open
            std::cout << door->getBlockedDescription() << "\n";
            return; //Don't move
        }

        //Determine the actual destination room
        Room* destinationRoom = nullptr;
        if (currentRoom == passage->getFrom()) {
            destinationRoom = passage->getTo();
        } else if (currentRoom == passage->getTo()) {
            //This means we are using the passage in the "reverse" direction
            //of its original definition, which is intended for our shared Door object.
            destinationRoom = passage->getFrom();
        } else {
            //This case implies a misconfiguration: the currentRoom is neither the
            //passage's fromRoom nor toRoom, which shouldn't happen if the passage
            //was correctly added to the currentRoom's passageMap.
            //For safety, we can default to the passage's defined toRoom,
            //or simply not move if this unexpected state occurs.
            //Given Room::getPassage returns NullPassage for non-existent directions,
            //this 'else' block might be rarely hit with correct setup.
            //Let's assume if we got a non-NullPassage, one of the above conditions should be true.
            //If not, it's safer not to move and indicate an issue.
            std::cout << "Error: Passage configuration issue from " << currentRoom->getName() << " via " << direction << ".\n";
            return;
        }

        //Move the player if the destination is valid and different
        if (destinationRoom && destinationRoom != currentRoom) {
            player->setCurrentRoom(destinationRoom);
            destinationRoom->enter(); //Execute the enter command of the new room
        } else if (destinationRoom == currentRoom) {
            //This implies a loop passage or already handled by NullPassage logic.
            //Room::getPassage would have printed "It is impossible to go..." if it returned NullPassage.
            //If it's a valid passage that loops to itself, this is the behavior.
        } else {
            //destinationRoom is nullptr, something is very wrong.
             std::cout << "Error: Could not determine destination through " << direction << ".\n";
        }
    }
    //If passage was a NullPassage, the message "It is impossible to go..." was already printed by Room::getPassage.
}


//handleLookCommand (assuming Room::getExitDirections() is implemented for full exit listing)
void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();
    if (!currentRoom) {
        std::cout << "You are in a void. There is nothing to see.\n";
        return;
    }
    if (arguments.empty()) {
        std::cout << currentRoom->getDescription() << "\n";
        const auto& itemsInRoom = currentRoom->getItems();
        if (!itemsInRoom.empty()) {
            std::cout << "Items here: ";
            for (size_t i = 0; i < itemsInRoom.size(); ++i) {
                if (itemsInRoom[i]) {
                    std::cout << itemsInRoom[i]->getName() << (i == itemsInRoom.size() - 1 ? "" : ", ");
                }
            }
            std::cout << ".\n";
        } else {
            std::cout << "There are no items in this room.\n";
        }
        //Proper exit listing (requires Room::getExitDirections())
        const auto& exitDirections = currentRoom->getExitDirections();
        if (!exitDirections.empty()) {
            std::cout << "Obvious exits: ";
            for (size_t i = 0; i < exitDirections.size(); ++i) {
                std::cout << exitDirections[i] << (i == exitDirections.size() - 1 ? "" : ", ");
            }
            std::cout << ".\n";
        } else {
            std::cout << "There are no obvious exits.\n";
        }
    } else {
        std::string targetName = arguments[0];
        if (arguments.size() > 1) {
            for (size_t i = 1; i < arguments.size(); ++i) {
                targetName += " " + arguments[i];
            }
        }
        Item* itemInRoom = currentRoom->getItem(targetName);
        if (itemInRoom) {
            std::cout << itemInRoom->getDescription() << "\n";
        } else {
            std::cout << "You don't see any '" << targetName << "' here.\n";
        }
    }
}

//handleTakeCommand
void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Take what?\n";
        return;
    }
    std::string itemName = arguments[0];
    if (arguments.size() > 1) {
        for (size_t i = 1; i < arguments.size(); ++i) { itemName += " " + arguments[i]; }
    }
    Room* currentRoom = player->getCurrentRoom();
    if (!currentRoom) { std::cout << "You can't take items from the void!\n"; return; }
    Item* itemToTake = currentRoom->removeItem(itemName);
    if (itemToTake) {
        player->addItem(itemToTake);
        std::cout << "You take the " << itemName << ".\n";
    } else {
        std::cout << "You don't see any '" << itemName << "' here to take.\n";
    }
}

//handleDropCommand
void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Drop what?\n";
        return;
    }
    std::string itemName = arguments[0];
    if (arguments.size() > 1) {
        for (size_t i = 1; i < arguments.size(); ++i) { itemName += " " + arguments[i]; }
    }
    Room* currentRoom = player->getCurrentRoom();
    if (!currentRoom) { std::cout << "You can't drop items into the void!\n"; return; }
    Item* itemToDrop = player->removeItem(itemName);
    if (itemToDrop) {
        currentRoom->addItem(itemToDrop);
        std::cout << "You drop the " << itemName << ".\n";
    } else {
        std::cout << "You don't have any '" << itemName << "' to drop.\n";
    }
}

//handleInventoryCommand
void ZOOrkEngine::handleInventoryCommand(std::vector<std::string> arguments) {
    const auto& playerInventory = player->getInventory();
    if (playerInventory.empty()) {
        std::cout << "You are not carrying anything.\n";
    } else {
        std::cout << "You are carrying:\n";
        for (const auto& item : playerInventory) {
            if (item) { std::cout << "  - " << item->getName() << "\n"; }
        }
    }
}

//handleUseCommand: processes "use <item> on <target>" or "use <item>"
void ZOOrkEngine::handleUseCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Use what? And on what?\n";
        return;
    }

    std::string itemName = arguments[0];
    std::string targetFeature; //e.g., "ice", "door", "lever"
    std::string targetDirection; //If the target is a door in a specific direction

    //Parse arguments: "use <item>" or "use <item> on <target>" or "use <item> on <target> <direction>"
    //This is a simple parser. A more robust one would be better.
    if (arguments.size() >= 3 && arguments[1] == "on") {
        targetFeature = arguments[2];
        if (arguments.size() >= 4) { //Potentially a direction for the feature
            //This logic is getting complex; for "melt ice with amulet", "ice" is the feature.
            //If "ice" is associated with a specific passage direction, we need that.
            //Let's assume for "melt ice with amulet", "ice" refers to any icy blockage in the current room.
            //Or, the "targetFeature" could be the direction of the door itself.
            //Let's simplify: "use <item> on <direction_of_door>"
            //Or "use <item>" and the game tries to figure out the target.
            //For "melt ice with amulet", let's assume the "targetFeature" is the key.
            //The command could be "use amulet on frozen pass" or "use amulet on north_door"
            //Let's make it "use <item_name> <direction_of_passage_to_unlock>"
            //e.g. "use amulet north" if the frozen pass is to the north.
            //Or, if the door has a name/feature like "ice": "use amulet ice"
            //Let's go with "use <item> <direction>" for now, targeting a passage.
            targetDirection = arguments[2]; // Assuming "use item direction"
        }
    } else if (arguments.size() == 2) { // "use item direction"
        targetDirection = arguments[1];
    } else if (arguments.size() == 1) { // "use item" - try item's default use()
        Item* itemToUse = player->getItem(itemName);
        if (itemToUse) {
            std::cout << "You use the " << itemName << ".\n";
            itemToUse->use(); //Calls Item's own useCommand
        } else {
            std::cout << "You don't have a " << itemName << ".\n";
        }
        return; //Done with "use item"
    } else {
        std::cout << "Use what on what? (e.g., 'use key north' or 'use key on door')\n";
        return;
    }


    //Attempting to use an item on a passage (specifically a Door)
    if (!targetDirection.empty()) {
        Item* itemInInventory = player->getItem(itemName);
        if (!itemInInventory) {
            std::cout << "You don't have a " << itemName << ".\n";
            return;
        }

        Room* currentRoom = player->getCurrentRoom();
        std::shared_ptr<Passage> passage = currentRoom->getPassage(targetDirection);
        Door* door = dynamic_cast<Door*>(passage.get());

        if (door) {
            if (door->isOpen()) {
                std::cout << "The way " << targetDirection << " is already open.\n";
            } else {
                //The Door::tryOpen method expects the player and the item name being used.
                //The item name here is itemName (e.g., "amulet").
                //The Door's keyItemName_ is what it *expects* (e.g., also "amulet").
                if (door->tryOpen(player, itemName)) { //Pass player and the item name from inventory
                    std::cout << "You use the " << itemName << ". The way " << targetDirection << " is now open!\n";
                } else {
                    std::cout << "Using the " << itemName << " on the passage " << targetDirection << " does nothing.\n";
                }
            }
        } else if (passage && passage->getName() != "null") { //It's a regular passage, not a door
            std::cout << "You can't use the " << itemName << " on the passage to the " << targetDirection << " in that way.\n";
        } else { //No passage in that direction (Room::getPassage already printed a message)
            //Message already printed by Room::getPassage
        }
    } else {
        //Handle "use item on feature" if targetFeature was parsed and not a direction
        //For now, we only handle "use item <direction>" or "use item"
        std::cout << "Use " << itemName << " on what? (Specify a direction or target feature).\n";
    }
}


//handleQuitCommand
void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT? (y/n)\n> ";
    std::getline(std::cin, input);
    std::string quitStr = makeLowercase(input);
    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
        std::cout << "Exiting ZOOrk. Goodbye!\n";
    } else {
        std::cout << "Quit aborted.\n";
    }
}

//tokenizeString
std::vector<std::string> ZOOrkEngine::tokenizeString(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        if (!token.empty()) {
            tokens.push_back(makeLowercase(token));
        }
    }
    return tokens;
}

//makeLowercase
std::string ZOOrkEngine::makeLowercase(std::string inputStr) {
    std::transform(inputStr.begin(), inputStr.end(), inputStr.begin(), ::tolower);
    return inputStr;
}