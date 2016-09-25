#include <iostream>
#include <string>
using namespace std;

template<typename DataType>
class Stack
{
private:
	DataType data[100];
	int top;
public:
	Stack() { top = -1; }
	~Stack() {}
	void push(DataType data);
	DataType pop();
	DataType get();
	bool empty();
	void clear();
};

template<typename DataType>
void Stack<DataType>::push(DataType data)
{
	if (top == 100)throw string("Stack Full");
	this->data[++top] = data;
}

template<typename DataType>
DataType Stack<DataType>::pop()
{
	if (top == -1)throw string("None");
	return data[top--];
}

template<typename DataType>
DataType Stack<DataType>::get()
{
	if (top == -1)throw string("None");
	return data[top];
}

template<typename DataType>
bool Stack<DataType>::empty()
{
	return top == -1 ? true : false;
}

template<typename DataType>
void Stack<DataType>::clear()
{
	top = -1;
}

bool high(char a, char b);
double calculate(double a, char operate, double b);
template<typename DataType1, typename DataType2>
void judge(Stack<DataType1>& number, Stack<DataType2>& symbol, string str, int i);

enum Operate { add = '+', minus = '-', multiply = '*', divide = '/', factorial = '!', square = ';', power = '^' };

int main()
{
	string str;
	Stack<char> symbol;
	Stack<double> number;
	cout << "            " << "科学计算器 V1.1" << endl;
	cout << "========================================" << endl;
	cout <<
		"加:" << (char)Operate::add <<
		" 减:" << (char)Operate::minus <<
		" 乘:" << (char)Operate::multiply <<
		" 除:" << (char)Operate::divide <<
		" 阶乘:" << (char)Operate::factorial <<
		" 根号:" << (char)Operate::square <<
		" 次方:" << (char)Operate::power
		<< endl;
	cout << "              " << "支持括号()" << endl;
	cout << "========================================" << endl;
	while (true) {
		symbol.clear();number.clear();
		cout << "input string:";
		cin >> str;
		str = str + "=";
		try
		{
			judge(number, symbol, str, 0);
			cout << "result:" << number.pop() << endl;
		}
		catch (string& e) { cout << e << endl; }
	}

	return 0;
}

bool high(char a, char b) {
	if (b == ')')
		return true;
	if (b == '=')
		return true;
	if (a == Operate::square)
		return true;
	if (a == Operate::factorial)
		if (b != Operate::square)
			return true;
		else
			return false;
	if (a == Operate::power)
		if (b != Operate::square && b != Operate::factorial)
			return true;
		else
			return false;
	if (a == Operate::multiply || a == Operate::divide)
		if (b != Operate::factorial && b != Operate::square)
			return true;
		else
			return false;
	if (a == Operate::add || a == Operate::minus)
		if (b == Operate::add || b == Operate::minus)
			return true;
		else
			return false;
	return false;
}

double calculate(double a, char operate, double b) {
	if (operate == Operate::multiply)
		return a*b;
	if (operate == Operate::divide)
		if (b == 0)throw string("除数不能为0");
		else
			return a / b;
	if (operate == Operate::add)
		return a + b;
	if (operate == Operate::minus)
		return a - b;
	if (operate == Operate::square)
		if (a < 0)throw string("被开方数不能为负数");
		else
			return sqrt(a);
	if (operate == Operate::factorial) {
		if (a < 0 || a != (int)a)throw string("阶乘格式错误");
		double sum = 1;
		for (int i = a;i > 1;i--)
			sum *= i;
		return sum;
	}
	if (operate == Operate::power)
		return pow(a, b);
	return 0;
}

template<typename DataType1, typename DataType2>
void judge(Stack<DataType1>& number, Stack<DataType2>& symbol, string str, int i)
{
	static double temp = -1;
	static bool point = false;
	static int position = 0;
	//get numbers
	if (str[i] == '.' || str[i] >= '0' && str[i] <= '9') {
		if (temp == -1)temp = 0;
		if (str[i] == '.')
			point = true;
		else if (point) {
			double right = 1;
			position++;
			for (int i = 0;i < position;i++)
				right *= 1.0 / 10;
			temp += (str[i] - '0')*right;
		}
		else
			temp = temp * 10 + (str[i] - '0');
	}
	//get stmbols
	else {
		//处理负数
		if (str[i] == Operate::minus) {
			if (i == 0 || str[i - 1] == '(')
				number.push(0);
		}
		//数字入栈
		if (temp >= 0) {
			number.push(temp);
			temp = -1;position = 0;point = false;
		}
		//运算处理
		if (str[i] == '(')
			symbol.push(str[i]);
		else {
			while (!symbol.empty() && !number.empty() && symbol.get() != '(' && high(symbol.get(), str[i])) {
				try {
					if (symbol.get() == Operate::factorial)
						number.push(calculate(number.pop(), symbol.pop(), 0));
					else if (symbol.get() == Operate::square)
						number.push(calculate(number.pop(), symbol.pop(), 0));
					else
						number.push(calculate(number.pop(), symbol.pop(), number.pop()));
				}
				catch (string& e) { throw e; }
			}
			//符号入栈
			if (str[i] == ')')
				symbol.pop();
			else
				symbol.push(str[i]);
		}
	}
	if (i < str.length() - 1)
		judge(number, symbol, str, i + 1);
}
