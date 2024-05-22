// json-parser.cc
// Creates a JSON parser 
// Anna Leitner: May 2024

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
   char word;
   // All that we care is that when things get opened, they get closed. 
   // That means, that we care that the number of things goes back to zero at the end. 
   int brackets = 0; 
   int first = 0; 
   int key = 0; 
   int val = 0; 
   int quotes = 0; 
   while(input >> word) {
      if (word == '{') {
         brackets++; 
      }
      if (first == 0) {
         if (brackets != 1) {
            cout << "Must begin syntax with {" << endl; 
            return 1; 
         }
         else {
            first = 1; 
         }
      }
      else if (word == '}') {
         brackets--; 
      }
      if (brackets < 0) {
         cout << "Invalid curly braces" << endl; 
         return 1; 
      }
      // Checking keys and values
      if (key == 0 && word == '"' && val == 0) {
         key++; 
      }
      else if (key == -1 && word == '"' && val == 0) {
         key = 1; 
      }
      else if (key == 1 && word == '"') {
         key = 0; 
      }
      if (word == ':' && key != 0) {
         cout << "Invalid Key" << endl; 
         return 1; 
      }
      else if (word == ':') {
         val = 1; 
      }
      if (word == ',') {
         val = 0; 
         if (quotes != 0 && quotes != 2) {
            cout << "Invalid string value" << endl; 
            return 1; 
         }
         quotes = 0; 
         key = -1; 
      }
      if (val) {
         if (word == '"') {
            quotes++; 
         }
      }
   }
   if (first == 0) {
      cout << "Empty input" << endl;
      return 1; 
   }
   if (key == -1) {
      cout << "Improper comma" << endl; 
      return 1; 
   }
   input.close(); 
   return 0;
}