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
    status += "\tCOL: " + std::to_string(x) + "\tROW: " + std::to_string(y); // takes the position and prints them to string
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

        }
    switch(mode)
        {
            case 'n': // normal mode has it's own key set we switch between them
                switch(i)
                    {
                        case 'x' : // exits 
                            mode = 'x' ;
                            break ;
                        case 'i': // switchs to insert mode 
                            mode = 'i';
                            break;
                        case 's' : //saves
                            saveFile();
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
                            if(x == 0 && y > 0 ) // if we are at the begining of a line  that is not the first
                            {
                                x = buffer->lines[y-1].length(); // changes the x to upove line length 
                                buffer->lines[y-1] += buffer-> lines[y] ; // adds the line to the line apove it 
                                deleteLine(); // the line it self didn't get deleted so we have to
                                moveUp();
                            }
                            else if (y>=0 && x!=0 )
                                buffer->lines[y].erase(--x,1); // "we" if the curser on e we delete w that's why we have the --x
                                // remove a char
                        break;
                        case KEY_DC : // the delete key
                             if(x == buffer->lines[y].length() && y != buffer->lines.size() - 1) // if we at the end of a line and it's not the last one 
                                {
                                     buffer->lines[y] += buffer->lines[y+1]; // addes the line below 
                                     deleteLine(y+1); // delete that line below
                                }
                             else
                                buffer->lines[y].erase(x, 1);                             
                        break;
                        case 10: // KEY_ENTER:
                            // puts the rest of the line down 
                            if(x < buffer->lines[y].length()){ // if we are not at the end of the line
                            buffer->insertLine(buffer->lines[y].substr(x, buffer->lines[y].length() - x), y + 1);
                                                //creates a substring of current line                    // adds it to the line below 
                            buffer->lines[y].erase(x, buffer->lines[y].length() - x);
                            // removes that of line that got down 
                            }
                            else // just adds an empty line 
                                 buffer->insertLine("", y+1); 
                        x=0 ;  // start at the end of line 
                        moveDown();
                        break;
                    case 9 : // the tab key
                        buffer->lines[y].insert(x, 4, ' '); // adds four spaces 
                        x+=4;
                        break;
                    default:
                        //for handling any other char
                        buffer->lines[y].insert(x, 1, char(i));
                        x++;
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
        if (x-1 >= 0)
            {
                x--;
                move(y,x);
            }
    }

void editor::moveRight()
{
    if(x+1 < COLS && x+1 <= buffer->lines[y].length()) // if we move right and still in the line
    {
        x++;
        move(y, x);
    }
}

void editor::moveUp()
{
    if(y-1 >= 0) // if we are not on the first 
        y--;
    if(x >= buffer->lines[y].length())
        x = buffer->lines[y].length();
    move(y, x);
}

void editor::moveDown()
{
    if(y+1 < LINES-1 && y+1 < buffer->lines.size())
        y++;
    if(x >= buffer->lines[y].length())
        x = buffer->lines[y].length();
    move(y, x);
}


void editor::printBuff()
{
    for(int i=0; i<LINES-11; i++) // for every line just before the border of the box
    {
        if(i >= buffer->lines.size())
        {
            move(i, 0); // move to the new line 
            clrtoeol(); // clear what is in that line 
        }
        else
        {
            mvprintw(i, 0, buffer->lines[i].c_str()); // an ncurses function prints to screen where you want to takes a line of type c string
        }
         clrtoeol(); // clear the rest of the line 
    }
   
   move(y, x); // move where the curser was before 
   
   
}

void editor::printStatusLine()
{
    
   
    refresh(); // refreshs before waiting input
    WINDOW* status_window; //creats a ncur window pointer 
    status_window = newwin(/*height*/9,/*width*/ COLS-10 ,/*y*/LINES-10,5/*x*/); // creats a new window beacuse all we created is a pointer
    
    box(status_window,0,0); //border for our window
    mvwprintw(status_window,1,1, status.c_str()); // print the status string
    mvwprintw(status_window,2,1, save_status.c_str()); 
    wrefresh(status_window); // refreshes the window

    
    
    
}

void editor::deleteLine()
{
    buffer->removeLine(y);
}

void editor::deleteLine(int i)
{
    buffer->removeLine(i);
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
                        filename=filename.erase((filename.length())-1); // erases the last element 
                            if (filename=="") // if the user goes byond that a buffer over flew happens so we should prevent that
                                {
                                    filename=temp; // returns file name to where it was 
                                    return; // exits the save 
                                }
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





