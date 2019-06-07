# Тест для комплексной проверки работы программы - совместное выполнение операций

# определение положительного целого параметра
DEFINE a 16
# добавление в стек положительного целого числа
PUSH a
# выполнение операции: SQRT(16) = 4
SQRT
# печать вершины стека (ожидается: "4")
PRINT
# определение отрицательного целого параметра
DEFINE b -5
# добавление в стек отрицательного целого числа
PUSH b
# выполнение операции: -5 - 4 = -9
-
# печать вершины стека (ожидается: "-9")
PRINT
# определение положительного дробного параметра
DEFINE c 12.5
# добавление в стек положительного дробного числа
PUSH c
# выполнение операции: 12.5 + (-9) = 3.5
+
# печать вершины стека (ожидается: "3.5")
PRINT
# добавление в стек нескольких чисел подряд
PUSH 1
PUSH 2
PUSH 3
# печать вершины стека (ожидается: "3")
PRINT
# выполнение операции: 3 / 2 = 1.5
/
# печать вершины стека (ожидается: "1.5")
PRINT
# выполнение операции: 1.5 + 1 = 2.5 (2 и 3 должны быть уже изъяты из стека)
+
# печать вершины стека (ожидается: "2.5")
PRINT
