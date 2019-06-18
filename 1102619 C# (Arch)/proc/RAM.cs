using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    // Оперативная память (Random Access Memory)
    class RAM
    {
        // Максимальный размер памяти
        const int size = 65536;

        // Массив байт (буфер памяти)
        char[] mem = new char[size];

        // Текущий адрес записи
        public int WriteAddr { get; set; } = 0;

        // Текущий адрес чтения
        public int ReadAddr { get; set; } = 0;

        // Операции чтения данных

        public char getByte(int addr)
        {
            return mem[addr];
        }

        public char getNextByte()
        {
            return mem[ReadAddr++];
        }

        public short getWord(int addr)
        {
            short result = (short)mem[addr];
            result |= (short)(mem[addr + 1] << 8);
            return result;
        }

        public short getNextWord()
        {
            short result = (short)mem[ReadAddr++];
            result |= (short)(mem[ReadAddr++] << 8);
            return result;
        }

        public int getDword(int addr)
        {
            int result = (int)mem[addr];
            result |= (mem[addr + 1] <<  8);
            result |= (mem[addr + 2] << 16);
            result |= (mem[addr + 3] << 24);
            return result;
        }

        public int getNextDword()
        {
            int result = (int)mem[ReadAddr++];
            result |= (mem[ReadAddr++] <<  8);
            result |= (mem[ReadAddr++] << 16);
            result |= (mem[ReadAddr++] << 24);
            return result;
        }

        // Операции записи данных

        public void setByte(int addr, char value)
        {
            mem[addr] = value;
        }

        public void setNextByte(char value)
        {
            mem[WriteAddr++] = value;
        }

        public void setWord(int addr, short value)
        {
            mem[addr + 0] = (char)(value >>  0);
            mem[addr + 1] = (char)(value >>  8);
        }

        public void setNextWord(short value)
        {
            mem[WriteAddr++] = (char)(value >> 0);
            mem[WriteAddr++] = (char)(value >> 8);
        }

        public void setDword(int addr, int value)
        {
            mem[addr + 0] = (char)(value >>  0);
            mem[addr + 1] = (char)(value >>  8);
            mem[addr + 2] = (char)(value >> 16);
            mem[addr + 3] = (char)(value >> 24);
        }

        public void setNextDword(int value)
        {
            mem[WriteAddr++] = (char)(value >>  0);
            mem[WriteAddr++] = (char)(value >>  8);
            mem[WriteAddr++] = (char)(value >> 16);
            mem[WriteAddr++] = (char)(value >> 24);
        }

        // Образ памяти для отладки
        public void dump(int size)
        {
            Console.WriteLine("------------ MEMORY DUMP ------------");
            for (int i = 0; i < size; i++)
            {
                int cell = (int)mem[i];
                Console.WriteLine(i.ToString("X4") + " " + cell.ToString("X"));
            }
            Console.WriteLine("-------------------------------------");
        }
    }
}
