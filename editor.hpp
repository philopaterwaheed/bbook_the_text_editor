#ifndef EDITOR_H
#define EDITOR_H
//????????????????????


#include <ncurses.h>
#include "buffer.hpp"



class editor {
int x,y ; // position on screen 
char mode ; 
buf* buffer ;
std :: string status ,save_status,filename;

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void deleteLine();                  // current 
    void deleteLine(int);               // nth line 
    void saveFile();                    

public: 
        editor();
        editor(std::string); // with a file name 
            char getMode() {return mode;}
            void handleInput(int);              // Handles keyboard input
            void printBuff();
            void printStatusLine();             // Prints the status line (like vim!!!)
            void updateStatus();                // Updates the status line (text, not display)
};

#endif