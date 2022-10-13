#include"editor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>  
// ############################# // 
editor:: editor () {
    x=y=0 ; mode = 'n' ;
    status = "normal mode";
    filename= "untitled"; 
    buffer = new buf(); // creats new buffer
    buffer->appendLine(""); // adds an empty line 
    
}


editor::editor(std::string file_name){
    x=y=0;mode='n';
    status = "Normal Mode";
    filename=file_name;
    
    buffer= new buf(); 
    std::ifstream file(file_name); // creates a fstream obj with our file
        if (file.is_open())
            {
                while (!file.eof() )/*while you can read form the file */   
                    {
                        std :: string temp ; // a temp string to store the line 
                        getline(file, temp); // get the line and store it in temp
                        buffer->appendLine(temp); // addes the line to the buffer to show on screen 

                    }

            }

        else {
           std:: cerr << " cannot open file : " << file_name ; //prints the error
            buffer->appendLine("");

        }
}

void editor::updateStatus()
{
    // the status on bottom 
    switch(mode)
    {
    case 'n':
        // view only mode
        status = "view only Mode";
        break;
    case 'i':
        // Insert mode
        status = "Insert Mode";
        break;
    case 'x':
        // Exiting
        status = "Exiting";
        break;
    }
    status += "\tCOL: " + std::to_string(x+buffer->l_char) + "\tROW: " + std::to_string(buffer->line); // takes the position and prints them to string
}


void editor::handleInput(int i)
{
    switch (i) // handles the movment // the i is the key inputed
        {
            case KEY_LEFT:
                moveLeft(); 
                return ;
            case KEY_RIGHT:
                moveRight();
                return ;
            case KEY_UP:
                moveUp();
                return ;
            case KEY_DOWN:
                moveDown();
                return ;
            case 20 : // ctrl + t for the terminal 
                
                terminal->getting_input_term =1;
                terminal_print();
                break;
        }
    switch(mode)
        {
            case 'n': // normal mode has it's own key set we switch between them
                switch(i)
                    {
                        case 27  : // exits by the escape key  
                            mode = 'x' ;
                            break ;
                        case 'i': // switchs to insert mode 
                            mode = 'i';
                            break;
                        case 's' : //saves
                            saveFile();
                            break;
                        case 67 :
                            buffer->is_coping = 1;
                            break;
                    }
                break;
            case 'i': // the insert mode 
                switch (i)
                    {
                        case 27 : // the exit of the insert mode // escape botton
                            mode = 'n';
                            break; 
                        case KEY_BACKSPACE:// handels the delete
                            if(x == 0 && buffer->line > 0 ) // if we are at the begining of a line  that is not the first
                            {
                                x = buffer->lines[buffer->line-1].length(); // changes the x to upove line length 
                                buffer->lines[buffer->line-1] += buffer-> lines[buffer->line] ; // adds the line to the line apove it 
                                deleteLine(); // the line it self didn't get deleted so we have to
                                moveUp();
                            }
                            else if (buffer->line >=0 && x!=0 )
                                {
                                    buffer->lines[buffer->line].erase(--x+buffer->l_char,1); // "we" if the curser on e we delete w that's why we have the --x
                                    // remove a char
                                
                                }

                        
                        break;
                        case KEY_DC : // the delete key
                             if(x == buffer->lines[buffer->line].length() && y != buffer->lines.size() - 1) // if we at the end of a line and it's not the last one 
                                {
                                     buffer->lines[buffer->line] += buffer->lines[buffer->line+1]; // addes the line below 
                                     deleteLine(buffer->line+1); // delete that line below
                                }
                             else
                                buffer->lines[buffer->line].erase(x+buffer->l_char, 1);                             
                        break;
                        case 10: // KEY_ENTER:
                            // puts the rest of the line down 
                            if(x < buffer->lines[buffer->line].length()){ // if we are not at the end of the line
                            buffer->insertLine(buffer->lines[buffer->line].substr(x+buffer->l_char, buffer->lines[buffer->line].length() - x), buffer->line + 1);
                                                //creates a substring of current line                    // adds it to the line below 
                            buffer->lines[buffer->line].erase(x+buffer->l_char, buffer->lines[buffer->line].length() - x);
                            // removes that of line that got down 
                            x=buffer->l_char=0;
                            }
                            else // just adds an empty line 
                                 buffer->insertLine("", buffer->line+1); 
                        x=0 ;  // start at the end of line 
                        moveDown();
                        break;
                    case 9 : // the tab key
                        buffer->lines[buffer->line].insert(x, 4, ' '); // adds four spaces 
                        x+=4;
                        break;
                    case 16 :
                        buffer->past_line(x);
                    default:
                        //for handling any other char
                            buffer->lines[buffer->line].insert(x+buffer->l_char, 1, char(i));
                        if(buffer->l_char==0)
                            x++;
                        else
                          buffer-> l_char++; 
                        save_status= "changes accured please save "; // changes happened so we say so 
                        break;
                    }
            break;  // just breaks the insert mode


                    }
   
}

// movement 
// note move () is a ncurses function to move curser 
void editor:: moveLeft ()
    {
       
        if (x == 0 &&  buffer->l_char !=0 )
                    buffer->l_char -- ;  
        else if (x-1 >= 0 ) // if we are not at the first element
            {
                x--; // move left 
               
            }
        else  // if we are at the begining of line 
            { 
                if (y==0 && buffer->l_line == 0) // makes sure that we are not at the first line of the file to not breake the program 
                    return ; //breaks the unction
                if(y-1 >= 0 ) // if we are not on the first  
                    {
                        y--;
                        buffer->line--;
                        x= buffer->lines[buffer->line].size();
                         buffer->l_char = (COLS < buffer->lines[buffer->line].size())?  buffer->lines[buffer->line].size() - COLS + 
                         1: 0;
                         x = (COLS < buffer->lines[buffer->line].size())?  COLS -1 : buffer->lines[buffer->line].size();
                    }
                else if (y-1 < 0 && buffer->l_line > 0 && buffer->l_line + y < buffer->lines.size()) // if y =0 and there are still lines in the file above 
                    {
                        buffer->l_line -- ;
                        buffer->line--;
                        x= buffer->lines[buffer->line].size();
                        buffer->l_char = (COLS < buffer->lines[buffer->line].size())?  buffer->lines[buffer->line].size() - COLS + 1 : 0;
                        x = (COLS < buffer->lines[buffer->line].size()) ?  COLS - 1  : buffer->lines[buffer->line].size();
                    }
                
            }
            move(y,x);
     
    
}
void editor::moveRight()
{
    if(x /*+ buffer->l_char*/+1 < COLS && x+1 < buffer->lines[buffer->line].length()) // if we move right and still in the line and still in the visible era of ncurses
    {
        x++;
    }
    else if (y+1 <= LINES-11 && y+1 < buffer->lines.size() && x + 1 +buffer->l_char >= buffer->lines[buffer->line].size() )  // if we are at the end of the line  
        {
            y++ ; //move to the line below
            buffer->line++;
            x= 0 ; // at the start of it
            buffer->l_char = 0 ; //(COLS < buffer->lines[buffer->line+1].size())?  buffer->lines[buffer->line+1].size() - COLS + 1 :0;

        }
    else if (x + 1 +buffer->l_char  >= buffer->lines[buffer->line].size() && y >= LINES-11 && buffer->l_line + +LINES-10< buffer->lines.size()) // if we are at the end of last line 
        {
                    buffer->l_line ++;
                    buffer->line++;
                    x=0;
                    buffer->l_char = 0 ; //  (COLS < buffer->lines[buffer->line+1].size())?  buffer->lines[buffer->line+1].size() - COLS + 1 : 0;
        }  
    else if ( x +1 == COLS && x + buffer->l_char< buffer->lines[buffer->line].length() )
        {
            buffer->l_char ++;
            //x++ ; 
        }   
    move(y, x);
}

void editor::moveUp()
{
    
    
        if(y-1 >= 0) // if we are not on the first 
            {
                    y--;
                    buffer->line--;
            }
        if (y-1 < 0 && buffer->l_line > 0 && buffer->l_line+y < buffer->lines.size()) // if we  are at first but there more lines in the file 
            {
                buffer->l_line -- ;
                buffer->line--;
            }

        /*if(x + buffer -> l_char < buffer->lines[buffer->line].size())
            {
                moveRight();
                moveLeft();
            }*/
        if (x + buffer -> l_char >= buffer->lines[buffer->line].size())
            {
                buffer->l_char = (COLS < buffer->lines[buffer->line].size())?  buffer->lines[buffer->line].size() - COLS+1  : 0; 
                x = (COLS < buffer->lines[buffer->line].size()) ?  COLS - 1  : buffer->lines[buffer->line].size();
            }
          
        move(y, x);
    
}

void editor::moveDown()
{
    if(y+1 <= LINES-11 && y+1 < buffer->lines.size())
        {
        buffer->line++;
        y++;
        }

     if (y >= LINES-11 && buffer->l_line + +LINES-10< buffer->lines.size())  // if we are at last but there are more lines in the file 
        {    
            buffer->l_line ++;
            buffer->line++;
        }
   
    // if(x >= buffer->lines[buffer->line].length())
    //     x = buffer->lines[buffer->line].length();
    if (x + buffer -> l_char >= buffer->lines[buffer->line].size())
        {
            buffer->l_char = (COLS < buffer->lines[buffer->line].size())?  buffer->lines[buffer->line].size() - COLS+1  : 0; 
            x = (COLS < buffer->lines[buffer->line].size()) ?  COLS - 1  : buffer->lines[buffer->line].size();  
        }
    refresh();
    move(y, x);
}


void editor::printBuff()
{
    refresh();
    for(int i=0; i<LINES-10; i++) // for every line just before the border of the box
    {   
        
        if( i + buffer->l_line  >= buffer->lines.size( ))
        {
            move(i, 0); // move to the new line 
            clrtoeol(); // clear what is in that line 
        }
        else  if (buffer->l_line > 0) 
        {
            std :: string temp = buffer->lines[i + buffer->l_line ];
            mvprintw(i, 0, "%s", temp.erase(0,buffer->l_char).c_str()); // an ncurses function prints to screen where you want to takes a line of type c string
        
        }
        else
        {
            std:: string temp = buffer->lines[i];
            mvprintw(i, 0, "%s",temp.erase(0,buffer->l_char).c_str() ); // an ncurses function prints to screen where you want to takes a line of type c string
        }
         
         clrtoeol(); // clear the rest of the line 
    }
   
   move(y, x); // move where the curser was before 
}
void editor::printStatusLine()
{
    
   
    refresh(); // refreshs before waiting input
    WINDOW* status_window; //creats a ncur window pointer 
    status_window = newwin(/*height*/8,/*width*/ COLS-10,/*y*/LINES-10,5/*x*/); // creats a new window beacuse all we created is a pointer
    
    box(status_window,0,0); //border for our window
    mvwprintw(status_window,1,1, "%s", status.c_str()); // print the status string
    mvwprintw(status_window,2,1, "%s", save_status.c_str()); 
    mvwprintw(status_window,1,COLS-28, "c_t -> terminal");
    wrefresh(status_window); // refreshes the window

    
    
    
}

void editor::deleteLine()
{
    buffer->removeLine(buffer->line);
    refresh();
}

void editor::deleteLine(int i)
{
    buffer->removeLine(i);
    refresh();
}

void editor::terminal_print()
{

    
    if (terminal->getting_input_term)
        {       
            wclear(terminal->terminal_window);
            terminal -> terminal_window = newwin(/*height*/LINES- 11,/*width*/ COLS-10,/*y*/1,5/*x*/);
            box(terminal -> terminal_window ,0,0);
            wrefresh( terminal -> terminal_window );
            terminal->get_input(); 
            
            
        }
        
   }
void editor::saveFile()
{   
    std::string temp = filename; // to save the file name before edit 
    move(LINES-7,6); // move to where our save status should be
    printw("file name is %s" , filename.c_str()); 
    while (1)
        {
            
            
            int input=getch(); // waits an input 
                if (input == 10) // didn't do switch beacuse a wanted to use the break;
                    break;
                else if (input == KEY_BACKSPACE)    // the erase 
                    {
                        
                            if (filename.size()==0) // if the user goes byond that a buffer over flew happens so we should prevent that
                                {
                                    filename=""; // just to prevent the 
                                }
                            else 
                                filename=filename.erase((filename.length())-1); // erases the last element 
                    }   
                else if (input == 27/* escape */) // if user wants to cancle 
                    {
                        filename=temp; // returns the file to how it was 
                        return; // exits the function
                    }
                else
                    filename+=char(input);
            move(LINES-7,6); // move to the orginal point 
            clrtoeol(); // clear the line
            printw("file name is %s" , filename.c_str()); //prints the file name the way it became 
            
        }
    if(filename == "")
    {
        // Set filename to untitled
        filename = "untitled";
        
    }

    std::ofstream f(filename);
    if(f.is_open())
    {
        
        for(int i=0; i<buffer->lines.size(); i++)
        {
            f << buffer->lines[i] << std::endl;
        }
       save_status = "Saved to file to " +filename +" !"; // displays where is the file saved
        
    }
    else
    {
        status = "Error: Cannot open file for writing!";
    }
    f.close();
    
}


