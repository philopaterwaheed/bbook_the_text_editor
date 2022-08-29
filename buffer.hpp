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
        int line ; // the line we are at  
        bool is_coping = 0 ; 
        std :: string temp_line; // used in copy and past 
        std :: vector <std::string> lines ; 
        void insertLine(std::string  , int); // repalce a line with anthor
        void appendLine(std::string);
        void removeLine(int);
        std::string remTabs(std::string);
        std::string copy_line (int); 
        std::string cut_line (std::string) ; 
        std::string past_line (int) ; 

}; 
#endif