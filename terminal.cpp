#include "terminal.hpp"
#include<iostream>

   void Term :: get_input()
    {
        
       // auto push_fun= &Term_output;
            while (1)
                { 
                    inputy = getch();
                    switch(inputy)
                    {        
                            case 10: //enter key 
                                {  
                                     this->Term_push();
                                    line ++ ;
                                }
                            break;
                            case KEY_UP: // key yp for hestory
                                if (line %2 ==0 && line >=0 )
                                    temp= terminal_buffer[line-2];
                                else if (line %2 !=0 && line >=0 )
                                     temp= terminal_buffer[line-1];
                            terminal_if_max_clear_buffer () ;
                            terminal_buffer.pop_back();
                            terminal_buffer.push_back(temp); 
                            terminal_buffer_print ();
                            break;
                            case KEY_BACKSPACE: // to delete 
                                    temp = temp.substr(0,temp.size()-1); 
                                    terminal_if_max_clear_buffer () ;
                                    terminal_buffer.pop_back();
                                    terminal_buffer.push_back(temp); 
                                    terminal_buffer_print ();
                                break;
                            case 20 : // cntrol + t to delete the object
                                //this->~Term();
                                return;
                            default:
                                
                                temp +=char(inputy) ;
                                terminal_if_max_clear_buffer () ;
                                terminal_buffer.pop_back();
                                terminal_buffer.push_back(temp);
                                terminal_buffer_print ();
                                break;
        
                    }
                    
                }
         terminal_buffer_print ();
    }


std::string Term::  Term_push  ()
    {
                
            char command[128] ;
            term_output="";
            // terminal_buffer.push_back(temp);
            // terminal_buffer_print (); 
            // Open pipe to file

            freopen("/dev/null", "w", stderr); // to get red of the stderr becuse it messes the hole thing
            FILE* pipe = popen(temp.c_str(), "r");

            if (!pipe) {
                return "popen failed!";
            }

            // read till end of process:
            while (!feof(pipe)) {

                // use buffer to read and add to result
                if (fgets(command, 128, pipe) != NULL )
                   term_output += command;
                   
            }

            pclose(pipe);
                                                std::stringstream buffer;
            std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());
            std::cout << buffer.str()<< std::endl;
            term_output += buffer.str();
              for (int i =0 ; i < term_output.size() ; i++) // this for loop is to clear \n from the string becuase it messes the output
                {
                    terminal_if_max_clear_buffer () ;
                    if (term_output.find("\n")<term_output.size())
                        {
                            //terminal_if_max_clear_buffer () ;
                            terminal_buffer.push_back(term_output.substr(0,term_output.find("\n"))); // pushes the line 
                            term_output = term_output.substr(term_output.find("\n")+1); // cuts the string after the \n
                            line ++ ;
                        }
                    else 
                        {
                            //terminal_if_max_clear_buffer () ;
                            terminal_buffer.push_back(term_output); // last one it wont find ant so we push the remaining 
                             line ++ ;
                             break;
                             
                        }
                }

            terminal_buffer.push_back("");
            terminal_buffer_print (); 
            temp = "" ;
            line ++ ;
            return term_output;

  
    }
void Term:: terminal_buffer_print (){
    
    for (int i = 0 ; i< terminal_buffer.size() ; i++ )
        {   
            mvprintw(i+3,7, "%s",terminal_buffer[i].c_str());
            clrtoeol();
        }

    
    wrefresh(  terminal_window );
    

}
void Term:: terminal_if_max_clear_buffer (){
    if (terminal_buffer.size()>LINES-15)
        {
           terminal_buffer.clear();
            line=0;
            terminal_buffer.push_back("");
            move(4,7); // just for clearing the seconed line  from previos output;
            clrtoeol();
        }
    else 
        return ;


}


