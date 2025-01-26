#pragma once
#include <iostream>
#include <string>

namespace dd{

  struct Say{
    std::string phrase_;
    Say(const std::string& phrase):phrase_(phrase){
#ifdef my_debug
      std::cerr << "==========Started " << phrase_ <<"===================" << '\n';
#endif
    }
    ~Say(){
#ifdef my_debug
      std::cerr << "===========Finished " << phrase_ << "====================" <<"\n\n";
#endif
    }
  };

}
