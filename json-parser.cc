// json-parser.cc
// Creates a JSON parser 
// Anna Leitner: May 2024

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std; 

// TO MAYBE DO: 
// Add the backslash rule to the NOT LIST part of the code 
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
   int squares = 0; 
   int first = 0; 
   int key = 0; 
   int val = 0; 
   int quotes = 0; 
   int num = 0; 
   int nullval = 0; 
   int boolean = 0; 
   string value = "";

   // list enforcers: 
   int character = 0; 
   int st = 0; 
   int number = 0; 
   int boo = 0; 
   int nul = 0; 
   int comma = 0; 
   int backslash; 
   while(input >> word) {
      //cout << "word: " << word << " " << key << " " << val; 
      if (word == '{') {
         brackets++; 
      }
      else if (word == '}') {
         brackets--; 
         val = 0; 
      }
      if (word == '[') {
         squares++; 
      }
      if (word == ']') {
         if (comma) {
            cout << "Invalid comma in list" << endl; 
            return 1; 
         }
         squares --; 
      }
      else if (brackets == 0 && word != '}' && !squares) {
         cout << "Improperly placed input" << endl; 
         return 1; 
      }
      if (first == 0) {
         if (brackets != 1 && squares != 1) {
            cout << "Must begin syntax with { or [" << endl; 
            return 1; 
         }
         else {
            first = 1; 
         }
      }
      if (squares < 0) {
         cout << "Invalid brackets" << endl; 
         return 1; 
      }
      if (brackets < 0) {
         cout << "Invalid curly braces" << endl; 
         return 1; 
      }
      // Checking keys and values
      if (key == 0 && word == '"' && val == 0 && brackets) {
         key++; 
      }
      else if (key == -1 && word == '"' && val == 0 && brackets) {
         key = 1; 
      }
      else if (key == 1 && word == '"' && brackets) {
         key = 0; 
      }
      if (word == ':' && key != 0 && brackets) {
         cout << "Invalid Key" << endl; 
         return 1; 
      }
      else if (word == ':' && brackets) {
         val = 1; 
      }
      else if (word == ',' && brackets) {
         if (!val) {
            cout << "Stray comma" << endl; 
            return 1; 
         }
         val = 0; 
         if (quotes != 0 && quotes != 2) {
            cout << "Invalid string value" << endl; 
            return 1; 
         }
         if (nullval && value != "null") {
            cout << "Invalid null value" << endl; 
            return 1; 
         }
         if (boolean && value != "true" && value != "false") {
            cout << "value: " << value << endl; 
            cout << "Invalid boolean value" << endl; 
            return 1; 
         }
         quotes = 0; 
         key = -1; 
         num = 0; 
         nullval = 0; 
         boolean = 0; 
         value = ""; 
      }
      else if (word != '"' && word != '{' && word != '[' && word != ' ' && key != 1 && val == 0 && brackets) {
         cout << "Unquoted key" << endl; 
         return 1; 
      }
      if (val && squares == 0 && word != ']') {
         if (word == '"') {
            quotes++; 
         }
         // we know then that we're in a bool or a numeric or a null
         if (quotes == 0 && word != ':') {
            if ((num) && !isdigit(word)) {
               cout << "Invalid number value" << endl; 
               return 1; 
            }
            else if (isdigit(word) && !boolean && !nullval && !num) {
               if (word == '0') {
                  cout << "Leading zeroes not allowed" << endl; 
                  return 1; 
               }
               num = 1; 
            }
            // nested
            else if (word == '{') {
               val = 0; 
            }

            else if (!boolean && !nullval) {
               if (word == 't' || word == 'f') {
                  boolean = 1; 
               }
               else if (word == 'n') {
                  nullval = 1; 
               }

               if (word != ' ' && !boolean && !nullval && !isdigit(word)) {
                  cout << "Invalid value" << endl; 
                  return 1; 
               }
            }
            if (word != ' ' && (boolean || nullval)) {
               value+= word; 
            }
            if (word == ' ' && (boolean || nullval || num)) {
               cout << "Invalid Space" << endl; 
               return 1; 
            }
            
         }
      }
      // we're in a list and we need to enforce norms for inside a list
      else if (squares == 1 && word != '[') {
         // need to check that backslash chars are caught 
         if (word == '\\' && (character || st)) {
            backslash = 1; 
         }
         else if (word == '\\' && !character && !st) {
            cout << "Backslash outside of string or character" << endl; 
            return 1; 
         }
         else if (backslash) {
            if (word == 'x') {
               cout << "Illegal escape" << endl; 
               return 1; 
            }
         }
         else {
            backslash = 0; 
         }
         if (word == ',' && (st != 0 || character != 0 || number != 0 || boo != 0 || nul != 0)) {
            if (character != 0) {
               cout << "Improper character in list" << endl;
               return 1; 
            }
            if (st != 0) {
               cout << "Improper string in list" << endl; 
               return 1; 
            }
            if (boo && value != "true" && value != "false") {
               cout << "value: " << value << endl; 
               cout << word << endl; 
               cout << "Improper boolean in list" << endl; 
               return 1; 
            }
            if (nul && value != "null") {
               cout << "Improper null value in list" << endl; 
               return 1; 
            }
            
            character = 0; 
            st = 0; 
            number = 0; 
            boo = 0; 
            nul = 0; 
            comma = 1; 
            value = ""; 
         }
         else if (word == ','){
            cout << "Improper list comma" << endl;
             return 1; 
         }
         // setting type for each item in a list
         else if (word == '\'' && !character) {
            character = 1; 
         }
         else if (word == '\'') {
            character = 0; 
            if (value.length() > 3) {
               cout << "Improper character in list" << endl; 
               return 1; 
            }
         }
         else if (word == 'n' && !st && !character) {
            nul = 1; 
         }
         else if (word == '"' && !st) {
            st = 1; 
         }
         else if (word == '"') {
            st = 0; 
         }
         else if ((word == 'f' || 't') && !st && !character) {
            boo = 1; 
         }
         if (word != ',') {
            comma = 0; 
         }
         // Checking bools and nulls
         if (boo || nul || character) {
            value+= word; 
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
   if (key != 0) {
      cout << "Unclosed key" << endl; 
      return 1; 
   }
   if (val != 0) {
      cout << "Unclosed val" << endl; 
      return 1; 
   }
   if (brackets != 0 || squares != 0) {
      cout << "Unbracketed input" << endl; 
      return 1; 
   }
   input.close(); 
   return 0;
}