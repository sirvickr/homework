using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    public enum Command
    {
        Input, // ввод
        Output, // вывод
        Stop, // останов
        Add, // сложение
        Sub, // вычитание
        //Cmp, // сравнение
        Ld, // загрузка
        //St, // выгрузка
        //LdA, // загрузка адреса
        //Jmp, // переход
        //Jz, // переход по нулю
        //Jn, // переход по минусу
    }

    class Computer
    {
        //CPU cu = new CPU();
        ALU alu = new ALU();
        CU cu = new CU();

        RAM ram = new RAM();
        Output output = new Output();
        Input input = new Input();
        public void runProgram()
        {
            /*
            ram.setByte(0, (char)0x4F);
            ram.setByte(1, (char)0x3F);
            ram.setByte(2, (char)0x2F);
            ram.setByte(3, (char)0x1F);
            Console.WriteLine("0\t" + ((int)ram.getByte(0)).ToString("X"));
            Console.WriteLine("1\t" + ((int)ram.getByte(1)).ToString("X"));
            Console.WriteLine("2\t" + ((int)ram.getByte(2)).ToString("X"));
            Console.WriteLine("3\t" + ((int)ram.getByte(3)).ToString("X"));
            Console.WriteLine("W\t" + ((int)ram.getWord(0)).ToString("X2"));
            Console.WriteLine("W\t" + ((int)ram.getWord(1)).ToString("X2"));
            Console.WriteLine("DW\t" + ((int)ram.getDword(0)).ToString("X4"));
            */

            ram.setByte(0, (char)Command.Stop);

            int cmdAddr = 0;// 8; // Регистр адреса
            // Загружаем код текущей команды в АЛУ
            alu.Cmd = (Command)ram.getByte(cmdAddr);
            while(alu.Cmd != Command.Stop)
            {
                // Извлекаем адрес операнда текущей команды
                int addr = ram.getWord(cmdAddr + 1);
                // Читаем из памяти значение операнда и загружаем его в АЛУ
                alu.Reg = ram.getDword(addr);

                Console.WriteLine("cmdAddr " + cmdAddr.ToString() + " cmd " + alu.Cmd.ToString() + " addr " + addr.ToString() + " R1 " + alu.Reg.ToString() + " S " + alu.Sum.ToString());
                // Выполняем текущую команду в АЛУ
                alu.execute();
                Console.WriteLine("  >> " + alu.Sum.ToString());

                //int cmdLen = ram.getByte(cmdAddr + 1);
                cmdAddr += commandLength(alu.Cmd);
                // Загружаем код текущей команды в АЛУ
                alu.Cmd = (Command)ram.getByte(cmdAddr);
            }
        }

        int commandLength(Command cmd)
        {
            if (cmd == Command.Input || cmd == Command.Output || cmd == Command.Stop)
                return 1;
            return 4;
        }
    }
}
