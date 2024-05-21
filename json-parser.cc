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
   int inkey; 
   int inval; 
   int type; 
   int str; 
   while (getline(input, word)) {
      inkey = 0; 
      inval = 0; 
      type = -1; 
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
         // 0 is not opened, 1 is opened but not closed, 2 is opened and closed
         str = 0; 
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
            if (inkey == 2 && word[i] == ':') {
               inval = 1; 
               // string
               if (word[i+2] == '"') {
                  type = 0; 
               }
               // bool 
               else if (word[i +2] == 'f' || word[i+2] == 't') {
                  type = 1; 
               }
               // null 
               else if (word[i+2] == 'n') {
                  type = 2; 
               }
               // numeric
               else {
                  type = 3; 
               }
            }
            else if (inval == 1 && word[i] == ' ') {
               if (!str) {
                  continue; 
               }
               else {
                  cout << "Improper space" << endl; 
                  return 1; 
               }
            }
            else if (inval == 1 && word[i] != ':' && word[i] != ',') { 
               if (type == 1 && word.substr(i, 4) != "true" && word.substr(i, 5) != "false") {
                  cout << "Invalid boolean value" << endl; 
                  return 1; 
               }
               else if (type == 1 && ((word.substr(i, 4) == "true"))) {
                  str = 2; 
                  i+= 4; 
               }
               else if (type == 1 && (word.substr(i, 5) == "false")) {
                  str = 2; 
                  i+=5; 
               }
               else if (type == 2 && word.substr(i, 4) != "null") {
                  cout << "Invalid null value" << endl; 
                  return 1; 
               }
               else if (type == 2 && word.substr(i, 4) == "null") {
                  str = 2; 
                  i+=4; 
               }
               else if (type == 3 && !isdigit(word[i])) {
                  cout << "Inavlid numeric value" << endl; 
                  return 1; 
               }
               else if (type == 3) {
                  str = 1; 
               }
               else if (type == 0) {
                  if (word[i] == '"' && str == 0) {
                     str = 1; 
                  }
                  else if (word[i] == '"' && str == 1) {
                     str = 2; 
                  }
                  else if (word[i] != '"' && word[i] != ' ' && str != 1) {
                     cout << "String value opening \" incorrect" << endl;
                     return 1; 
                  }
                  else if (str == 2) {
                     cout << "String value improperly closed" << endl; 
                     return 1; 
                  }
               }
            }
            if (word[i] == ',') {
               inval++; 
            }
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
         str = 0; 
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
            else if (inval == 1 && word[i] == ' ') {
               if (!str) {
                  continue; 
               }
               else {
                  cout << "Improper space" << endl; 
                  return 1; 
               }
            }
            else if (inval == 1 && word[i] != ':' && word[i] != ',') { 
               if (type == 1 && (word.substr(i, 4) != "true" && word.substr(i, 5) != "false")) {
                  cout << "Invalid boolean value" << endl; 
                  return 1; 
               }
               else if (type == 1 && ((word.substr(i, 4) == "true"))) {
                  i+= 4; 
                  str = 2; 
               }
               else if (type == 1 && (word.substr(i, 5) == "false")) {
                  i+=5; 
                  str = 2; 
               }
               else if (type == 2 && word.substr(i, 4) != "null") {
                  cout << "Invalid null value" << endl; 
                  return 1; 
               }
               else if (type == 2 && word.substr(i, 4) == "null") {
                  i+=4; 
               }
               else if (type == 3 && !isdigit(word[i])) {
                  cout << "Inavlid numeric value" << endl; 
                  return 1; 
               }
               else if (type == 3) {
                  str = 1; 
               }
               else if (type == 0) {
                  if (word[i] == '"' && str == 0) {
                     str = 1; 
                  }
                  else if (word[i] == '"' && str == 1) {
                     str = 2; 
                  }
                  else if (word[i] != '"' && word[i] != ' ' && str != 1) {
                     cout << "String value opening \" incorrect" << endl;
                     return 1; 
                  }
                  else if (str == 2) {
                     cout << "String value improperly closed" << endl; 
                     return 1; 
                  }
               }
            }
            if (word[i] == ',') {
               inval= 0; 
               str = 0; 
               type = -1; 
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