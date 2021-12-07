#include <stdio.h>
#include <locale.h>
#include <Windows.h>

typedef struct {
	int day;
	int month;
	int year;
	int isLeap;
	int maxDays;
	int BC;
} date;

int isLeapYear(int);
int maxDaysInMonth(int, int);
int dateIsCorrect(int, int, int);
void printDate(date);
date addDays(date, int);
date subtractDays(date, int);

const int maxDaysArr[12] = {
	31, 28, 31, 30,
	31, 30, 31, 31,
	30, 31, 30, 31
};

const char* monthsRU[12] = {
	"Января", "Февраля", "Марта",
	"Апреля", "Май", "Июня",
	"Июля", "Августа", "Сентября",
	"Октября", "Ноября", "Декабря",
};

int main() {
	setlocale(LC_ALL, "Rus");
	int cont = 1, trash = 0, enterCount = 0, addedDays;
	int tempDate[3];

	while (cont) {
		tempDate[0] = 0;
		tempDate[1] = 0;
		tempDate[2] = 0;
		// Ввод даты
		do {
			printf("Введите дату(Пример: 31 12 2020): ");
			enterCount = scanf_s("%d%d%d", &tempDate[0], &tempDate[1], &tempDate[2]);
			while (trash = getchar() != '\n');
			if (enterCount < 3) {
				system("cls");
				printf("Вводите только числа\n");
			}
		} while (enterCount < 3);

		while (!dateIsCorrect(tempDate[0], tempDate[1], tempDate[2])) {
			system("cls");
			printf("Некорректная дата\nПопробуйте заново\n");
			do {
				printf("Введите дату(Пример: 31 12 2020): ");
				enterCount = scanf_s("%d%d%d", &tempDate[0], &tempDate[1], &tempDate[2]);
				while (trash = getchar() != '\n');
				if (enterCount < 3) printf("Вводите только числа\n");
			} while (enterCount < 3);
		}

		date userDate = {
			.day = tempDate[0],
			.month = tempDate[1],
			.year = tempDate[2] > 0 ? tempDate[2] : -tempDate[2],
			.isLeap = isLeapYear(tempDate[2]),
			.maxDays = maxDaysInMonth(tempDate[1], tempDate[2]),
			.BC = tempDate[2] < 0 ? 1 : 0,
		};

		printf("Вы ввели дату: ");
		printDate(userDate);
		printf("\n");
		
		// Ввод прибавляемых дней
		do {
			printf("\nВведите кол-во дней, которые нужно прибавить: ");
			enterCount = scanf_s("%d", &addedDays);
			while (trash = getchar() != '\n');
			if (enterCount < 1) {
				printf("Вводите только числа\n");
			}
		} while (enterCount < 1);

		userDate = addedDays >= 0 ? addDays(userDate, addedDays) : subtractDays(userDate, addedDays);
		printf("Новая дата: ");
		printDate(userDate);
		printf("\n");


		printf("\n");
	}
	return 0;
}

int isLeapYear(int year) {
	if (year % 100 == 0 && year % 400 != 0) return 0;
	if (year % 4 == 0) return 1;
	return 0;
}

int maxDaysInMonth(int month, int year) {
	return isLeapYear(year) ? (month == 2 ? 29 : maxDaysArr[month - 1]) : maxDaysArr[month - 1];
}

int dateIsCorrect(int days, int month, int year) {
	if (days < 1 || month < 1 || month > 12 || year == 0) return 0;
	int maxDays = maxDaysInMonth(month, year);
	if (days > maxDays) return 0;
	return 1;
}

void printDate(date _date) {
	printf("%d %s %dг.", _date.day, monthsRU[_date.month - 1], _date.year);
	if (_date.BC) printf(" до н.э");
}

date addDays(date _date, int addedDays) {
	for (int i = 0; i < addedDays; i++) {
		// Если пора прибавлять месяцы
		if (_date.day + 1 > _date.maxDays) {
			// Если пора прибавлять года
			if (_date.month + 1 > 12) {
				_date.month = 1;
				_date.BC ? _date.year-- : _date.year++;
				if (_date.year == 0) {
					_date.year = 1;
					_date.BC = 0;
				}
				_date.isLeap = isLeapYear(_date.year); // Это просто для красоты
			}
			else {
				_date.month++;
				_date.maxDays = maxDaysInMonth(_date.month, _date.year);
			}
			_date.day = 1;
		}
		else _date.day++;
	}
	return _date;
}

date subtractDays(date _date, int subtractedDays) {
	for (int i = -subtractedDays; i > 0; i--) {
		// Если пора прибавлять месяцы
		if (_date.day - 1 < 1) {
			// Если пора прибавлять года
			if (_date.month - 1 < 1) {
				_date.month = 12;
				_date.BC ? _date.year++ : _date.year--;
				if (_date.year == 0) {
					_date.year = 1;
					_date.BC = 1;
				}
				_date.isLeap = isLeapYear(_date.year); // Это просто для красоты
			}
			else {
				_date.month--;
				_date.maxDays = maxDaysInMonth(_date.month, _date.year);
			}
			_date.day = _date.maxDays;
		}
		else _date.day--;
	}
	return _date;
}