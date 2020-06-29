#include<iostream>
#include<cstring>
#include<cmath>
#include<ctime>
#include<cstdlib> 
#include<iomanip> 
using namespace std;

/*
	Date: 14-April-2020
	Description:标定一些常数
				const double PI 函数 Π数值 
				const double PRO_INHERITANCE 交叉概率 
				const double PRO_VARIATION 变异概率 
				const double PRO_INTERACTION 干扰概率 
				const int N 物种的数量 
*/
const double PI = M_PI;
const double PRO_INHERITANCE = 0.9;
const double PRO_VARIATION = 0.15;
const double PRO_INTERACTION = 0.5;
const int N = 25;

/* 
	Date: 14-April-2020
	Description:定义一些数组和可变的变量 
				double init[N] 初始化数值，即单个物种 
				double fitness[N] 存储适应度 
				int array[N][22] 存储二进制文件 
				double max_x 最大值数值 
				double max_fitness 最大适应度 
*/
double init[N] = {1.038745, 0.508504, 0.849797, -0.737197};//前四个为前期训练所标定物种 
double fitness[N];
int array[N][22];
double max_x = -2;
double max_fitness = -2;

/*
	Date: 14-April-2020
	Description:声明函数
				double Rapid_power() 快速幂算法 
				void Todecimals() 将二进制转换为小数 
				void Tobinary() 将小数转换为二进制 
				void Initialize() 初始化数组 
				void Getfitness() 计算适应度 
				void Selected() 选择种群 
				void Inheritance() 交叉继承 
				void Variation() 随机变异 
				void Interaction() 干扰编译 
*/
double Rapid_power(double base, int exp);
void Todecimals();
void Tobinary();
void Initialize();
void Getfitness();
void Selected();
void Inheritance();
void Variation();
void Interaction();

/*
	Name: main() 
	Date: 14-April-2020
	Description:主函数，用于调用其他函数进行代码运行
				首先初始化->将小数转化为二进制->获得适应度->循环物种更新->打印结果 
				循环100000次，当达到变异概率时发生变异 
*/
int main()
{
	Initialize();
	Tobinary();
	Getfitness();
	for(int i=0;i<100000;i++){
		Selected();
		if(((rand() % 1000) / 1000.0) < PRO_INHERITANCE){
			Inheritance();
		}
		if(((rand() % 1000) / 1000.0) < PRO_VARIATION){
			Variation();
		}
		if(((rand() % 1000) / 1000.0) < PRO_INTERACTION){
			Interaction();
		}
		Todecimals();
		Getfitness();
	}
	cout<<"最大点 = "<<max_x<<endl;
	cout<<"最大值 = "<<max_fitness<<endl;
	return 0;
}

/*
	Name: Rapid_power()
	Date: 14-April-2020
	Description:快速幂算法，功能是快速获得 x^n结果
	 			返回值是双精度小数，传入两个参数值，一个是底，一个是指数 
	 			sum是用于保存计算的结果 
				被 Todecimals()模块所调用 
*/
double Rapid_power(double base, int exp){
	double sum = 1;
	while(exp){
		if((exp & 1) == 1){
			sum = sum * base;
		}
		exp = exp >> 1;
		base = base * base;
	}
	return sum; 
} 

/*
	Name: Todecimals()
	Date: 14-April-2020
	Description:没有返回值，功能是将二进制转化为小数
				将每一个小数都进行转换，22位长度，第一位代表符号，第二位代表整数部分，剩下 20位为小数部分
				先将小数按照二进制与十进制进行转化，从前往后
				然后加上第一位代表的整数，最后获得第一位的符号，如果为负则将结果取反 
*/
void Todecimals(){
	for(int i=0;i<N;i++){
		double n = 0;
		for(int j=2;j<22;j++){
			n = n + array[i][j]*Rapid_power(0.5 ,1+j-2);
		}
		for(int j=1;j<2;j++){
			if(array[i][j] == 1){ 
				n = n + 1;
			}
		}
		for(int j=0;j<1;j++){
			if(array[i][j] == 1){
				n = -n;
			}
		}
		init[i] = n;
	}
}

/*
	Name: Tobinary()
	Date: 14-April-2020
	Description:没有返回值，将小数转换为二进制
				与转换为小小数相反，首先判断数值正负，正则第一位取 0否则取 1
				然后通过第二位判断整数部分，第三部分按照十进制与二进制转换法则进行转换
				不断除 2取余 
*/
void Tobinary(){
	cout<<setiosflags(ios::fixed)<<setprecision(6);
	for(int i=0;i<N;i++){
		for(int j=0;j<1;j++){
			if(init[i] >= 0){
				array[i][j] = 0;
			}else{
				array[i][j] = 1;
			}
		}
		int integer = abs(init[i]);
		for(int j=1;j<2;j++){
			array[i][j] = integer % 2;
		}
		double temo = abs(init[i]);
		for(int j=2;j<22;j++){
			temo = (temo - floor(temo)) * 2.0;
			array[i][j] = floor(temo);
		}
	}
}

/*
	Name: Initialize()
	Date: 14-April-2020
	Description:没有返回值，初始化数组 
				使用函数 memset将初始值均设为 0并且分配空间 
				初始化种群，即随机产生[-1，2]的小数 
*/
void Initialize(){
	memset(fitness,0,sizeof(fitness));
	memset(array,0,sizeof(array));
	for(int i=0;i<N;i++){
		double x = ((rand() % 100) / 100.0) * 3 - 1;
		init[i] = x;
	}
	cout<<"Total : "<<N<<endl; 
	cout<<"Please wait for the result !"<<endl; 
}

/*
	Name: Getfitness()
	Date: 14-April-2020
	Description:没有返回值，计算适应值 
				按照公式计算 f(x) = x * sin(10 * PI * x) + 2.0
				计算后判断是否达到历史最优，是则记录，不是则无操作 
*/
void Getfitness(){
	//f(x) = x * sin(10 * PI * x) + 2.0
	double n;
	for(int i=0;i<N;i++){
		n = init[i] * sin( 10 * PI * init[i] ) + 2.0;
		//n = init[i] * init[i];
		fitness[i] = n; 
	}
	for(int i=0;i<N;i++){
		if(fitness[i] > max_fitness){
			max_fitness = fitness[i];
			max_x = init[i];
		}
	}
} 

/*
	Name: Selected()
	Date: 14-April-2020
	Description:没有返回值，功能是选择下一代种群
				首先计算所有物种适应度总值，计算每个个体所占百分比
				获得一个累计的百分比，产生一个随机数，在哪个区间则改物种被选中
				存入事先准备的数组中，选择完毕后覆盖初始数组并且重新进行转化二进制和计算适应值工作 
*/
void Selected(){ 
	double possibility[N];
	double temporary_init[N];
	memset(possibility,0,sizeof(possibility));
	memset(temporary_init,0,sizeof(temporary_init));
	
	double all_pro = 0;
	for(int i=0;i<N;i++){
		all_pro = all_pro + fitness[i];
	}
	for(int i=0;i<N;i++){
		possibility[i] = fitness[i] / all_pro;
	}
	for(int i=1;i<N;i++){
		possibility[i] = possibility[i] + possibility[i-1];
	}
	for(int i=0;i<N;i++){
		double x = (rand() % 100) * 0.01;
		for(int j=0;j<N;j++){
			if(x < possibility[j]){
				temporary_init[i] = init[j];
				break;
			}
		}
	}
	for(int i=0;i<N;i++){
		init[i] = temporary_init[i];
	}
	Tobinary();
	Getfitness();
}

/*
	Name: Inheritance()
	Date: 14-April-2020
	Description:没有返回值，功能是继承交叉 
				首先随机产生一个节点 node作为交叉的点位
				随机生成一个序号作为物种，与其下一位物种进行交叉 
				交换节点以后的所有小数部分 
*/
void Inheritance(){
	int node = rand() % 22;
	int a1 = rand() % N;
	int a2 = (a1 + 1) % N;
	int temp;
	for(int i=node+1;i<22;i++){
		temp = array[a1][i]; 
		array[a1][i] = array[a2][i]; 
		array[a2][i] = temp;
	}
}

/*
	Name: Variation()
	Date: 14-April-2020
	Description:没有返回值，功能是随机变异 
				随机产生一个数，作为物种序号
				再产生一个随机数作为小数部分的 0，1基因
				选中后将该点的基因位进行反转，1->0或者 0->1 
*/
void Variation(){
	int a1 = (rand() % N);
	int a2 = (rand() % 20) + 2;
	array[a1][a2] == 1 ? array[a1][a2] = 0:array[a1][a2] = 1;
}

/*
	Name: Interaction()
	Date: 14-April-2020
	Description:没有返回值，功能是强制干扰 
				避免一个物种进入早熟
				随机选中一个物种，并且选择一个基因
				如果所有物种该基因都为 0或者 1则随机选择一个物种，将该基因变为不同值 
				如果有不同，则无操作 
*/
void Interaction(){
	int a1 = (rand() % N);
	int a2 = (rand() % 20) + 2;
	int s = array[0][a2];
	bool flag = 1;
	for(int i=1;i<N;i++){
		if(array[i][a2] != s){
			flag = 0;
			break;
		}
	}
	if(flag == 1){
		s == 1 ? s = 0:s = 1;
		array[a1][a2] = s;
	}
}
