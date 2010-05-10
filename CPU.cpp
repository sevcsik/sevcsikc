#include "Header.h"

#include <iostream>
#include <exception>
using namespace std;

CPU::CPU(Memory *mem, unsigned size) : mem(mem) 
{
   acc = new double[size];
}

CPU::~CPU()
{
   delete[] acc;
}


void CPU::run(unsigned addr)
{
   try
   {
      mem->setpos(addr);
   } catch (exception &ex) {
      cerr << ex.what();
      return;
   }
   
   while (Instruction *inst = mem->get()) // Memory::get() gives next inst
   {
      cout << mem->getpos() - 1 << ": \t"; // get() already incremented pos
      inst->print();
      inst->exec(acc);
   } 
   
   cout << "CPU: no more instructions, terminating\n";  
}
