# Лабораторная D

## Вариант 25

Упаковка в рюкзак
Условие
Задано конечное множество предметов U , размер каждого предмета s(u),  u из U,
стоимость каждого предмета v(u),  u из U,
 положительное целое число B -
вместимость рюкзака и положительное целое число K . Существует ли такое множество
U' из U, что: s(u)<= B и sum(v(u)) >=K
Исходные данные
В первой строке текстового файла input.txt записаны три целых положительных числа
|U| ,B,K . |U| < 1000, B < 2^32, K < 2^32. В следующей строке записано |U| целых
положительных чисел – веса предметов, а в третьей строке записано |U| целых
положительных чисел – стоимость предметов. Найдите множество U’ – удовлетворяющее
условию задачи. Выведите в текстовый файл output.txt номера элементов попавших в U’.
Нумерация предметов начинается с 1. Если такого множества не существует выведите 0.