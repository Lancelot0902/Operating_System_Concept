#pragma once
#ifndef BANKER_H
#define BANKER_H

#include <mutex>
#include <vector>
#include <thread>
#include <iostream>
#include <condition_variable>

#define PROCESS_NUMS 5
#define SOURCE_NUMS 3

std::vector<int> avaliable;                //每种资源现有的实例数量
std::vector<std::vector<int>> max;         //每个进程对每种资源的最大需求
std::vector<std::vector<int>> allocation;  //每个进程现已分配的每种资源的数量
std::vector<std::vector<int>> need;        //每个进程还需每种资源的数量

void init() {
	/* init avaliable[] */
	avaliable.push_back(3);
	avaliable.push_back(3);
	avaliable.push_back(2);

	/* init max */
	max.resize(PROCESS_NUMS);
	for (int i = 0; i != PROCESS_NUMS; ++i)
		max[i].resize(SOURCE_NUMS);
	{
		max[0][0] = 7;
		max[0][1] = 5;
		max[0][2] = 3;
		max[1][0] = 3;
		max[1][1] = 2;
		max[1][2] = 2;
		max[2][0] = 9;
		max[2][1] = 0;
		max[2][2] = 2;
		max[3][0] = 2;
		max[3][1] = 2;
		max[3][2] = 2;
		max[4][0] = 4;
		max[4][1] = 3;
		max[4][2] = 3;
	}
	/* init allocation */
	allocation.resize(PROCESS_NUMS);
	for (int i = 0; i != PROCESS_NUMS; ++i)
		allocation[i].resize(SOURCE_NUMS);
	{
		allocation[0][0] = 0;
		allocation[0][1] = 1;
		allocation[0][2] = 0;
		allocation[1][0] = 2;
		allocation[1][1] = 0;
		allocation[1][2] = 0;
		allocation[2][0] = 3;
		allocation[2][1] = 0;
		allocation[2][2] = 2;
		allocation[3][0] = 2;
		allocation[3][1] = 1;
		allocation[3][2] = 1;
		allocation[4][0] = 0;
		allocation[4][1] = 0;
		allocation[4][2] = 2;
	}
	/* init need */
	need.resize(PROCESS_NUMS);
	for (int i = 0; i != PROCESS_NUMS; ++i)
		need[i].resize(SOURCE_NUMS);
	{
		need[0][0] = 7;
		need[0][1] = 4;
		need[0][2] = 3;
		need[1][0] = 1;
		need[1][1] = 2;
		need[1][2] = 2;
		need[2][0] = 6;
		need[2][1] = 0;
		need[2][2] = 0;
		need[3][0] = 0;
		need[3][1] = 1;
		need[3][2] = 1;
		need[4][0] = 4;
		need[4][1] = 3;
		need[4][2] = 1;
	}
}

/* 安全性算法 */
bool isSafe() {
	std::vector<int> work(avaliable);
	std::vector<bool> finish(PROCESS_NUMS, false);
	while (true) {
		bool found = false;
		for (int i = 0; i != PROCESS_NUMS; ++i) {
			if (finish[i] == false) {
				int num = 0;
				for (int j = 0; j != SOURCE_NUMS; ++j){
					if (need[i][j] <= work[j])
						num++;
				}		
				if (num == SOURCE_NUMS) {
					finish[i] = true;
					for (int j = 0; j != SOURCE_NUMS; ++j)
						work[j] += allocation[i][j];
					found = true;
				}
			}
		}
		if (found == false)
			break;
	}
	for (int i = 0; i != PROCESS_NUMS; ++i)
		if (finish[i] == false)
			return false;
	return true;
}

/* 资源请求 */
int request(int pid, std::vector<int>& request) {
	for (int i = 0; i != SOURCE_NUMS; ++i)
		if (request[i] > need[pid - 1][i])
			/* 报错 */
			return -1;
	for (int i = 0; i != SOURCE_NUMS; ++i)
		if (request[i] > avaliable[i])
			/* 等待 */
			return 0;
	std::vector<int> avaliable_cp(avaliable);
	std::vector<int> allocation_cp(PROCESS_NUMS);
	std::vector<int> need_cp(PROCESS_NUMS);
	for (int i = 0; i != SOURCE_NUMS; ++i) {
		/* 保存状态 */
		allocation_cp[i] = allocation[pid - 1][i];
		need_cp[i] = need[pid - 1][i];
		/* 模拟分配 */
		avaliable[i] -= request[i];
		allocation[pid - 1][i] += request[i];
		need[pid - 1][i] -= request[i];
	}
	/* 安全,完成分配 */
	if (isSafe())
		return 1;
	/* 不安全,恢复状态 */
	else {
		for (int i = 0; i != SOURCE_NUMS; ++i) {
			avaliable[i] = avaliable_cp[i];
			allocation[pid - 1][i] = allocation_cp[i];
			need[pid - 1][i] = need_cp[i];
		}
		return 0;
	}
}
#endif // !BANKER_H
