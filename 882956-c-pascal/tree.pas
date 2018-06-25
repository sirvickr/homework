{Вводим новый тип данных}
type ref = ^node;
node = record
  key, count: integer;
  left, right: ref;
end;
{Печать содержимого бинарного дерева}
procedure Print_Btree (w: ref; l: integer);
var i : integer;
begin
  if (w <> Nil) then
  with w^ do
  begin
    Print_Btree(right, l + 1);
    for i := 1 to l * 4 do
      write(' ');
      //writeln('значение = ',key,' счетчик = ', count);
    writeln(key);
    Print_Btree(left, l + 1);
  end;
end;
{Построение дерева повторений}
procedure Include (x: integer; var p:ref);
begin
  if p = Nil then
  begin {Добавляем вершину}
    new (p);
    with p^ do
    begin
      key := x;
      count := 1;
      left := Nil;
      right := Nil;
    end;
  end
  else
  begin
    if x = p^.key then
      {Если число есть в дереве, то увеличиваем счѐтчик}
      p^.count := p^.count + 1
    else
      if x > p^.key then
        {Добавляем в правое поддерево}
        Include(x, p^.right)
      else
        {Добавляем в левое поддерево}
        Include(x, p^.left);
  end;
end;
{Построение идеально-сбалансированного дерева с n вершинами}
function tree (n : integer): ref;
var
newnode : ref;
x, l, r : integer;
begin
  if n = 0 then tree := nil else
  begin
    l := n div 2; r := n - l - 1;
    read(x);
    {Добавляем вершину}
    new (newnode);
    with newnode^ do
    begin
      key := x;
      left := tree(l);
      right := tree(r);
    end;
    tree := newnode;
  end;
end;
{Построение дерева повторений}
procedure Swap_Max_Min (var btree: ref);
var
  pmin, pmax: ref;
  value, count: integer;
begin
  if btree <> Nil then
  begin
    {спускаемся к минимальному элементу}
    pmin := btree;
    while true Do
    begin
      if pmin^.left = Nil then
        break;
      pmin := pmin^.left;
    end;
    {спускаемся к максимальному элементу}
    pmax := btree;
    while true Do
    begin
      if pmax^.right = Nil then
        break;
      pmax := pmax^.right;
    end;
    {swap}
    value := pmax^.key;
    count := pmax^.count;
    pmax^.key := pmin^.key;
    pmax^.count := pmin^.count;
    pmin^.key := value;
    pmin^.count := count;
  end;
end;
{Основная программа}
var
root: ref;
k: integer;
begin
  { возможная тестовая последовательность: 3, -1, 5, 9, 8, -7, -5, 2, 1, 4 }
  root := Nil;
  writeln('Дерево повторений.');
  writeln('Вводи числа (0 - конец ввода)');
  read(k);
  while k <> 0 Do
  begin
    Include (k,root);
    read(k);
  end;
  writeln('Печать дерева');
  Print_Btree (root,2);
  Swap_Max_Min(root);
  writeln('Печать дерева (после смены мест максималльного и минимального элементов)');
  Print_Btree (root,2);
end.
