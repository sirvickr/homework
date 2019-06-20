using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bakery
{
    class Program
    {
        static List<Factory> factories = new List<Factory>();
        static List<Product> products = new List<Product>();
        static List<Ingredient> ingredients = new List<Ingredient>();

        static string[] paths = new string[]{ "factories", "products", "ingredients" };

        static void Main(string[] args)
        {
            ReadList(0, 2);

            foreach (var item in factories)
            {
                Console.WriteLine(item.Key.ToString() + ": name " + item.Name + " dt " + item.ProdDate.ToString());
            }

            ReadList(1, 5);

            foreach (var item in products)
            {
                Console.WriteLine(item.Key.ToString() + ": name " + item.Name + " w " + item.Weight.ToString() + " dt " + item.ExpiryDate.ToString() + " count " + item.Count.ToString() + " price " + item.Price.ToString());
            }

            ReadList(2, 5);

            foreach (var item in ingredients)
            {
                Console.WriteLine(item.Key.ToString() + ": name " + item.Name + " w " + item.Weight.ToString() + " ddt " + item.DeliveryDate.ToString() + " edt " + item.ExpiryDate.ToString());
            }

            Console.WriteLine("Нажмите Enter");
            Console.ReadKey(true);

            while (true)
            {
                char choice = UserChoice();
                Console.WriteLine("Вы выбрали действие: " + choice);
                if (choice == '0')
                {
                    break;
                }
                switch(choice)
                {
                    case '1': // Перечень изделий, выпускаемых хлебозаводом
                        AllProductsList();
                        break;
                    case '2': // Суммарная стоимость изделий, выпускаемых хлебозаводом
                        TotalCost();
                        break;
                    case '3': // Изделия с просроченными ингредиентами
                        ExpiredProductsList();
                        break;
                    case '4': // Список хлебозаводов в порядке убывания объёма производства
                        DescendingFactoriesList();
                        break;
                    case '5': // Изделие с наибольшим количеством ингредиентов
                        MaxIngredientsProduct();
                        break;
                    case '6': // Изделие с наибольшей прибылью при реализации
                        MaxRevenueProduct();
                        break;
                    case '7': // Добавить изделие, выпускаемое хлебозаводом
                        AddProduct();
                        break;
                    case '8': // Удалить изделие, выпускаемое хлебозаводом
                        RemoveProduct();
                        break;
                    default:
                        Console.WriteLine("Такого действия нет в списке");
                        break;
                }
                Console.Write("Нажмите любую клавишу для продолжения работы ");
                Console.ReadKey(true);
            }
        }

        static char UserChoice()
        {
            Console.Clear();
            Console.WriteLine("0 - Выход");
            Console.WriteLine("1 - Перечень изделий, выпускаемых хлебозаводом");
            Console.WriteLine("2 - Суммарная стоимость изделий, выпускаемых хлебозаводом");
            Console.WriteLine("3 - Изделия с просроченными ингредиентами");
            Console.WriteLine("4 - Список хлебозаводов в порядке убывания объёма производства");
            Console.WriteLine("5 - Изделие с наибольшим количеством ингредиентов");
            Console.WriteLine("6 - Изделие с наибольшей прибылью при реализации");
            Console.WriteLine("7 - Добавить изделие, выпускаемое хлебозаводом");
            Console.WriteLine("8 - Удалить изделие, выпускаемое хлебозаводом");
            return Console.ReadKey(true).KeyChar;
        }
        // Перечень изделий, выпускаемых хлебозаводом
        static void AllProductsList()
        {
        }
        // Суммарная стоимость изделий, выпускаемых хлебозаводом
        static void TotalCost()
        {
        }
        // Изделия с просроченными ингредиентами
        static void ExpiredProductsList()
        {
        }
        // Список хлебозаводов в порядке убывания объёма производства
        static void DescendingFactoriesList()
        {
        }
        // Изделие с наибольшим количеством ингредиентов
        static void MaxIngredientsProduct()
        {
        }
        // Изделие с наибольшей прибылью при реализации
        static void MaxRevenueProduct()
        {
        }
        // Добавить изделие, выпускаемое хлебозаводом
        static void AddProduct()
        {
        }
        // Удалить изделие, выпускаемое хлебозаводом
        static void RemoveProduct()
        {
        }
        // Загрузка списка из файла
        static void ReadList(int index, int fieldCount)
        {
            if(index >= paths.Length)
                return;
            StreamReader stream = new StreamReader(paths[index], Encoding.Default);
            string line;
            int key = 1;
            switch(index)
            {
                case 0:
                    while((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if(fields.Length == fieldCount)
                        {
                            Factory item = new Factory();
                            item.Key = key++;
                            item.Name = fields[0];
                            item.ProdDate = DateTime.Parse(fields[1]);
                            factories.Add(item);
                        }
                    }
                    break;
                case 1:
                    while ((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if (fields.Length == fieldCount)
                        {
                            Product item = new Product();
                            item.Key = key++;
                            item.Name = fields[0];
                            item.Weight = Convert.ToDouble(fields[1]);
                            item.ExpiryDate = DateTime.Parse(fields[2]);
                            item.Count = Convert.ToInt32(fields[3]);
                            item.Price = Convert.ToDouble(fields[4]);
                            products.Add(item);
                        }
                    }
                    break;
                case 2:
                    while ((line = stream.ReadLine()) != null)
                    {
                        string[] fields = line.Split(';');
                        if (fields.Length == fieldCount)
                        {
                            Ingredient item = new Ingredient();
                            item.Key = key++;
                            item.Name = fields[0];
                            item.Weight = Convert.ToDouble(fields[1]);
                            item.DeliveryDate = DateTime.Parse(fields[2]);
                            item.ExpiryDate = DateTime.Parse(fields[3]);
                            ingredients.Add(item);
                        }
                    }
                    break;
            } // switch
            Console.WriteLine(paths[index]);
        }
    }
}
