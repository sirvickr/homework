using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    // Оперативная память (Random Access Memory)
    class RAM
    {
        const int size = 65536;
        char[] mem = new char[size];
        
        public char getByte(int addr)
        {
            return mem[addr];
        }

        public short getWord(int addr)
        {
            short result = (short)mem[addr];
            result |= (short)(mem[addr + 1] << 8);
            return result;
        }

        public int getDword(int addr)
        {
            int result = (int)mem[addr];
            result |= (mem[addr + 1] << 8);
            result |= (mem[addr + 2] << 16);
            result |= (mem[addr + 3] << 24);
            return result;
        }

        public void setByte(int addr, char value)
        {
            mem[addr] = value;
        }

        public void setWord(int addr, short value)
        {
            mem[addr + 0] = (char)(value >>  0);
            mem[addr + 1] = (char)(value >>  8);
        }

        public void setDword(int addr, int value)
        {
            mem[addr + 0] = (char)(value >> 0);
            mem[addr + 1] = (char)(value >> 8);
            mem[addr + 2] = (char)(value >> 16);
            mem[addr + 3] = (char)(value >> 24);
        }
    }
}
