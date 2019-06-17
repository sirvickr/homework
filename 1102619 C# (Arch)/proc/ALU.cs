using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    // Арифметико-логическое устройство (ALU - Arihmetic-Logic Unit)
    class ALU
    {
        public Command Cmd
        {
            get
            {
                return _cmd;
            }
            set
            {
                _cmd = value;
            }
        }
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
        public void execute()
        {
            switch (_cmd)
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
        int _reg = 0;
        int _sum = 0;
        Command _cmd = Command.Stop;
    }
}
