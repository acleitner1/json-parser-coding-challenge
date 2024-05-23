// json-parser.cc
// Creates a JSON parser 
// Anna Leitner: May 2024

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std; 

// To Do: 
// Parse the input and break it into pieces that will then go into a list 
// Check each piece that it matches a token 
// If it doesn't, throw an exception 

int main(int argc, char** argv) {  
   // Input should contain a file
   if (argc < 2) {
      cout << "Must include a file to parse" << endl; 
      exit(1); 
   }
   ifstream input;  
   string filename = argv[1]; 
   input.open(filename); 
   string data; 
   string temp; 
   while(getline(input, temp)) {
      data+= temp; 
   }
   cout << data << endl; 
   input.close(); 
   return 0;
}