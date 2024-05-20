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
   string word; 
   int open = -1; 
   // int inkey = -1; 
   // int inval = -1; 
   while (getline(input, word)) {
      if (word == "{") {
         if (open == 1) {
            cout << "Invalid opening brace" << endl; 
            return 1; 
         }
         open = 1; 
      }
      if (word == "}") {
         if (!open) {
            cout << "Invalid closing brace" << endl; 
            return 1; 
         }
         open = 0; 
      }
      if (open == -1) {
         if (word[0] == '{' && word[word.size() - 1] == '}') {
            open = 0; 
         }
      }
   }
   if (open != 0) {
      cout << "Lacks opening or closing brace" << endl; 
      return 1; 
   }
   return 0;
}