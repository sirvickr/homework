using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    // Арифметико-логическое устройство (ALU - Arihmetic-Logic Unit)
    class ALU
    {
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
        public void execute(Command op)
        {
            switch (op)
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
