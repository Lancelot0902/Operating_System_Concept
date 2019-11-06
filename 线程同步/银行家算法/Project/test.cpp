#include "banker.h"

int main(int argc, char** argv) {
	init();
	std::vector<int> nums{ 1,0,2 };
	std::cout << request(2, nums) << std::endl;
	system("pause");
	return 0;
}