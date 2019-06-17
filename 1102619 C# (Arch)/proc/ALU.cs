using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    // Арифметико-логическое устройство (ALU - Arihmetic-Logic Unit)
    class ALU
    {
        // Код операции
        public Command OpCode
        {
            get
            {
                return _op;
            }
            set
            {
                _op = value;
            }
        }
        // Регистр адреса (содержит адрес очередной операции)
        public int Addr
        {
            get
            {
                return _addr;
            }
            set
            {
                _addr = value;
            }
        }
        // Регистр текущего операнда
        public int Reg
        {
            get
            {
                return _reg;
            }
            set
            {
                _reg = value;
            }
        }
        // Регистр сумматора
        public int Sum
        {
            get
            {
                return _sum;
            }
            set
            {
                _sum = value;
            }
        }
        // Выполнить текущую операцию
        public void execute()
        {
            switch (_op)
            {
                case Command.Add:
                    _sum += _reg;
                    break;
                case Command.Sub:
                    _sum -= _reg;
                    break;
                case Command.Ld:
                    _sum = _reg;
                    break;
            }
        }
        // Закрытые члены класса
        int _reg = 0;
        int _sum = 0;
        int _addr = 0;
        Command _op = Command.Stop;
    }
}
