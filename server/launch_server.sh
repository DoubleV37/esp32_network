#!/bin/bash

cpp_file="server.cpp"

executable="serv"

c++ -o $executable $cpp_file

if [ $? -eq 0 ]; then
	./serv
fi
