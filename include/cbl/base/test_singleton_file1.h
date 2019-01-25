#pragma once

#include "singleton.h"
#include <iostream>

class Foo: public cbl::singleton<Foo> {
 public:
   Foo() = default;
   ~Foo() {
     std::cout << "Foo:~Foo" << std::endl;
   }

   void Say() {
     std::cout << "Foo::Say" << std::endl;
   }
};

class S1 {
 public:
   S1() {
     std::cout << "S1::S1" << std::endl;
   }
   ~S1() {
     std::cout << "S1::~S1" << std::endl;
     //Foo::instance().Say();
     //std::cout << "S1::~S1" << std::endl;
   }
   void Say() {
     //Foo::instance().Say();
     std::cout << "S1::Say()" << std::endl;
   }
};
