#ifndef BUFFER_H
#define BUFFER_H
// @@@@@@@@@@@@@@@@@@ // 
#include <string>
#include <vector>
// ################## // 


struct buf {
        buf(); 
        int l_line = 0 ; // this should be the last showable line it increases if we reach the last line of the page so we can print the LINES -10 line in the print
        int l_char = 0 ;
        int line ; // the line we are at (a replacement of ncurses's x, y the moving functions handels it  )
        bool is_coping = 0 ; 
        std::string goto_line; 
        std :: string temp_line; // used in copy and past 
        std :: vector <std::string> lines ; 
        void insertLine(std::string  , int ); // repalce a line with anthor
        void appendLine(std::string);
        void removeLine(int);
        void injectIntoLine(std::string,int);
        int space_for_numbers(int);
        std::string remTabs(std::string);
        std::string copy_line (int); 
        std::string cut_line (std::string) ; 
        std::string paste_line (int, int x =0 ) ; 

}; 
#endif