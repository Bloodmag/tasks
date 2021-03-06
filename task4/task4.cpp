// Задумка моего решения такова: время хранится в минутах, где 8:00 соответствует 0 и 20:00 соответствует 720. Создаётся список, который проще всего представить
// себе, как временной отрезок, протяжённостью в весь рабочий день. Также этот отрезок хранит в себе число посетителей (до чтения файла оно равно нулю).
// После этого мы начинаем читать файл, последовательно берём посетителей и "наносим" их на наш "отрезок". Получаем кучу мелких отрезков. Затем мы "склеиваем" соседние 
// отрезки времени, в которые количество посетителей было равным. Максимум, если каждую минуту кто-то приходил или уходил, мы получим список из 720 структур, с 
// чем должен справиться любой компьютер.

#include <iostream>
#include <iomanip>
#include <list>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

struct visitors {
	int bgn; // время начала отрезка [мин]
	int end; // время конца отрезка [мин]
	long long quantity; // количество поетителей на данном временном отрезке
};

void insert(list <visitors>* l,visitors v); // вставка нового посетителя в общую временную шкалу
void printList(list <visitors>* l); // функция для вывода на экран всего списка (подробный отчёт)
void rearrange(list <visitors>* l);  // функция "склейки" соседних отрезков при условии их равенства
bool parseStr(string* s, visitors *v);  // функция, которая строку входного файла превращает в данные для функции вставки

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	//переменные
	list <visitors> l;   //используем список. моё решение редполагает множество вставок, перед которыми наш контейнер все равно придётся "обыскивать" с начала до конца
	ifstream fin;
	visitors v = {0,720,0};
	int max = 0;
	char fileName[1024];
	string str;
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

	l.push_back(v); // инициализируем список

	while (getline(fin,str)) // читаем файл
	{
		if (parseStr(&str, &v)) {
			insert(&l, v);
		}
	}

	rearrange(&l);

	for (visitors i: l) // находим максимум
	{
		if (i.quantity > max) max = i.quantity;
	}

	cout << setfill('0');
	cout << "Наибольшее число посетителей (" << max << ") было в следующие промежутки времени:" << endl;
	for (visitors i : l)  //  выводим максимумы
	{
		if (i.quantity == max) cout << setw(2) << i.bgn / 60 + 8 << ":" << setw(2) << i.bgn % 60 << " - " << setw(2) << i.end / 60 + 8 << ":" << setw(2) << i.end % 60 << endl;
	}

	//cout << endl ;
	// printList(&l);

	system("pause");
	return 0;
}

void insert(list<visitors>* l, visitors v)
{
	auto it = l->begin();
	while (1) {
		if (it->end > v.bgn) {
			if (it->bgn == v.bgn) {
				if (v.end < it->end) {
					l->insert(next(it, 1), *it);
					next(it, 1)->bgn = v.end;
					it->end = v.end;
					it->quantity++;
					return;
				} else if (v.end == it->end) {
					it->quantity++;
					return;
				} else {
					it->quantity++;
					v.bgn = it->end;//next
				}
			} else {//v.bgn > it->bgn 
				l->insert(next(it, 1), *it);
				next(it, 1)->bgn = v.bgn;
				it->end = v.bgn;
			}
		}
		if (it == l->end()) break;
		it++;
	}
	return;
}

void printList(list<visitors>* l)
{
	cout << setfill('0');
	for (auto it = l->begin(); it != l->end(); ++it)
		cout << setw(2) << it->bgn / 60 + 8 << ":" << setw(2) << it->bgn % 60 << " - " << setw(2) << it->end / 60 + 8 << ":" << setw(2) << it->end % 60 << "     " << it->quantity << " visitors" << endl;
}

void rearrange(list<visitors>* l)
{
	auto it = l->begin();
	if (l->size() > 1) {
		it++;
		while (1) {
			if (prev(it, 1)->quantity == it->quantity) {
				prev(it, 1)->end = it->end;
				it = l->erase(it);
				continue;
			}
			if (it->end == 720) return;
			it++;
		}
	}
}


bool parseStr(string* s, visitors *v) {
	int bgn, end;
	if (s->size() >= 10) {
		if (s->find(":") != -1 && s->find_last_of(":") != s->find(":")) {//проверка на наличие 2-х знаков ":"
			if (s->find(":") >= 1 && s->find_last_of(":") <= s->size() - 1) {//проверка-защита от выхода за границы
				try {
					bgn = (stoi(s->substr(0, s->find(":"))) - 8) * 60 + stoi(s->substr(s->find(":") + 1, 2));
					end = (stoi(s->substr(s->find_last_of(":") - 2, 2)) - 8) * 60 + stoi(s->substr(s->find_last_of(":") + 1, 2));
				}
				catch (const std::invalid_argument ia) {
					return false;
				}
				if (bgn>=0 && bgn<end && end<=720) {
					v->bgn = bgn;
					v->end = end;
					return true;
				}
			}
		}
	} 
	return false;
}