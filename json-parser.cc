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
   int instring = 0; 
   int key = 0; 
   int value = 0; 
   // 0: string, 1: numeric, 2: boolean, 3: numeric
   int val_type = -1; 
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
      if (character == '"' && !instring) {
         if (!key && !value) {
            key = 1; 
         }
         instring = 1; 
      }
      else if (character == '"' && instring) {
         instring = 0; 
      }
      if (isalpha(character) && !instring) {
         cout << "Invalid file: character outside of a string" << endl; 
         return 1; 
      }
      if (character == ':' && !instring && key) {
         value = 1; 
         key = 0; 
      } 
      else if (character == ':' && !instring) {
         cout << "Invalid file: key, value pair" << endl; 
         return 1; 
      }
      if (character == ',' && value && !instring) {
         value = 0; 
         key = 1; 
         val_type = -1; 
      } 
      else if (character == ',' && !instring) {
         cout << "Invalid file: value without a key" << endl; 
         return 1; 
      }
      // set val_type
      if (val_type == -1 && value) {
         if (character == '"') {
            val_type = 0; 
         }
         else if (character == 't' || 'f') {
            val_type = 2; 
         }
         else if (character == 'n') {
            val_type = 3;
         }
         else { 
            val_type = 1; 
         }
      }
   }
   if (key) {
      cout << "Invalid file: key opened but not completed" << endl; 
      return 1; 
   }
   if (inobject != 0 || !opened) {
      cout << "Invalid file: braces" << endl; 
      return 1; 
   }
   return 0;
}