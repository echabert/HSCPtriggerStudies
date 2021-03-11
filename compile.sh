#compile the class
g++ -c src/TrigEff.cpp -I. `root-config --glibs --cflags` -o obj/TrigEff.o
#compile the macro
g++ macros/AnaEff.cpp `root-config --glibs --cflags` -o bin/AnaEff

