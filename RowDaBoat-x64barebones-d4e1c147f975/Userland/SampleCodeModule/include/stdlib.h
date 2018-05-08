#ifndef STDLIB_H
#define STDLIB_H

	typedef struct processInfo {
	    int pid;
	    long long sizeAllocated;
	    char state;
	    int priority;
	}processInfo;

	typedef struct processesInfoTable {
	    int numberOfProcessesOnTable;
	    processInfo* list;
	}processesInfoTable;

	void printf(char*);
	int strlen(char*);
	int scanf(char *);
	char getChar();
	void putchar(char);
	void printInt(int a);
	int countDigits(int n);
	int parseNumber(char * s, double * d);
	int isNumeric(char c);
	void clear();
	int strcmp(const char* s1, const char* s2);
	int startsWith(const char* str, const char* start);
	void* malloc(int size);
	void free(void* ptr);
	void exit();
	int newProcess(void* ptr);
	int getPid();
	void ps();

	extern void clearScreen();
	extern void displayTimeData();
	extern int scan(char * str, int length);
	extern int print(char * str, int length);
	extern void* memoryAllocation(int size);
	extern void memoryFree(void* ptr);
	extern void exitProcess();
	extern int createProcess(void* ptr);
	extern int getProcessPid();
	extern void getProcessesInfo(processesInfoTable* processes);

#endif