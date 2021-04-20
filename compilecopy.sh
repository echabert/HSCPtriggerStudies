g++ -c macros/CopyTree.cpp -I. `root-config --glibs --cflags` -o obj/CopyTree.o
#compile the macro
g++ -I. obj/CopyTree.o `root-config --glibs --cflags` -o bin/CopyTree
