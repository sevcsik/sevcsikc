#include "Header.h"

#include <cstdlib>
using namespace std;

Memory::Memory(unsigned size) : pos(0), size(size)
{
   data = new Instruction *[size];
   for (unsigned i = 0; i < size; i++) data[i] = NULL;
}

Memory::~Memory()
{
   for (unsigned i = 0; i < size; i++) delete data[i];
   delete[] data;
}

Instruction * Memory::get()
{
   if (pos == size) return NULL;
   else return data[pos++];
}

Instruction * Memory::get(unsigned addr)
{
   if (addr >= size) throw MemEx("Address is out of bounds", pos);
   return data[addr];
}

void Memory::put(Instruction *inst, unsigned addr)
{
   if (addr >= size) throw MemEx("Address is out of bounds", pos);
   inst->mem = this;
   data[addr] = inst;
}


void Memory::setpos(unsigned addr)
{
   if (addr >= size) throw MemEx("Address is out of bounds", pos);
   pos = addr;
}


Memory & Memory::operator<<(Instruction *inst)
{
   if (pos >= size - 1) throw MemEx("Out of space", pos);
   data[pos++] = inst;
   inst->mem = this;
   return *this;
}
