#include "buffer.hpp"
// for new programmers 
// you can declare a method out side of it's class like this 
// return_type class_name :: function_name (){}

buf::buf() {




}
std::string buf::remTabs(std::string line){

int tab = line.find("\t"); // returns the first tab place
    if (tab == line.npos) // i don't know what that actually does but i guess it checks if the is no tabs
        return line ; 
    else 
        return remTabs(line.replace(tab, 1, "   ")) ; // redo the whole thing with 
}


void buf :: insertLine(std::string line , int n ){
   //line = remTabs(line); // you can remove this if you want tabs
    lines.insert(lines.begin()+n, line); // puts the line in the right place 
}

void buf::appendLine(std::string line)
{
    line = remTabs(line); // just delete if you want tabs
    lines.push_back(line.c_str()); // add the line to the end ; 
    
}
void buf:: injectIntoLine (std::string line  ,int x ) 
{
    this->lines[this->line].insert(x, line) ;
    //this->lines[line].insert(lines[line].begin()+x, line)
    
}
void buf::removeLine(int n)
{
    lines.erase(lines.begin()+n); // removes the nth line 
    lines.shrink_to_fit();
}
void  buf::  copy_line (int sch , int fch ,int sl ,int fli  ){
    this->lines_paste = getStrings_from_lines(false , sch , fch , sl , fli) ; 
}

std::string buf::  cut_line  (int sch , int fch ,int sl ,int fli ){
    this->lines_paste = getStrings_from_lines(true , sch , fch , sl , fli) ; 
}
std::string buf::  paste_line (int line , std :: vector  <std::string> lines_paste,int x ){
    int size = lines_paste.size() ; 
    if (size== 1 )
        injectIntoLine(lines_paste[0],x);
    else 
        {
            lines_paste.push_back(lines[line].substr(x,lines[line].size())); // for pasting inside of a line 
            lines[line].erase(x,lines[line].size());
            size = lines_paste.size() ; 
            for (int i = 0 ; i<size; i++ )
            {
                if ( (i ==0 || i == size - 1 )  && lines_paste[i] == "" ) // the two aditional lines are added if we are adding to an empty line ( not wanted )
                    continue;
                lines.insert(lines.begin()+line+i+1,lines_paste[i]);
            }
        }
}

int buf:: space_for_numbers( int size ){
    int count= 0  ; 
    while (size !=0 )
    {   
        size/=10  ; 
        count++ ;
    }
    return count+2;
}
std :: vector <std::string> buf :: getStrings_from_lines (bool cut  , int startc , int endc , int startl , int endl ) 
{   
    std :: vector <std::string> output = {}; 
   // if (!cut )
        // {
            if (startl== endl)
                output.push_back( lines[startl].substr(startc , endc+1) ) ;
            else 
                for (int i = startl ; i <= endl ;  i++)
                    {
                        if ( i == startl)
                            output.push_back(lines[startl].substr(startc)) ;
                        else if ( i == endl )
                            output.push_back( lines[endl].substr(0, endc+1) ) ;
                        else 
                            output.push_back(lines[i]);
                    }
        // }
    // else 
    if ( cut)
    {
        bool all_first; 
        int f =1; 
        if (startl== endl)
        lines[startl].erase(startc , endc - startc +1) ; 
        else 
        {
            for (int i = startl ; i <= endl ;  i++)
                {

                        if ( i == startl)
                        {
                            //lines.erase(lines.begin()+startl);
                            //else 
                           if (startc != 0 )
                           {
                            lines[startl].erase(startc) ;
                            continue;
                           }
                           else  
                           {
                            lines.erase(lines.begin()+startl );
                            f = 0 ;
                           }
                        }
                        else if ( i == endl )   //ddsssdadsa
                        {
                            // lines.erase(lines.begin()+startl);
                            // break ;
                            // }
                            // else 
                            if (endc != lines[startl+f].size() )
                            {
                                lines[startl+f].erase( 0 ,endc+1) ;
                                continue;
                            }
                            else                 lines.erase(lines.begin()+startl );
                        }
                        else
                            {
                                lines.erase(lines.begin()+ startl + f );
                            }
                    
                
                }

        }
    }

    return  output ; 

}