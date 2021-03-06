// task1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	//переменные
	vector <long long> num; //нахождение точного значения медианы/перцентиля, увы, требует хранить в памяти всю последовательность
	ifstream fin;
	long long buff,min,max,count,sum;
	char fileName[1024];
	//читаем файлы (либо из аргументов, либо из консоли)
	if (argc > 1) {
		fin.open(argv[1]);
		if (!fin.is_open()) {
			cout << "Ваш файл не открылся по неизвестным причинам =(" << endl;
			getchar();
		}
	}else{
		cout<<"Эта программа способна принимать файл с данными в качестве аргумента, однако, при желании, вы можете ввести его здесь:"<<endl;
		cin.getline(fileName, 1024);
		fin.open(fileName);
		if (!fin.is_open()) {
			cout << "Ваш файл не открылся по неизвестным причинам =( Пожалуйста, перезапустите программу." << endl;
			getchar();
			return 2;
		}
	}

	//====================================================== основная работа выполняется ниже ==================================================================

	//задаём первые значения переменных поиска минимума и максимума, араллельно проверяя, загрузилось ли хоть что-то из файла
	if (fin >> buff) {
		min = buff;
		max = buff;
		sum = buff;
		count = 1;
		num.push_back(buff);
	}else{
		cout << "Ни одного числа в файле! Как так?" << endl;
		getchar();
		return 1;
	}

	while (fin>>buff)// readin' from file!
	{
		num.push_back(buff);
		if (max < buff) max = buff;
		if (min > buff) min = buff;
		sum += buff;
		count++;
	}
	fin.close();
	
	sort(num.begin(), num.end());

	cout << "90 percentile " << num[round((long double)num.size()*0.9-1)] << endl;
	cout << "median " << ((bool)(num.size() % 2) ? num[(long long)num.size() / 2] : (long double)(num[(long long)num.size() / 2 - 1] + num[(long long)num.size() / 2])/2 ) << endl;// нам нужен срединный элемент. их кол-во чётно? делим в целых числах, результат подходит, ибо нумерация с нуля : среднее арифм. двух "срединных" элементов
	cout << "average " << (double long)sum/count << endl;
	cout << "max " << max << endl;
	cout << "min " << min << endl;
	getchar();

	return 0;
}
