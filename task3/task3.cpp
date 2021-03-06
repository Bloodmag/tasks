// В условиях задачи не уточняется, в каком виде требуется интервал и со скольки до скольки магазин работает. Для простоты проверки я решил дать ответ в двух
// вариантах: время, предполагая, что магазин работает с 10:00 и номер интервала (отсчёт для удобства чтения человеком начинается с 1). ИМХО данная задача 
// трудна лишь нормальной реализацией ввода данных из файлов.

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <iomanip>
using namespace std;

const long double EPSILON = .000000000000001;// если разность двух чисел с плавающей точкой ниже данной константы, они будут считаться равными

bool inline isEqual(long double left, long double right);
//функция для сравнения равенства чисел с плавающей точкой
bool readFile(long double* data, ifstream* fin);


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	//переменные
	ifstream fin;
	char fileName[1024];
	long double data[5][16], avg[16], max = 0;
	int filesRead = 0;
	//читаем файлы (либо из аргументов, либо из консоли)
	if (argc >1) {
		for (int i = 0; i < 5; i++) {
			fin.open(argv[i+1]);
			if (!fin.is_open()) {
				cout << "Файл "<<argv[i+1]<<" не открылся по неизвестным причинам =( Вы можете попробовать ввести их названия вручную." << endl;
				filesRead = 0;
				break;
			}
			if (!readFile(data[i], &fin)) {
				cout << "При чтении файла " << argv[i+1] << " произошла ошибка =( Вы можете попробовать ввести их названия вручную." << endl;
				filesRead = 0;
				break;
			}
			filesRead++;
			fin.close();
		}
	} else {
		cout << "Эта программа способна принимать файл с данными в качестве аргумента, однако, при желании, вы можете ввести его здесь." << endl;
	}
	if(filesRead!=5){
		cout << "Сейчас вам придётся ввести имена пяти файлов вручную:" << endl;
		while (filesRead < 5)
		{
			cout << "Введите имя "<<filesRead+1<<"-го файла:" << endl;
			//std::cin.ignore();
			cin.getline(fileName, 1024);
			fin.open(fileName);
			if (!fin.is_open()) {
				cout << "Ваш файл не открылся по неизвестным причинам =( Пожалуйста, попробуйте ещё раз." << endl;
				fin.close();
				continue;
			}
			if (!readFile(data[filesRead], &fin)) {
				cout << "При чтении файлa произошла ошибка =( Попробуем ещё раз?" << endl;
				fin.close();
				continue;
			} else {
				filesRead++;
			}
			fin.close();
		}
	}
	
	for (int i = 0; i < 16; i++) {
		avg[i] = data[0][i] + data[1][i] + data[2][i] + data[3][i] + data[4][i];
		if (max < avg[i]) max = avg[i];
	}
	cout << setfill('0');
	for (int i = 0; i < 16; i++) {
		if (isEqual(avg[i], max))
			cout << 10 + i / 2 << ":" << setw(2) << (i % 2) * 30 << " - " << 10 + i / 2 << ":" << setw(2) << ((i + 1) % 2) * 30 << "     интервал " << i + 1 << endl;
	}

	system("pause");
	return 0;
}

bool inline isEqual(long double left, long double right) {
	return fabsl(left - right) < EPSILON;
}

bool readFile(long double* data, ifstream* fin) {
	double long b;
	int i = 0;
	while (*fin >> b) {
		data[i] = b;
		i++;
		if (i == 16) return true;
	}
	return false;
}