#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
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
void changeLtoN(char* strLetters, char* strNumbers, char mchar[10], int mint[10]) {
	int num;
	char l[2] = { '\0' };
	num = findN(strLetters, mchar, mint);
	_itoa(num, l, 10);
	for (int i = 0; strNumbers[i] != '\0'; i++)
		if (strNumbers[i] == l[0]) strNumbers[i] = strLetters;
}

//Функция проверки найденного решения
void checking(char* str, char words[][MAX], char* answer, int* still_Letters, bool* match, int* attempts, int* wordsLen, int answerLen)
{
	int sumNum= 0;
	int sumAnswer = 0, minDigits = 0;
	char c;
	char digit[2] = { '\0' };
	for (int i = 1; !*still_Letters && i <= answerLen; i++)
	{
		for (int j = 0; !*still_Letters && words[j][0]; j++)
		{

			c = words[j][wordsLen[j] - i];
			if (48 <= c && c <= 57)
			{
				digit[0] = c;

				sumNum += atoi(digit) * (int)pow(10, minDigits);
			}
			if ((65 <= c && c <= 90))
			{
				*still_Letters = 1;
			}
		}
		c = answer[answerLen - i];
		if (48 <= c && c <= 57)
		{
			digit[0] = c;

			sumAnswer += atoi(digit) * (int)pow(10, minDigits);
		}
		if ((65 <= c && c <= 90))
		{
			*still_Letters = 1;
		}
		if (*still_Letters == 0)
		{
			minDigits++;
		}
	}

	if (!minDigits)
		*match = true;
	else if (!*still_Letters)
	{
		if (sumNum == sumAnswer)
			*match = true;
	}
	else
	{
		int numDivider = (int)pow(10, minDigits);
		if (sumNum % numDivider == sumAnswer % numDivider)
			*match = true;
	}
	(*attempts)++;
}

// разбиение строки на слова
void partitioning(char* str, char words[][MAX], char* answer)
{
	int i = 0;
	int j = 0;
	int wordsCount = 0;
	for (i; str[i] != '='; i++)
	{
		if ((48 <= str[i] && str[i] <= 57) || (65 <= str[i] && str[i] <= 90))
		{
			words[wordsCount][j] = str[i];
			j++;
		}
		if (str[i] == '+')
		{
			wordsCount++;
			j = 0;
		}
	}
	j = 0;
	for (i; str[i] != '\0'; i++)
	{
		if ((48 <= str[i] && str[i] <= 57) || (65 <= str[i] && str[i] <= 90))
		{
			answer[j] = str[i];
			j++;
		}
	}
}

//рекурсивная функция, выполняющая полный перебор
char* recovery(char* str, char mchar[10], int mint[10], int* attempts, int* wordsLen, int answerLen)
{
	char strNumbers[MAX];
	char* result = NULL;
	bool isbegining = false;
	bool match = false;
	strncpy(strNumbers, str, strlen(str) + 1);
	int still_Letters = 0;
	char words[7][MAX] = { 0 };
	char answer[MAX] = { 0 };

	partitioning(strNumbers, words, answer);

	checking(strNumbers, words, answer, &still_Letters, &match, attempts, wordsLen, answerLen);
	if (match) { if (!still_Letters)return strNumbers; }
	else return 0;

	char letter;
	char c = 0;
	int found = 0;
	int strlenWords[7];
	for (int i = 0; i < 7 && words[i][0]; i++)
		strlenWords[i] = strlen(words[i]);
	int strlenAnswer = strlen(answer);

	for (int i = 1; !found; i++)
	{
		for (int j = 0; words[j][0] && !found; j++)
		{
			c = words[j][strlenWords[j] - i];
			if ((65 <= c && c <= 90))
			{
				letter = c;
				found = 1;
			}
		}
		c = answer[strlenAnswer - i];
		if ((65 <= c && c <= 90))
		{
			letter = c;
			found = 1;
		}
	}

	if (letter == '\0') return 0;
	if (found) {
		if (strNumbers[0] == letter)
			isbegining = true;
		else {
			int i = 1;
			while (strNumbers[i])
			{
				if (strNumbers[i] == letter && strNumbers[i - 1] == ' ')
					isbegining = true;
				i++;
			}
		}
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
		result = (recovery(strNumbers, mchar, mint, attempts, wordsLen, answerLen));
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
	static char answer[MAX] = { 0 };
	int answerLen;int wordsLen[7]; static char words[7][MAX] = { 0 };
	char str[MAX];
	int attempts = 0;
	char mchar[10] = { NULL };
	int mint[10] = { NULL };
	clock_t time1, time2;
	printf("%s\n", "enter the rebus:");
	gets(str);

	partitioning(str, words, answer);
	for (int i = 0; i < 7 && words[i][0]; i++)
	{
		wordsLen[i] = strlen(words[i]);
	}
	answerLen = strlen(answer);

	time1 = clock();
	printf("%s\n", recovery(str, mchar, mint, &attempts, wordsLen, answerLen));
	time2 = clock();
	printf("number of attempts: %d\n", attempts);
	printf("time spent: %.10f seconds", ((float)(time2 - time1) / CLK_TCK));
}