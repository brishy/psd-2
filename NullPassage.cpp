//
// Created by Richard Skarbez on 5/7/23.
//

#include "NullPassage.h"
#include "Room.h"

NullPassage::NullPassage(Room* from) : Passage("null", "Time is a flat circle.", from, from) {}