namespace bakery
{
    // Связь между изделием и ингрединентом
    class Constitution
    {
        // уникальный ключ
        public int Key { set; get; } = 0;
        // внешний ключ - изделие
        public int ProductKey { set; get; } = 0;
        // внешний ключ - ингредиент
        public int IngredientKey { set; get; } = 0;
    }
}
