//进程调度程序 
//先来先服务调度算法
//短作业优先调度算法
//优先级调度算法（非抢占式 抢占式）
//算法评价
//输出 调度结果 算法评价指标

#include<iostream>
#include "Structs.h"

using namespace std;

//主函数
int main() {
	//4个处理机对象，4个生成器对象，一一对应，互不干涉
	Cpu * cpu1 = new Cpu();
	Cpu * cpu2 = new Cpu();
	Cpu * cpu3 = new Cpu();
	Cpu * cpu4 = new Cpu();
	Generator * gen1 = new Generator(cpu1->getQuene(), TaskNum);
	Generator * gen2 = new Generator(cpu2->getQuene(), TaskNum);
	Generator * gen3 = new Generator(cpu3->getQuene(), TaskNum);
	Generator * gen4 = new Generator(cpu4->getQuene(), TaskNum);

	//执行完一个算法之后再执行下一个
	gen1->Start();//启动生成器
	cpu1->FCFS();//先来先服务
	cpu1->Evaluate();//评价当前算法
	delete gen1;
	delete cpu1;

	gen2->Start();//启动生成器
	cpu2->SJF();//短作业优先
	cpu2->Evaluate();//评价当前算法
	delete gen2;
	delete cpu2;

	gen3->Start();//启动生成器
	cpu3->Priority_non_Preemptive();//优先级调度-非抢占式
	cpu3->Evaluate();//评价当前算法
	delete gen3;
	delete cpu3;

	gen4->Start();//启动生成器
	cpu4->Priority_Preemptive();//优先级调度-抢占式
	cpu4->Evaluate();//评价当前算法
	delete gen4;
	delete cpu4;

	//暂停程序
	cin.get();
	return 0;
}