//ZOOrkEngine.h
//Defines the ZOOrkEngine class, the main controller for the game.
//It manages the game loop, player commands, and overall game state.

#ifndef ZOORK_ZOORKENGINE_H //Include guard
#define ZOORK_ZOORKENGINE_H

#include "Player.h" //Engine needs access to the Player
#include "Room.h"   //Engine needs to interact with Rooms (e.g., starting room)

#include <iostream> //For I/O operations
#include <sstream>  //For string stream processing (tokenizing)
#include <string>   //For string manipulation
#include <vector>   //For storing command arguments
#include <memory>   //For std::shared_ptr (used in constructor)

class ZOOrkEngine {
public:
    //Constructor: takes a shared_ptr to the starting Room for the game.
    ZOOrkEngine(std::shared_ptr<Room> startRoom);

    //run: starts and manages the main game loop.
    void run();

private:
    bool gameOver = false; //Flag to control the game loop
    Player* player;        //Pointer to the single Player instance

    //Command handler methods
    void handleGoCommand(std::vector<std::string> arguments);
    void handleLookCommand(std::vector<std::string> arguments);
    void handleTakeCommand(std::vector<std::string> arguments);
    void handleDropCommand(std::vector<std::string> arguments);
    void handleInventoryCommand(std::vector<std::string> arguments);
    void handleUseCommand(std::vector<std::string> arguments); //<<< NEW COMMAND HANDLER
    void handleQuitCommand(std::vector<std::string> arguments);

    //Utility methods for command processing
    static std::vector<std::string> tokenizeString(const std::string& input); //Splits input into words
    static std::string makeLowercase(std::string inputStr); //Converts string to lowercase
};

#endif //ZOORK_ZOORKENGINE_H