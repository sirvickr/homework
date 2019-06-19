using System;

namespace proc
{
    public enum Command
    {
        Output, // вывод
        Stop, // останов
        Add, // сложение
        Sub, // вычитание
        Ld, // загрузка
        Mv, // выгрузка в ОП
    }

    class Computer
    {
        // Арифметико-логическое устройство
        ALU alu = new ALU();
        // Устройство управления
        CU cu = new CU();
        // Оперативная память
        RAM ram = new RAM();
        // Устройство вывода
        Display display = new Display();

        // Выполнение программы
        public void runProgram()
        {
            // Загружаем в УУ адрес текущей команды
            cu.RA = 0;
            // Загружаем в УУ код текущей команды
            cu.RС = (Command)ram.getByte(cu.RA);
            while(cu.RС != Command.Stop)
            {
                if(hasOperand(cu.RС))
                {
                    // Извлекаем адрес операнда текущей команды
                    int addr = ram.getWord(cu.RA + 1);
                    // Читаем из памяти значение операнда и загружаем его в АЛУ
                    alu.Reg = ram.getDword(addr);
                    //display.print("RA " + cu.RA.ToString() + " RC " + cu.RС.ToString() + " op addr " + addr.ToString() + " R " + alu.Reg.ToString() + " S " + alu.Sum.ToString());
                }
                else
                {
                    //display.print("RA " + cu.RA.ToString() + " RC " + cu.RС.ToString());
                }
                // Выполняем текущую команду
                if (cu.RС == Command.Output)
                {
                    display.print(alu.Sum.ToString());
                }
                else
                {
                    alu.execute(cu.RС);
                }
                //display.print("  >> " + alu.Sum.ToString());

                // Смещаемся на адрес следующей команды
                cu.RA += commandLength(cu.RС);
                // Загружаем код следующей команды в АЛУ
                cu.RС = (Command)ram.getByte(cu.RA);
            }
        }

        // Признак наличия операнда у команды
        bool hasOperand(Command cmd)
        {
            if (cmd == Command.Ld || cmd == Command.Add || cmd == Command.Sub)
                return true;
            return false;
        }

        // Полный размер команды
        int commandLength(Command cmd)
        {
            if (cmd == Command.Output || cmd == Command.Stop)
                return 1;
            return 5;
        }

        // Подготовка к рагрузке в память
        public void prepareLoad()
        {
            ram.WriteAddr = 0;
        }

        // Функция ввода отдельной команды
        // Возвращаемое значение:
        // false - продолжаем ввод команд
        // true - завершаем работу
        public bool addCommand(string cmd, bool trace)
        {
            // Комментарии и пустые строки пропускаем
            if (cmd.Length == 0 || cmd[0] == '#')
            {
                return false;
            }

            // Выводим исходную команду на консоль
            // (при вводе с клавиатуры не имеет смысла,
            // т.к. пользователь уже её сам набрал в консоли)
            if (trace)
            {
                Console.WriteLine(cmd);
            }

            // Переводим все символы в верхний регистр
            // (убираем зависимость от регистра)
            cmd = cmd.Trim().ToUpper();

            // Разбиваем команду на имя команды 
            // и аргументы (если есть)
            string[] tokens = cmd.Split(new char[] { ' ' });

            // Первый элемент обязан содержать имя команды
            if (tokens.Length == 0)
            {
                return false;
            }

            if (tokens[0] == "MV")
            {
                /* Задать значение параметра
                 ожидаем: MV <адрес ОП> <размер> <значение>
                 примеры
                 MV 1024 1 16
                 MV 1025 2 821
                 MV 1027 4 223501
                */
                if (tokens.Length == 4)
                {
                    int addr = int.Parse(tokens[1]);
                    int size = int.Parse(tokens[2]);
                    int value = int.Parse(tokens[3]);
                    switch (size)
                    {
                        case 1:
                            ram.setByte(addr, (char)value);
                            break;
                        case 2:
                            ram.setWord(addr, (short)value);
                            break;
                        case 4:
                            ram.setDword(addr, value);
                            break;
                    }
                }
            }
            else if (tokens[0] == "LD")
            {
                // Загрузить слово из памяти в сумматор АЛУ
                // ожидаем: LD <адрес ОП>
                if (tokens.Length == 2)
                {
                    int addr = int.Parse(tokens[1]);
                    ram.setNextByte((char)Command.Ld);
                    ram.setNextDword(addr);
                }
            }
            else if (tokens[0] == "ADD")
            {
                // Загрузить слово из памяти в сумматор АЛУ
                // ожидаем: ADD <адрес ОП>
                if (tokens.Length == 2)
                {
                    int addr = int.Parse(tokens[1]);
                    ram.setNextByte((char)Command.Add);
                    ram.setNextDword(addr);
                }
            }
            else if (tokens[0] == "SUB")
            {
                // Загрузить слово из памяти в сумматор АЛУ
                // ожидаем: ADD <адрес ОП>
                if (tokens.Length == 2)
                {
                    int addr = int.Parse(tokens[1]);
                    ram.setNextByte((char)Command.Sub);
                    ram.setNextDword(addr);
                }
            }
            else if (cmd == "OUT")
            {
                // Отправляем значение сумматора в устройство вывода
                ram.setNextByte((char)Command.Output);
            }
            else if (tokens[0] == "DUMP")
            {
                // Образ памяти для отладки
                int size = 16;
                if (tokens.Length > 1)
                    size = int.Parse(tokens[1]);
                ram.dump(size);
            }
            else if (cmd == "END")
            {
                // Записываем команду: останов
                ram.setNextByte((char)Command.Stop);
                // Возвращаем признак завершения работы
                return true;
            }
            else
            {
                // Сообщаем об ошибке, но не прерываем работу
                Console.WriteLine("Неизвестная команда: \"" + cmd + "\"");
            }
            // Возвращаем признак продолжения работы
            return false;
        }
    }
}
