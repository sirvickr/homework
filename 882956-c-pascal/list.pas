{Вводим новый тип данных}
Type BT = LongInt;
Type U = ^Zveno;
Zveno = Record Inf : BT; Next: U End;
{Проверка на пустой элемент - нулевой адрес}
Function Pust(First : U) : Boolean;
Begin
  Pust := First = Nil
End;
{Процедура добавления элемента в начало списка; в x содержится добавляемая информация}
Procedure V_Nachalo(Var First : U; X : BT);
Var Vsp : U;
Begin
  New(Vsp);
  Vsp^.Inf := X;
  {Тот элемент, что был заглавным, становится вторым по счѐту}
  Vsp^.Next := First;
  First := Vsp; {Новый элемент становится заглавным}
End;
{Процедура удаления элемента из начала списка; в x содержится информация из удалѐнного звена}
Procedure Iz_Nachala(Var First : U; Var X : BT);
Var Vsp : U;
Begin
  Vsp := First; {Забираем ссылку на текущее заглавный элемент}
  First := First^.Next; {Второй элемент становится заглавным}
  X := Vsp^.Inf; {Забираем информацию из удаляемого элемента}
  Dispose(Vsp); {Уничтожаем элемент}
End;
{Процедура добавления элемента в конец списка}
Procedure V_Konez(Var First : U; X : BT);
Var Vsp, Tmp : U;
Begin
  New(Vsp); {Создаем пустый элемент}
  Vsp^.Inf := X; {Заносим информацию}
  Vsp^.Next:= Nil; {Обнуляем ссылку}
  {Если список был пустой - включаем первый элемент}
  If First = Nil Then First := Vsp
  Else
  Begin
    Tmp := First; {Проходим до конца списка}
    While Not Pust(Tmp^.Next) Do Tmp := Tmp^.Next;
    Tmp^.Next := Vsp; {Вставляем элемент последним}
  End;
End;
{Процедура удаления элемента из конца списка}
Procedure Iz_Konza(Var First : U; Var X : BT);
Var Vsp, Tmp : U;
Begin
  Tmp := First;
  Vsp := First;
  {Если список состоит из одного элемента}
  If First^.Next = Nil Then First := Nil
  Else
  Begin
    {Проходим до предпоследнего элемента}
    While Not Pust(Tmp^.Next^.Next) Do Tmp := Tmp^.Next;
    Vsp := Tmp^.Next; {Запоминаем адрес удаляемого элемента}
    Tmp^.Next := Nil; {Определяем новый конец списка}
  End;
  X := Vsp^.Inf; {Забираем информацию из удаляемого звена}
  Dispose(Vsp); {Уничтожаем элемент}
End;
{Процедура печати списка}
Procedure Print(First : U);
Begin
  If First = Nil Then writeln('Пустой список!')
  Else
  Begin
    While First <> Nil Do
    Begin
      Write(First^.Inf : 6);
      First := First^.Next
    End;
  End;
  WriteLn
End;
{основная программа - вызовы функций}
var chislo : BT;
Var L1, L2, L3 : U;
Var First : U;
{Var Vsp, Tmp : U;}
Begin
  L1 := Nil;
  L2 := Nil;
  L3 := Nil;
  {Вводим первый список}
  writeln('Ввод первого списка.');
  writeln('вводи числа(0 - конец ввода)');
  read(chislo);
  While chislo <> 0 Do
  Begin
    V_Konez(L1, chislo);
    read(chislo);
  End;
  writeln('Первый список:');
  Print(L1);
  {Вводим второй список}
  writeln('Ввод второго списка.');
  writeln('вводи числа(0 - конец ввода)');
  read(chislo);
  While chislo <> 0 Do
  Begin
    V_Konez(L2, chislo);
    read(chislo);
  End;
  writeln('Второй список:');
  Print(L2);
  {Объединяем списки, создавая на их основе третий список}
  First := L1;
  While First <> Nil Do
  Begin
    V_Konez(L3, First^.Inf);
    First := First^.Next
  End;
  First := L2;
  While First <> Nil Do
  Begin
    V_Konez(L3, First^.Inf);
    First := First^.Next
  End;
  writeln('Третий список:');
  Print(L3);
End.