#include <iostream>
#include <string>
#include <fstream>

using namespace std;

unsigned long int konFactorial(long int sim, long int n) // факториал считается с конца
{
	if (n != 0) {
		unsigned long int res = sim;
		while (n != 1) {
			res = res * (sim - 1);
			sim--;
			n--;
		}
		return res;
	}
	return 1;
}

unsigned long int Factorial(int number) //факториал считается с начала
{
	if (number > 0)
		return number * Factorial(number - 1);
	else
		return 1;
}

unsigned long int CFunc(long int first, long int second) //сочитание
{
	long int k, n, l;
	unsigned long int res,predres;
	k = first - second; // разница между первым и вторым
	if (k >= second) {
		n = first - k; // n - количество факториала с конца
		predres = konFactorial(first, n);
		l = Factorial(second);
		res = predres / l;
	}
	else {
		n = first - second;
		predres = konFactorial(first, n);
		l = Factorial(k);
		res = predres / l;
	}

	return res;
}
unsigned long int AFunc(long int first, long int second) // размещение
{
	if (first - second == 0) {
		return konFactorial(first, first);
	}
	else {
		if (first - second == first) {
			return 1;
		}
		else {
			return konFactorial(first, second);
		}

	}
}



double error(const char* s)
{
	cerr << "error: " << s << "\n";//cerr-вывод ошибки, так как мы работаем через буфер, нам не нужно чтобы он засорялся
	return 0;
}

enum tokenValue //"Состояния" enum - перечисления
{
	NUMBER, END, PLUS = '+',
	MINUS = '-', MUL = '*', DIV = '/',
	PRINT = ';', ASSIGN = '=', LP = '(', RP = ')',
	P = 'P', A = 'A', C = 'C', POW = '^', CP = 'c', AP = 'a'
};

tokenValue currTok;//Переменная принимающая разные состояния

long double numberValue;

tokenValue getToken()//Берем лексему
{
	char ch;
	do
	{ // пропускает обобщенные пробелы за исключением '\n'
		if (!cin.get(ch)) return currTok = END;//cin.get() посимвольно читает из потока
	} while (ch != '\n' && isspace(ch));
	switch (ch)
	{
	case '\n':
		cin >> ws; // пропуск пробела при переводе строки
		return currTok = PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
		return currTok = tokenValue(ch);
	case '(':
	case ')':
	case '=':
	case 'A':
	case 'P':
	case 'C':
	case '^':
	case 'c':
	case 'a':
		return currTok = tokenValue(ch);
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		cin.putback(ch);//возвращает символ обратно в буфер
		cin >> numberValue;
		return currTok = NUMBER;//приравниваем лексему к числу
	default:
		error("недопустимая лексема");
		return currTok = END;
	}
}

double expr();//Прототип функции сложения и вычитания

double prim()// обрабатывает первичное(Возвращает Цифру)
{
	switch (currTok)
	{
	case NUMBER: // константа с плавающей точкой
		getToken();
		return numberValue;
	case MINUS: // минус
		getToken();
		return -prim();
	case LP:
	{
		getToken();
		long double e = expr();
		if (currTok != RP) return error("требуется )");
		getToken();
		return e;
	}
	case END:
		return 0;
	default:
		return error("требуется первичное");
	}
}

double term() // функция умножения и сложения
{
	long double left = prim();
	long double d;
	for (;;)
		switch (currTok)
		{
		case MUL:
			getToken(); // случай '*'
			left *= prim();
			break;
		case DIV:
			getToken();// случай '/'
			d = prim();
			if (d == 0)
				return error("деление на 0");
			left /= d;
			break;
		case CP:
			getToken();
			d = prim();
			left = CFunc(left + d - 1, left - 1);
			break;
		case AP:
			getToken();
			d = prim();
			left = pow(left, d);
			break;
		case C:
			getToken();
			d = prim();
			left = CFunc(left, d);
			break;
		case A:
			getToken();
			d = prim();
			left = AFunc(left, d);
			break;
		case P:
			getToken();
			left = Factorial(left);
			break;
		case POW:
			getToken();
			d = prim();
			left = pow(left, d);
			break;
		default:
			return left;
		}
}

double expr() // складывает и вычитает термы
{
	double left = term();
	for (;;)//``вечно''
		switch (currTok)
		{
		case PLUS:
			getToken(); // случай '+'
			left += term();
			break;
		case MINUS:
			getToken();// случай '-'
			left -= term();
			break;
		default:
			return left;
		}
}

void main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Сочетание из n по m -> (nCm)\nСочетание с повторениями из n по m -> (ncm)\n";
	cout << "Размещение из n по m -> (nAm)\nРазмещение с повторениями из n по m -> (nam)\n";
	cout << "Перестановка из n -> (nP)\nВвод каждого комбинаторного объекта осуществляется в скобках\n";
	cout << "После выражения обязательно должен стоять знак =\nСчитывание выражений происходить до введения неверной лексемы\n";
	cout << "Введите выражения : \n";
	do
	{
		getToken();
		if (currTok == END) break;
		if (currTok == PRINT) continue;
		printf("%.0Lf", expr());
		cout << endl;
	} while (cin);
	system("Pause");
}
/*
long int konFactorial(long int sim, long int n) // факториал считается с конца
{
	if (n != 0) {
		long int res = sim;
		while (n != 1) {
			res = res * (sim - 1);
			sim--;
			n--;
		}
		return res;
	}
	return 1;
}

long int Factorial(int number) //факториал считается с начала
{
	if (number > 0)
		return number * Factorial(number - 1);
	else
		return 1;
}

long int CFunc(long int first, long int second) //сочитание
{
	long int k, n, res, l, predres;
	k = first - second; // разница между первым и вторым
	if (k >= second) {
		n = first - k; // n - количество факториала с конца
		predres = konFactorial(first, n);
		l = Factorial(second);
		res = predres / l;
	}
	else {
		predres = konFactorial(first, k);
		l = Factorial(k);
		res = predres / l;
	}

	return res;
}

long int AFunc(long int first, long int second) // размещение
{
	if (first - second == 0) {
		return konFactorial(first, first);
	}
	else if (first - second == first)
	{
		return 1;
	}
	else {
		return konFactorial(first,second);
	}
}


double error(const char* s)
{
	cerr << "error: "<< s << "\n";//cerr-вывод ошибки, так как мы работаем через буфер, нам не нужно чтобы он засорялся
	return 0;
}

enum tokenValue //"Состояния" enum - перечисления
{
	NUMBER, END, PLUS = '+',
	MINUS = '-', MUL = '*', DIV = '/',
	PRINT = ';', ASSIGN = '=', LP = '(', RP = ')',
	P = 'P', A = 'A', C = 'C', POW = '^'
};

tokenValue currTok;//Переменная принимающая разные состояния

long double numberValue;

tokenValue getToken()//Берем лексему
{
	char ch;
	do
	{ // пропускает обобщенные пробелы за исключением '\n'
		if (!cin.get(ch)) return currTok = END;//cin.get() посимвольно читает из потока
	} while (ch != '\n' && isspace(ch));
	switch (ch)
	{
	case '\n':
		cin >> ws; // пропуск пробела при переводе строки
		return currTok = PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
		return currTok = tokenValue(ch);
	case '(':
	case ')':
	case '=':
	case 'A':
	case 'P':
	case 'C':
	case '^':
		return currTok = tokenValue(ch);
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		cin.putback(ch);//возвращает символ обратно в буфер
		cin >> numberValue;
		return currTok = NUMBER;//приравниваем лексему к числу
	default:
		error("недопустимая лексема");
		return currTok = END;
	}
}

double expr();//Прототип функции сложения и вычитания

double prim()// обрабатывает первичное(Возвращает Цифру)
{
	switch (currTok)
	{
	case NUMBER: // константа с плавающей точкой
		getToken();
		return numberValue;
	case MINUS: // минус
		getToken();
		return -prim();
	case LP:
	{
		getToken();
		long double e = expr();
		if (currTok != RP) return error("требуется )");
		getToken();
		return e;
	}
	case END:
		return 0;
	default:
		return error("требуется первичное");
	}
}

double term() // функция умножения и сложения
{
	long double left = prim();
	long double d;
	for (;;)
		switch (currTok)
		{
		case MUL:
			getToken(); // случай '*'
			left *= prim();
			break;
		case DIV:
			getToken();// случай '/'
			d = prim();
			if (d == 0)
				return error("деление на 0");
			left /= d;
			break;
		case C:
			getToken();
			d = prim();
			left = CFunc(left, d);
			break;
		case A:
			getToken();
			d = prim();
			left = AFunc(left, d);
			break;
		case P:
			getToken();
			left = Factorial(left);
			break;
		case POW:
			getToken();
			d = prim();
			left = pow(left, d);
			break;
		default:
			return left;
		}
}

double expr() // складывает и вычитает термы
{
	double left = term();
	for (;;)// ``вечно''
		switch (currTok)
		{
		case PLUS:
			getToken(); // случай '+'
			left += term();
			break;
		case MINUS:
			getToken();// случай '-'
			left -= term();
			break;
		default:
			return left;
		}
}

void main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Сочетание из n по m запись nCm, аналогично с размещением nAm, перестановка из n  nP\n";
	cout << "Введите выражения : \n";
		//while (cin)
		//{
			//getToken();
			//if (currTok == END) break;
			//if (currTok == PRINT) continue;
			//answer << expr() << '\n';
		//}
	do
	{
		getToken();
		if (currTok == END) break;
		if (currTok == PRINT) continue;
		cout << expr() << '\n';
	} while (cin);
	system("Pause");
}
*/