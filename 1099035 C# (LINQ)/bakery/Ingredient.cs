using System;

namespace bakery
{
    // Ингредиент
    class Ingredient
    {
        // уникальный ключ
        public int Key { set; get; } = 0;
        // наименование
        public string Name { set; get; }
        // вес (кг)
        public double Weight { set; get; }
        // дата поставки
        public DateTime DeliveryDate { set; get; }
        // дата истечения срока годности
        public DateTime ExpiryDate { set; get; }
    }
}
