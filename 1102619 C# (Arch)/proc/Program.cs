using System;

namespace proc
{
    class Program
    {
        static void Main(string[] args)
        {
            Computer computer = new Computer();
            computer.runProgram();
            Console.WriteLine("Работа завершена");
        }
    }
}
