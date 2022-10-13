#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <ncurses.h>
class Term {
public:
    char * term_input = (char*) "" ; 
    bool getting_input_term ;
   void term_get_in()
    {
        
            while (1)
                { 
                    int input = getch();
                    if ( input == 10)  
                            break;
                   
                    term_input += char (input);
                        
                }
    }


   std::string  Term_output () {
    
     FILE * put = popen(term_input , "r"); // a pointer to a stream object 
     std::ostringstream output ; // sstream output object 
     constexpr std::size_t MAX_LINE_SZ = 1024 ; 
     char line[MAX_LINE_SZ] ; // an array of chars 
        while( fgets( line, MAX_LINE_SZ, put ) )  // turns a string to a stream 
            output << line << '\n' ; 
        return output.str();
    }
};
int main () {
    initscr(); // starts in nc mode 
        noecho();  // don't echo keystrokes
        cbreak();  // stops line buffering
        keypad(stdscr , true); 
 Term terminal ; 
   // terminal.term_get_in();
  std:: cout << terminal.Term_output().c_str();
     refresh();                      // Refresh display
  //  endwin();                       // End ncurses mode
        return 0;

}