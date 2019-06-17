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
        Ld, // загрузка
    }

    class Computer
    {
        // Арифметико-логическое устройство (TODO: часть CPU)
        ALU alu = new ALU();
        // Устройство управления (TODO: часть CPU)
        CU cu = new CU(); // TODO?
        // Оперативная память
        RAM ram = new RAM();
        // Устройство вывода
        Output output = new Output();
        // Устройство ввода (TODO)
        Input input = new Input();
        // Выполнение программы
        public void runProgram()
        {
            // Начальная загрузка программы (команд и данных) в память
            // TODO из файла
            ram.setDword(256, 2); // записать значение "2"
            ram.setDword(260, 3); // записать значение "3"

            ram.setByte(0, (char)Command.Ld);  // записать команду: загрузить операнд [256] в АЛУ
            ram.setDword(1, 256);

            ram.setByte(5, (char)Command.Add);  // записать команду: прибавить операнд [260]
            ram.setDword(6, 260);

            ram.setByte(10, (char)Command.Stop);  // записать команду: останов

            // Выполнение программы

            // Загружаем в АЛУ адрес текущей команды
            alu.Addr = 0;
            // Загружаем в АЛУ код текущей команды
            alu.OpCode = (Command)ram.getByte(alu.Addr);
            while(alu.OpCode != Command.Stop)
            {
                // Извлекаем адрес операнда текущей команды
                int addr = ram.getWord(alu.Addr + 1);
                // Читаем из памяти значение операнда и загружаем его в АЛУ
                alu.Reg = ram.getDword(addr);

                output.push("cmdAddr " + alu.Addr.ToString() + " cmd " + alu.OpCode.ToString() + " addr " + addr.ToString() + " R1 " + alu.Reg.ToString() + " S " + alu.Sum.ToString());
                // Выполняем текущую команду в АЛУ
                alu.execute();
                output.push("  >> " + alu.Sum.ToString());

                // Смещаемся на адрес следующей команды
                alu.Addr += commandLength(alu.OpCode);
                // Загружаем код следующей команды в АЛУ
                alu.OpCode = (Command)ram.getByte(alu.Addr);
            }
        }

        int commandLength(Command cmd)
        {
            if (cmd == Command.Input || cmd == Command.Output || cmd == Command.Stop)
                return 1;
            return 5;
        }
    }
}
