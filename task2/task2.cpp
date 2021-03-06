// Данную задачу можно решить множеством различных способов. Мне больше всего понравился следующий вариант: поскольку четырёхугольник выпуклый (более того, вершины заданы по часовой), мы 
// можем принять его стороны за векторы и определить, находится ли точка справа от каждого из них. Та же самая функция проверит, не принадлежит ли точка одной с вектором прямой, то есть
// всю "магию" сделает одна функция в 4 строки.

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

const long double EPSILON = .000000000000001;// если разность двух чисел с плавающей точкой ниже данной константы, они будут считаться равными

int vecAndPoint(long double vx1, long double vy1, long double vx2, long double vy2, long double px, long double py);
//^^^ функция для определения положения точки относительно вектора ^^^
bool inline isEqual(long double left, long double right);
//функция для сравнения равенства чисел с плавающей точкой
bool inline contains(long double left, long double right, long double x);
//эту функцию я написал для удобочитаемости моих стоэтажных IF-ов. Она оределяет истинность x Є [min(left,right),max(left,right)]

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	//переменные
	ifstream fin;
	char fileName[1024];
	long double x[4], y[4], px, py;
	//читаем файлы (либо из аргументов, либо из консоли)
	if (argc > 1) {
		fin.open(argv[1]);
		if (!fin.is_open()) {
			cout << "Ваш файл не открылся по неизвестным причинам =(" << endl;
			getchar();
		}
	}
	else {
		cout << "Эта программа способна принимать файл с данными в качестве аргумента, однако, при желании, вы можете ввести его здесь:" << endl;
		cin.getline(fileName, 1024);
		fin.open(fileName);
		if (!fin.is_open()) {
			cout << "Ваш файл не открылся по неизвестным причинам =( Пожалуйста, перезапустите программу." << endl;
			getchar();
			return 2;
		}
	}
	for (int i = 0; i < 4; i++) {//read vertexes from the file
		fin >> x[i];
		fin >> y[i];
	}

	//====================================================== основная работа выполняется ниже ==================================================================
	cout << "Пожалуйста, введите координаты точки через пробел(x y):" << endl;
	cin >> px >> py;

	//logics
	//проверяем, не совпадает ли точка с одной из вершин
	if ((isEqual(px, x[0]) && isEqual(py, y[0])) 
	||	(isEqual(px, x[1]) && isEqual(py, y[1])) 
	||	(isEqual(px, x[2]) && isEqual(py, y[2])) 
	||	(isEqual(px, x[3]) && isEqual(py, y[3]))) {
		cout << "Данная точка является вершиной четырёхугольника" << endl;
		//Проверяем, не лежит ли точка на одной прямой с вектором стороны, одновременно находясь в одном с ним в одном AABB
	}	else if ((!vecAndPoint(x[0], y[0], x[1], y[1], px, py)   && contains(x[0], x[1], px) && contains(y[0], y[1], py))
	||			   (!vecAndPoint(x[1], y[1], x[2], y[2], px, py) && contains(x[1], x[2], px) && contains(y[1], y[2], py))
	||			   (!vecAndPoint(x[2], y[2], x[3], y[3], px, py) && contains(x[2], x[3], px) && contains(y[2], y[3], py))
	||			   (!vecAndPoint(x[3], y[3], x[0], y[0], px, py) && contains(x[3], x[0], px) && contains(y[3], y[0], py))) {
		cout << "Данная точка лежит на стороне четырёхугольника" << endl;
		//проверяем, не находится ли точка справа векторов, образуемых вершинами четырёхугольника при обходе о часовой стрелке (именно так они даются по условию)
	}	else if ((vecAndPoint(x[0], y[0], x[1], y[1], px, py) == -1) 
	&&			 (vecAndPoint(x[1], y[1], x[2], y[2], px, py) == -1)
	&&			 (vecAndPoint(x[2], y[2], x[3], y[3], px, py) == -1)
	&&			 (vecAndPoint(x[3], y[3], x[0], y[0], px, py) == -1))	{
		cout << "Точка лежит внутри четырёхугольника" << endl;
	}// если ничего из вышеперечисленного, то точка лежит вне фигуры.
	else cout << "Точка лежит снаружи четырёхугольника" << endl;
	system("pause");
	return 0;
}


int vecAndPoint(long double vx1, long double vy1, long double vx2, long double vy2, long double px, long double py) {
	double long s = (vx2 - vx1)*(py - vy1) - (vy2 - vy1)*(px - vx1);
	if (isEqual(s, 0)) return 0; //принадлежит одной с вектором прямой
	else if (s > 0) return  1; //слева 
	else return -1; //справа
}
bool inline isEqual(long double left, long double right) {
	return fabsl(left - right) < EPSILON;
}
bool inline contains(long double left, long double right, long double x) {
	return (x > min(left, right) || isEqual(x, min(left, right))) && (x < max(left, right) || isEqual(x, max(left, right)));
}