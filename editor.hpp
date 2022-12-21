#ifndef EDITOR_H
#define EDITOR_H
//????????????????????


#include <ncurses.h>
#include "buffer.hpp"
#include "terminal.hpp"
#include "history.hpp"

class editor {
int x  ,y , space_for_numbers ;  // position on screen 
char mode ; 
buf * buffer ;
Term * terminal = terminal = new Term;
WINDOW* main_win ;//=newwin(/*height*/LINES-10,/*width*/ COLS-space_for_numbers,/*y*/0,space_for_numbers/*x*/); ; 
std :: string status ,save_status,filename;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void deleteLine();                  // current 
    void deleteLine(int);               // nth line 
    void saveFile();                    
    int goto_firstofline();
    int  at_first_line(); 
public: 
        
        editor();
        editor(std::string); // with a file name 
            char getMode() {return mode;}
            void handleInput(int);              // Handles keyboard input
            void printBuff();
            void printStatusLine();             // Prints the status line (like vim!!!)
            void print_Numbers();
            void updateStatus();                // Updates the status line (text, not display)
            void terminal_print (); 
            
    
    };

#endif