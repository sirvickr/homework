# Тест для проверки операции вычитания

# определение положительного целого параметра
DEFINE a 8
# определение отрицательного целого параметра
DEFINE b -3
# определение положительного дробного параметра
DEFINE c 4.5
# определение отрицательного дробного параметра
DEFINE d -3.25
# добавление в стек положительного целого числа
PUSH a
# попытка выполнения операции при недостаточном количестве операндов (ожидается сообщение об ошибке)
-
# добавление в стек положительного целого числа
PUSH 3
# выполнение операции: 3 - 8 = -5
-
# печать вершины стека (ожидается: "-5")
PRINT
# добавление в стек отрицательного целого числа
PUSH b
# выполнение операции: -3 - (-5) = 2
-
# печать вершины стека (ожидается: "2")
PRINT
# добавление в стек положительного дробного числа
PUSH c
# выполнение операции: 4.5 - 2 = 2.5
-
# печать вершины стека (ожидается: "2.5")
PRINT
# добавление в стек отрицательного дробного числа
PUSH d
# выполнение операции: -3.25 - 2.5 = -5.75
-
# печать вершины стека (ожидается: "-5.75")
PRINT
