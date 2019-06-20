using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bakery
{
    class Product
    {
        public int Key { set; get; } = 0;
        public int FactoryKey { set; get; } = 0;
        public string Name { set; get; }
        public double Weight { set; get; }
        public DateTime ExpiryDate { set; get; }
        public int Count { set; get; } = 0;
        public double Price { set; get; } = 0.0;
    }
}
