//NullRoom.h
//Created by Richard Skarbez on 5/7/23.
//Defines a NullRoom, a safe default/placeholder Room.

#ifndef ZOORK_NULLROOM_H
#define ZOORK_NULLROOM_H

#include "Room.h" //NullRoom IS A Room, needs full definition.

class NullRoom : public Room {
public:
    NullRoom();
};

#endif //ZOORK_NULLROOM_H