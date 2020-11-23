using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab9
{
    // Класс для примера. Накапливает сумму целых чисел.
    public class Accumulator
    {
        public Accumulator(int initialValue = 0)
        {
            _value = initialValue;
        }

        public void Add(int value)
        {
            _value += value;
        }

        public int Value()
        {
            return _value;
        }

        private int _value;
    }
}
