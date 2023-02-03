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
        int line_st_highlight  = -1 , line_lst_highlight = -1 ;
	int first_ch_highlight = -1 , last_ch_highlight = -1 ;
        bool is_coping = 0 ; 
        std::string goto_line; 
        std :: string temp_line; // used in copy and past 
        std :: vector <std::string> lines ; 
        std:: vector<std::string > lines_paste ;   
        void insertLine(std::string  , int ); // repalce a line with anthor
        void appendLine(std::string);
        void removeLine(int);
        void injectIntoLine(std::string,int);
        int space_for_numbers(int);
        std:: vector<std::string> getStrings_from_lines(bool ,int ,int ,int ,int); // the bool is for cutting it or not   
        std::string remTabs(std::string);
        void copy_line (int ,int ,int ,int ); 
        std::string cut_line (int ,int ,int ,int ) ; 
        std::string paste_line (int,  std::vector <std::string> & lines_paste , int x =0 ) ; 

}; 
#endif