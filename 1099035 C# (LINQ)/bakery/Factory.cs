using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bakery
{
    class Factory
    {
        /*public Factory(int key, string name, DateTime dt)
        {
            Key = key;
            Name = name;
            ProdDate = dt;
        }*/
        public int Key { set; get; } = 0;
        public string Name { set; get; }
        public DateTime ProdDate { set; get; }
    }
}
