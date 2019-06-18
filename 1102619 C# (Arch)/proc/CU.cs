using System;
using System.Collections.Generic;
using System.Text;

namespace proc
{
    // Управляющее устройство (CU - Control Unit)
    class CU
    {
        // Регистр адреса
        public int RA { get; set; } = 0;
        // Регистр команд
        public Command RС { get; set; } = 0;
    }
}
