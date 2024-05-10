// json-parser.cc
// Creates a JSON parser 
// Anna Leitner: May 2024

#include <fstream>
#include <iostream>
#include <string>

using namespace std; 
int main(int argc, char** argv) {  
   // Input should contain a file
   if (argc < 2) {
      cout << "Must include a file to parse" << endl; 
      exit(1); 
   }
   ifstream input;  
   string filename = argv[1]; 
   input.open(filename); 
   char character; 
   int inobject = 0;
   int opened = 0; 
   while (input >> character) {
      if (character == '{') {
         opened = 1; 
         inobject++; 
      }
      if (character == '}') {
         inobject--; 
         if (inobject < 0) {
            cout << "Invalid closing brace" << endl; 
            return 1; 
         }
      }
   }
   if (inobject != 0 || !opened) {
      cout << "Invalid file" << endl; 
      return 1; 
   }
   return 0;
}