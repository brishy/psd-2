//main.cpp
//Main entry point for the ZOOrk game.
//Sets up the initial game world (rooms, passages, items, doors) and starts the game engine.

#include "Room.h"    //For Room objects
#include "Passage.h" //For Passage objects
#include "Door.h"    //For Door objects (Part 2 Extension)
#include "Item.h"    //For Item objects
#include "Player.h"  //Player is used by ZOOrkEngine
#include "ZOOrkEngine.h" //The game engine
#include "RoomDefaultEnterCommand.h" //Awareness of default commands
#include "NullCommand.h" //For default item use commands
#include <memory>   //For std::shared_ptr and std::make_shared
#include <iostream> //For std::cout, std::cin

int main() {
    //--- Create Rooms ---
    auto west_of_house = std::make_shared<Room>("West of House",
        "You are standing in an open field west of a white house, with a boarded front door.\nA rubber mat saying 'Welcome to ZOOrk!' lies by the door.");
    auto south_of_house = std::make_shared<Room>("South of House",
        "You are facing the south side of a white house. There is no door here, and all the windows are barred.");
    auto behind_house = std::make_shared<Room>("Behind House",
        "You are behind the white house. A path leads into the forest to the east.\nIn one corner of the house there is a small window which is slightly ajar.");
    auto forest_path1 = std::make_shared<Room>("Forest Path - Entrance",
        "You are on a narrow path at the edge of a dark forest. The white house is to the west.");
    auto deep_forest = std::make_shared<Room>("Deep Forest",
        "The trees are dense here, and sunlight barely filters through. The path continues north and south.");
    auto ancient_clearing = std::make_shared<Room>("Ancient Clearing",
        "You've stumbled into a small, circular clearing. A moss-covered stone altar stands in the center.\nPaths lead west (back to deep forest) and east (deeper into mystery).");
    auto murky_swamp_edge = std::make_shared<Room>("Murky Swamp Edge",
        "The forest path ends here at the edge of a murky, foul-smelling swamp to the south. The path back to the deep forest is north.\nA chill wind blows from a narrow cave opening to the east.");
    auto inside_house_kitchen = std::make_shared<Room>("Kitchen",
        "You've managed to slip through the ajar window into what appears to be a dusty kitchen.\nThere's a table, some cupboards, and a door leading west (likely to the front of the house, but it's locked from this side). The window you came through is to the east.");
    auto dark_cellar = std::make_shared<Room>("Dark Cellar",
        "A rickety wooden staircase leads down into a damp, dark cellar beneath the kitchen. It smells of earth and something else... less pleasant.\nStairs lead up.");
    auto hidden_attic = std::make_shared<Room>("Dusty Attic",
        "A pull-down ladder, cleverly hidden in the kitchen ceiling, leads to this cramped, dusty attic. Cobwebs are everywhere.");
    
    //New room for Part 2
    auto icy_cavern_interior = std::make_shared<Room>("Icy Cavern Interior",
        "The air here is biting cold, and the walls shimmer with a thick layer of ice.\nA narrow passage continues deeper into the cold darkness to the east. The way out is west.");

    //--- Create Items ---
    Item* mailbox = new Item("mailbox", "A small, weathered mailbox. It's closed.");
    Item* leaflet = new Item("leaflet", "A crumpled leaflet: 'ZOOrk - Great Underground Adventures Await!'");
    Item* mat = new Item("mat", "A rubber welcome mat. Standard issue.");
    Item* window_latch = new Item("latch", "A simple, rusty latch on the small window. It seems movable.");
    Item* rusty_key = new Item("key", "A small, rusty key. It looks like it might fit a simple lock.");
    Item* dusty_book = new Item("book", "A heavy, dust-covered tome. The title is illegible.");
    Item* strange_gem = new Item("gem", "A fist-sized, oddly glowing gem rests on the altar.");
    Item* old_note = new Item("note", "A faded note: 'The way forward is opened by the house's heart.'");
    Item* cobweb = new Item("cobweb", "A large, sticky cobweb. Gross.");
    Item* sunstone_amulet = new Item("amulet", "A smooth, warm stone that pulses with a faint inner light. It feels like concentrated sunshine."); //Part 2 Item

    //--- Add Items to Rooms ---
    west_of_house->addItem(mailbox);
    west_of_house->addItem(mat);
    //Leaflet could be inside mailbox, requiring an "open mailbox" command (another Part 2 idea!)
    //For now, let's put it in the kitchen to be found after entering the house.
    inside_house_kitchen->addItem(leaflet);

    south_of_house->addItem(rusty_key);
    behind_house->addItem(window_latch);
    ancient_clearing->addItem(strange_gem);
    ancient_clearing->addItem(old_note);
    ancient_clearing->addItem(sunstone_amulet); //Place the amulet here
    inside_house_kitchen->addItem(dusty_book);
    hidden_attic->addItem(cobweb);

    //Set default use commands
    mailbox->setUseCommand(std::make_shared<NullCommand>());
    leaflet->setUseCommand(std::make_shared<NullCommand>());
    mat->setUseCommand(std::make_shared<NullCommand>());
    window_latch->setUseCommand(std::make_shared<NullCommand>());
    rusty_key->setUseCommand(std::make_shared<NullCommand>());
    dusty_book->setUseCommand(std::make_shared<NullCommand>());
    strange_gem->setUseCommand(std::make_shared<NullCommand>());
    old_note->setUseCommand(std::make_shared<NullCommand>());
    cobweb->setUseCommand(std::make_shared<NullCommand>());
    sunstone_amulet->setUseCommand(std::make_shared<NullCommand>()); //Default use does nothing, specific use via "use amulet on <target>"

    //--- Create Passages and Doors ---
    Passage::createBasicPassage(west_of_house.get(), south_of_house.get(), "south", true);
    Passage::createBasicPassage(south_of_house.get(), behind_house.get(), "east", true);
    Passage::createBasicPassage(behind_house.get(), forest_path1.get(), "east", true);
    Passage::createBasicPassage(behind_house.get(), inside_house_kitchen.get(), "window", false); //Enter through window
    Passage::createBasicPassage(inside_house_kitchen.get(), behind_house.get(), "out", false); //Exit through window

    Passage::createBasicPassage(forest_path1.get(), deep_forest.get(), "east", true); //Changed from "east" to "south" for map logic
    Passage::createBasicPassage(deep_forest.get(), ancient_clearing.get(), "west", true); //Path from deep forest to clearing
    Passage::createBasicPassage(deep_forest.get(), murky_swamp_edge.get(), "south", true);

    Passage::createBasicPassage(inside_house_kitchen.get(), dark_cellar.get(), "down", true);
    Passage::createBasicPassage(inside_house_kitchen.get(), hidden_attic.get(), "up", true);

    //Part 2: Frozen Pass (Door) from Murky Swamp Edge to Icy Cavern Interior
    auto frozen_pass = std::make_shared<Door>(
        "Frozen Pass", //Name of the passage object
        "A narrow cave opening, shimmering with an unnatural frost.", //Description of the passage itself when looked at (if we implement looking at passages)
        murky_swamp_edge.get(),      //From this room
        icy_cavern_interior.get(), //To this room
        "amulet",                    //The keyItemName required to open it (matches sunstone_amulet's name)
        "A shimmering sheet of magical ice blocks the cave entrance. It feels unnaturally cold." //Blocked description
    );
    murky_swamp_edge->addPassage("east", frozen_pass); //The frozen pass is to the "east" from swamp edge

    //Way back from Icy Cavern (let's make it a simple passage for now, assuming once melted it stays open from both sides implicitly)
    //Or, the Door logic could be enhanced for two-way state.
    //For simplicity, if the door is open, it's open. The "go" command in ZOOrkEngine checks this.
    //To go back, we need a passage from icy_cavern_interior to murky_swamp_edge.
    //This passage should ideally also be the *same* Door object if we want its state to be consistent.
    //Passage::createBasicPassage would create a new, separate Passage object.
    //So, we add the same door object to the other room, but with the opposite direction.
    icy_cavern_interior->addPassage("west", frozen_pass);


    //--- Initialize and Run Game Engine ---
    ZOOrkEngine zoork(west_of_house);
    zoork.run();

    //--- Cleanup Dynamically Allocated Items ---
    delete mailbox;
    delete leaflet;
    delete mat;
    delete window_latch;
    delete rusty_key;
    delete dusty_book;
    delete strange_gem;
    delete old_note;
    delete cobweb;
    delete sunstone_amulet; //Don't forget the new item

    return 0;
}