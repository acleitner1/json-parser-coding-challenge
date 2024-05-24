// json-parser.cc
// Creates a JSON parser 
// Anna Leitner: May 2024

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std; 

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
         if (backslash) {
            if (json[0] == 'x' || json[0] == '0') {
               cout << "Illegal Escape" << endl; 
               exit(1); 
            }
         }
         if (json[0] == '\\' && !backslash) {
            backslash = 1; 
         }
         else {
            backslash = 0; 
         }
         returnable += json[0]; 
         json = json.substr(1); 
      }
      if (json.length() == 0) {
         cout << "Unclosed string" << endl; 
         exit(1); 
      }
      else {
         returnable+= '"'; 
         json = json.substr(1); 
      }
   }
   if (returnable.find("   ") != string::npos) {
      cout << "Tabs are illegal" << endl; 
      exit(1); 
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
   while ((isdigit(json[0]) || json[0] == 'e' || json[0] == 'E' || json[0] == '.' || json[0] == '-' || json[0] == '+') && json.length()) {
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
   if (tokens.size() == 0) {
      cout << "Invalid list" << endl; 
      exit(1); 
   }

   string item = tokens[0]; 
   if (item == "]") {
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      return returnable_list; 
   }
   while (tokens.size()) {
      item = parse_tokens(tokens); 
      returnable_list.push_back(item); 
      if (tokens.size() == 0) {
         cout << "Invalid list" << endl; 
         exit(1); 
      }
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
         if (tokens.size() && tokens[0] == ",") {
            cout << "Double comma in list" << endl; 
            exit(1);
         }
      }
   }
   cout << "Missing bracket at end of list" << endl; 
   exit(1); 
   return returnable_list; 
}

vector<string> parse_obj(vector<string>& tokens) {
   vector<string> returnable_obj; 
   string first = tokens[0]; 
   string key; 
   string val; 
   if (first == "}") {
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      return returnable_obj; 
   }  

   while (tokens.size()) {
      key = tokens[0]; 
      // check that the key is a string
      if (key[0] == '"' && key[key.size() - 1] == '"') {
         returnable_obj.push_back(key); 
         tokens.erase(tokens.begin(), tokens.begin()+1); 
      }
      else {
         cout << "Keys in a object must be strings" << endl; 
         exit(1); 
      }

      if (tokens[0] != ":") {
         cout << "Keys in an object must be followed by a colon" << endl; 
         exit(1); 
      }
      // erase the colon 
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      // the value we get out should be the former first item in tokens, but its already been removed
      val = parse_tokens(tokens); 
      returnable_obj.push_back(val); 
      // now, the first thing in the returnable obj should be a right curly bracket or a comma 
      if (!tokens.size()) {
         cout << "Object lacks closing curly bracket" << endl; 
         exit(1); 
      }
      if (tokens[0] == "}") {
         tokens.erase(tokens.begin(), tokens.begin()+1); 
         return returnable_obj; 
      }
      else if (tokens[0] == ",") {
         tokens.erase(tokens.begin(), tokens.begin()+1);
      }
      else {
         cout << "Comma expected in between items in object" << endl; 
         exit(1); 
      }
   }
   cout << "Object lacks closing curly bracket" << endl; 
   exit(1); 

}

string parse_tokens(vector<string>& tokens) {
   if (tokens.size() == 0) {
      cout << "Missing closing bracket or brace" << endl; 
   }
   string item = tokens[0]; 

   // either, start parsing a new object or a new string (recursion baby) or, return the key/next item in the list
   if (item == "{") {
      tokens.erase(tokens.begin(), tokens.begin() + 1); 
      parse_obj(tokens); 
      return item; 
   }
   else if (item == "[") {
      tokens.erase(tokens.begin(), tokens.begin() +1); 
      parse_list(tokens); 
      return item; 
   }
   else if (item == "]" || item == "}") {
      cout << "Extra closing brace or bracket" << endl;
      exit(1);
   }
   else {
      tokens.erase(tokens.begin(), tokens.begin() +1); 
      return item; 
   }
}



// function that will break the json string into tokens
int parse(string json) {
   // going to cut string 
   string temp_string; 
   vector<string> tokens; 
   while (json.length()) { 
      while (json[0] == ' ') {
         json = json.substr(1); 
      }
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
      if (json[0] == '{' || json[0] == '}' || json[0] == '[' || json[0] == ']' || json[0] == ',' || json[0] == ':') {
         temp_string = ""; 
         temp_string+= json[0]; 
         tokens.push_back(temp_string); 
         json = json.substr(1); 
      }
      else if (json[0] != ' ') {
         cout << "Parsing Error: " << json[0] << endl; 
         exit(1); 
      }
   }
   // Tokens should now be produced. 
   if (tokens.size() > 0) {
      if (tokens[0] != "{" && tokens[0] != "[") {
         cout << "Invalid opening to input" << endl; 
         exit(1); 
      }
      else if (tokens[tokens.size() - 1] != "}" && tokens[tokens.size() - 1] != "]") {
         cout << "Invalid closing to input" << endl; 
      }
      while (tokens.size()) {
         parse_tokens(tokens); 
      }
   }
   else {
      cout << "Empty Input" << endl; 
      return 1; 
   }

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
      cout << "temp: " << temp << endl; 
      for (int i = 0; i < temp.length();i++) {
         cout << temp[i] << endl; 
      }
      data+= temp; 
   }
   int returnable = parse(data);
   input.close(); 
   // if returnable is 0, return 0
   // else, have a series of error codes to return based upon what returnable is 
   return returnable;
}