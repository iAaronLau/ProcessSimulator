#include<iostream>
#include<thread>
#include<chrono>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "Structs.h"
#include <mutex>

using namespace std;

//互斥锁
mutex coutMutex1;
mutex coutMutex2;
mutex coutMutex3;
mutex coutMutex4;

//堆栈构造函数
Stack::Stack() {
	this->top = StackSize - 1;
}

//压栈
bool Stack::push(Task *n) {
	if (this->top >=0) {
		this->base[this->top] = n;
		this->top--;
		return true;
	}
	else {
		return false;
	}
}

//弹栈
Task* Stack::pop() {
	if (this->top<StackSize-1) {
		this->top++;
		return this->base[this->top];
	}
	else {
		return nullptr;
	}
}

//判断是否栈空
bool Stack::isVoid() {
	if (this->top < StackSize-1 ) {
		return false;
	}
	else {
		return true;
	}
}

//队列构造函数
Queue::Queue() {
	this->head = this->tail = nullptr;
	this->size = 0;
}

//入队
void Queue::Push(Task *n) {
	if (this->head==nullptr) {
		this->head = this->tail = n;
	}
	else{
		this->tail->next = n;
		n->pre = this->tail;
		this->tail = n;
	}
}

//出队优先级最高的一个Task
Task* Queue::PopUrgent(){
	Task * q, * p;
	q = this->head;
	p = this->head;
	for (;q->next!=this->tail &&q->next != nullptr;) {
		if (p->priority < q->next->priority) {
			p = q->next;
		}
		q = q->next;
	}
	if (p == this->head) {
		return this->Pop();
	}
	else if (p == this->tail) {
		Task * t = this->tail;
		this->tail = this->tail->pre;
		return t;
	}
	else {
		return this->Pop(p);
	}
}

//出队优先级最高的一个Task
Task* Queue::PopUrgent(Task * q) {
	Task * p,*t;
	if (q == nullptr) {
		return nullptr;
	}
	p=t = q;
	for (; p->next != this->tail&&q->next != nullptr;) {
		if (t->priority < p->next->priority) {
			t = p->next;
		}
		p= p->next;
	}
	if (t == q) {
		return nullptr;
	}
	else {
		return this->Pop(t);
	}
}

//出队最短进程
Task* Queue::PopShort(){
	Task * q, * p;
	q = this->head;
	p = this->head;
	for (; q->next != this->tail&&q->next != nullptr;) {
		if (p->request_t < q->next->request_t) {
			p = q->next;
		}
		q = q->next;
	}
	if (p==this->head) {
		return this->Pop();
	}
	else {
		return this->Pop(p);
	}
}

//中间出队
Task* Queue::Pop(Task * q) {
	if (q==this->head) {
		return this->Pop();
	}
	else if (q==this->tail) {
		Task * t = this->tail;
		this->tail = this->tail->pre;
		return t;
	}
	else {
		q->next->pre = q->pre;
		q->pre->next = q->next;
	}
	return q;
}

//出队
Task* Queue::Pop() {
	Task * q;
	for (; this->head==nullptr;);
	q = this->head;
	this->head = this->head->next;
	return q;
}

//判断是否队空
bool Queue::isVoid() {
	if (this->head != nullptr) {
		return false;
	}
	else {
		return true;
	}
}

//返回队尾
Task * Queue::getTail(){
	return this->tail;
}

//处理机构造函数
Cpu::Cpu(){
	this->q = Queue();//Task队列
	this->s = Stack();//Task栈
	this->record = Queue();//Task记录
	this->taskNum = TaskNum;
}

//先来先服务算法
int Cpu::FCFS(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "************************" << endl;
	cout<<"****先来先服务(FCFS)****"<<endl;
	cout << "************************" << endl;
	for (int i=0;i<TaskNum;i++) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.Pop();
		t->response_t = GetTickCount();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "执行-进程-" << t->index << endl;
		Sleep(t->request_t);
		t->finish_t = GetTickCount();
		t->wait_t=t->response_t - t->submit_t;
		this->record.Push(t);

		coutMutex2.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "完成-进程-" << t->index << endl;
		coutMutex2.unlock();

	}
	return 0;
}

//短进程优先算法
int Cpu::SJF(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "************************" << endl;
	cout << "****短作业优先(SJF)****" << endl;
	cout << "************************" << endl;
	for (int i = 0; i < TaskNum; i++) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.PopShort();
		t->response_t = GetTickCount();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "执行-进程-" << t->index << endl;
		Sleep(t->request_t);
		t->finish_t = GetTickCount();
		t->wait_t=t->response_t - t->submit_t;
		this->record.Push(t);

		coutMutex2.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "完成-进程-" << t->index << endl;
		coutMutex2.unlock();
	}
	return 0;
}

//优先级调度算法(非抢占式)
int Cpu::Priority_non_Preemptive(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "**********************************" << endl;
	cout << "****优先级调度算法(非抢占式)****" << endl;
	cout << "**********************************" << endl;
	for (int i = 0; i < TaskNum; i++) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.PopUrgent();
		t->response_t = GetTickCount();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "执行-进程-" << t->index << endl;
		Sleep(t->request_t);
		t->finish_t = GetTickCount();
		t->wait_t = t->response_t - t->submit_t;
		this->record.Push(t);

		coutMutex2.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "完成-进程-" << t->index << endl;
		coutMutex2.unlock();
	}
	return 0;
}

//优先级调度算法(抢占式)
int Cpu::Priority_Preemptive() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "**********************************" << endl;
	cout << "****优先级调度算法(抢占式)******" << endl;
	cout << "**********************************" << endl;
	for (; this->taskNum > 0;) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.PopUrgent();
		t->response_t = GetTickCount();
		this->Priority_Preemptive_Execult(t);
	}
	return 0;
}

//抢占式执行
int Cpu::Priority_Preemptive_Execult(Task * t){
	Task * x;
	bool flag = false;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "执行-进程-" << t->index << endl;
	for (int j = 0; j < (t->request_t - t->run_t); j++) {
		Sleep(1);
		x = this->q.PopUrgent(t);//尝试出队优先级更高的Task
		if (x!=nullptr) {
			t->run_t += j;
			flag = true;
			this->Interrupt(t, x);//进入中断
		}
	}
	if (!flag) {
		t->finish_t = GetTickCount();
		this->record.Push(t);
		this->taskNum--;

		coutMutex4.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "完成-进程-"<< t->index  << endl;
		coutMutex4.unlock();
	}
	return 0;
}

//中断处理
int Cpu::Interrupt(Task * tOld, Task * tNew) {

	coutMutex3.lock();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
	cout << "中断-进程-" << tOld->index << " --->" << tNew->index << endl;
	coutMutex3.unlock();

	tOld->interrupt_t = GetTickCount();
	this->s.push(tOld);//旧进程压栈
	tNew->response_t = GetTickCount();
	this->Priority_Preemptive_Execult(tNew);//新进程执行
	this->Restore();//中断恢复
	return 0;
}

//中断恢复
int Cpu::Restore() {
	Task * t = this->s.pop();//旧进程弹栈

	coutMutex2.lock();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_GREEN);
	cout << "恢复-进程-" << t->index << endl;
	coutMutex2.unlock();

	t->wait_t += (GetTickCount() - t->interrupt_t);
	this->Priority_Preemptive_Execult(t);//旧进程执行
	return 0;
}

//算法评价
int Cpu::Evaluate() {
	double avgWait_t = 0, avgTotal_t = 0, avgWeightTotal_t = 0, avgResponse_t = 0;
	double maxWait_t = 0, maxTotal_t = 0, maxWeightTotal_t = 0, maxResponse_t = 0;
	cout << endl <<"完成顺序:"<<endl;
	for (int i = 0; i < TaskNum; i++) {
		Task *t = this->record.Pop();
		t->total_t = t->finish_t - t->submit_t;
		t->response_t = t->response_t - t->submit_t;
		t->weight_total_t = t->total_t / t->request_t;
		avgTotal_t += t->total_t;
		avgWeightTotal_t += t->weight_total_t;
		avgWait_t += t->wait_t;
		avgResponse_t += t->response_t;
		if (t->wait_t > maxWait_t) {
			maxWait_t = t->wait_t;
		}
		if (t->total_t > maxTotal_t) {
			maxTotal_t = t->total_t;
		}
		if (t->weight_total_t > maxWeightTotal_t) {
			maxWeightTotal_t = t->weight_total_t;
		}
		if (t->response_t > maxResponse_t) {
			maxResponse_t = t->response_t;
		}
		cout<<t->index<<"-->";
	}
	avgTotal_t /= TaskNum;
	avgWeightTotal_t /= TaskNum;
	avgWait_t /= TaskNum;
	avgResponse_t /= TaskNum;
	cout << endl << "*************************************************" << endl;
	cout << "处理的进程总数 = " << TaskNum << endl;
	cout << "*************************************************" << endl;
	cout << "此算法 平均等待时间 = " << avgWait_t << "ms"
		<< " 平均周转时间 = " << avgTotal_t << "ms"
		<< " 平均带权周转时间 = " << avgWeightTotal_t << "ms"
		<< " 平均响应时间 = " << avgResponse_t << "ms"
		<< endl;
	cout << "*************************************************" << endl;
	cout << "单个进程的最大等待时间 = " << maxWait_t << "ms"
		<< " 最大周转时间 = " << maxTotal_t << "ms"
		<< " 最大带权周转时间 = " << maxWeightTotal_t << "ms"
		<< " 最大响应时间 = " << maxResponse_t << "ms"
		<< endl;
	cout << "*************************************************" << endl << endl << endl;
	return 0;
}

//获取该对象的队列成员的指针
Queue * Cpu::getQuene(){
	return &this->q;
}

//生成器构造函数
Generator::Generator(Queue *q, int taskNum){
	this->existingTasks = 0;
	this->q = q;
	this->taskNum = taskNum;
	this->genThread = nullptr;
}

//生成器线程执行的函数
void Generator::Generate(){
	for (int i=0;i< this->taskNum;i++) {
		Task *t;
		t = (Task *)malloc(sizeof(Task));
		t->index = i;//Task唯一识别序号
		t->submit_t = GetTickCount();//提交时间
		t->request_t = this->Rand(MinWait, MaxWait);//运行需时
		t->run_t = 0;//已运行时间
		t->start_t = 0;//开始时间
		t->response_t = 0;//响应时间
		t->wait_t = 0;//等待时间
		t->finish_t = 0;//完成时间
		t->total_t = 0;//周转时间
		t->weight_total_t = 0;//带权周转时间
		t->interrupt_t = 0;//进入中断时的时间
		t->priority = this->Rand(MinPirorty, MaxPirorty);//优先级
		t->pre = nullptr;//前一个Task的指针
		t->next = nullptr;//后一个Task的指针
		this->q->Push(t);//新的进程入队
		this->existingTasks ++;

		//coutMutex1.lock();
		//unique_lock<std::mutex> lock(coutMutex1);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//unique_lock<std::mutex> lock(coutMutex1);
		//cout<<"生成进程"<< i <<endl;
		//coutMutex1.unlock();
		this_thread::sleep_for(std::chrono::milliseconds(2));
		//定时休眠
		if (this->existingTasks%GenerateGap ==0) {
			this_thread::sleep_for(std::chrono::milliseconds(GenerateSleep));
		}
	}
}

void Generator::Start(){
	if (this->genThread==nullptr) {
		//启动生成器线程
		this->genThread = new thread(&Generator::Generate, this);
	}
}

//终止生成
void Generator::Stop(){
	this->genThread = nullptr;
}

//清空进程计数器
void Generator::Clear(){
	this->Stop();
	this->existingTasks = 0;
	this->q = nullptr;
}

//随机数生成器，范围a-b
int Generator::Rand(int a, int b){
	srand((int) time(0) + rand());
	return (rand() % (b - a)) + a;
}
