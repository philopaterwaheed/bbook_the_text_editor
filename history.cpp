#include <iostream>
#include <queue>
#include <fstream>
#include "json.hpp"
class History{
  public :
    // class Change;
    // static std::queue<Change> changes ; 
    int x =6, y =9 ; // the the change occured  ; 



}; 

class Change{



};
using json = nlohmann::json; // name space for json
using namespace nlohmann::literals;
// ...

std::ifstream f("history.json"); // a struct maybe 
json data = json::parse(f);  // parses the object to json

json ex2 = R"(
  {
    "pi": 3.141,
    "happy": true
  }
)"_json; // just a reminder of suffix ; 


int main(){
    data["philo"] =  {true ,false,true}; 
    data.erase("philo");
    History ss  ; 
    json data1; 
    data1 = (json::parse(ss));
    data.merge_patch(data1);
    std :: string s = data.dump() ;
    std :: cout << s ; 
    std::ofstream file("output.json");
    file << data ;
    file.close() ;

}