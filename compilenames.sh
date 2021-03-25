#compile the class
g++ -c macros/FindListTriggers.cpp -I. `root-config --glibs --cflags` -o obj/FindListTriggers.o
#compile the macro
g++ -I. obj/FindListTriggers.o `root-config --glibs --cflags` -o bin/FindAll
