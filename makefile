export cxx = g++
export sfml_include = /usr/include/SFML
export sfml_lib = /usr/lib
export included_libs = -lsfml-graphics -lsfml-window -lsfml-system
battle_files = main.o classes/Ship.o classes/Field.o classes/Entity.o classes/Colours.o classes/GameStates.o

.PHONY: classes clean

battleship: classes main.o
	mkdir -p bin
	$(cxx) $(battle_files) -o bin/battleship -L $(sfml_lib) $(included_libs)

main.o: classes main.cpp
	$(cxx) -c -Wall main.cpp -I $(sfml_include)

classes:
	$(MAKE) -C classes

clean:
	rm -r *.o
	rm -r classes/*.o

run:
	bin/battleship
