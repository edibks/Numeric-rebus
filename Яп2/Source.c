#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define MAX 1000

// нахождение числа сопоставленного букве
int findN(char* str, char mchar[10], int mint[10])
{
	for (int j = 0; j <= 10; j++)
	{
		if (mchar[j] == str) return mint[j];
	}
	return -1;
}

// замена числа на строку 
void changeNtoL(char* strLetters, char* strNumbers, char mchar[10], int mint[10]) {
	int num;
	char c[2] = { '\0' };
	for (int i = 0; strLetters[i] != '\0'; i++)
	{
		num = findN(strLetters[i], mchar, mint);
		if (num != -1)
		{
			_itoa(num, c, 10);
			strNumbers[i] = c[0];
		}
	}
}

// замена строки на число
void changeLtoN (char* strLetters, char* strNumbers, char mchar[10], int mint[10]) {
	int num;
	char l[2] = { '\0' };
	num = findN(strLetters, mchar, mint);
	_itoa(num, l, 10);
	for (int i = 0; strNumbers[i] != '\0'; i++)
	if (strNumbers[i] == l[0]) strNumbers[i] = strLetters;
}

//Функция проверки найденного решения
int checking(char* str, bool* match, int* attempts)
{
	int numbers[7] = { 0 };
	int n = 0, i = 0, answer = 0;
	char digit[2];

	for (i; str[i] != '='; i++)
	{
		if (isalpha(str[i])) return 0;//проверка является ли введенное число буквой
		if (str[i] == '+') n++;
		if (isdigit(str[i]))//проверка является ли введенное число десятичной цифрой
		{
			digit[0] = str[i];
			numbers[n] = numbers[n] * 10 + atoi(digit);
		}
	}
	for (i; str[i] != '\0'; i++)
	{
		if (isalpha(str[i])) return 0;
		if (isdigit(str[i]))
		{
			digit[0] = str[i];
			answer = answer * 10 + atoi(digit);//число в строку
		}
	}
	int sum = 0;
	for (int i = 0; i < n + 1; i++)
		sum += numbers[i];
	if (sum == answer)
		*match = 1;
	(*attempts)++;
	return 1;
}

//рекурсивная функция, выполняющая полный перебор
char* recovery(char* str, char mchar[10], int mint[10], int* attempts)
{
	char strNumbers[MAX];
	char* result = NULL;
	bool isbegining = false; 
	bool match = false;
	strncpy(strNumbers, str, strlen(str) + 1);

	if (checking(strNumbers, &match, attempts) == 1)// Функция проверки найденного решения
	{
		if (match) return strNumbers;
		else return 0;
	}
	
	char letter;
	char c;
	for (int i = 0; i <= strlen(strNumbers); i++)
	{

		if (isalpha(strNumbers[i]) || strNumbers[i] == '\0')
		{
			c = strNumbers[i];
			if (i == 0 || strNumbers[i - 1] == ' ') isbegining = true;
			break;
		}
	}
	letter = c;

	for (; findN(letter, mchar, mint) != -1;)
	{
		for (int i = 0; i <= strlen(strNumbers); i++)
		{

			if (isalpha(strNumbers[i]) || strNumbers[i] == '\0')
			{
				c = strNumbers[i];
				if (i == 0 || strNumbers[i - 1] == ' ') isbegining = true;
				break;
			}
		}
		letter = c;
		if (letter == '\0') return 0;
	}
	
	int count = 0;
	if (isbegining)count = 1;
	for (count; count <= 10; count++)
	{
		if (mchar[count] == NULL)
		{
			mchar[count] = letter;
			mint[count] = count;
		}
		else continue;

		changeNtoL(str, strNumbers, mchar, mint);
		//printf("%s\n", strNumbers);
		result = (recovery(strNumbers, mchar, mint, attempts));
		if (!result)
		{
			changeLtoN(letter, strNumbers, mchar, mint);
			mchar[count] = NULL;
			mint[count] = NULL;
		}
	}
	return result;
}

int main()
{
	char str[MAX];
	int attempts = 0;
	char mchar[10] = {NULL};
	int mint[10] = {NULL};
	clock_t time1, time2;
	printf("%s\n", "enter the rebus:");
	gets(str);

	time1 = clock();
	printf("%s\n", recovery(str, mchar, mint, &attempts));
	time2 = clock();
	printf("number of attempts: %d\n", attempts);
	printf("time spent: %.10f seconds", ((float)(time2 - time1) / CLK_TCK));
}