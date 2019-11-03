#include <iostream>
#include <thread>
#include <Windows.h>
using namespace std;
HANDLE mutex;
int number = 0;
unsigned long __stdcall fun1(LPVOID lp) {
	while (number < 100) {
		WaitForSingleObject(mutex, INFINITE);
		cout << "thread 1 :" << number << endl;
		++number;
		Sleep(100);
		ReleaseMutex(mutex);
	}
}
unsigned long __stdcall fun2(LPVOID lp) {
	while (number < 100) {
		WaitForSingleObject(mutex, INFINITE);
		cout << "thread 2 :" << number << endl;
		++number;
		Sleep(100);
		ReleaseMutex(mutex);
	}
}
void main(int argc, char** argv) {
	mutex = CreateMutex(NULL, false, NULL);
	HANDLE p1 = CreateThread(NULL, 0, fun1, NULL, 0, NULL);
	HANDLE p2 = CreateThread(NULL, 0, fun2, NULL, 0, NULL);
	Sleep(10 * 1000);
	system("pause");
}