// json-parser.cc
// Parses an input file to determine if the content of the file conforms to JSON lexical and syntactical norms. 
// Returns 0 if true and exits with an error if not 
// Anna Leitner: May 2024

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std; 


// Determine if the beginning of the argument, json, is a JSON string
// If so, return the string. Otherwise, return an empty string
string lexical_string(string& json) {
   string returnable = ""; 
   int backslash = 0; 

   // If json doesn't begin with ", it cannot be a valid JSON string
   if (json[0] != '"') {
      return returnable; 
   }
   else {
      json = json.substr(1); 
      returnable+= '"'; 

      // Continue until an end quote or the string is consumed
      while (((json[0] != '"') || (json[0] == '"' && backslash)) && json.length()) {

         // Check for illegal exits
         if (backslash) {
            if (json[0] == 'x' || json[0] == '0') {
               cout << "Illegal Escape" << endl; 
               exit(1); 
            }
         }
         
         // Account for \" characters in a string
         if (json[0] == '\\' && !backslash) {
            backslash = 1; 
         }
         else {
            backslash = 0; 
         }

         // Build the JSON string to add to vector of tokens
         returnable += json[0]; 
         json = json.substr(1); 

         // Strings cannot contain newlines
         if (json[0] == '\n') {
            cout << "Newlines are illegal" << endl; 
            exit(1); 
         }
      }

      // Strings must end in "
      if (json.length() == 0) {
         cout << "Unclosed string" << endl; 
         exit(1); 
      }
      else {
         returnable+= '"'; 
         json = json.substr(1); 
      }
   }

   // Strings cannot contain tabs
   if (returnable.find("   ") != string::npos) {
      cout << "Tabs are illegal" << endl; 
      exit(1); 
   }
   return returnable; 
}


// Determine if the beginning of the argument, json, is a JSON number
// If so, return the number as a string. Otherwise, return an empty string
string lexical_num(string& json) {
   string returnable = ""; 
   int e = 0; 
   int dec = 0; 
   
   // Numbers must begin with a number or a negative sign 
   if (!isdigit(json[0]) && json[0] != '-') {
      return returnable; 
   }

   // Add the first digit to the string 
   returnable += json[0]; 
   json = json.substr(1); 

   // Leading zeroes are only allowed if followed by a . or , 
   if (returnable == "0" && json[0] != '.' && json[0] != ',') {
      cout << "Leading zeroes not allowed" << endl; 
      exit(1); 
   }
   else if (returnable == "0" && json[0] == '.') {
      returnable += json[0]; 
      json = json.substr(1); 
      dec = 1; 
   }

   // Continue until a valid number ends 
   while ((isdigit(json[0]) || json[0] == 'e' || json[0] == 'E' || json[0] == '-' || json[0] == '+' || json[0] == '.') && json.length()) {
      
      // Check that only one exponent is present in a number 
      if (json[0] == 'e' || json[0] == 'E') {
         if (e) {
            cout << "Only one exponent per number allowed" << endl; 
            exit(1); 
         }
         e = 1; 
      }

      // Check that only one decimal point is present in a number
      if (json[0] == '.') {
         if (dec) {
            cout << "Only one decimal point per number allowed" << endl; 
            exit(1); 
         }
         dec = 1; 
      }
      // Check that plus and minus signs are properly used
      if (json[0] == '-' || json[0] == '+') {
         if (returnable[returnable.size() - 1] != 'e' && returnable[returnable.size() - 1] != 'E') {
            cout << "Plus and minus can only follow an exponent" << endl; 
            exit(1); 
         }
      }
      returnable+= json[0]; 
      json = json.substr(1); 
   }

   // Check that json doesn't end without a terminating ] or }
   if (json.length() == 0) {
      cout << "Invalid end of input" << endl; 
      exit(1); 
   }
   return returnable;
}


// Determine if the beginning of the argument, json, is a JSON boolean
// If so, return the boolean in string form. Otherwise, return an empty string
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


// Determine if the beginning of the argument, json, is a JSON null
// If so, return null in string form. Otherwise, return an empty string
string lexical_null(string& json) {

   if (json.substr(0, 4) == "null") {
      json = json.substr(4); 
      return "null"; 
   }
   return ""; 
}


string parse_tokens(vector<string>& tokens); 


// Parse a list present within a list of tokens 
// and determine if it aligns with JSON lexical and syntactical guidelines
void parse_list(vector<string>& tokens) {

   // If tokens is empty, the list is not closed 
   if (tokens.size() == 0) {
      cout << "Invalid list" << endl; 
      exit(1); 
   }

   string item = tokens[0]; 

   // Check if list is closed
   if (item == "]") {
      tokens.erase(tokens.begin(), tokens.begin()+1); 
      return; 
   }

   // While there are more tokens, iterate through them
   while (tokens.size()) {

      // Check if the item in the list is a nested list, nested object, or neither
      item = parse_tokens(tokens); 

      // A list cannot end without a ]
      if (tokens.size() == 0) {
         cout << "Invalid list" << endl; 
         exit(1); 
      }

      // Check that the list is ending or that the next item is comma delimited 
      item = tokens[0]; 
      if (item == "]") {
         tokens.erase(tokens.begin(), tokens.begin()+1); 
         return; 
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

   // Lists must end in a bracket; 
   cout << "Missing bracket at end of list" << endl; 
   exit(1); 
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
      while (json[0] == ' ' || json[0] == '\n') {
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
      else if (json[0] != ' ' && json[0] != '\n' && json.length() > 0) {
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
         exit(1); 
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
      data+= temp; 
      data+= "\n"; 
   }
   parse(data);
   input.close(); 
   return 0;
}