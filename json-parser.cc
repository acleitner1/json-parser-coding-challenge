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
   }
   if (first == 0) {
      cout << "Empty input" << endl;
      return 1; 
   }
   input.close(); 
   return 0;
}