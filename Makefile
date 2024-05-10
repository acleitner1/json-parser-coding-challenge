# Makefile for wc coding challenge

CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11 -g -ldl

parser: json-parser.cc
	$(CXX) $(CXXFLAGS) -o $@ json-parser.cc $(LIBS)

clean:
	rm -rf parser json-parser.cc