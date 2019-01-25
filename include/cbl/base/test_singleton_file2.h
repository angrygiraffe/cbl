#include "singleton.h"

class Bar: public cbl::singleton<Bar> {
 public:
   ~Bar() {
     std::cout << "Bar::~Bar" << std::endl;
   }
   void Say() {
     std::cout << "Bar::Say" << std::endl;
   }
 private:
   friend cbl::singleton<Bar>;
   Bar() = default;
};

class S2 {
 public:
   S2() {
     std::cout << "S2::S2" << std::endl;
   }
   ~S2() {
     std::cout << "S2::~S2" << std::endl;
   }
   void Say() {
     Bar::instance().Say();
     std::cout << "S2::Say()" << std::endl;
   }
};
