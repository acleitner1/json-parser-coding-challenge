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
   int inkey = 0; 
   int inval = 0; 
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
      // check for key-val pairs on a line that should have a key value pair
      if (open == 1 && word != "{" && word != "}") {
         // first, check for a colon
         inkey = 0; 
         inval = 0; 
         for (int i = 0; i < word.size(); i++) {
            if (word[i] == '"' && !inkey) {
               inkey = 1; 
            }
            else if (word[i] == '"' && inkey == 1) {
               inkey = 2; 
               if (word[i+1] != ':') {
                  cout << "Invalid colon" << endl; 
                  return -1; 
               }
            }
            if (inkey == 2 && word[i] == '"') {
               inval++; 
            }
            // if (inval >= 2 && word[i] == ',') {
            //    cout << "Invalid comma after key value pair" << endl; 
            //    return 1; 
            // }
         }
         if (inval != 2 && inkey != 2) {
            cout << "Invalid key value pair" << endl; 
            return 1; 
         }

      }
      // In here, going to have to break it into lines and then run the above code 
      if (open == -1) {
         // established that we have braces 
         if (word[0] == '{' && word[word.size() - 1] == '}') {
            open = 0; 
         }
         for (int i = 0; i < word.size(); i++) {
            if (word[i] == '"') {
               if (!inkey && !inval) {
                  inkey = 1; 
               }
               else if (inkey == 1) {
                  inkey = 2; 
               }
               else if (inkey == 0 && inval == 1) {
                  inval = 2; 
                  //inkey = 0; 
               }
               else if (inval == 2) {
                  inval = 0; 
               }
               else if (inval == 1) {
                  cout << "Missing colon" << endl; 
                  return 1; 
               }
            }
            if (word[i] == ':' && inkey == 2) {
               inval = 1; 
               inkey = 0; 
            }
         //   cout << "inval: " << inval << " inkey: " << inkey << " letter: " << word[i] << endl; 
         }
         if (inval == 1) {
            cout << "Invalid comma" << endl; 
            return 1; 
         }
         if (inkey != 0) {
            cout << "Invalid Key-value pair" << endl; 
            return 1; 
         }
         
      }
      if (open != 0 && open != 1) {
         cout << "Invalid opening brace" << endl; 
         return 1; 
      }
   }
   if (open != 0) {
      cout << "Lacks opening or closing brace" << endl; 
      return 1; 
   }
   if (word[word.size() - 2] == ',') {
      cout << "Invalid closing comma" << endl; 
      return 1; 
   }
   return 0;
}