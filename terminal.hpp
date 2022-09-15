#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <ncurses.h>
class Term {

public:
    WINDOW* terminal_window; //creats a ncur window pointer 
    std::string term_input = ""  ; 
    std :: string temp = "",tempo;
    bool getting_input_term = 0;
    int inputy =0 , pushed_t_buffer = 0 , line=0 ;
    std :: string term_output ="";
    std :: vector <std::string> terminal_buffer = {""};
    std::string Term_push () ;
    void get_input();
    void terminal_buffer_print ();
    void terminal_if_max_clear_buffer  ();

};
