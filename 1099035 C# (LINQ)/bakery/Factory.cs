using System;

namespace bakery
{
    // Хлебозавод
    class Factory
    {
        // уникальный ключ
        public int Key { set; get; } = 0;
        // название предприятия
        public string Name { set; get; }
        // начало производства
        public DateTime ProdDate { set; get; }
    }
}
