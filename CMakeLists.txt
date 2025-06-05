cmake_minimum_required(VERSION 3.23)
project(ZOOrk)

set(CMAKE_CXX_STANDARD 20)

add_executable(ZOOrk 
    main.cpp 
    Item.cpp Item.h
    Command.h 
    Character.cpp Character.h 
    Location.cpp Location.h 
    GameObject.cpp GameObject.h 
    Room.cpp Room.h 
    Passage.cpp Passage.h 
    Door.cpp Door.h   # <<< ADDED HERE
    NullRoom.cpp NullRoom.h 
    NullCommand.cpp NullCommand.h 
    Player.cpp Player.h 
    RoomDefaultEnterCommand.cpp RoomDefaultEnterCommand.h 
    ZOOrkEngine.cpp ZOOrkEngine.h 
    PassageDefaultEnterCommand.cpp PassageDefaultEnterCommand.h 
    NullPassage.cpp NullPassage.h
)