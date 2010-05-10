#include "Header.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
using namespace std;

ParseEx::ParseEx(const char *msg, unsigned line) : msg(msg), line(line)
{
}

const char *ParseEx::what() const throw()
{
   stringstream str;
   str << "Line " << line << ": " << msg;
   return str.str().c_str();
}

/** Parses input and loads instructions to memory
  * @param in input stream
  * @param mem pointer to memory
  */
unsigned parse(istream &in, Memory *mem)
{
   unsigned n = 0;
   unsigned i1, i2, i3, org;
   double f1;
   bool foundOrg = false;

   for (n = 1; !in.eof(); n++)
   {
      string str, token;
      getline(in, str);
      stringstream line(str);
      line >> token;

      if (token.size() < 1) continue; // skip empty lines
      if (token.substr(0, 2) == "//") continue; // and comments
      if (!foundOrg) // we need start position first
      {
         if (token == "org")
         {
            line >> org;
            mem->setpos(org); // set start position
            foundOrg = true;
            continue;
         }
         else throw ParseEx("I need an org first", n);
      }
      if (token == "end") break; // we're done
      if (token == "data")
      {
         line >> f1;
         *mem << new Data(f1);
         continue;
      }
      if (token == "load")
      {
         line >> f1 >> i1;
         *mem << new Load(f1, i1);
         continue;
      }
      if (token == "put")
      {
         line >> i1 >> i2;
         *mem << new Put(i1, i2);
         continue;
      }
      if (token == "get")
      {
         line >> i1 >> i2;
         *mem << new Get(i1, i2);
         continue;
      }
      if (token == "add")
      {
         line >> i1 >> i2 >> i3;
         *mem << new Add(i1, i2, i3);
         continue;
      }
      if (token == "subtract")
      {
         line >> i1 >> i2 >> i3;
         *mem << new Subtract(i1, i2, i3);
         continue;
      }
      if (token == "multiply")
      {
         line >> i1 >> i2 >> i3;
         *mem << new Multiply(i1, i2, i3);
         continue;
      }
      if (token == "divide")
      {
         line >> i1 >> i2 >> i3;
         *mem << new Divide(i1, i2, i3);
         continue;
      }
      if (token == "goto")
      {
         line >> i1;
         *mem << new Goto(i1);
         continue;
      }
      if (token == "nzgoto")
      {
         line >> i1 >> i2;
         *mem << new NZGoto(i1, i2);
         continue;
      }
      if (token == "zgoto")
      {
         line >> i1 >> i2;
         *mem << new ZGoto(i1, i2);
         continue;
      }
      if (token == "print")
      {
         line >> i1;
         string format = str.substr(str.find("\"") + 1,
                                    str.rfind("\"") - str.find("\"") - 1);
         *mem << new Print(i1, format.c_str());
      }
      else throw ParseEx("Unknown token", n);
   }
   return org;
}

/** Creates Memory and CPU and start the whole thing
  * @param argc number of arguments
  * @param argv array of arguments
  */
int main(int argc, char **argv)
{
   Memory mem(1024);
   CPU cpu(&mem, 32);
   unsigned org;

   try
   {
      if (argc == 1) org = parse(cin, &mem);
      else
      {
         ifstream f(argv[1]);
         if (!f.is_open())
         {
            cerr << "Can't open file " << argv[1] << endl;
            return -1;
         }
         org = parse(f, &mem);
      }
   } catch (exception &ex) {
      cerr << ex.what() << endl;
      return -2;
   }
   
   cpu.run(org);
   cout << "Good bye, and thanks for all the fish" << endl;
   
   return 0;
}
