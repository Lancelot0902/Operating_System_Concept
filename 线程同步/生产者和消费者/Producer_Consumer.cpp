#include <iostream>           
#include <queue>
#include <thread>             
#include <mutex>
#include <Windows.h>
#include <condition_variable> 

#define MAX_SIZE 20; 

std::mutex Mutex;
std::condition_variable empty;
std::condition_variable full;

std::queue<int> Queue;

void _consumer() {
	do {
		std::unique_lock<std::mutex> lck(Mutex);
		empty.wait(lck, [] {return Queue.size() != 0; });

		Queue.pop();
		std::cout << "consumer consume one item: "
			<< "Queue size is " << Queue.size() << std::endl;
		Sleep(100);

		full.notify_one();
	} while (1);
}

void _producer() {
	do {
		std::unique_lock<std::mutex> lpk(Mutex);
		full.wait(lpk, [] {return Queue.size() != MAX_SIZE; });

		Queue.push(rand());
		std::cout << "produce produce one item"
			<< "Queue size is " << Queue.size() << std::endl;
		Sleep(100);

		empty.notify_one();
	} while (1);
}

int main(int argc, char** argv) {
	auto c = std::thread(_consumer);
	auto p = std::thread(_producer);
	p.join();
	c.join();
	return 0;
}