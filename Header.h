#ifndef HEADERS_H
#define HEADERS_H

#include <exception>
using namespace std;

class Memory;
class Instruction;
class CPU;

/** @mainpage sevcsikc interpreter
  * @section Disclaimer
  * <b>sevcsikc</b> is an interpreter for the <b>sevcsik</b> language, which is
  * an extremely minimalistic, assembly-like language. It was created as a
  * homework for the 'softlab2' course of Budapest University of Technology and
  * Economics, and it's not intented for use in production environments. This
  * product doesn't give you any guarantees, it can burn your house or
  * scare your cat, <i>you have been warned</i>.
  *
  * @section Usage
  * <b>sevcsikc</b> can get the source code from stdin, or from a file which can
  * passed as the first commandline argument.
  *
  * @section Language The sevcsik language
  * The code looks like this:
  * @code
org       0
load      0  0        // 0 Counter = *0
load      5  1        // 1 Value to compare
load      1  2        // 2 Value to increment with
print     0  "Counter: %lf" // 3 Print counter value
add       0  2  0     // 4 Increment counter
subtract  0  1  3     // 5 Save comprasion result
nzgoto    3  3        // 6 If comprasion is false, continue
print     0 "Done"    // 7 Goodbye
end
  @endcode
  * The first instruction <i>must be</i> <b>org n</b>, that tells the
  * interpreter where to place the first instruction. This comes handy if you
  * want to store variables in the memory, and don't want to screw up your
  * instructions.
  *
  * Instructions are the same as class names derived from Instruction class,
  * but lowercase. Arguments follow the instructions, everything is seperated
  * with whitespaces. There are no syntax error handling, to teach programmers
  * to concentrate.
  *
  * Comments after arguments can be anything, but it looks more professional if
  * they start with '//'. Comments at the beginning of the line must start with
  * '//'.
  */

/// Exception thrown during input parse
class ParseEx : public exception
{
   private:
      const char *msg;
      unsigned line;
   public:
      /** constructor
        * @param msg error message to display
        * @param line line number
        */
      ParseEx(const char *msg, unsigned line);
      virtual const char *what() const throw();
};

/// Instructions can throw this exception
class InstEx : public exception
{
   private:
      const char *msg;
      unsigned pos;
   public:
      /** constructor
        * @param msg error message to display
        * @param pos position of instruction in memory
        */
      InstEx(const char *msg, unsigned pos);
      virtual const char *what() const throw();
};

/// Exception thrown by Memory
class MemEx : public exception
{
   private:
      const char *msg;
      unsigned pos;
   public:
      /** constructor
        * @param msg error message to display
        * @param pos argument of the failed request
        */
      MemEx(const char *msg, unsigned pos);
      virtual const char *what() const throw();
};

/// CPU class
class CPU
{
   private:
      double *acc;
      Memory *mem;
   public:
      /** constructor
        * @param mem pointer to a memory object that contains instructions
        * @param size number of registers (instructions aren't aware of this!)
        */
      CPU(Memory *mem, unsigned size); 
      ~CPU();
      /** start executing instructions from memory
        * @param addr first instruction's address
        */
      void run(unsigned addr);
};

/// Memory class
class Memory
{
   private:
      Instruction ** data;
      unsigned pos;
      unsigned size;
   public:
      /** constructor
        * @param size number of instructions that memory can hold
        */
      Memory(unsigned size);
      /** destructor, it deletes instructions too */
      ~Memory();
      /** returns the instruction at <i>pos</i>, and increments it
        * @return pointer to instruction
        */
      Instruction * get();
      /** returns a specific instruction
        * @param addr address of instruction
        * @return pointer to instruction
        */
      Instruction * get(unsigned addr);
      /** puts an instruction into memory, and deletes the actual instruction
        * from the address
        * @param inst instruction to save. must be <i>new</i>'d, ~Memory() will
        *             <i>delete</i> it
        * @param addr address of instruction
        */
      void put(Instruction *inst, unsigned addr);
      /** puts an instruction into memory, to <i>pos</i>, and increments it
        * @param inst pointer to instruction
        * @return returns itself, to be used in chain
        */
      Memory & operator<<(Instruction *inst);
      /** gets current position
        * @return current position
        */
      unsigned getpos() {return pos;};
      /** sets current position, used by goto instructions
        * @param addr target address
        */
      void setpos(unsigned addr);
};

/// Abstract instruction class
class Instruction 
{
   public:
      /// pointer to the memory where the instruction lives
      Memory *mem;
      /** executes the instruction
        * @param acc pointer to CPU's accumulator array
        * @return undefined, except for @link Data @endlink instruction
        */
      virtual double exec(double *acc) = 0;
      /** print a nice text about the instruction like
        * @verbatim 12: Load 3.4 to *2 @endverbatim
        * @verbatim 13: Put *2 to @5 @endverbatim
        * '*' means register, '@' means memory address
        */
      virtual void print() = 0;
      /// empty destructor for Print to override
      virtual ~Instruction() {}
};

/// Dummy instruction to hold data
class Data : public Instruction
{
   private:
      double data;
   public:
      /// @param data literal to hold
      Data(double data);
      double exec(double *acc);
      void print();
};

/// Load literal to register
class Load : public Instruction
{
   private:
      double data;
      unsigned dest;
   public:
      /** @param data literal
        * @param dest target register
        */
      Load(double data, unsigned dest);
      double exec(double *acc);
      void print();
};

/// Save register content to memory
class Put : public Instruction
{
   private:
      unsigned arg;
      unsigned addr;
   public:
      /** @param arg source register
        * @param addr target address
        */
      Put(unsigned arg, unsigned addr);
      double exec(double *acc);
      void print();
};

/** @brief Load data from memory to a register
  *
  * Load data from memory to a register (using it with other
  * instruction than Data is undetermined)
  */
class Get : public Instruction
{
   private:
      unsigned addr;
      unsigned dest;
   public:
      /** @param addr source address
        * @param dest target register
        */
      Get(unsigned addr, unsigned dest);
      double exec(double *acc);
      void print();
};

/// Add two registers
class Add : public Instruction
{
   private:
      unsigned arg1;
      unsigned arg2;
      unsigned dest;
   public:
      /** @param arg1 register that contains first argument
        * @param arg2 register that contains secont argument
        * @param dest register to hold the result
        */
      Add(unsigned arg1, unsigned arg2, unsigned dest);
      double exec(double *acc);
      void print();
};

/// Subtract one register from another
class Subtract : public Instruction
{
   private:
      unsigned arg1;
      unsigned arg2;
      unsigned dest;
   public:
      /** @param arg1 register that contains first argument
        * @param arg2 register that contains secont argument
        * @param dest register to hold the result
        */
      Subtract(unsigned arg1, unsigned arg2, unsigned dest);
      double exec(double *acc);
      void print();
};

/// Multiply one register with another
class Multiply : public Instruction
{
   private:
      unsigned arg1;
      unsigned arg2;
      unsigned dest;
   public:
      /** @param arg1 register that contains first argument
        * @param arg2 register that contains secont argument
        * @param dest register to hold the result
        */      
      Multiply(unsigned arg1, unsigned arg2, unsigned dest);
      double exec(double *acc);
      void print();
};

/// Divide one register with another
class Divide : public Instruction
{
   private:
      unsigned arg1;
      unsigned arg2;
      unsigned dest;
   public:
      /** @param arg1 register that contains first argument
        * @param arg2 register that contains secont argument
        * @param dest register to hold the result
        */
      Divide(unsigned arg1, unsigned arg2, unsigned dest);
      double exec(double *acc);
      void print();
};

/// Jump to specified instruction
class Goto : public Instruction
{
   private:
      unsigned addr;
   public:
      /** @param addr target position
        */
      Goto(unsigned addr);
      double exec(double *acc);
      void print();
};

/// Jump to specified instruction if register is nonzero
class NZGoto : public Instruction
{
   private:
      unsigned arg;
      unsigned addr;
   public:
      /** @param arg register to check
        * @param addr target position
        */
      NZGoto(unsigned arg, unsigned addr);
      double exec(double *acc);
      void print();
};

/// Jump to specified instruction if register is zero
class ZGoto : public Instruction
{
   private:
      unsigned arg;
      unsigned addr;
   public:
      /** @param arg register to check
        * @param addr target position
        */
      ZGoto(unsigned arg, unsigned addr);
      double exec(double *acc);
      void print();
};

/// printf() register with specified format
class Print: public Instruction
{
   
   private:
      char *format;
      unsigned arg;
   public:
      /** @param arg register to print
        * @param format format string (register contains double, so use %lf or
        * similar)
        */
      Print(unsigned arg, const char *format);
      ~Print();
      double exec(double *acc);
      void print();
};


#endif
