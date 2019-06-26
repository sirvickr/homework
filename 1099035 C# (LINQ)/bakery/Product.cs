using System;

namespace bakery
{
    // Изделие (партия изделий)
    class Product
    {
        // уникальный ключ
        public int Key { set; get; } = 0;
        // внешний ключ - хлебозавод
        public int FactoryKey { set; get; } = 0;
        // наименование
        public string Name { set; get; }
        // вес (кг)
        public double Weight { set; get; }
        // дата истечения срока годности
        public DateTime ExpiryDate { set; get; }
        // объём партии
        public int Count { set; get; } = 0;
        // цена одного изделия
        public double Price { set; get; } = 0.0;
    }
}
