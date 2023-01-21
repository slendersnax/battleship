.PHONY: all

all: Ship.o Field.o Entity.o Colours.o GameStates.o

Ship.o: Ship.cpp
	$(cxx) -c -Wall Ship.cpp -I $(sfml_include)

Field.o: Field.cpp
	$(cxx) -c -Wall Field.cpp -I $(sfml_include)

Entity.o: Entity.cpp
	$(cxx) -c -Wall Entity.cpp -I $(sfml_include)

Colours.o: Colours.cpp
	$(cxx) -c -Wall Colours.cpp -I $(sfml_include)

GameStates.o: GameStates.cpp
	$(cxx) -c -Wall GameStates.cpp