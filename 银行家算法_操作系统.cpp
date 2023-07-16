#include<iostream>
#include<iterator>
#include<vector>
#include<deque>
//本函数假设初始情况下必存在安全序列!
using namespace std;
class Resource_Name {
private:
	vector<string>resource;
public:
	Resource_Name(vector<string>& resource) {
		this->resource = resource;
	}
	Resource_Name() {};
	void Show_Resource_Name() {
		vector<string>::iterator ed = resource.end();
		vector<string>::iterator st = resource.begin();
		cout << "资源名称:";
		while (st != ed) {
			cout << "      " << *st;
			st++;
		}
	}
	void Create_Resource_Index(vector<string>& resource, int Resource_Num) {
		string tem;
		for (int i = 0; i < Resource_Num; i++) {
			cin >> tem;
			resource.push_back(tem);
		}
	}
	~Resource_Name() {};
	friend  class Basic_Process_Message;
	friend  class Process_Table;

};
class Basic_Process_Message {
private:
	string Process_ND;
	vector<int>Max_Need;                  //各个进程的最大需求
	vector<int>Allocation;                 //各个进程的已分配资源
	vector<int>Need;                       //各个进程当前所需的资源数
	vector<int>Request;                    //进程发出请求
	Resource_Name rs;
public:
	Basic_Process_Message(vector<string>& resource) :rs(resource)
	{
		int tem;
		vector<string>::iterator it = rs.resource.begin();
		vector<string>::iterator ed = rs.resource.end();
		Request.resize(rs.resource.size());
		cin >> Process_ND;
		while (it != ed) {
			cin >> tem;
			if (tem < 0) {
				cout << "录入错误!" << endl;
				return;
			}
			Max_Need.push_back(tem);
			it++;

		}
		it = rs.resource.begin();
		while (it != ed) {
			cin >> tem;
			if (tem < 0) {
				cout << "录入错误!" << endl;
				return;
			}
			Allocation.push_back(tem);
			it++;
		}
		it = rs.resource.begin();
		int j;
		for (tem = 0; it != ed; tem++)
		{
			if ((j = Max_Need[tem] - Allocation[tem]) >= 0) {
				Need.push_back(j);
				it++;
			}
			else {
				cout << "输入错误!" << endl;
				return;
			}
		}
	}
	Basic_Process_Message() {};
	void ShowMaxNeed() {
		for (int i = 0; i < rs.resource.size(); i++)
			cout << "     " << Max_Need[i];
	}
	void ShowAlloCation() {
		for (int i = 0; i < rs.resource.size(); i++)
			cout << "     " << Allocation[i];
	}
	void ShowPresentNeed() {
		for (int i = 0; i < rs.resource.size(); i++)
			cout << "     " << Need[i];
	}
	bool SendRequest() {
		bool flag = true;
		int i;
		cout << endl << "进程" << Process_ND << "发起资源申请!请依次输入各个资源申请数量!" << endl << endl;
		cout << endl;
		for (i = 0; i < rs.resource.size(); i++) {
			cin >> Request[i];
			if (Request[i] > Need[i] || Request[i] < 0)
				flag = false;
		}
		if (flag == false) {
			cout << "申请资源有误,拒绝本次资源申请!" << endl;
		}
		else {
			cout << endl << "进程" << Process_ND << "资源申请情况如下:" << endl << endl;
			for (i = 0; i < rs.resource.size(); i++)
				cout << "申请资源" << rs.resource[i] << "的数量: " << Request[i] << endl << endl;
		}
		return flag;
	}
	~Basic_Process_Message() {};
	friend class Process_Table;
};
class Process_Table {
public:
	vector<int>Available;
	vector<bool>Finish;                                                               //判断假设经过一次资源分配,所有进程是否还存在一个安全序列
	vector<Basic_Process_Message>ProcessTable;
	vector<int>Work;
	int Safe_Flag;
public:
	Process_Table(vector<Basic_Process_Message>& bpm, int Process_Num) {              //此处完成进程表的初始化!
		int tem;
		int j;
		ProcessTable = bpm;                                                           //进程表创建完毕
		Safe_Flag = 1;
		for (tem = 0; tem < Process_Num; tem++) {
			Finish.push_back(false);                                                  //初始化Finish矩阵
		}
		bpm[0].rs.Show_Resource_Name();                                               //这里做的不是很好,每一个向量都可以展示资源,而且是属于难以改进的那种
		cout << endl << endl;
		for (tem = 0; tem < bpm[0].rs.resource.size(); tem++) {
			cin >> j;
			Available.push_back(j);
		}
		Work = Available;                                                             //Work数组的初始化
	}
	void Initial_Process_Index(vector<Basic_Process_Message>& bpm,                    //初始化进程数组表
		int Process_Num, vector<string>& resource, int Resource_Num) {
		Basic_Process_Message* Basic_pm;
		for (int i = 0; i < Process_Num; i++) {
			Basic_pm = new Basic_Process_Message(resource);
			bpm.push_back(*Basic_pm);
		}
	}
	Process_Table() {}
	void Show_Table_Data() {                                                          //仅展示进程表内部信息
		cout << "      进程名称       " << "Max_Need       " << "   Allocation     " << "     Need      " << endl << endl;
		for (int i = 0; i < ProcessTable.size(); i++) {
			cout << "      " << ProcessTable[i].Process_ND << "      ";
			ProcessTable[i].ShowMaxNeed();
			ProcessTable[i].ShowAlloCation();
			ProcessTable[i].ShowPresentNeed();
			cout << endl << endl;
		}
	}
	void Show_Resource_Can_Be_Used() {
		cout << endl << "      可用资源数:  ";
		for (int i = 0; i < Available.size(); i++)
			cout << Available[i] << "     ";
		cout << endl;
	}
	void ShowFinish_Data() {
		cout << endl << "      Finish状态:  ";
		for (int i = 0; i < Finish.size(); i++)
			cout << Finish[i] << "     ";
		cout << endl << endl;
	}
	void ShowWorkData() {
		cout << endl << "        Work状态:  ";
		for (int i = 0; i < Work.size(); i++)
			cout << Work[i] << "     ";
		cout << endl;
	}
	void ShowAllOfData() {                                                            //此函数实为信息展示函数                 
		this->Show_Table_Data();
		this->Show_Resource_Can_Be_Used();
		this->ShowWorkData();
		this->ShowFinish_Data();
	}
	void Safety_Detected(bool flag, string Process_ND) {                              //重点:安全检验函数
		if (flag == false)
			return;
		int i, Present_Process_ID, Start_Process;
		bool SrcC_tBR = false, flag0 = false;
		int  Count;
		Start_Process = 0;
		vector<int> tem_vec = Available;                                             //保存初始状态

		deque<string>path;

		for (i = 0; i < Finish.size(); i++) {
			if (ProcessTable[i].Process_ND == Process_ND)
			{
				Present_Process_ID = i;                                              //知道处理的是第几个进程
			}
		}
		vector<int>tem_Need = ProcessTable[Present_Process_ID].Need;                 //可能恢复need

		if (Finish[Present_Process_ID] == 1)                                         //进程已经完成,无法继续申请资源
		{
			cout << "该进程已完成,无法申请资源!" << endl;
			return;
		}
		for (i = 0; i < Available.size(); i++) {                                      //判断可用资源是否满足申请的资源数
			if (ProcessTable[Present_Process_ID].Request[i] > Available[i])
				flag0 = true;
		}

		if (flag0) {
			Safe_Flag = false;
			cout << "申请资源数超出可用资源!" << endl;
			return;

		}


		for (i = 0; i < Available.size(); i++) {                                     //可以分配,于此分配,并进行相关参数的改变
			Available[i] -= ProcessTable[Present_Process_ID].Request[i];
			Work[i] -= ProcessTable[Present_Process_ID].Request[i];
			ProcessTable[Present_Process_ID].Allocation[i] += ProcessTable[Present_Process_ID].Request[i];
			ProcessTable[Present_Process_ID].Need[i] -= ProcessTable[Present_Process_ID].Request[i];
			if (ProcessTable[Present_Process_ID].Allocation[i] < ProcessTable[Present_Process_ID].Max_Need[i])       //判断分配资源后,该进程能否释放所有资源
				SrcC_tBR = true;
		}
		cout << endl << "开始安全性检验!" << endl << endl;                           //准备进入安全序列函数!

		if (SrcC_tBR) {                                                              //该进程无法释放所有资源
			Count = 0;
			int Finished_Num = GetFinishedProcessNum();                              //方便后序进程申请资源的操作.可以实现 P1申请资源:P1->P2->P3 ,P2申请资源: P2->P3的类似操作
			SearchSafetyList_Condition_1(path, Count, Present_Process_ID, Finished_Num);                              //使用获取安全序列_1函数
		}
		else {                                                                       //该进程可以释放所有资源

			Count = 0;
			string stack_low = ProcessTable[Present_Process_ID].Process_ND;          //获取此时进程名称,要求输出从该进程开始的安全序列.如:P1释放资源,输出P1->P2->P3,P1->P3->P2
			path.push_back(ProcessTable[Present_Process_ID].Process_ND);
			for (i = 0; i < Work.size(); i++) {
				Work[i] += ProcessTable[Present_Process_ID].Allocation[i];
				Available[i] += ProcessTable[Present_Process_ID].Allocation[i];

			}
			Finish[Present_Process_ID] = true;
			int Finished_Num = GetFinishedProcessNum();
			SearchSafetyList_Condition_2(path, Count, Present_Process_ID, Finished_Num, stack_low);        //使用获取安全序列_2函数

		}
		if (Count == 0)                                                                 //没有安全序列
			Safe_Flag = 0;
		else
			Safe_Flag = 1;

		if (Safe_Flag == 0) {                                                        //分配资源后没有安全序列,恢复初始状态                   
			Available = tem_vec;
			Work = tem_vec;
			ProcessTable[Present_Process_ID].Need = tem_Need;
			for (i = 0; i < Available.size(); i++)
			{
				ProcessTable[Present_Process_ID].Allocation[i] = ProcessTable[Present_Process_ID].Max_Need[i] - tem_Need[i];
			}
		}
		for (i = 0; i < Work.size(); i++) {
			ProcessTable[Present_Process_ID].Request[i] = 0;                         //将该进程的Request清零,为下一次申请做准备
		}
		Bank();                                                                      //表明是否可以分配
		this->ShowAllOfData();                                                       //输出一下所有信息

	}
	bool Allow_Allocate(int Present_Process) {                                       //方便回溯求解,相当于约束函数,判断某一进程是否满足被分配资源
		for (int j = 0; j < Available.size(); j++)
			if (ProcessTable[Present_Process].Need[j] > Available[j] || Finish[Present_Process] == true)
				return false;

		return true;
	}
	int GetFinishedProcessNum() {                                                     //获取当前所有完成的进程数,方便完成P1->P2->P3->P4-
		int tem = 0;
		for (int i = 0; i < Finish.size(); i++) {
			if (Finish[i] == 1)
				tem += 1;
		}
		return tem;                                                                  //tem即为当前完成的进程数
	}
	void SearchSafetyList_Condition_1(deque<string>path, int & Count, int Present_Process, int Finished_Num) {    //情况1:当分配资源后,进程无法释放所有资源


		if (path.size() == this->Finish.size() - Finished_Num) {         //可以使得不输出Finish位已经是1的进程(此处不减一!)
			PrintWholePath(path);
			Count++;
			return;
		}
		int  i;
		for (Present_Process = 0; Present_Process < this->Finish.size(); Present_Process++)   //为进程j试探分配资源(回溯法输出所有安全路径)
		{
			if (Allow_Allocate(Present_Process)) {
				for (i = 0; i < Work.size(); i++) {
					Work[i] += ProcessTable[Present_Process].Allocation[i];
					Available[i] += ProcessTable[Present_Process].Allocation[i];
				}
				Finish[Present_Process] = true;
				path.push_back(ProcessTable[Present_Process].Process_ND);
				SearchSafetyList_Condition_1(path, Count, Present_Process + 1, Finished_Num);
				path.pop_back();
				Finish[Present_Process] = false;
				for (i = 0; i < Work.size(); i++) {
					Work[i] -= ProcessTable[Present_Process].Allocation[i];
					Available[i] -= ProcessTable[Present_Process].Allocation[i];
				}

			}
		}

	}
	void SearchSafetyList_Condition_2(deque<string>path, int & Count, int Present_Process, int Finished_Num, string stack_low) {      //情况2:分配资源后,当前进程可以释放所有资源


		if (path.size() == this->Finish.size() || path.size() == this->Finish.size() - Finished_Num + 1) {                //仅输出以当前进程为起点的安全路径!                                                                             //两次reversepath是为了判定栈底元素是否符合条件!(此处队列更好)
			
			if (path.front() == stack_low) {
				PrintWholePath(path);
				Count++;
			}
			return;
		}
		int  i, j;
		for (j = 0; j < this->Finish.size(); j++)   //为进程j试探分配资源
		{
			if (Allow_Allocate(j)) {
				for (i = 0; i < Work.size(); i++) {

					Work[i] += ProcessTable[j].Allocation[i];
					Available[i] += ProcessTable[j].Allocation[i];

				}

				Finish[j] = true;
				path.push_back(ProcessTable[j].Process_ND);
				SearchSafetyList_Condition_2(path, Count, j + 1, Finished_Num, stack_low);
				path.pop_back();
				Finish[j] = false;
				for (i = 0; i < Work.size(); i++) {

					Work[i] -= ProcessTable[j].Allocation[i];
					Available[i] -= ProcessTable[j].Allocation[i];


				}

			}
		}



	}
	void Bank() {                                                                                                 //银行家函数仅根据标志位进行进行判定

		if (Safe_Flag) {
			cout << "安全,可以分配资源" << endl << endl;
		}
		else
			cout << "不安全,拒绝分配资源!" << endl << endl;

	}
	void PrintWholePath(deque<string>path) {                                                                     //输出整个栈的元素(从栈底到栈顶)
		deque<string>tem;
		string* s;
		while (!path.empty()) {
			s = new string;
			*s = path.back();
			tem.push_back(*s);
			path.pop_back();
		}
		while (!tem.empty()) {
			s = new string;
			*s = tem.back();
			path.push_back(*s);
			tem.pop_back();
			if (!tem.empty())
				cout << *s << "->";
			else
				cout << *s << endl << endl;

		}

	}
	~Process_Table() {};
};
void main() {
	FILE* stream4;
	freopen_s(&stream4, "D:\\码农人码农魂\\操作系统\\Input_File4.txt", "r", stdin);

	int Resource_Num, Process_Num;
	cin >> Resource_Num >> Process_Num;
	Resource_Name resource_name;
	vector<string> resource;
	vector<Basic_Process_Message> bpm;
	Process_Table pt;
	//=============================================================================以上为需在main函数中定义的基本数据结构	
	resource_name.Create_Resource_Index(resource, Resource_Num);
	pt.Initial_Process_Index(bpm, Process_Num, resource, Resource_Num);         //初始化进程数组
	pt = Process_Table(bpm, Process_Num);                                       //至此完成初始化
	freopen_s(&stream4, "CON", "r", stdin); //定向输入到控制台
	pt.ShowAllOfData();                                                       //该函数即为当前资源状况函数 
	//=============================================================================
		//以下部分为测试部分
	
	int Request_Process, Exit_Flag;
	string Process_ND;
	Exit_Flag =0;
	
	while (!Exit_Flag) {
		cout << "请依次输入申请资源的进程编号,进程名和退出标志" << endl;       //在下一次循环开始后结束
		cin >> Request_Process >> Process_ND >> Exit_Flag;
		pt.Safety_Detected(pt.ProcessTable[Request_Process].SendRequest(), Process_ND);
		
	}
	fclose(stream4);
}