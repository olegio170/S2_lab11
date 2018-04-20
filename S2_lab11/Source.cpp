#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define white 0
#define grey 1
#define black 2

#define marked -2
#define processed -1

// �������� ����
int N; // ʳ������ ������
int **graphMatrix; // ������� ��������

// ������ � ������
typedef struct QueueNode {
	int data;
	QueueNode *next;
} QueueNode;

void queue_init(int data, QueueNode **queue_head);
QueueNode *queue_find_end(QueueNode *queue_head);
void queue_push(int data, QueueNode **queue_head);
void queue_free(QueueNode *queue_head);

// ��������� �� �������� �������
int **createSquareMatrix(int N);
void freeSquareMatrix(int **arr, int N);

// T��������� ����������
int *getWorkArray();
void reBildGraph(int *newNm);
void topologicSort(int *levels, int *newNames, int *oldNames);

// ������ � �������
void checkFilePtr(FILE *file);
FILE* openNewFile(char *fileName, char *type);
void inputName(char *str, char *input);
void fileInput();

// �������� ������ ������������ �����
unsigned int readUInt(char buff[]);

// �������� ����� �� ����������
int dfs(int v, int *colorArr);
void checkCycle();

// ����� �� �������� ���������� ����� �� ������ �����
typedef struct resultOnePoint {
	int wayLen;
	QueueNode *queueHead;
} resultOnePoint;

resultOnePoint findTheLongestWayToPoint(int fin, int *levels);
void printTheLongestWayToPoint(resultOnePoint *longestWay, int *oldNames);
void startSearchOnePoint(int fin);


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	N = readUInt("������ N - �-�� ������ �����:");
	graphMatrix = createSquareMatrix(N);
	
	fileInput(); // ������� ���� ����� � ����� 
	checkCycle(); // ��������� �� � ����� � �����

	int	finish = readUInt("\n������ ������ �������:") - 1;

	startSearchOnePoint(finish);

	freeSquareMatrix(graphMatrix, N);
	return 0;
}


// ��������� �� �������� �������
int **createSquareMatrix(int N)
{
	int** arr = (int **)calloc(N, sizeof(int *));
	for (int i = 0; i < N; ++i)
		arr[i] = (int*)calloc(N, sizeof(int));
	return arr;
}

void freeSquareMatrix(int **arr, int N)
{
	for (int i = 0; i < N; ++i)
		free(arr[i]);
	free(arr);
}


// ������ � ������
void queue_init(int data, QueueNode **queue_head)
{
	*queue_head = (QueueNode *)malloc(sizeof(QueueNode));
	(*queue_head)->data = data;
	(*queue_head)->next = NULL;
}

QueueNode *queue_find_end(QueueNode *queue_head)
{
	QueueNode *lastNode = queue_head;
	while (lastNode->next)
		lastNode = lastNode->next;

	return lastNode;
}

void queue_push(int data, QueueNode **queue_head)
{
	if (!*queue_head)
	{
		queue_init(data, queue_head);
		return;
	}

	QueueNode *lastNode = queue_find_end(*queue_head);

	QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
	lastNode->next = newNode;
	newNode->next = NULL;
	newNode->data = data;
}

void queue_free(QueueNode *queue_head)
{
	if (!queue_head)
		return;

	QueueNode* prev = NULL;
	while (queue_head->next) {
		prev = queue_head;
		queue_head = queue_head->next;
		free(prev);
	}
	free(queue_head);
}


// T��������� ����������
int *getWorkArray()
{
	int *workArray = (int *)calloc(N, sizeof(int));
	for (int i = 0; i < N; i++)
	{
		workArray[i] = 0;
		for (int j = 0; j < N; j++)
			workArray[i] += graphMatrix[j][i];
	}

	return workArray;
}

void reBildGraph(int *newNm)
{
	int **newMatrix = createSquareMatrix(N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graphMatrix[i][j])
				newMatrix[newNm[i]][newNm[j]] = 1;
			else
				newMatrix[newNm[i]][newNm[j]] = 0;

	freeSquareMatrix(graphMatrix, N);
	graphMatrix = newMatrix;
}

void topologicSort(int *levels, int *newNames, int *oldNames)
{
	int *workArray = getWorkArray();

	int completedCounter = 0;
	int currentLevel = 0;
	while (completedCounter < N)
	{
		// ��� �� ��������� ������ � �����, �� ������� � ������
		// ������ ���������, ������� �� ��� ���� ��������
		for (int i = 0; i < N; i++)
			if (workArray[i] == 0)
				workArray[i] = marked;

		for (int i = 0; i < N; i++)
		{
			if (workArray[i] == marked)
			{
				levels[completedCounter] = currentLevel;
				newNames[i] = completedCounter;
				oldNames[completedCounter] = i;

				for (int j = 0; j < N; j++)
					if (graphMatrix[i][j])
						workArray[j]--;

				workArray[i] = processed; // ������� ������, �� ������� �����������
				completedCounter++;
			}
		}
		currentLevel++;
	}

	reBildGraph(newNames);
}


// ������ � �������
void checkFilePtr(FILE *file)
{
	if (file == NULL) {
		printf("\n����� ����� �� ����!\n");
		system("pause");
		exit(1);
	}
}

FILE* openNewFile(char *fileName, char *type)
{
	FILE* file = fopen(fileName, type);

	checkFilePtr(file);

	return file;
}

void inputName(char *str, char *input)
{
	printf("\n����� ����� ����� %s:", str);
	rewind(stdin);
	gets_s(input, FILENAME_MAX);
}

void fileInput()
{
	FILE *fileInput;

	char fileNameInput[FILENAME_MAX];
	inputName("��� ��������", fileNameInput);
	fileInput = openNewFile(fileNameInput, "rt");

	int first, second;
	do
	{
		fscanf(fileInput, "%i%i", &first, &second);
		printf("\n����� %i ---> %i", first, second);

		if (first > N || second > N || first < 1 || second < 1)
		{
			printf("\n���������� ������� ����� �����!\n");
			system("pause");
			exit(1);
		}

		graphMatrix[first - 1][second - 1] = 1; // ������� � ��������� ������� ��������
	} while (!feof(fileInput));

	fclose(fileInput);
}


// �������� ������ ������������ �����
unsigned int readUInt(char buff[])
{
	unsigned int input;
	int isSuccess;
	while (1) {
		printf("%s", buff);
		isSuccess = scanf("%u", &input);

		if (!isSuccess) {
			printf("\n����������� ���!(���������� � ���� ���������� �����)\n");
			rewind(stdin);
		}
		else
			return input;
	}
}


// �������� ����� �� ����������
int dfs(int v, int *colorArr) {
	if (colorArr[v] == grey)return 1; // � ����
	if (colorArr[v] == black)return 0;

	colorArr[v] = grey;

	for (int i = 0; i < N; i++)
		if (graphMatrix[v][i])
			if (dfs(i, colorArr)) return 1; // � ���� � �������

	colorArr[v] = black;

	return 0;
}

void checkCycle()
{
	int *colorArr = (int *)calloc(N, sizeof(int));
	if (dfs(0, colorArr))
	{
		printf("\n� ����� �������� ���� !\n�������� ���� ����� �����������.\n");
		system("pause");
		exit(0);
	}
	free(colorArr);
}


// ����� �� �������� ���������� ����� �� ������ �����
resultOnePoint findTheLongestWayToPoint(int fin, int *levels)
{
	resultOnePoint result = { 0, NULL };

	if (!levels[fin])
	{
		result.wayLen = 1;
		queue_push(fin, &result.queueHead);
		return result;
	}

	for (int i = 0; i < fin; i++)
	{
		if (graphMatrix[i][fin])
		{
			resultOnePoint res = findTheLongestWayToPoint(i,  levels);

			if (res.wayLen > result.wayLen) {
				result = res;
				// ���� �� ���� ���� ������, �� ������ ����
				if (res.wayLen == levels[fin])
					break;
			}
			else
				queue_free(res.queueHead);
		}
	}

	result.wayLen += 1;
	queue_push(fin, &result.queueHead);
	return result;
}

void printTheLongestWayToPoint(resultOnePoint *longestWay, int *oldNames)
{
	longestWay->wayLen--;
	if (!longestWay->wayLen)
	{
		printf("\n����� �� ���� ����� �� ���� !\n");
		system("pause");
		return;
	}

	printf("\n��������� ���� ��������� %i ���", longestWay->wayLen);
	if (longestWay->wayLen == 1)
		printf("�");
	else if (longestWay->wayLen <= 4)
		printf("�");

	QueueNode *queuePtr = longestWay->queueHead;
	printf("\n����: %i ", oldNames[queuePtr->data] + 1);
	queuePtr = queuePtr->next;
	while (queuePtr)
	{
		printf("- %i ", oldNames[queuePtr->data] + 1);
		queuePtr = queuePtr->next;
	}
	queue_free(longestWay->queueHead);

	printf("\n");
	system("pause");
}

void startSearchOnePoint(int fin)
{
	int *levels = (int *)calloc(N, sizeof(int));
	int *newNames = (int *)calloc(N, sizeof(int));
	int *oldNames = (int *)calloc(N, sizeof(int));

	topologicSort(levels, newNames, oldNames);
	fin = newNames[fin];

	resultOnePoint way = findTheLongestWayToPoint(fin, levels);
	printTheLongestWayToPoint(&way, oldNames);

	free(levels);
	free(newNames);
	free(oldNames);
}

