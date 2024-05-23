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
   int backslash = 0; 
   if (json[0] != '"') {
      return returnable; 
   }
   else {
      json = json.substr(1); 
      returnable+= '"'; 
      while (((json[0] != '"') || (json[0] == '"' && backslash)) && json.length()) {
         if (json[0] == '\\') {
            if (backslash == 1) {
               backslash = 0; 
            }
            else {
               backslash = 1; 
            }
         }
         else {
            backslash = 0; 
         }
         returnable += json[0]; 
         json = json.substr(1); 
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
   // // if (returnable == "0" && json[0] != '.') {
   // //    cout << "Leading zeroes not allowed" << endl; 
   // //    exit(1); 
   // // }
   // else if (returnable == "0" && json[0] == '.') {
   //    returnable += json[0]; 
   //    json = json.substr(1); 
   // }
   while ((isdigit(json[0]) || json[0] == 'e' || json[0] == 'E' || json[0] == '.' || json[0] == '-' || json[0] == '+') && json.length()) {
      cout << "rawr" << endl; 
      cout << "json 0: " << json[0] << endl; 
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
   if (json.substr(0, 4) == "true") {
      json = json.substr(4); 
      return "true"; 
   }
   else if (json.substr(0, 5) == "false") {
      json = json.substr(5); 
      return "false"; 
   }
   return ""; 
}

string lexical_null(string& json) {
   if (json.substr(0, 4) == "null") {
      json = json.substr(4); 
      return "null"; 
   }
   return ""; 
}
string parse_tokens(vector<string>& tokens); 


vector<string> parse_list(vector<string>& tokens) {
   vector<string> returnable_list; 

   string item = tokens[0]; 
   if (item == "]" || item == "}") {
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      return returnable_list; 
   }
   while (tokens.size()) {
      item = parse_tokens(tokens); 
      returnable_list.push_back(item); 

      item = tokens[0]; 
      if (item == "]") {
         tokens.erase(tokens.begin(), tokens.begin()+1); 
         return returnable_list; 
      }
      else if (item != ",") {
         cout << "Missing comma in list" << endl;
         exit(1);
      }
      else {
         tokens.erase(tokens.begin(), tokens.begin()+1); 
      }
   }
   cout << "Missing bracket at end of list" << endl; 
   exit(1); 
   return returnable_list; 
}

vector<string> parse_obj(vector<string> tokens) {
   vector<string> returnable; 
   string item = tokens[0]; 
   string key; 

   if (item == "}") {
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      return returnable; 
   }

   while (tokens.size()) {
      key = tokens[0]; 
      if (key[0] == '"' && key[key.length() - 1] == '"' ) {
         tokens.erase(tokens.begin(), tokens.begin() +1); 
      }
      else {
         cout << "Key must be a string" << endl; 
         exit(1); 
      }

      if (tokens[0] != ":") {
         cout << "Must follow a key with a colon" << endl; 
         exit(1); 
      }
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      item = parse_tokens(tokens); 
      returnable.push_back(item); 
      item = tokens[0]; 
      if (item == "}") {
         tokens.erase(tokens.begin(), tokens.begin() + 1); 
         return returnable; 
      }
      else if (item != ",") {
         cout << "Expect comma between objects" << endl; 
         exit(1); 
      }
      tokens.erase(tokens.begin(), tokens.begin()+1); 
   }
   cout << "Missing closing curly bracket" << endl; 
   exit(1); 
   return returnable; 

}

string parse_tokens(vector<string>& tokens) {
   string item = tokens[0]; 

   if (item == "{" || item == "}") {
      tokens.erase(tokens.begin(), tokens.begin() + 1); 
      parse_obj(tokens); 
   }
   else if (item == "[") {
      tokens.erase(tokens.begin(), tokens.begin() +1); 
      parse_list(tokens); 
   }
   else {
      tokens.erase(tokens.begin(), tokens.begin() +1); 
      return item; 
   }
   cout << "shouldn't get here...." << endl; 
   return item; 
}



// function that will break the json string into tokens
int parse(string json) {
   // going to cut string 
   string temp_string; 
   vector<string> tokens; 
   while (json.length()) { 
      cout << "json length: " << json.length() << endl; 

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
         cout << "the real culprit!" << endl; 
         json = json.substr(1); 
      }
      if (json[0] == '{' || json[0] == '}' || json[0] == '[' || json[0] == ']' || json[0] == ',' || json[0] == ':') {
         temp_string = ""; 
         temp_string+= json[0]; 
         tokens.push_back(temp_string); 
         json = json.substr(1); 
      }
   }
   // Tokens should now be produced. 
   // TODO: iterate through the tokens and make sure that they match a valid grammar
   for (int i = 0; i < tokens.size(); i++) {
      cout << tokens[i] << endl; 
   }
   string parser_returnable = parse_tokens(tokens); 

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
   string data = ""; 
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