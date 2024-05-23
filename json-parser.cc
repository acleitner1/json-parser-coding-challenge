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

// Look for opening and closing quotes and then return what it is
string lexical_string(string& json) {
   string returnable = ""; 
   if (json[0] != '"') {
      return returnable; 
   }
   else {
      json = json.substr(1); 
      returnable+= '"'; 
      while (json[0] != '"' && json.length()) {
         returnable += json[0]; 
         json.substr(1); 
      }
      if (json.length() == 0) {
         // NEED TO EXIT 
         cout << "Unclosed string" << endl; 
         exit(1); 
      }
      else {
         returnable+= '"'; 
         json = json.substr(1); 
      }
   }
   return returnable; 
}
string lexical_num(string& json) {
   string returnable = ""; 
   if (!isdigit(json[0]) && json[0] != '-') {
      return returnable; 
   }
   returnable += json[0]; 
   json = json.substr(1); 
   if (returnable == "0" && json[0] != '.') {
      cout << "Leading zeroes not allowed" << endl; 
      exit(1); 
   }
   else if (returnable == "0" && json[0] == '.') {
      returnable += json[0]; 
      json = json.substr(1); 
   }
   while ((isdigit(json[0]) || json[0] == 'e') && json.length()) {
      returnable+= json[0]; 
      json = json.substr(1); 
   }
   if (json.length() == 0) {
      cout << "Invalid end of input" << endl; 
      exit(1); 
   }
   return returnable;
}

string lexical_bool(string& json) {
   return json; 
}

string lexical_null(string& json) {
   return json; 
}
// function that will break the json string into tokens
int parse(string json) {
   // going to cut string 
   string temp_string; 
   vector<string> tokens; 
   while (json.length()) { 
      temp_string = lexical_string(json); 
      if (temp_string != "") {
         tokens.push_back(temp_string);
      }
      temp_string = lexical_num(json); 
      if (temp_string != "") {
         tokens.push_back(temp_string);
      }
      temp_string = lexical_bool(json); 
      if (temp_string != "") {
         tokens.push_back(temp_string);
      }
      temp_string = lexical_null(json); 
      if (temp_string != "") {
         tokens.push_back(temp_string);
      }
      while (json[0] == ' ') {
         json = json.substr(1); 
      }
      if (json[0] == '{' || json[0] == '}' || json[0] == '[' || json[0] == ']' || json[0] == ',' || json[0] == ':') {
         temp_string = ""; 
         temp_string+= json[0]; 
         tokens.push_back(temp_string); 
         json = json.substr(1); 
      }
   }

   // After doing this, we'll have tokens
   return 0; 
}

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
   int returnable = parse(data);
   input.close(); 
   // if returnable is 0, return 0
   // else, have a series of error codes to return based upon what returnable is 
   return returnable;
}