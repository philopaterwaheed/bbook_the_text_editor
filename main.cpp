#include <ncurses.h>
#include <string>
#include "editor.hpp"
// vars / /
std::string file_name ; 
editor Editor;
//##################################//   
    void curse_init(){
        initscr(); // starts in nc mode 
        noecho();  // don't echo keystrokes
        cbreak();  // stops line buffering
        keypad(stdscr , true); 
        //start_color(); 


        }
// ################################ //
int main (int argc, char *argv[]) {

// to take the file name 
        if (argc > 1) 
            {
                file_name = argv[1];
                Editor = file_name; // if the user provided a file name to edit // the = is just a way to call the constructor
            }
        else // if he didn't
                Editor = {}; // equals editor Editor (); 
//***********************// 
curse_init() ; // time to start the curse 

    while(Editor.getMode() != 'x')
{
    Editor.updateStatus();
    Editor.printStatusLine();
    Editor.printBuff();
    int input = getch();                // gets input and Blocking until input 
    Editor.handleInput(input);

}

        refresh();                      // Refresh display
        endwin();                       // End ncurses mode
        return 0;
}        