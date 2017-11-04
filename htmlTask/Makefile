# author:Jiri Burant
# date created: 14.9.2017
#
# Makefile for the htmlTask project. The program entry is the main method from TaskMain.cpp
# The libraries used, apart from std, are: libcurl and zlib

CC=g++
CFLAGS=-I. -pthread
DEPS = Downloader.h Parser.h TaskTypes.h TaskFacade.h
OBJ = TaskMain.o Downloader.o Parser.o TaskFacade.o

%.o: %.cpp $(DEPS)
	$(CC) -std=gnu++11  -c -o $@ $< $(CFLAGS) -lcurl -lz

TaskMain: $(OBJ)
	g++ -std=gnu++11 -o $@ $^ $(CFLAGS) -lcurl -lz
