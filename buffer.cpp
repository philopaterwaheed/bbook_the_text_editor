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
std::string buf::  copy_line (int c_char ){
    temp_line += lines[line][c_char] ;
}

std::string buf::  cut_line (std::string){
    temp_line = lines[line] ;
    removeLine(line); 
}
std::string buf::  paste_line (int line , int x ){
   injectIntoLine("temp_line",x);
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
