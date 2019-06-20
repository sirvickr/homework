using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bakery
{
    class Ingredient
    {
        public int Key { set; get; } = 0;
        public string Name { set; get; }
        public double Weight { set; get; }
        public DateTime DeliveryDate { set; get; }
        public DateTime ExpiryDate { set; get; }
    }
}
