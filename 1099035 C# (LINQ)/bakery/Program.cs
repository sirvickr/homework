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

        static string[] paths = new string[]{
            "factories",
            "products",
            "ingredients"
        };

        static void Main(string[] args)
        {
            // Загрузить список хлебзаводов
            ReadList(0, 2);

            // Загрузить список изделий
            ReadList(1, 6);

            // Загрузить список ингредиентов
            ReadList(2, 5);

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
                        FactoryProductsList();
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

        // Вывод пользовательского меню и запрос выбора
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

        // Запрос у пользователя номера интересующего его хлебозавода
        static int RequestFactoryKey()
        {
            Console.WriteLine("Список заводов:");
            Console.WriteLine("№\tНазвание");
            foreach (var item in factories)
            {
                Console.WriteLine("{0}\t{1}", item.Key, item.Name);
            }
            Console.Write("Введите № завода: ");
            return Convert.ToInt32(Console.ReadLine().Trim());
        }

        // Перечень изделий, выпускаемых хлебозаводом
        static int FactoryProductsList(bool choose = false)
        {
            int id = RequestFactoryKey();
            var result = products.Where(p => p.FactoryKey == id);
            // тот же запрос через операторы LINQ:
            // var result = from p in products where p.FactoryKey == id select p;
            Console.WriteLine("№\tВес\tЦена\tВыпуск\tГоден до\t\tНазвание");
            foreach (var p in result)
            {
                Console.WriteLine("{0}\t{1}\t{2}\t{3}\t{4}\t{5}",
                    p.Key, p.Weight, p.Price, p.Count, p.ExpiryDate, p.Name);
            }
            if (choose)
            {
                Console.Write("Введите № партии: ");
                return Convert.ToInt32(Console.ReadLine().Trim());
            }
            return 0;
        }

        // Суммарная стоимость изделий, выпускаемых хлебозаводом
        static void TotalCost()
        {
            int id = RequestFactoryKey();
            var result = products.Where(p => p.FactoryKey == id)
                .GroupBy(p => p.FactoryKey)
                .Select(g => new {
                    Id = g.Key,
                    Total = g.Sum(p => p.Price * p.Count)
                });
            /* тот же запрос через операторы LINQ:
            var result = from p in products where p.FactoryKey == id
                         group p by p.FactoryKey into g
                         select new { 
                            Id = g.Key, 
                            Total = g.Sum(p => p.Price * p.Count) 
                         };
            */
            foreach(var group in result)
            {
                Console.WriteLine("Суммарная стоимость выпущенных изделий: {0} руб.", group.Total);
            }
        }

        // Изделия с просроченными ингредиентами
        static void ExpiredProductsList()
        {
        }

        // Список хлебозаводов в порядке убывания объёма производства
        static void DescendingFactoriesList()
        {
            var result = factories.GroupJoin(
                products,
                f => f.Key,
                p => p.FactoryKey,
                (fact, prods) => new
                {
                    fact.Key,
                    fact.Name,
                    Count = prods.Sum(prod => prod.Count)
                }
            ).OrderByDescending(f => f.Count);
            Console.WriteLine("№\tНазвание\tОбъём производства");
            foreach (var f in result)
            {
                Console.WriteLine("{0}\t{1}\t\t{2}", f.Key, f.Name, f.Count);
            }
        }

        // Изделие с наибольшим количеством ингредиентов
        static void MaxIngredientsProduct()
        {
        }

        // Изделие с наибольшей прибылью при реализации
        static void MaxRevenueProduct()
        {
            var view = products.Select(p => new {
                p.Name,
                Revenue = p.Price * p.Count
            });
            var maxVal = view.Max(v => v.Revenue);
            var result = view.Where(v => v.Revenue == maxVal);
            Console.WriteLine("Изделия с наибольшей прибылью:");
            foreach (var item in result)
            {
                Console.WriteLine("{0} ({1} руб).", item.Name, item.Revenue);
            }
        }

        // Добавить изделие, выпускаемое хлебозаводом
        static void AddProduct()
        {
            int factoryKey = RequestFactoryKey();
            int key = products.Count + 1;
            Console.Write("Введите наименование продукции: ");
            string name = Console.ReadLine().Trim();
            Console.Write("Введите вес единицы продукции (кг): ");
            double weight = Convert.ToDouble(Console.ReadLine().Trim());
            Console.Write("Введите дату окончания срока годности (ГГГГ-ММ-ДД): ");
            DateTime date = DateTime.Parse(Console.ReadLine().Trim());
            Console.Write("Введите объём партии (шт): ");
            int count = Convert.ToInt32(Console.ReadLine().Trim());
            Console.Write("Введите цену единицы продукции: ");
            double price = Convert.ToDouble(Console.ReadLine().Trim());
            Product product = new Product();
            product.Key = key;
            product.FactoryKey = factoryKey;
            product.Name  = name;
            product.Weight = weight;
            product.ExpiryDate =date ;
            product.Count = count;
            product.Price = price;
            products.Add(product);
        }

        // Удалить изделие, выпускаемое хлебозаводом
        static void RemoveProduct()
        {
            int key = FactoryProductsList(true);
            var item = products.SingleOrDefault(p => p.Key == key);
            if (item != null)
            {
                products.Remove(item);
            }
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
                            item.FactoryKey = Convert.ToInt32(fields[0]);
                            item.Name = fields[1];
                            item.Weight = Convert.ToDouble(fields[2]);
                            item.ExpiryDate = DateTime.Parse(fields[3]);
                            item.Count = Convert.ToInt32(fields[4]);
                            item.Price = Convert.ToDouble(fields[5]);
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
