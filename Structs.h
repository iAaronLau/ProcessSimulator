#pragma once
#include<thread>
using namespace std;

const int TaskNum = 15;//生成的进程总数目
const int StackSize = TaskNum;//栈大小
const int GenerateGap = 300;//生成器休眠间隔
const int GenerateSleep = 40;//生成器休眠时常
const int MinWait = 1;//单个进程最小执行时间
const int MaxWait = 6;//单个进程最大执行时间
const int MinPirorty = 0;//单个进程最小优先级
const int MaxPirorty = 10;//单个进程最大优先级

//进程结构体
typedef struct Task {
	unsigned int index; //Task唯一识别序号
	unsigned long submit_t;//提交时间
	int request_t;//运行需时
	int run_t;//已运行时间
	int start_t;//开始时间
	int response_t;//响应时间
	int wait_t;//等待时间
	unsigned long finish_t;//完成时间
	int total_t;//周转时间
	double weight_total_t;//带权周转时间
	unsigned long interrupt_t;//上次进入中断时的时间
	int priority;//优先级
	Task * pre;//前一个Task的指针
	Task * next;//后一个Task的指针
};

//队列, 链表实现
class Queue {
	Task * head;//队首指针
	Task * tail;//队尾指针
	int size;//队列大小
public:
	Queue();//构造函数
	void Push(Task * t);//入队
	Task * Pop();//出队
	Task * Pop(Task * q);//出队
	Task * PopUrgent(Task * q);//优先级最高出队
	Task * PopUrgent();//优先级最高出队
	Task * PopShort();//作业最短出队
	bool isVoid();//判断是否队空
	Task * getTail();//获取队尾指针
};

//堆栈
class Stack {
private:
	int top;//栈顶指示器
public:
	Stack();//构造函数
	Task * base[StackSize];//堆栈储存体
	bool push(Task * t);//压栈
	Task * pop();//弹栈
	bool isVoid();//判断是否栈空
};

//进程生成器
class Generator {
private:
	int taskNum;//生成Task数量
	int existingTasks;//已生成的进程的数量
	Queue * q;//生成器持有的处理机Task队列对象的指针
	thread * genThread;//生成器线程对象
public:
	Generator(Queue * q, int taskNum);//构造函数
	void Generate();//生成进程
	void Start();//启动生成器
	void Stop();//终止生成
	void Clear();//清空进程计数器
	static int Rand(int a, int b);//随机数生成器，范围a-b
};

//处理机
class Cpu {
private:
	Queue q;//Task队列
	Stack s;//Task栈
	Queue record;//Task记录
	int taskNum;//剩余Task数量
public:
	Cpu();//构造函数
	int FCFS();//先来先服务
	int SJF();//短作业优先
	int Priority_Preemptive();//优先级调度-抢占式
	int Priority_non_Preemptive();//优先级调度-非抢占式
	int Interrupt(Task * tOld, Task * tNew);//CPU中断
	int Restore();//中断恢复
	int Evaluate();//评价当前算法
	int Priority_Preemptive_Execult(Task * t);//抢占式执行
	Queue * getQuene();//获取该对象的队列成员的指针
};