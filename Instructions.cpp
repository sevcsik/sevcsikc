#include "Header.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
using namespace std;

InstEx::InstEx(const char *msg, unsigned pos) : msg(msg), pos(pos)
{
}

const char * InstEx::what() const throw()
{
   stringstream message;
   message << "Instruction " << pos << ": " << msg;
   return message.str().c_str();
}

MemEx::MemEx(const char *msg, unsigned pos) : msg(msg), pos(pos)
{
}

const char * MemEx::what() const throw()
{
   stringstream message;
   message << "Memory error:" << msg << ": pos";
   return message.str().c_str();
}

Data::Data(double data) : data(data)
{
}

double Data::exec(double *acc)
{
   return data;
}

void Data::print()
{
   cout << "Data " << data << endl;
}

Load::Load(double data, unsigned dest) : data(data), dest(dest)
{
}

double Load::exec(double *acc)
{
   acc[dest] = data;
   return data;
}

void Load::print()
{
   cout << "Load " << data << " to *" << dest << endl;
}

Add::Add(unsigned arg1, unsigned arg2, unsigned dest)
   : arg1(arg1), arg2(arg2), dest(dest)
{
}

double Add::exec(double *acc)
{
   acc[dest] = acc[arg1] + acc[arg2];
   return acc[dest];
}

void Add::print()
{
   cout << "Add *" << arg1 << " *" << arg2 << endl;
}

Subtract::Subtract(unsigned arg1, unsigned arg2, unsigned dest)
   : arg1(arg1), arg2(arg2), dest(dest)
{
}

double Subtract::exec(double *acc)
{
   acc[dest] = acc[arg1] - acc[arg2];
   return acc[dest];
}

void Subtract::print()
{
   cout << "Subtract *" << arg1 << " *" << arg2 << endl;
}

Multiply::Multiply(unsigned arg1, unsigned arg2, unsigned dest)
   : arg1(arg1), arg2(arg2), dest(dest)
{
}

double Multiply::exec(double *acc)
{
   acc[dest] = acc[arg1] * acc[arg2];
   return acc[dest];
}

void Multiply::print()
{
   cout << "Multiply *" << arg1 << " *" << arg2 << endl;
}

Divide::Divide(unsigned arg1, unsigned arg2, unsigned dest)
   : arg1(arg1), arg2(arg2), dest(dest)
{
}

double Divide::exec(double *acc)
{
   acc[dest] = acc[arg1] / acc[arg2];
   return acc[dest];
}

void Divide::print()
{
   cout << "Divide *" << arg1 << " *" << arg2 << endl;
}

Put::Put(unsigned arg, unsigned addr): arg(arg), addr(addr)
{
}

double Put::exec(double *acc)
{
   delete mem->get(addr);
   mem->put(new Data(acc[arg]), addr);
   return acc[arg];
}

void Put::print()
{
   cout << "Put *" << arg << " to @" << addr << endl;
}

Get::Get(unsigned addr, unsigned dest): addr(addr), dest(dest)
{  
}

double Get::exec(double *acc)
{
   Instruction *inst = mem->get(addr);
   if (!inst) throw InstEx("Can't access address", mem->getpos());
   else acc[dest] = inst->exec(acc);
   return inst->exec(acc);
}

void Get::print()
{
   cout << "Get @" << addr << " to *" << dest << endl;
}

Goto::Goto(unsigned addr) : addr(addr)
{
}

double Goto::exec(double *acc)
{
   mem->setpos(addr);
   return 1;
}

void Goto::print()
{
   cout << "Goto @" << addr << endl;
}

ZGoto::ZGoto(unsigned arg, unsigned addr) : arg(arg), addr(addr)
{
}

double ZGoto::exec(double *acc)
{
   if (acc[arg] == 0) mem->setpos(addr);
   return 1;
}

void ZGoto::print()
{
   cout << "ZGoto *" << arg << " to @" << addr << endl;
}

NZGoto::NZGoto(unsigned arg, unsigned addr) : arg(arg), addr(addr)
{
}

double NZGoto::exec(double *acc)
{
   if (acc[arg] != 0) mem->setpos(addr);
   return 1;
}

void NZGoto::print()
{
   cout << "NZGoto *" << arg << " to @" << addr << endl;
}

Print::Print(unsigned arg, const char *format_) : arg(arg)
{
   format = new char[strlen(format_) + 1];
   strcpy(format, format_);
}

Print::~Print()
{
   delete[] format;
}

double Print::exec(double *acc)
{
   printf(format, acc[arg]);
   cout << endl;
   return *acc;
}

void Print::print()
{
   cout << "Print " << arg << " like \"" << format << "\"" << endl;
}

