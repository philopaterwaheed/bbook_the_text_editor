#pragma once
#ifndef BUFFER_H
#define BUFFER_H
// @@@@@@@@@@@@@@@@@@ // 
#include <string>
#include <vector>
// ################## // 


class buf {
    public:
        buf(); 

        std :: vector <std::string> lines ; 
        void insertLine(std::string  , int); // repalce a line with anthor
        void appendLine(std::string);
        void removeLine(int);
        std::string remTabs(std::string);




}; 
#endif