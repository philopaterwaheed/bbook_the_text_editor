#include"editor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>  
// ############################# // 
editor:: editor () 
    {
        x=y=0 ; mode = 'n' ;
        status = "normal mode";
        filename= "untitled"; 
        buffer = new buf(); // creats new buffer
        buffer->appendLine(""); // adds an empty line 
        space_for_numbers = buffer->space_for_numbers(buffer->lines.size());
    }

editor::editor(std::string file_name)
{
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
    space_for_numbers = buffer->space_for_numbers(buffer->lines.size()); // take the max digits number and store them in a var to make space for line number 
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
    status += "\tCOL: " + std::to_string(x+buffer->l_char) + "\tROW: " +std::to_string(buffer->line)+ " "+ std::to_string(buffer->first_ch_highlight) + std::to_string(buffer->last_ch_highlight) +" "" "+buffer->goto_line; // takes the position and prints them to string
}

void editor::handleInput(int i)
{
    switch (i) // handles the movment // the i is the key inputed // universal
        {
            case KEY_LEFT:
                moveLeft(); 
                break_highlight();
                return ;
            case KEY_RIGHT:
                moveRight();
                break_highlight(); 
                return ;
            case KEY_UP:
                moveUp();
                break_highlight();
                return ;
            case KEY_DOWN:
                moveDown();
                break_highlight(); 
                return ;
            case 262 : // home botton
                goto_firstofline() ;
                break_highlight();
                return ; //to prevent typing it's code while in insert 
            case 7:
                char goto_line[20];
                scanw("%19s",goto_line);
                buffer->goto_line=goto_line;
                int temp ;
                char * end ; 
                temp=std::strtol(goto_line , &end , 0 ); // end stores the first char after the int 
                if (temp>=0 && temp < buffer->lines.size())
			buffer->line= temp ;
		else 
			return;
            	if (buffer->l_line+temp- buffer->l_line>=0 && buffer->l_line+ temp- buffer->l_line < buffer->lines.size() )
                buffer->l_line+=temp- buffer->l_line;                       
                goto_firstofline();
                y=0;
                break_highlight();
                return ;
            case 20 : // ctrl + t for the terminal 
                
                terminal->getting_input_term =1;
                terminal_print();
                break;
            case 360 : // end button 
                goto_lastof_line();
                return; 
            case  393 : // shiftleft
                highlight_logic(393);
                return; 
            case  402  : // shift right
                highlight_logic(402);
                return ; 
            case  337 :  // shift up
                highlight_logic(337);
                return;
            case  336:  // shift down
                highlight_logic(336);
                return;
        }
    switch(mode)
        {
            case 'n': // normal mode has it's own key set we switch between them
                // this is the logic for the normal mode 
                switch(i) // for any input while in normal mode 
                    {
                        case 27  : // exits by the escape key  
                            mode = 'x' ;
                            break;
                        case 'i': // switchs to insert mode 
                            mode = 'i';
                            break;
                        case 's' : //saves
                            saveFile();
                            break;
                        case KEY_DC : // the delete key // deletes the whole line in normal modde 
                                    goto_firstofline();
                                    switch (at_first_line())// made this condition just to debug a problem
                                    {
                                        case 1 :
                                            {
                                                deleteLine();
                                                moveUp();        
                                                break ;                                  
                                            }
                                        case 2:
                                            { 
                                                moveUp(); 
                                                deleteLine(buffer->line+1);
                                                break; 
                                            }
                                        case 0:
                                            {  
                                                if (buffer->lines.size() ==1 )
                                                {
                                                    buffer ->appendLine("") ;
                                                    deleteLine();
                                                }
                                                else 
                                                    deleteLine();
                                                break;
                                            
                                            }                               
                                       
                                    }
                                break;
                        case 9 : 
                            {
                                for (int v = 0 ; v  < 4 ; v++)
                                moveRight();
                            }
                            break;
                        case 67 : //shift + c 
                            buffer->copy_line(buffer->first_ch_highlight , buffer->last_ch_highlight , buffer->line_st_highlight,buffer->line_lst_highlight ) ;
                            break ; 
                        case 88 : // shift + x 
                            buffer->cut_line(buffer->first_ch_highlight , buffer->last_ch_highlight , buffer->line_st_highlight,buffer->line_lst_highlight ) ;
                            break; 
                        case 86 : // shift + v 
                            buffer->paste_line(buffer->line , buffer->lines_paste ,x+ buffer->l_char) ; 
                            break;
                        default : 
                            return ; 
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
                                goto_lastof_line(1);
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
                                    buffer->lines.shrink_to_fit();
                                }
                             else
                                buffer->lines[buffer->line].erase(x+buffer->l_char, 1); // enters the vector of strings thins deletes the bufferlinesth line and starts deleting form it                             
                        break;
                        case 10: // KEY_ENTER:
                                // puts the rest of the line down 
                                if(x < buffer->lines[buffer->line].length()) // if we are not at the end of the line
                                {
                                    buffer->insertLine(buffer->lines[buffer->line].substr(x+buffer->l_char, buffer->lines[buffer->line].length() - x), buffer->line + 1);
                                                    //creates a substring of current line                    // adds it to the line below 
                                    buffer->lines[buffer->line].erase(x+buffer->l_char, buffer->lines[buffer->line].length() - x);
                                        // removes that of line that got down 
                                    goto_firstofline();
                                    
                                }
                                else // just adds an empty line 
                                    buffer->insertLine("", buffer->line+1); 
                            goto_firstofline();
                            moveDown();
                            break;
                        case 9 : // the tab key
                            buffer->lines[buffer->line].insert(x+buffer->l_char, 4, ' '); // adds four spaces  to x + l_char
                            
                            break;
                        case 16 :
                           // buffer->past_line(x);
                        default:
                            //for handling any other char
                            buffer->lines[buffer->line].insert(x+buffer->l_char, 1, char(i));
                            if(buffer->l_char==0 || x +1 < COLS - space_for_numbers  && x + buffer->l_char< buffer->lines[buffer->line].length())
                                x++;
                            else
                                buffer-> l_char++; 
                            save_status= "changes accured please save "; // changes happened so we say so 
                            break;
                    }
            break;  // just breaks the insert mode


                    }

if ( at_lastof_line () == 3  ) // make sure every command will be line index safe 
{
goto_lastof_line ();
}
space_for_numbers = buffer->space_for_numbers(buffer->lines.size()); // updates the spcace for the numbers  
break_highlight(); // every action breaks the highlight also every return should do  
}

//  ### movement ###
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
                        buffer->l_char = (COLS - space_for_numbers  < buffer->lines[buffer->line].size())?  buffer->lines[buffer->line].size() - COLS + 1 + space_for_numbers : 0;
                        x = (COLS - space_for_numbers < buffer->lines[buffer->line].size())?  COLS -1 - space_for_numbers  : buffer->lines[buffer->line].size();
                    }
                else if (y-1 < 0 && buffer->l_line > 0 && buffer->l_line + y < buffer->lines.size()) // if y =0 and there are still lines in the file above 
                    {
                        buffer->l_line -- ;
                        buffer->line--;
                        x= buffer->lines[buffer->line].size();
                        buffer->l_char = (COLS - space_for_numbers  < buffer->lines[buffer->line].size())?  buffer->lines[buffer->line].size() - COLS + 1 - space_for_numbers : 0;
                        x = (COLS - space_for_numbers  < buffer->lines[buffer->line].size()) ?  COLS - space_for_numbers - 1  : buffer->lines[buffer->line].size();
                    }
                
            }
            wmove(main_win,y, x);
     
    
}
void editor::moveRight()
{
    //if(x /*+ buffer->l_char*/+ 1  < COLS - space_for_numbers  && x  < buffer->lines[buffer->line].length()) // if we move right and still in the line and still in the visible erea of ncurses
    if( at_lastof_line() == 0 )
    {
        x++;
    }
    else if ( y+1 <= LINES-11 && y+1 < buffer->lines.size() && x  +buffer->l_char  >= buffer->lines[buffer->line].size() && buffer->line + 1 < buffer->lines.size() )  // if we are at the end of the line  
        {
            goto_firstofline();
            y++ ; //move to the line below
            buffer->line++;
            

        }
    else if (at_lastof_line() ==1 ) // if we are at the end of last line 
        {           
            goto_firstofline();
            buffer->l_line ++;
            buffer->line++;
                    
        }  
    else if ( at_lastof_line() == 2 )
        {
            buffer->l_char ++;
            //x++ ; 
        }   
    wmove(main_win,y, x);
}

void editor::moveUp()
{
    
    
        switch (at_first_line ()) 
            {
             case 1 : // if we are not on the first 
                {
                    y--;
                    buffer->line--;
                    break ; 
                }
             case 2 : // if we  are at first but there more lines in the file 
                {
                    buffer->l_line -- ;
                    buffer->line--;
                    break ; 
                } 
            }
            if ( at_lastof_line () == 3  ) 
            {
               goto_lastof_line ();
            }
          
        wmove(main_win,y, x);
    
}

void editor::moveDown()
{    
    if (y+1 > LINES-11 && buffer->l_line + LINES-10 < buffer->lines.size() && buffer->line + 1 < buffer->lines.size())  // if we are at last but there are more lines in the file 
        {    
           buffer->l_line ++;
            buffer->line++;
        }
    else if(y+1 <= LINES-11 && y < buffer->lines.size() && buffer->line + 1 < buffer->lines.size() )
        {
            buffer->line++;
            y++;
        }
     if ( at_lastof_line () == 3 ) 
        {
            goto_lastof_line();
        }
    refresh();
    wmove(main_win,y, x);
    
}
//#######printing #####

void editor::printBuff()
{
    
    
    
    WINDOW* line_num_win ; 
    main_win = newwin(/*height*/LINES-10,/*width*/ COLS-space_for_numbers,/*y*/0,space_for_numbers/*x*/);
    line_num_win = newwin (/*height*/LINES-10,/*width*/ space_for_numbers ,/*y*/0,0/*x*/) ;
    refresh();
    //wchgat( main_win,-1, A_REVERSE , 0 , NULL );
    for(int i=0 ; i<LINES-10; i++) // for every line just before the border of the box
    { 
        wattrset(line_num_win,A_BOLD );
        if( i + buffer->l_line  >= buffer->lines.size(          ) )
        {
             // move to the new line
            wmove(main_win,i, 0);
            wclrtoeol(main_win);
            wmove(line_num_win,i, 0);
            wclrtoeol(line_num_win); // clear what is in that line 
        }
        else  if (buffer->l_line > 0) 
        {
            std :: string temp = buffer->lines[i + buffer->l_line ];
            mvwprintw(line_num_win,i, 0, "%s", (std::to_string(i + buffer->l_line )+"|" ).c_str() );
            temp.erase(0,buffer->l_char);
            for (int o =0 ; o <temp.size()  ; o++)
            {
                               
                if (( i + buffer->l_line  >= buffer->line_st_highlight &&  i + buffer->l_line <= buffer->line_lst_highlight) && buffer->line_st_highlight != buffer->line_lst_highlight)
                 {
                    if (( i + buffer->l_line  == buffer->line_st_highlight && o >=  buffer->first_ch_highlight ) || ( i + buffer->l_line > buffer->line_st_highlight &&  i + buffer->l_line < buffer->line_lst_highlight ) || ( i + buffer->l_line  == buffer-> line_lst_highlight && o <= buffer->last_ch_highlight )) 
                        wattrset(main_win,A_REVERSE);
                    else 
                        wattroff(main_win,A_REVERSE); 
                 }
                 else if ( i + buffer->l_line  == buffer->line_st_highlight &&   i + buffer->l_line  == buffer->line_lst_highlight && buffer->line_st_highlight == buffer->line_lst_highlight) 
                    {
                        if (( o >= buffer->first_ch_highlight  && o <= buffer->last_ch_highlight ))
                            wattrset(main_win,A_REVERSE);
                         else
                            wattroff(main_win,A_REVERSE); 
                    }
                 else 
                   wattroff(main_win,A_REVERSE);  
                mvwprintw(main_win,i, o, "%c", (temp)[o] ); // an ncurses function prints to screen where you want to takes a line of type c string                   

            }
        }   
        
        else
        {
            std:: string temp = buffer->lines[i]; 
            mvwprintw(line_num_win,i, 0, "%s", (std::to_string(i + buffer->l_line )+"|" ).c_str() );
            temp.erase(0,buffer->l_char) ;
            for (int o = 0 ; o < temp.size()  ; o++)
            {
                
                if ((i >= buffer->line_st_highlight &&  i <= buffer->line_lst_highlight) && buffer->line_st_highlight != buffer->line_lst_highlight)
                 {
                    if ((i == buffer->line_st_highlight && o >=  buffer->first_ch_highlight - buffer->l_char ) || (i > buffer->line_st_highlight && i < buffer->line_lst_highlight ) || (i == buffer-> line_lst_highlight && o <= buffer->last_ch_highlight - buffer->l_char )) 
                        wattrset(main_win,A_REVERSE);
                    else 
                        wattroff(main_win,A_REVERSE); 
                 }
                 else if (i == buffer->line_st_highlight &&  i == buffer->line_lst_highlight ) 
                    {
                        if (( o >= buffer->first_ch_highlight - buffer->l_char  && o <= buffer->last_ch_highlight - buffer->l_char ))
                            wattrset(main_win,A_REVERSE);
                         else
                         wattroff(main_win,A_REVERSE); 
                    }
                 else 
                   wattroff(main_win,A_REVERSE);  
                mvwprintw(main_win,i, o, "%c" , temp[o] ); // an ncurses function prints to screen where you want to takes a line of type c string
             }  
                
        }   
        
        // highlighting the selected 

         wclrtoeol(line_num_win);
         wclrtoeol(main_win); // clear the rest of the line 
         
    }
    
    

    wmove(main_win,y, x);//   
    wrefresh(line_num_win);
    wrefresh(main_win);
    
    // move where the curser was before 
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
    
    
    // Check if running with sudo
        if (geteuid() == 0) {
            // Change file permissions to make it writable
            if (chmod(filename.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) != 0) {
                std :: cerr << filename << " can't be written to" ;
                save_status = "cannot write into " +filename +" !";
            }
        }
    std::ofstream f (filename , std::ios::out);
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
    // Change file permissions back to read-only if running with sudo
    if (geteuid() == 0) {
        if (chmod(filename.c_str(), S_IRUSR | S_IRGRP | S_IROTH) != 0) {
            // Handle error
        }
    }
}
//##### place checing ######
void editor:: goto_lastof_line (int n  )
{
    buffer->l_char = (COLS - space_for_numbers < buffer->lines[buffer->line-n].size())?  buffer->lines[buffer->line-n].size() - COLS + 1 + space_for_numbers : 0;
    x = (COLS - space_for_numbers < buffer->lines[buffer->line-n].size()) ?  COLS - 1 - space_for_numbers  : buffer->lines[buffer->line-n].size(); 
}
int editor:: goto_firstofline()
{
    if (x==0 && buffer->l_char ==0 ) // just a check if we  were at the begin befor calling the function 
        return 1;   
    x=buffer->l_char=0;
    return 0 ; 
}
int editor:: at_lastof_line() 
{
    if (x + 1 + buffer->l_char  >= buffer->lines[buffer->line].size() && y >= LINES-11 && buffer->l_line +LINES-10< buffer->lines.size()) // if we are at the end of last line )
        return 1;
    else if(x +1  == COLS - space_for_numbers && x + buffer->l_char  < buffer->lines[buffer->line].length() ) // if we are at the last of the page but still there is in the page
        return 2 ;
    else if (x + buffer -> l_char >= buffer->lines[buffer->line].size()) // if we are beyond it 
        return 3;
    else 
        return 0 ; 
}
int editor:: at_first_line ()
{
    if (y-1 >= 0 ) // if we are not on the first 
        return 1 ; 
    else if (y-1 < 0 && buffer->l_line > 0 && buffer->l_line+y < buffer->lines.size()) // if we  are at first but there more lines in the file 
        return 2 ;
    else //if (y ==0 && buffer->l_line == 0 && buffer->l_line+ y == buffer->lines.size() )// at the first line and it is the first line of the file 
        return 0 ; 
}
void  editor::highlight_logic (int input ) {
    switch (input)
    {
        case 393 : // sheft +left 
    
            if  (buffer->line_st_highlight == -1 && buffer->line_lst_highlight == -1  ) // checks the line we are at after moving 
            { 
                
                if  (buffer->last_ch_highlight == -1 )  
                {    
                    buffer->last_ch_highlight = buffer-> first_ch_highlight= buffer->l_char + x ;
                    buffer->line_lst_highlight  = buffer->line_st_highlight =  buffer->line  ; 
                     
                }
            }
            else  
                {
                if (x + buffer->l_char  == buffer->first_ch_highlight && buffer->line == buffer->line_st_highlight) 
                    {
                        moveLeft();
                        buffer-> first_ch_highlight = buffer->l_char + x ;
                        buffer->line_st_highlight =  buffer->line ;
                    }
                else if ( x+ buffer->l_char == buffer->last_ch_highlight && buffer->line == buffer->line_lst_highlight)
                    {
                        moveLeft();  
                       buffer->last_ch_highlight = buffer->l_char + x ;
                        buffer->line_lst_highlight =  buffer->line ;
                        break;
                    }

                }
            
        break; 
    case 402: // shift right 
            if  (buffer->line_st_highlight == -1 && buffer->line_lst_highlight == -1  ) // checks the line we are at after moving 
                { 
                
                if  (buffer->last_ch_highlight == -1 )  
                {    
                    buffer->last_ch_highlight = buffer-> first_ch_highlight= buffer->l_char + x ;
                    buffer->line_lst_highlight  = buffer->line_st_highlight = buffer->line  ; 
                }
            }
            else  
                {
                if (x + buffer->l_char  == buffer->last_ch_highlight && buffer->line == buffer->line_lst_highlight) 
                    {
                        moveRight();
                        buffer-> last_ch_highlight = buffer->l_char + x ;
                        buffer->line_lst_highlight =  buffer->line ;
                    }
                else if ( x+ buffer->l_char == buffer->first_ch_highlight && buffer->line == buffer->line_st_highlight)
                    {
                        moveRight();  
                        buffer->first_ch_highlight = buffer->l_char + x ;
                        buffer->line_st_highlight =  buffer->line ;
                    }

                }
        break;
    case 337 :
        if  (buffer->line_st_highlight == -1 && buffer->line_lst_highlight == -1  ) // checks the line we are at after moving 
                { 
                
                if  (buffer->last_ch_highlight == -1 )  
                {    
                    buffer->last_ch_highlight = buffer->l_char + x ;
                    buffer->line_lst_highlight  = buffer->line  ;
                    moveUp() ; 
                    buffer-> first_ch_highlight = buffer->l_char + x ;
                    buffer->line_st_highlight = buffer->line  ;
                }
            }
        else  
            {
                if (x + buffer->l_char  == buffer->first_ch_highlight && buffer->line == buffer->line_st_highlight) 
                    {
                        moveUp();
                        buffer-> first_ch_highlight = buffer->l_char + x ;
                        buffer->line_st_highlight =  buffer->line ;
                    }
                else if ( (x+ buffer->l_char == buffer->last_ch_highlight && buffer->line == buffer->line_lst_highlight))
                    {
                        moveUp();  
                        
                       buffer->last_ch_highlight = buffer->l_char + x ;
                        buffer->line_lst_highlight =  buffer->line ;
                        if ( buffer->line_lst_highlight==  buffer->line_st_highlight && buffer->last_ch_highlight < buffer->first_ch_highlight ) // if we moved to a place before the last one 
                            {
                                int temp = buffer->first_ch_highlight  ;
                                buffer->first_ch_highlight = buffer->last_ch_highlight;
                                 buffer->last_ch_highlight =temp;  
                            }
                    }

                }
        break;  
    case 336 : 
        if  (buffer->line_st_highlight == -1 && buffer->line_lst_highlight == -1  ) // checks the line we are at after moving 
                { 
                
                if  (buffer->last_ch_highlight == -1 )  
                {    
                    buffer->first_ch_highlight = buffer->l_char + x ;
                    buffer->line_st_highlight  = buffer->line  ;
                    moveDown() ; 
                    buffer-> last_ch_highlight = buffer->l_char + x ;
                    buffer->line_lst_highlight = buffer->line  ;
                }
            }
        else  
                {
                if (x + buffer->l_char  == buffer->last_ch_highlight && buffer->line == buffer->line_lst_highlight) 
                    {
                        moveDown();
                        buffer-> last_ch_highlight = buffer->l_char + x ;
                        buffer->line_lst_highlight =  buffer->line ;
                    }
                else if ( x+ buffer->l_char == buffer->first_ch_highlight && buffer->line == buffer->line_st_highlight && buffer->line_st_highlight != buffer->line_lst_highlight)
                    {   
                        if ( buffer->line_lst_highlight==  buffer->line_st_highlight )
                            {
                                int temp = buffer->line_st_highlight;
                                buffer->line_st_highlight= buffer->line_lst_highlight ;
                                buffer->line_lst_highlight =temp;  
                            }
                        moveDown();  
                        buffer->first_ch_highlight = buffer->l_char + x ;
                        buffer->line_st_highlight =  buffer->line ;

                    }
                else 
                    {
                        buffer->first_ch_highlight = buffer->l_char + x ;
                        buffer->line_st_highlight  = buffer->line  ;
                        moveDown();
                        buffer-> last_ch_highlight = buffer->l_char + x ;
                        buffer->line_lst_highlight =  buffer->line ;
                    }

                }

}

} 
void editor ::  break_highlight() {
    buffer->line_st_highlight  =  buffer-> line_lst_highlight = 
	buffer->first_ch_highlight =  buffer->last_ch_highlight = -1 ;
}
