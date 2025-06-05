//Passage.h
//Created by Richard Skarbez on 5/7/23.
//Defines connections between Rooms.

#ifndef ZOORK_PASSAGE_H
#define ZOORK_PASSAGE_H

#include "Location.h" //Passage inherits from Location
#include <string>     //For names, directions
#include <memory>     //For std::shared_ptr
#include "Command.h"  //For constructor taking a Command
//iostream was here, but typically not needed in a header defining a class like Passage unless for inline debug prints.
//It's better in Passage.cpp if needed there.

//Forward declarations to break include cycles:
class Room;
//class NullRoom; //NullRoom is a Room, so Room* is sufficient for type declarations.
                  //The full NullRoom.h will be included in Passage.cpp if needed.

class Passage : public Location {
public:
    //Static helper to create a standard passage between two rooms.
    static void createBasicPassage(Room* from, Room* to, const std::string& direction, bool bidirectional = true);

    //Constructor for a passage between two rooms.
    Passage(const std::string& name, const std::string& description, Room* from, Room* to);

    //Constructor for a passage with a custom enter command.
    Passage(const std::string& name, const std::string& description, std::shared_ptr<Command> enterCommand, Room* from, Room* to);

    //Setters and getters for the rooms connected by the passage.
    void setFrom(Room* r);
    Room* getFrom() const;
    void setTo(Room* r);
    Room* getTo() const;

protected:
    //Helper to get the opposite direction string.
    static std::string oppositeDirection(const std::string& s);

    Room* fromRoom; //Pointer to the room this passage originates from.
    Room* toRoom;   //Pointer to the room this passage leads to.
};

#endif //ZOORK_PASSAGE_H