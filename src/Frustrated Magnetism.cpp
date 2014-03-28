//============================================================================
// Name        : Frustrated.cpp
// Author      : BSheng  &  YHH  & LH
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include<iostream>
#include <time.h>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
#include <sstream>
#include <omp.h>

#include<windows.h>

using namespace std;

class BuyImformation;
class person;

int compare(BuyImformation a, BuyImformation b);
void BubbleSort(BuyImformation * pData, int Count);
int compare(BuyImformation a, BuyImformation b);
double* score(int ** arry1, int *len_arry1_ev, int len_arry1, int ** arry2, int *len_arry2_ev, int len_arry2);
void findconnect(person * arry_person_input,int len_arry_person_input,int ** & arry_person_connect,int * & len_a_p_c,int *&buy_num);

class BuyImformation {
public:
	int brand_id;
	int type; //点击：0；购买：1；收藏：2；购物车：3
	int visit_datetime_month;
	int visit_datetime_day;

	void initialize(int brand_id_input, int type_input,
			int visit_datetime_month_input, int visit_datetime_day_input) {
		brand_id = brand_id_input;
		type = type_input;
		visit_datetime_month = visit_datetime_month_input;
		visit_datetime_day = visit_datetime_day_input;
	}

	BuyImformation & operator=(BuyImformation a) {

		this->brand_id = a.brand_id;
		this->type = a.type;
		this->visit_datetime_day = a.visit_datetime_day;
		this->visit_datetime_month = a.visit_datetime_month;


		return *this;
	}
	BuyImformation(void){
		this->brand_id =-1;
		this->type = -1;
		this->visit_datetime_day = -1;
		this->visit_datetime_month = -1;
	}

	BuyImformation(const BuyImformation & a){
		this->brand_id = a.brand_id;
		this->type = a.type;
		this->visit_datetime_day = a.visit_datetime_day;
		this->visit_datetime_month = a.visit_datetime_month;
	}
};

class person {
private:
	int p_id;

public:
	BuyImformation * p_buyimformation;
	int leng_s_p_buyim;
	int leng_p_buyim;

	person() {
		p_id = 0;
		leng_s_p_buyim = 0;
		leng_p_buyim = 0;
		p_buyimformation = NULL;
	}



	int get_person_id() {
		return p_id;
	}
	void change_person_id(int p_id_input) {
		p_id = p_id_input;
	}

	//分配地址
	void allocate_p_buy(int leng) {
		if (p_buyimformation != NULL) {
			delete[] p_buyimformation;
			p_buyimformation = NULL;
		}
		p_buyimformation = new BuyImformation[leng];
		leng_p_buyim = leng;
		leng_s_p_buyim = 0;
	}

	void push_p_buy(int brand_id_input, int type_input,
			int visit_datetime_month_input, int visit_datetime_day_input) {
		if (leng_s_p_buyim == leng_p_buyim && leng_s_p_buyim > 0) {
			BuyImformation * p_buyimformation_local =
					new BuyImformation[leng_p_buyim];
			for (int i = 0; i < leng_p_buyim; i++) {
				p_buyimformation_local[i] = p_buyimformation[i];
			}
			delete[] p_buyimformation;
			p_buyimformation = NULL;
			p_buyimformation = new BuyImformation[2 * leng_p_buyim];
			for (int i = 0; i < leng_p_buyim; i++) {
				p_buyimformation[i] = p_buyimformation_local[i];
			}
			leng_p_buyim *= 2;
		}

		p_buyimformation[leng_s_p_buyim].initialize(brand_id_input, type_input,
				visit_datetime_month_input, visit_datetime_day_input);
		leng_s_p_buyim++;

	}

	//优化BuyImformation数组使其储存长度等于申请长度，以节省内存
	void optimize_p_buy();

	//输入品牌在BuyImformation数组中的地址位置（从0开始）将其删除
	void delete_one_p_buy(int i_brand){
		//i_brand为数组位置
		for (int i = i_brand; i < leng_s_p_buyim - 1; i++) {
			this->p_buyimformation[i] = this->p_buyimformation[i + 1];
		}
		leng_s_p_buyim--;


		this->optimize_p_buy();



	}

	//删除从d_month,d_day日期开始之后的数据，该函数主要目的利用本地数据进行预测时，会使用
	void delete_by_day(int d_month, int d_day);



	person & operator=(person a) {
		p_id = a.p_id;
		leng_p_buyim = a.leng_p_buyim;
		leng_s_p_buyim = a.leng_s_p_buyim;

		if (a.leng_p_buyim >= 0) {
			delete[] this->p_buyimformation;
			this->p_buyimformation = NULL;
			this->p_buyimformation = new BuyImformation[a.leng_p_buyim];
			for (int i = 0; i < leng_s_p_buyim; i++) {
				this->p_buyimformation[i] = a.p_buyimformation[i];
			}
		}else{
			cout<<"operat erro"<<endl;
			exit(0);
		}
		return *this;
	}

	person(const person &a) {
		p_id = a.p_id;
		leng_p_buyim = a.leng_p_buyim;
		leng_s_p_buyim = a.leng_s_p_buyim;

		if (a.leng_p_buyim >= 0) {
			this->p_buyimformation = NULL;
			this->p_buyimformation = new BuyImformation[a.leng_p_buyim];
			for (int i = 0; i < leng_s_p_buyim; i++) {
				this->p_buyimformation[i] = a.p_buyimformation[i];
			}

		}

		else{
			cout<<a.leng_p_buyim<<endl;
			cout<<"operat erro"<<endl;
			exit(0);
		}

	}

	~person() {
		delete[] p_buyimformation;
		p_buyimformation = NULL;
	}

};

void person::optimize_p_buy() {
	if (leng_s_p_buyim != leng_p_buyim) {
		BuyImformation * p_buyimformation_local =
				new BuyImformation[leng_s_p_buyim];
		for (int i = 0; i < leng_s_p_buyim; i++) {
			p_buyimformation_local[i] = this->p_buyimformation[i];
		}
		delete[] this->p_buyimformation;
		this->p_buyimformation = NULL;
		this->p_buyimformation = new BuyImformation[leng_s_p_buyim];
		for (int i = 0; i < leng_s_p_buyim; i++) {
			this->p_buyimformation[i] = p_buyimformation_local[i];
		}
		leng_p_buyim = leng_s_p_buyim;
		delete[] p_buyimformation_local;
		p_buyimformation_local = NULL;
	}

}

void person::delete_by_day(int d_month, int d_day){


	BuyImformation d_b;
	d_b.visit_datetime_month = d_month;
	d_b.visit_datetime_day = d_day;


	for(int i=0;i<leng_s_p_buyim;i++){

		if(compare(p_buyimformation[i],d_b)>=0){
			delete_one_p_buy(i);
			i--;
		}
	}
}



//将b_month,b_day开始的购买的商品id返回出来
void chose_kind_from_day(int b_month, int b_day,int e_month,int e_day,person * arry_in,int len_arry_in,
		int ** &arry_buy_list,int &len_arry_buy_list,int *&len_arry_buy_list_ev,int kind){

	if(kind !=1 && kind != 2 && kind !=3 && kind !=0){
		cout<<"kind erro"<<endl;
		return ;
	}

	len_arry_buy_list = 0;
	int *len_a_b_l_re = new int [len_arry_in];//储存arry_buy_list实际每一行的声明长度
	for(int i=0;i<len_arry_in;i++){
		len_a_b_l_re[i] = 100;
	}

	arry_buy_list = new int * [len_arry_in];
	len_arry_buy_list_ev = new int [len_arry_in];
	for(int i=0;i<len_arry_in;i++){
		arry_buy_list[i] = new int [len_a_b_l_re[i]];
		len_arry_buy_list_ev[i] = 0;
	}

	for(int i=0;i<len_arry_in;i++){
		for(int j=0;j<arry_in[i].leng_p_buyim;j++){
			BuyImformation com_local_b;
			com_local_b.visit_datetime_month = b_month;
			com_local_b.visit_datetime_day = b_day;
			BuyImformation com_local_e;
						com_local_e.visit_datetime_month = e_month;
						com_local_e.visit_datetime_day = e_day;
			//如果日期大于选择开始日期，并且type==1则加入购买名单
			if(compare(com_local_b,arry_in[i].p_buyimformation[j]) <0 && compare(com_local_e,arry_in[i].p_buyimformation[j]) >0 && arry_in[i].p_buyimformation[j].type == kind){
				//如果len_arry_buy_list_ev[len_arry_buy_list]为0,说明还没有储存user——id，先储存user_id
				if(len_arry_buy_list_ev[len_arry_buy_list] == 0){
					arry_buy_list[len_arry_buy_list][0] = arry_in[i].get_person_id();
					arry_buy_list[len_arry_buy_list][1] = arry_in[i].p_buyimformation[j].brand_id;
					len_arry_buy_list_ev[len_arry_buy_list] = 2;
				}else{
					//检查该id之前是否添加过{----------------------------------------------------------------------
					int k=0;
					for(;k<len_arry_buy_list_ev[len_arry_buy_list];k++){
						if(arry_buy_list[len_arry_buy_list][k] == arry_in[i].p_buyimformation[j].brand_id){
							break;
						}
					}
					//end}==================================================================================
					if(k == len_arry_buy_list_ev[len_arry_buy_list]){
						//如果相等说明没有被储存
					arry_buy_list[len_arry_buy_list][len_arry_buy_list_ev[len_arry_buy_list]] = arry_in[i].p_buyimformation[j].brand_id;
					len_arry_buy_list_ev[len_arry_buy_list] ++;

					//判断是否需要扩容{-----------------------------------------------------------------------
					if(len_arry_buy_list_ev[len_arry_buy_list] == len_a_b_l_re[len_arry_buy_list]){
						int * cp_buy = new int [len_a_b_l_re[len_arry_buy_list]];
						for(int i_cp=0;i_cp<len_a_b_l_re[len_arry_buy_list];i_cp++){
							cp_buy[i_cp] = arry_buy_list[len_arry_buy_list][i_cp];
						}
						delete [] arry_buy_list[len_arry_buy_list];
						arry_buy_list[len_arry_buy_list] = NULL;
						arry_buy_list[len_arry_buy_list] = new int [len_a_b_l_re[len_arry_buy_list]*2];
						for(int i_cp=0;i_cp<len_a_b_l_re[len_arry_buy_list];i_cp++){
							arry_buy_list[len_arry_buy_list][i_cp] = cp_buy[i_cp];
						}
						len_a_b_l_re[len_arry_buy_list] *= 2;
						delete [] cp_buy;
					}
					//扩容end}==============================================================================
				}
				}
			}
		}

		if(len_arry_buy_list_ev[len_arry_buy_list] != 0){
			len_arry_buy_list++;
		}
	}

	delete [] len_a_b_l_re;
}

//检查arry中是否有重复的商品，有的话剔除
void check_arry_re(int ** &arry_buy_list,int &len_arry_buy_list,int *&len_arry_buy_list_ev){
	for(int i=0;i<len_arry_buy_list;i++){
		int i_ev = 2;
		for(;i_ev<len_arry_buy_list_ev[i];i_ev++){
			int j_ev = 1;
			for(;j_ev<i_ev;j_ev++){
				if(abs(arry_buy_list[i][i_ev]) == abs(arry_buy_list[i][j_ev])){
					if(arry_buy_list[i][i_ev] > 0){
						arry_buy_list[i][i_ev] = 0;
					}else if(arry_buy_list[i][j_ev] > 0){
						arry_buy_list[i][j_ev] = 0;
					}else
						arry_buy_list[i][i_ev] = 0;
				}

			}
		}


		for(i_ev = 0;i_ev<len_arry_buy_list_ev[i];i_ev++){
			if(arry_buy_list[i][i_ev] == 0 || arry_buy_list[i][i_ev]<0){
				for(int j=i_ev;j<len_arry_buy_list_ev[i]-1;j++){
					arry_buy_list[i][j] = arry_buy_list[i][j+1];
				}
				i_ev--;
				len_arry_buy_list_ev[i]--;
			}

		}
	}
}

int main() {
	void read_txt(person* &arry_person, int & leng_s_arry_person);
	void solution1(person * arry_person_input, int leng_s_arry_person_input);
	void solution2(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
			int f_m_e=9,int f_d_e=15);
	void solution3(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
			int f_m_e=9,int f_d_e=15);
	void solution4(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
			int f_m_e=9,int f_d_e=15);
	void solution5(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
				int f_m_e=9,int f_d_e=15);
	srand((unsigned) time(NULL));
	person * arry_person = NULL;
	int leng_s_arry_person = 0;

	//cout<<leng_s_arry_person<<endl;

	read_txt(arry_person, leng_s_arry_person);


	//solution4(arry_person, leng_s_arry_person,7,16,8,15);
	solution5(arry_person, leng_s_arry_person,7,16,8,15);

	/*
	for(int i=0;i<len_arry_cnnct;i++){
		for(int j=0;j<len_arry_cnnct_ev[i];j++){
			cout<<arry_cnnct[i][j]<<"\t";
		}
		cout<<endl;
	}
	*/
	return 0;
}

double randoms(double b = 0.0, double e = 1.0) {

	return (e - b) * double(rand()) / double(RAND_MAX) + b;
}

void read_txt(person* &arry_person_inpute, int & leng_s_arry_person) {

	FILE* fp;

	if ((fp = fopen("t_alibaba_data.csv", "r")) < 0) {
		printf("open the file is error!\n");
		exit(0);
	}

	int i_line = 0;
	int p_id_pre = -1;
	int leng_arry_person = 1000; //数组长度

	leng_s_arry_person = 0; //已储存个数
	//cout<<"==================="<<endl;
	person * arry_person = new person[leng_arry_person];

	while (!feof(fp)) {

		int p_id_pre_local, brand_id_local, type_local,
				visit_datetime_day_local, visit_datetime_month_local;

		fscanf(fp, "%d,%d,%d,%d月%d日\n", &p_id_pre_local, &brand_id_local,
				&type_local, &visit_datetime_month_local,
				&visit_datetime_day_local);

		i_line++;

		if (p_id_pre == p_id_pre_local) {
			//继续当前用户

			arry_person[leng_s_arry_person - 1].push_p_buy(brand_id_local,
					type_local, visit_datetime_month_local,
					visit_datetime_day_local);

		} else {
			//开始新用户

			//判断是否需要扩容{-----------------------------------------------------
			if (leng_s_arry_person == leng_arry_person) {
				//数组长度不足需要扩容
				//建立临时数组进行复制
				person * arry_person_local = new person[leng_arry_person];
				for (int i = 0; i < leng_arry_person; i++) {
					arry_person_local[i] = arry_person[i];
				}

				//将长度扩为原先2倍

				delete[] arry_person;
				arry_person = NULL;
				arry_person = new person[leng_arry_person * 2];

				for (int i = 0; i < leng_arry_person; i++) {
					arry_person[i] = arry_person_local[i];
				}
				leng_arry_person *= 2;

				delete[] arry_person_local;
				arry_person_local = NULL;
			}
			//判断扩容end}=========================================================

			//新用户初始化{---------------------------------------------------------

			p_id_pre = p_id_pre_local;
			arry_person[leng_s_arry_person].change_person_id(p_id_pre);

			arry_person[leng_s_arry_person].allocate_p_buy(100);

			arry_person[leng_s_arry_person].p_buyimformation[0].initialize(
					brand_id_local, type_local, visit_datetime_month_local,
					visit_datetime_day_local);

			leng_s_arry_person++;

			//初始化结束}===========================================================

			//将上一个用户内存进行优化{-------------------------------------------------
			//将p_buyimformation中没有使用的多余的数组删掉，使得leng=leng_s

			if (leng_s_arry_person > 1) {

				//如果用户在某一天day1购买了商品a则在day1当天查看a的信息删除

				person & p_buyim_local = arry_person[leng_s_arry_person
						- 2];
				int i_brand_id = 0;

				do {
					int buyday(0), buymonth(0), buybrand_id(0);

					for (;
							i_brand_id
									< arry_person[leng_s_arry_person - 2].leng_s_p_buyim;
							i_brand_id++) {
						if (p_buyim_local.p_buyimformation[i_brand_id].type == 1) {
							buybrand_id = p_buyim_local.p_buyimformation[i_brand_id].brand_id;
							buymonth =
									p_buyim_local.p_buyimformation[i_brand_id].visit_datetime_month;
							buyday =
									p_buyim_local.p_buyimformation[i_brand_id].visit_datetime_day;
							break;
						}
					}



					if (i_brand_id
							>= arry_person[leng_s_arry_person - 2].leng_s_p_buyim)
						break;

					for (int i = 0;
							i
									< arry_person[leng_s_arry_person - 2].leng_s_p_buyim;
							i++) {
						if (p_buyim_local.p_buyimformation[i].brand_id == buybrand_id
								&& p_buyim_local.p_buyimformation[i].visit_datetime_month
										== buymonth
								&& p_buyim_local.p_buyimformation[i].visit_datetime_day == buyday
								&& (p_buyim_local.p_buyimformation[i].type == 0 || p_buyim_local.p_buyimformation[i].type == 3 )  ) {
							//满足同天购买 查看加购物车条件，将该条删除

							arry_person[leng_s_arry_person - 2].delete_one_p_buy(
									i);



							if (i < i_brand_id) {
								i_brand_id--;

							}

							i--;

						} else if (p_buyim_local.p_buyimformation[i].brand_id == buybrand_id
								&& p_buyim_local.p_buyimformation[i].visit_datetime_month
										== buymonth
								&& p_buyim_local.p_buyimformation[i].visit_datetime_day == buyday
								&& p_buyim_local.p_buyimformation[i].type == 1
								&& i != i_brand_id) {
							//一天中多次购买一样商品也会删除掉

							arry_person[leng_s_arry_person - 2].delete_one_p_buy(
									i);


							if (i < i_brand_id) {
								i_brand_id--;

							}
							i--;

						}
					}

					i_brand_id++;

				} while (1);

				//arry_person[leng_s_arry_person - 2].optimize_p_buy();

			}
			//优化}===============================================================

		}
		//cout<<arry_person[0].p_buyimformation[0].brand_id<<endl;
	}

	//读取完毕
	//cout<<"读取完毕"<<endl;
	//cout<<arry_person[0].p_buyimformation[0].brand_id<<endl;

	if (arry_person_inpute == NULL) {
		arry_person_inpute = new person[leng_s_arry_person];
	} else {
		delete[] arry_person_inpute;
		arry_person_inpute = NULL;
		arry_person_inpute = new person[leng_s_arry_person];
	}

	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person_inpute[i] = arry_person[i];
	}
}

//返回两个日期之间的天数间隔
int day_gap(int month1, int day1, int month2, int day2) {

	int m_b = month1;
	int m_s = month2;
	int d_b = day1;
	int d_s = day2;
	int sign = 1;

	if (month2 > month1) {
		m_b = month2;
		d_b = day2;
		m_s = month1;
		d_s = day1;
		sign = -1;
	} else if (month1 == month2) {
		return (day1 - day2);
	}

	return sign * ((m_b - m_s) * 30 - d_b + d_s);

}

//算法1
/*
 * by YHH
 * 采用完全随机的思想
 *
 * 购买者在购买物品a后的20天内查看不做贡献，预测按照该顾客以往查看商品种类以1/2的概率来进行之后是否购买
 * 挑选预测购买者名单：以随机的概率进行随机挑选
 */

void solution1(person * arry_person_input, int leng_s_arry_person_input) {
	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}

	//购买物品a后的20天内查看记录删除{----------------------------------------------------------------------

	for (int i_s = 0; i_s < leng_s_arry_person; i_s++) {
		int i_brand_id = 0;
		person &p_buyim_local = arry_person[i_s];
		do {
			int buyday(0), buymonth(0), buybrand_id(0);
			for (; i_brand_id < arry_person[i_s].leng_s_p_buyim; i_brand_id++) {
				if (p_buyim_local.p_buyimformation[i_brand_id].type == 1) {
					buybrand_id = p_buyim_local.p_buyimformation[i_brand_id].brand_id;
					buymonth = p_buyim_local.p_buyimformation[i_brand_id].visit_datetime_month;
					buyday = p_buyim_local.p_buyimformation[i_brand_id].visit_datetime_day;
					break;
				}
			}
			if (i_brand_id == arry_person[i_s].leng_s_p_buyim)
				break;
			for (int i = 0; i < arry_person[i_s].leng_s_p_buyim; i++) {
				int d_gap = day_gap(p_buyim_local.p_buyimformation[i].visit_datetime_month,
						p_buyim_local.p_buyimformation[i].visit_datetime_day, buymonth, buyday);
				if (!(d_gap > 0 && d_gap < 20))
					continue;

				if (p_buyim_local.p_buyimformation[i].brand_id == buybrand_id
						&& p_buyim_local.p_buyimformation[i].type == 0) {
					//满足20天内查看

					arry_person[i_s].delete_one_p_buy(i);


					if (i < i_brand_id) {
						i_brand_id--;
					}
					i--;
				}
			}
			i_brand_id++;
		} while (1);
	}
	//20天end}===========================================================================================

	FILE *fp = fopen("solution1.txt", "w");

	double P_person = randoms();	//预测者名单概率
	for (int i_s = 0; i_s < leng_s_arry_person; i_s++) {
		if (randoms() > P_person)
			continue;

		double P_buy = randoms();
		int leng_buylist = 100;
		int leng_s_buylist = 0;
		int *Buylist = new int[leng_buylist];

		person & p_s_local = arry_person[i_s];
		for (int i_pbi = 0; i_pbi < arry_person[i_s].leng_s_p_buyim; i_pbi++) {
			//P_buy = randoms();
			if (randoms() > P_buy && p_s_local.p_buyimformation[i_pbi].type != 1) {
				continue;
			}

			if (leng_s_buylist == leng_buylist) {
				//扩容
				int *Buylist_local = new int[leng_buylist];
				for (int i = 0; i < leng_buylist; i++) {
					Buylist_local[i] = Buylist[i];
				}
				delete[] Buylist;
				Buylist = NULL;
				Buylist = new int[leng_buylist * 2];
				for (int i = 0; i < leng_buylist; i++) {
					Buylist[i] = Buylist_local[i];
				}
				leng_buylist *= 2;
				delete[] Buylist_local;
				Buylist_local = NULL;
			}
			Buylist[leng_s_buylist] = p_s_local.p_buyimformation[i_pbi].brand_id;
			leng_s_buylist++;
		}

		//检查删除buylist中重复的项
		for (int i = 0; i < leng_s_buylist; i++) {
			for (int j = 0; j < leng_s_buylist; j++) {
				if (i == j)
					continue;
				if (Buylist[i] == Buylist[j]) {
					for (int k = j; k < leng_s_buylist - 1; k++) {
						Buylist[k] = Buylist[k + 1];
					}

					if (j < i)
						i--;
					j--;
					leng_s_buylist--;
				}
			}
		}
		//输出
		fprintf(fp, "%d\t", arry_person[i_s].get_person_id());
		for (int i = 0; i < leng_s_buylist - 1; i++) {
			fprintf(fp, "%d,", Buylist[i]);
		}
		fprintf(fp, "%d\n", Buylist[leng_s_buylist - 1]);
	}

}
//算法2
/*

 * 采用加权平均
 * 提出两个概念：
 * 1.购买考察期：购买者在购买物品的前期查看天数，例如：购买者P在买物品a之前提前b天开始查看
 * 2.购买（概）率：购买者对于有规律的购买物品种类数/查看物品种类数
 */

void solution2(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
		int f_m_e=9,int f_d_e=15) {

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b,f_d_b);
	}


	//我们认为一个人在买完一个商品连续的时间内查看商品的信息是对购买信息没有帮助的，应当删除
	/*测试出用户的购买习惯:
	 * 1.购买考察期：一个用户在购买前，一般会提前多少天开始购买
	 * 2.考察频率，强度：用户在考察一个商品时，大致多长时间会浏览一次商品，以及用户在考察时单天考察次数
	 *
	 * 具体刻画方法：
	 * 1.存在考察周期d
	 * 2.考察强度平均值：考察总次数/考察周期
	 * 3.考察强度方差：为考察强度平均值的方差，反应单天考察强度
	 * 4.考察平均值：考察时间距购买时间的平均值
	 */

	/*
	 * 2014/3/18
	 * 现改变想法，仅用两个值来刻画
	 * 购买周期和购买周期内查看次数，并且利用这两个值的方差来描述正态分布，计算购买概率
	 */

	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l=new int [leng_s_arry_person];
	int *leng_a_b_l = new int [leng_s_arry_person];


	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = 200;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);
		//{----------------------------------------------------------------------------------------
		//看一个人买一个东西后又买一次的可能性

		for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {

				double T = 0;
				BuyImformation buy[100];
				int leng_s_buy = 0;
				for (int i = i_brand_b; i < i_brand_e; i++) {
					if (p.p_buyimformation[i].type == 1) {
						if (leng_s_buy == 0) {
							buy[leng_s_buy] = p.p_buyimformation[i];
							leng_s_buy++;
						} else {
							/*
							if (abs(
									day_gap(
											buy[leng_s_buy - 1].visit_datetime_month,
											buy[leng_s_buy - 1].visit_datetime_day,
											p.p_buyimformation[i].visit_datetime_month,
											p.p_buyimformation[i].visit_datetime_day)) <= 2)
								continue;
							else {
								*/
								buy[leng_s_buy] = p.p_buyimformation[i];
								leng_s_buy++;
								T += day_gap(
										buy[leng_s_buy - 1].visit_datetime_month,
										buy[leng_s_buy - 1].visit_datetime_day,
										p.p_buyimformation[i].visit_datetime_month,
										p.p_buyimformation[i].visit_datetime_day);
							//}
						}
					}
				}

				if (leng_s_buy > 1) {

						//在预测区间，添加到购买列表中
						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								buy[0].brand_id;
						leng_s_a_b_l[i_a_p]++;
						//删除掉该品牌的全部信息
						for (int i = i_brand_b; i < i_brand_e; i++) {
							arry_person[i_a_p].delete_one_p_buy(i_brand_b);
						}
						i_brand_e = i_brand_b;


				}

				i_brand_b = i_brand_e;
				i_brand_e--;

			}

		}

		//第二判断阶段，如果一个人


		//购买重复模块end}=================================================================================


	}


	const int num_kind_arry_pbhbt = 5;
	double *arry_p_b_hbt[num_kind_arry_pbhbt];
	for(int i=0;i<num_kind_arry_pbhbt;i++){
		arry_p_b_hbt[i] = new double [leng_s_arry_person];
	}
	//[0]位储存购买天数平均值，[1]位储存天数标准差，[2]位储存购买个数，[3]查看次数平均值，4为其标准差，
	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {


		for (int i = 0; i < num_kind_arry_pbhbt; i++) {
			arry_p_b_hbt[i][i_a_p] = 0;

		}

		person & p_bimf_l = arry_person[i_a_p];

		for (int i_b = 0; i_b < arry_person[i_a_p].leng_s_p_buyim; i_b++) {
			if (p_bimf_l.p_buyimformation[i_b].type == 1) {
				BuyImformation b_local;
				b_local = p_bimf_l.p_buyimformation[i_b];
				//检查之前是否购买过同个商品

				int i_b_b = i_b - 1;	//运行后刚好是一个brand_id的起始位置
				int i_b_b_l = 0;
				for (;
						(p_bimf_l.p_buyimformation[i_b_b].brand_id == b_local.brand_id
								&& i_b_b >= 0); i_b_b--) {
					if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

						i_b_b_l = i_b_b;
					}
				}
				i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

				//开始记录平均值
				int day_gap_value = day_gap(p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
						p_bimf_l.p_buyimformation[i_b].visit_datetime_day,
						p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_month,
						p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_day);
				if (day_gap_value < 0)
					exit(0);
				if (day_gap_value == 0)
					continue;
				//cout<<day_gap_value<<endl;

				arry_p_b_hbt[0][i_a_p] += double(day_gap_value);
				arry_p_b_hbt[1][i_a_p] += double(day_gap_value * day_gap_value);
				arry_p_b_hbt[2][i_a_p] += 1.0;

				double num_check_rel = (double(i_b)-double(i_b_b_l))/double(day_gap_value);
				/*
				double num_check_rel = 0.;
				for(int i=i_b_b_l+1;i<i_b;i++){
					if(compare(p_bimf_l.p_buyimformation[i],p_bimf_l.p_buyimformation[i-1])==0)
						continue;
					else
						num_check_rel++;
				}
				num_check_rel/=double(day_gap_value);
				*/
				arry_p_b_hbt[3][i_a_p] += num_check_rel;
				arry_p_b_hbt[4][i_a_p] += num_check_rel * num_check_rel;

			}
		}

		if (arry_p_b_hbt[2][i_a_p] != 0) {
			if (arry_p_b_hbt[2][i_a_p] != 1) {
				arry_p_b_hbt[1][i_a_p] = sqrt(
						arry_p_b_hbt[1][i_a_p] / arry_p_b_hbt[2][i_a_p]
								- arry_p_b_hbt[0][i_a_p]
										* arry_p_b_hbt[0][i_a_p]
										/ (arry_p_b_hbt[2][i_a_p]
												* arry_p_b_hbt[2][i_a_p]));
				arry_p_b_hbt[4][i_a_p] = sqrt(
						arry_p_b_hbt[4][i_a_p] / arry_p_b_hbt[2][i_a_p]
								- arry_p_b_hbt[3][i_a_p]
										* arry_p_b_hbt[3][i_a_p]
										/ (arry_p_b_hbt[2][i_a_p]
												* arry_p_b_hbt[2][i_a_p]));
			} else {
				arry_p_b_hbt[1][i_a_p] = 0;
				arry_p_b_hbt[4][i_a_p] = 0;
			}

			arry_p_b_hbt[0][i_a_p] /= arry_p_b_hbt[2][i_a_p];
			arry_p_b_hbt[3][i_a_p] /= arry_p_b_hbt[2][i_a_p];

		}
	}

	ofstream fout("solution2.txt");
	//预测将会购买的物品
	//f(x)=1/(sqrt(2*Pi)*sigma)*exp(-1*(x-miu)^2/(2*sigma^2))
	//商品是从8月15号开始的预测
	//x轴的零点是开始查看的日期

	double total = 0;
	double acc = 0;
	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		person & p = arry_person[i_a_p];

		int i_brand_e = 0;
		int i_brand_b = 0;
		BuyImformation buy_local;
		bool ifbuy = false;
		for (; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {


			//我们认为在4个月的时间内，买过一个物品后段时间之内是不会再买该物品的，将买过的物品买后一个月到第一次查看的信息清除
			if(p.p_buyimformation[i_brand_e].type == 1 && p.p_buyimformation[i_brand_b].brand_id == p.p_buyimformation[i_brand_e].brand_id){
				ifbuy = true;
				buy_local = p.p_buyimformation[i_brand_e];
			}

			if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {

				if(ifbuy){
					//删除该物品购买后一个月内及购买前的全部信息
					int i=0;
					int gap_local = i_brand_e-i_brand_b;
					for(;i<gap_local;i++){
						if(day_gap(p.p_buyimformation[i+i_brand_b].visit_datetime_month,p.p_buyimformation[i+i_brand_b].visit_datetime_day,
								buy_local.visit_datetime_month,buy_local.visit_datetime_day) < 30){
							arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							p.p_buyimformation = arry_person[i_a_p].p_buyimformation;
						}else{
							i++;
							break;
						}
					}
					i_brand_e -= i;

					ifbuy = false;
				}


				double num_check=double(i_brand_e-i_brand_b);
				for(int i=i_brand_b+1;i<i_brand_e;i++){
					//将品牌最早查看日期找到，并且将查看次数统计出来
					//由于按时间排序，第一个就是开始日期
					if(compare(p.p_buyimformation[i],p.p_buyimformation[i-1]) == 0){
						num_check--;
					}
				}
				//计算购买概率{---------------------------------------------------------------

				double d_x = double(day_gap(f_m_b,f_d_b,p.p_buyimformation[i_brand_b].visit_datetime_month,p.p_buyimformation[i_brand_b].visit_datetime_day));
				const double c = 0.159155;
				if(arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[4][i_a_p] > 0.0000001){
					total ++;
					//cout<<arry_p_b_hbt[i_a_p][1]<<"\t"<<arry_p_b_hbt[i_a_p][4]<<endl;



				double P = exp(-0.5*((d_x-arry_p_b_hbt[0][i_a_p])*(d_x-arry_p_b_hbt[0][i_a_p])
						/(arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[1][i_a_p])+(num_check-arry_p_b_hbt[3][i_a_p])*(num_check-arry_p_b_hbt[3][i_a_p])
						/(arry_p_b_hbt[4][i_a_p]*arry_p_b_hbt[4][i_a_p])));

				P /= (arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[4][i_a_p]);
				P *= c;



				if(false && randoms()<P){
					acc++;
					//扩充
					if(leng_s_a_b_l[i_a_p] == leng_a_b_l[i_a_p]){
						int * arry_b_l_copy = new int [leng_a_b_l[i_a_p]];
						for(int i=0;i<leng_a_b_l[i_a_p];i++){
							arry_b_l_copy[i] = arry_buy_list[i_a_p][i];
						}
						delete [] arry_buy_list[i_a_p];
						arry_buy_list[i_a_p] = NULL;
						arry_buy_list[i_a_p] = new int [2*leng_a_b_l[i_a_p]];
						for(int i=0;i<leng_a_b_l[i_a_p];i++){
							arry_buy_list[i_a_p][i] = arry_b_l_copy[i];
						}
						leng_a_b_l[i_a_p] *= 2;
						delete [] arry_b_l_copy;
					}

					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = p.p_buyimformation[i_brand_b].brand_id;
					//cout<<p[i_brand_b].brand_id<<endl;
					leng_s_a_b_l[i_a_p]++;

				}else{
					//cout<<i_a_p<<"\t"<<P<<endl;
				}
				}else{
					//cout<<i_a_p<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"!"<<endl;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

		}
		if(leng_s_a_b_l[i_a_p]>0){
		fout<<arry_person[i_a_p].get_person_id()<<"\t";
		for(int i=0;i<leng_s_a_b_l[i_a_p];i++){
			if(i<leng_s_a_b_l[i_a_p]-1){
			fout<<arry_buy_list[i_a_p][i]<<",";
			}else{
				fout<<arry_buy_list[i_a_p][i]<<endl;
			}

		}
		}


	}
	cout<<"接受率"<<acc/total<<endl;

	if(!(f_m_b == 8 && f_d_b == 16)){
		//启动本地预判分系统
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 =0;
		//现判断一下len_arry1的大小
		for(int i=0;i<leng_s_arry_person;i++){
			if(leng_s_a_b_l[i]>0){
				len_arry1++;
			}
		}
		arry1 = new int *[len_arry1];
		len_arry1_ev = new int [len_arry1];
		int i_arry_buy_list = 0;
		for(int i=0;i<len_arry1;i++){
			len_arry1_ev[i] = 0;
			for(;i_arry_buy_list<leng_s_arry_person;i_arry_buy_list++){
				if(leng_s_a_b_l[i_arry_buy_list] > 0){
					len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list]+1;
					arry1[i] = new int [leng_s_a_b_l[i_arry_buy_list]+1];
					arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

					for(int j=0;j<leng_s_a_b_l[i_arry_buy_list];j++){
						arry1[i][j+1] = arry_buy_list[i_arry_buy_list][j];
					}
					i_arry_buy_list++;
					break;
				}
			}
		}


		int ** arry2;
		int * len_arry2_ev = NULL;
		int len_arry2 =0;

		chose_kind_from_day(f_m_b,f_d_b,f_m_e,f_d_e,arry_person_input,leng_s_arry_person_input,arry2,len_arry2,len_arry2_ev,1);


		double * value_score = NULL;
		value_score = score(arry1,len_arry1_ev,len_arry1,arry2,len_arry2_ev,len_arry2);
		cout<<value_score[0]*100.0<<"%\t"<<value_score[1]*100.0<<"%\t"<<value_score[2]*100.0<<"%"<<endl;


	}
}


void solution3(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
		int f_m_e=9,int f_d_e=15) {

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b,f_d_b);
	}


	//我们认为一个人在买完一个商品连续的时间内查看商品的信息是对购买信息没有帮助的，应当删除
	/*测试出用户的购买习惯:
	 * 1.购买考察期：一个用户在购买前，一般会提前多少天开始购买
	 * 2.考察频率，强度：用户在考察一个商品时，大致多长时间会浏览一次商品，以及用户在考察时单天考察次数
	 *
	 * 具体刻画方法：
	 * 1.存在考察周期d
	 * 2.考察强度平均值：考察总次数/考察周期
	 * 3.考察强度方差：为考察强度平均值的方差，反应单天考察强度
	 * 4.考察平均值：考察时间距购买时间的平均值
	 */

	/*
	 * 2014/3/18
	 * 现改变想法，仅用两个值来刻画
	 * 购买周期和购买周期内查看次数，并且利用这两个值的方差来描述正态分布，计算购买概率
	 */

	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l=new int [leng_s_arry_person];
	int *leng_a_b_l = new int [leng_s_arry_person];


	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = 200;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);
		//{----------------------------------------------------------------------------------------
		//看一个人买一个东西后又买一次的可能性

		int i_brand_e = 0;
		int i_brand_b = 0;
		for (; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {

				BuyImformation buy[20];
				int leng_s_buy = 0;
				for (int i = i_brand_b; i < i_brand_e; i++) {
					if (p.p_buyimformation[i].type == 1) {
						if (leng_s_buy == 0) {
							buy[leng_s_buy] = p.p_buyimformation[i];
							leng_s_buy++;
						} else {
							if (abs(
									day_gap(
											buy[leng_s_buy - 1].visit_datetime_month,
											buy[leng_s_buy - 1].visit_datetime_day,
											p.p_buyimformation[i].visit_datetime_month,
											p.p_buyimformation[i].visit_datetime_day)) <= 2)
								continue;
							else {
								buy[leng_s_buy] = p.p_buyimformation[i];
								leng_s_buy++;
							}
						}
					}
				}

				if (leng_s_buy > 1) {
					//粗暴的认为超过两次就有购买周期
					int T = double(
							day_gap(buy[leng_s_buy - 1].visit_datetime_month,
									buy[leng_s_buy - 1].visit_datetime_day,
									buy[0].visit_datetime_month,
									buy[0].visit_datetime_day))
							/ double(leng_s_buy);
					//看一下下一次购买是否在预测区间内
					if (day_gap(f_m_b, f_d_b, buy[leng_s_buy - 1].visit_datetime_month,
							buy[leng_s_buy - 1].visit_datetime_day) <= T
							&& day_gap(f_m_e, f_d_e,
									buy[leng_s_buy - 1].visit_datetime_month,
									buy[leng_s_buy - 1].visit_datetime_day)
									> T) {
						//在预测区间，添加到购买列表中
						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								buy[0].brand_id;
						leng_s_a_b_l[i_a_p]++;
						//删除掉该品牌的全部信息
						for (int i = i_brand_b; i < i_brand_e; i++) {
							arry_person[i_a_p].delete_one_p_buy(i_brand_b);
						}
						i_brand_e = i_brand_b;
					}

				}

				i_brand_b = i_brand_e;
				i_brand_e--;

			}

		}

	}
     //购买重复模块end}=================================================================================

	const int num_kind_arry_pbhbt = 5;
	double *arry_p_b_hbt[num_kind_arry_pbhbt];
	for(int i=0;i<num_kind_arry_pbhbt;i++){
		arry_p_b_hbt[i] = new double [leng_s_arry_person];
	}
	//[0]位储存购买天数平均值，[1]位储存天数标准差，[2]位储存购买个数，[3]查看次数平均值，4为其标准差，
	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {


		for (int i = 0; i < num_kind_arry_pbhbt; i++) {
			arry_p_b_hbt[i][i_a_p] = 0;

		}

		person & p_bimf_l = arry_person[i_a_p];

		for (int i_b = 0; i_b < arry_person[i_a_p].leng_s_p_buyim; i_b++) {
			if (p_bimf_l.p_buyimformation[i_b].type == 1) {
				BuyImformation b_local;
				b_local = p_bimf_l.p_buyimformation[i_b];
				//检查之前是否购买过同个商品

				int i_b_b = i_b - 1;	//运行后刚好是一个brand_id的起始位置
				int i_b_b_l = 0;
				for (;
						(p_bimf_l.p_buyimformation[i_b_b].brand_id == b_local.brand_id
								&& i_b_b >= 0); i_b_b--) {
					if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

						i_b_b_l = i_b_b;
					}
				}
				i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

				//开始记录平均值
				int day_gap_value = day_gap(p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
						p_bimf_l.p_buyimformation[i_b].visit_datetime_day,
						p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_month,
						p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_day);
				if (day_gap_value < 0)
					exit(0);
				if (day_gap_value == 0)
					continue;
				//cout<<day_gap_value<<endl;

				arry_p_b_hbt[0][i_a_p] += double(day_gap_value);
				arry_p_b_hbt[1][i_a_p] += double(day_gap_value * day_gap_value);
				arry_p_b_hbt[2][i_a_p] += 1.0;

				double num_check_rel = (double(i_b)-double(i_b_b_l))/double(day_gap_value);
				/*
				double num_check_rel = 0.;
				for(int i=i_b_b_l+1;i<i_b;i++){
					if(compare(p_bimf_l.p_buyimformation[i],p_bimf_l.p_buyimformation[i-1])==0)
						continue;
					else
						num_check_rel++;
				}
				num_check_rel/=double(day_gap_value);
				*/
				arry_p_b_hbt[3][i_a_p] += num_check_rel;
				arry_p_b_hbt[4][i_a_p] += num_check_rel * num_check_rel;

			}
		}

		if (arry_p_b_hbt[2][i_a_p] != 0) {
			if (arry_p_b_hbt[2][i_a_p] != 1) {
				arry_p_b_hbt[1][i_a_p] = sqrt(
						arry_p_b_hbt[1][i_a_p] / arry_p_b_hbt[2][i_a_p]
								- arry_p_b_hbt[0][i_a_p]
										* arry_p_b_hbt[0][i_a_p]
										/ (arry_p_b_hbt[2][i_a_p]
												* arry_p_b_hbt[2][i_a_p]));
				arry_p_b_hbt[4][i_a_p] = sqrt(
						arry_p_b_hbt[4][i_a_p] / arry_p_b_hbt[2][i_a_p]
								- arry_p_b_hbt[3][i_a_p]
										* arry_p_b_hbt[3][i_a_p]
										/ (arry_p_b_hbt[2][i_a_p]
												* arry_p_b_hbt[2][i_a_p]));
			} else {
				arry_p_b_hbt[1][i_a_p] = 0;
				arry_p_b_hbt[4][i_a_p] = 0;
			}

			arry_p_b_hbt[0][i_a_p] /= arry_p_b_hbt[2][i_a_p];
			arry_p_b_hbt[3][i_a_p] /= arry_p_b_hbt[2][i_a_p];

		}
	}

	ofstream fout("solution2.txt");
	//预测将会购买的物品
	//f(x)=1/(sqrt(2*Pi)*sigma)*exp(-1*(x-miu)^2/(2*sigma^2))
	//商品是从8月15号开始的预测
	//x轴的零点是开始查看的日期

	double total = 0;
	double acc = 0;
	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		person & p = arry_person[i_a_p];

		int i_brand_e = 0;
		int i_brand_b = 0;
		BuyImformation buy_local;
		bool ifbuy = false;
		for (; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {


			//我们认为在4个月的时间内，买过一个物品后段时间之内是不会再买该物品的，将买过的物品买后一个月到第一次查看的信息清除
			if(p.p_buyimformation[i_brand_e].type == 1 && p.p_buyimformation[i_brand_b].brand_id == p.p_buyimformation[i_brand_e].brand_id){
				ifbuy = true;
				buy_local = p.p_buyimformation[i_brand_e];
			}

			if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {

				if(ifbuy){
					//删除该物品购买后一个月内及购买前的全部信息
					int i=0;
					int gap_local = i_brand_e-i_brand_b;
					for(;i<gap_local;i++){
						if(day_gap(p.p_buyimformation[i+i_brand_b].visit_datetime_month,p.p_buyimformation[i+i_brand_b].visit_datetime_day,
								buy_local.visit_datetime_month,buy_local.visit_datetime_day) < 30){
							arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							p.p_buyimformation = arry_person[i_a_p].p_buyimformation;
						}else{
							i++;
							break;
						}
					}
					i_brand_e -= i;

					ifbuy = false;
				}


				double num_check=double(i_brand_e-i_brand_b);
				/*
				for(int i=i_brand_b+1;i<i_brand_e;i++){
					//将品牌最早查看日期找到，并且将查看次数统计出来
					//由于按时间排序，第一个就是开始日期
					if(compare(p.p_buyimformation[i],p.p_buyimformation[i-1]) == 0){
						num_check--;
					}
				}
				*/
				//计算购买概率{---------------------------------------------------------------

				double d_x = double(day_gap(f_m_b,f_d_b,p.p_buyimformation[i_brand_b].visit_datetime_month,p.p_buyimformation[i_brand_b].visit_datetime_day));
				const double c = 0.159155;
				if(arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[4][i_a_p] > 0.0000001){
					total ++;
					//cout<<arry_p_b_hbt[i_a_p][1]<<"\t"<<arry_p_b_hbt[i_a_p][4]<<endl;



				double P = exp(-0.5*((d_x-arry_p_b_hbt[0][i_a_p])*(d_x-arry_p_b_hbt[0][i_a_p])
						/(arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[1][i_a_p])+(num_check-arry_p_b_hbt[3][i_a_p])*(num_check-arry_p_b_hbt[3][i_a_p])
						/(arry_p_b_hbt[4][i_a_p]*arry_p_b_hbt[4][i_a_p])));

				P /= (arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[4][i_a_p]);
				P *= c;


					//double P = arry_p_b_hbt[4][i_a_p]/arry_p_b_hbt[3][i_a_p];



				//if(P<1.2 && arry_p_b_hbt[3][i_a_p]*d_x < num_check*10./8.){
				if(randoms()<P){
					acc++;
					//扩充
					if(leng_s_a_b_l[i_a_p] == leng_a_b_l[i_a_p]){
						int * arry_b_l_copy = new int [leng_a_b_l[i_a_p]];
						for(int i=0;i<leng_a_b_l[i_a_p];i++){
							arry_b_l_copy[i] = arry_buy_list[i_a_p][i];
						}
						delete [] arry_buy_list[i_a_p];
						arry_buy_list[i_a_p] = NULL;
						arry_buy_list[i_a_p] = new int [2*leng_a_b_l[i_a_p]];
						for(int i=0;i<leng_a_b_l[i_a_p];i++){
							arry_buy_list[i_a_p][i] = arry_b_l_copy[i];
						}
						leng_a_b_l[i_a_p] *= 2;
						delete [] arry_b_l_copy;
					}

					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = p.p_buyimformation[i_brand_b].brand_id;
					//cout<<p[i_brand_b].brand_id<<endl;
					leng_s_a_b_l[i_a_p]++;

				}else{
					//cout<<i_a_p<<"\t"<<P<<endl;
				}
				}else{
					//cout<<i_a_p<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"!"<<endl;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

		}
		if(leng_s_a_b_l[i_a_p]>0){
		fout<<arry_person[i_a_p].get_person_id()<<"\t";
		for(int i=0;i<leng_s_a_b_l[i_a_p];i++){
			if(i<leng_s_a_b_l[i_a_p]-1){
			fout<<arry_buy_list[i_a_p][i]<<",";
			}else{
				fout<<arry_buy_list[i_a_p][i]<<endl;
			}

		}
		}


	}
	cout<<"接受率"<<acc/total<<endl;

	if(!(f_m_b == 8 && f_d_b == 16)){
		//启动本地预判分系统
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 =0;
		//现判断一下len_arry1的大小
		for(int i=0;i<leng_s_arry_person;i++){
			if(leng_s_a_b_l[i]>0){
				len_arry1++;
			}
		}
		arry1 = new int *[len_arry1];
		len_arry1_ev = new int [len_arry1];
		int i_arry_buy_list = 0;
		for(int i=0;i<len_arry1;i++){
			len_arry1_ev[i] = 0;
			for(;i_arry_buy_list<leng_s_arry_person;i_arry_buy_list++){
				if(leng_s_a_b_l[i_arry_buy_list] > 0){
					len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list]+1;
					arry1[i] = new int [leng_s_a_b_l[i_arry_buy_list]+1];
					arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

					for(int j=0;j<leng_s_a_b_l[i_arry_buy_list];j++){
						arry1[i][j+1] = arry_buy_list[i_arry_buy_list][j];
					}
					i_arry_buy_list++;
					break;
				}
			}
		}


		int ** arry2;
		int * len_arry2_ev = NULL;
		int len_arry2 =0;

		chose_kind_from_day(f_m_b,f_d_b,f_m_e,f_d_e,arry_person_input,leng_s_arry_person_input,arry2,len_arry2,len_arry2_ev,1);


		double * value_score = NULL;
		value_score = score(arry1,len_arry1_ev,len_arry1,arry2,len_arry2_ev,len_arry2);
		cout<<value_score[0]*100.0<<"%\t"<<value_score[1]*100.0<<"%\t"<<value_score[2]*100.0<<"%"<<endl;


	}
}

void solution4(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
		int f_m_e=9,int f_d_e=15){

	person * arry_person = new person[leng_s_arry_person_input];
		int leng_s_arry_person = leng_s_arry_person_input;
		for (int i = 0; i < leng_s_arry_person; i++) {
			arry_person[i] = arry_person_input[i];
			arry_person[i].delete_by_day(f_m_b,f_d_b);
		}

		int **arry_buy_list = new int *[leng_s_arry_person];
		int *leng_s_a_b_l=new int [leng_s_arry_person];
		int *leng_a_b_l = new int [leng_s_arry_person];

		double * P_buy = new double [leng_s_arry_person];//一个人的购买率：买过的品牌数/看过的品牌数

		int ** arry2;
					int * len_arry2_ev = NULL;
					int len_arry2 =0;

					chose_kind_from_day(f_m_b,f_d_b,f_m_e,f_d_e,arry_person_input,leng_s_arry_person_input,arry2,len_arry2,len_arry2_ev,1);


		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

			leng_s_a_b_l[i_a_p] = 0;
			leng_a_b_l[i_a_p] = 1000;
			arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
			person &p = arry_person[i_a_p];
			//先按id排个序
			BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);


			//{----------------------------------------------------------------------------------------
			//看一个人买一个东西后又买一次的可能性


			double buy_num_total = 0;//一共买过的次数
			double buy_num_again = 0;//回头次数
			double check_num_total = 0;//一共看过的商品数

			for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
				if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {

					double T = 0;
					BuyImformation buy[100];
					int leng_s_buy = 0;
					for (int i = i_brand_b; i < i_brand_e; i++) {
						if (p.p_buyimformation[i].type == 1) {

							if (leng_s_buy == 0) {
								buy[leng_s_buy] = p.p_buyimformation[i];
								leng_s_buy++;
							} else {
								/*
								if (abs(
										day_gap(
												buy[leng_s_buy - 1].visit_datetime_month,
												buy[leng_s_buy - 1].visit_datetime_day,
												p.p_buyimformation[i].visit_datetime_month,
												p.p_buyimformation[i].visit_datetime_day)) <= 2)
									continue;
								else {
									*/
									buy[leng_s_buy] = p.p_buyimformation[i];
									leng_s_buy++;
									T += day_gap(
											buy[leng_s_buy - 1].visit_datetime_month,
											buy[leng_s_buy - 1].visit_datetime_day,
											p.p_buyimformation[i].visit_datetime_month,
											p.p_buyimformation[i].visit_datetime_day);
								//}
							}
						}
					}

					if(leng_s_buy >= 1)
						buy_num_total++;

					check_num_total++;

					if (leng_s_buy > 1) {
						buy_num_again ++;

							//在预测区间，添加到购买列表中
							arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
									buy[0].brand_id;
							leng_s_a_b_l[i_a_p]++;
							//删除掉该品牌的全部信息
							for (int i = i_brand_b; i < i_brand_e; i++) {
								arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							}
							i_brand_e = i_brand_b;


					}

					i_brand_b = i_brand_e;
					i_brand_e--;

				}
				P_buy[i_a_p] = buy_num_total/check_num_total;
			}


			//第二判断阶段，判断一个人买过的商品买第二次的可能性
			/*
			double P_again = (buy_num_total >2)? buy_num_again/buy_num_total:0;//回头概率
			//如果一个人回头率比较高，我们认为他买过的商品还会买
			if(P_again > 0.35){
				for(int i=0;i<arry_person[i_a_p].leng_s_p_buyim;i++){
					if(p.p_buyimformation[i].type == 1){
						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								p.p_buyimformation[i].brand_id;
						leng_s_a_b_l[i_a_p]++;
					}
				}
			}
			*/
			/*
			else if(P_again <0.2){
				for(int i=0;i<arry_person[i_a_p].leng_s_p_buyim;i++){
					if(p.p_buyimformation[i].type == 1){
						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								-1*p.p_buyimformation[i].brand_id;
						leng_s_a_b_l[i_a_p]++;
					}
				}
			}
			*/
			//购买重复模块end}=================================================================================
/*
			//收藏模块{--------------------------------------------------------------------------------------
			//查看是否收藏商品，认为收藏商品会购买
			for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
				if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
					//该循环是每一个品牌，i_brand_b是起始值，i_brand_e是结束值（不包含i_brand_e)

					//如果一个品牌发生了购买行为，我们将认为其不会再购买{------------------------------------------
					bool ifbuy_local = false;
					for (int i = i_brand_b; i < i_brand_e; i++) {
						if (p.p_buyimformation[i].type == 1) {
							ifbuy_local = true;
							break;
						}
					}
					if(ifbuy_local)
						continue;
					//判断购买结束}======================================================================

					BuyImformation buy;


					for (int i = i_brand_b; i < i_brand_e; i++) {
						if (p.p_buyimformation[i].type == 2) {

								buy = p.p_buyimformation[i];
								break;
						}
					}

					if (buy.type == 2) {

							//在预测区间，添加到购买列表中
							arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
									buy.brand_id;
							leng_s_a_b_l[i_a_p]++;

							//删除掉该品牌的全部信息
							for (int i = i_brand_b; i < i_brand_e; i++) {
								arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							}
							i_brand_e = i_brand_b;


					}

					i_brand_b = i_brand_e;
					i_brand_e--;

				}

			}

		//收藏模块}=======================================================================================
*/




		}




		const int num_kind_arry_pbhbt = 6;
		double *arry_p_b_hbt[num_kind_arry_pbhbt];
		for(int i=0;i<num_kind_arry_pbhbt;i++){
			arry_p_b_hbt[i] = new double [leng_s_arry_person];
		}
		//[0]位储存购买天数平均值，[1]位储存天数标准差，[2]位储存购买个数，[3]查看次数平均值，4为其标准差，5为个数
		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {


			for (int i = 0; i < num_kind_arry_pbhbt; i++) {
				arry_p_b_hbt[i][i_a_p] = 0;

			}

			person & p_bimf_l = arry_person[i_a_p];

			for (int i_b = 0; i_b < arry_person[i_a_p].leng_s_p_buyim; i_b++) {
				if (p_bimf_l.p_buyimformation[i_b].type == 1) {
					BuyImformation b_local;
					b_local = p_bimf_l.p_buyimformation[i_b];
					//检查之前是否购买过同个商品

					int i_b_b = i_b - 1;	//运行后刚好是一个brand_id的起始位置
					int i_b_b_l = 0;
					for (;
							(p_bimf_l.p_buyimformation[i_b_b].brand_id == b_local.brand_id
									&& i_b_b >= 0); i_b_b--) {
						if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

							i_b_b_l = i_b_b;
						}
					}
					i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

					//开始记录平均值
					//day_gap_value1描述查看天数
					double day_gap_value1 = day_gap(p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
							p_bimf_l.p_buyimformation[i_b].visit_datetime_day,
							p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_month,
							p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_day);

					//int day_gap_value2描述有几天查看
					double day_gap_value2 = 0;
					for(int i_check_d = i_b_b_l+1;i_check_d<i_b;i_check_d++){
						if(compare(p_bimf_l.p_buyimformation[i_check_d],p_bimf_l.p_buyimformation[i_check_d-1]) != 0){
							day_gap_value2++;
						}
					}
					if (day_gap_value1 < 0)
						exit(0);
					if (day_gap_value1 == 0)
						continue;
					//cout<<day_gap_value<<endl;

					arry_p_b_hbt[0][i_a_p] += double(day_gap_value1);
					arry_p_b_hbt[1][i_a_p] += double(day_gap_value1 * day_gap_value1);
					arry_p_b_hbt[2][i_a_p] += 1.0;


					double num_check_rel = day_gap_value2 >0 ?(double(i_b)-double(i_b_b_l))/double(day_gap_value2):0;
					/*
					double num_check_rel = 0.;
					for(int i=i_b_b_l+1;i<i_b;i++){
						if(compare(p_bimf_l.p_buyimformation[i],p_bimf_l.p_buyimformation[i-1])==0)
							continue;
						else
							num_check_rel++;
					}
					num_check_rel/=double(day_gap_value);
					*/
					arry_p_b_hbt[3][i_a_p] += num_check_rel;
					arry_p_b_hbt[4][i_a_p] += num_check_rel * num_check_rel;

				}
			}



			arry_p_b_hbt[3][i_a_p] = 0;
			arry_p_b_hbt[4][i_a_p] = 0;
			for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
				if(i_brand_e != 0 && (p_bimf_l.p_buyimformation[i_brand_e-1].brand_id == p_bimf_l.p_buyimformation[i_brand_e].brand_id) &&p_bimf_l.p_buyimformation[i_brand_e].type == 1){
					i_brand_b = i_brand_e+1;
					continue;
				}
			if (p_bimf_l.p_buyimformation[i_brand_b].brand_id
					!= p_bimf_l.p_buyimformation[i_brand_e].brand_id) {
				double day_gap_value2 = 0;					//实际查看天数
				for (int i_check_d = i_brand_b + 1; i_check_d < i_brand_e;
						i_check_d++) {
					if (compare(p_bimf_l.p_buyimformation[i_check_d],
							p_bimf_l.p_buyimformation[i_check_d - 1]) != 0) {
						day_gap_value2++;
					}

				}

				double num_check_rel =
						day_gap_value2 > 0 ?
								(double(i_brand_e - i_brand_b))
										/ double(day_gap_value2) :
								0;

				arry_p_b_hbt[3][i_a_p] += num_check_rel;
				arry_p_b_hbt[4][i_a_p] += num_check_rel * num_check_rel;
				if(num_check_rel != 0)
					arry_p_b_hbt[5][i_a_p] += 1;

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

			}






			if (arry_p_b_hbt[2][i_a_p] != 0) {
				if (arry_p_b_hbt[2][i_a_p] != 1) {
					arry_p_b_hbt[1][i_a_p] = sqrt(
							arry_p_b_hbt[1][i_a_p] / arry_p_b_hbt[2][i_a_p]
									- arry_p_b_hbt[0][i_a_p]
											* arry_p_b_hbt[0][i_a_p]
											/ (arry_p_b_hbt[2][i_a_p]
													* arry_p_b_hbt[2][i_a_p]));
					arry_p_b_hbt[4][i_a_p] = sqrt(
							arry_p_b_hbt[4][i_a_p] / arry_p_b_hbt[5][i_a_p]
									- arry_p_b_hbt[3][i_a_p]
											* arry_p_b_hbt[3][i_a_p]
											/ (arry_p_b_hbt[5][i_a_p]
													* arry_p_b_hbt[5][i_a_p]));
				} else {
					arry_p_b_hbt[1][i_a_p] = 0;
					arry_p_b_hbt[4][i_a_p] = 0;
				}

				arry_p_b_hbt[0][i_a_p] /= arry_p_b_hbt[2][i_a_p];
				arry_p_b_hbt[3][i_a_p] /= arry_p_b_hbt[5][i_a_p];

			}
		}



		//购买率模块{---------------------------------------------------------------------------------------
		/*
		 * 利用购买率判断的结果不好  3%
		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

			if(false){
				person &p = arry_person[i_a_p];
				for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
					//从购买的下一个开始
					if(p.p_buyimformation[i_brand_e].type == 1){
						i_brand_b = i_brand_e+1;
						continue;
					}

							if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
								double day_gap_value2 = 0;//实际查看天数
								for(int i_check_d = i_brand_b+1;i_check_d<i_brand_e;i_check_d++){
									if(compare(p.p_buyimformation[i_check_d],p.p_buyimformation[i_check_d-1]) != 0){
										day_gap_value2++;
									}
								}
								if(day_gap_value2 != 0 && double(i_brand_e-i_brand_b)/day_gap_value2 > 3.0/sqrt(P_buy[i_a_p])*arry_p_b_hbt[3][i_a_p]
								                                                                                           &&
								 day_gap(f_m_b,f_d_b,p.p_buyimformation[i_brand_b].visit_datetime_month,p.p_buyimformation[i_brand_b].visit_datetime_day)< arry_p_b_hbt[0][i_a_p] + 0.5*arry_p_b_hbt[1][i_a_p])
								{	//添加到购物列表中
									arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = p.p_buyimformation[i_brand_b].brand_id;
									leng_s_a_b_l[i_a_p]++;
								}

								i_brand_b = i_brand_e;
								i_brand_e--;
							}
				}

			}

		}
		*/
		//购买率模块}=======================================================================================

		//直接检查查看次数{---------------------------------------------------------------------------------

		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {
			person &p = arry_person[i_a_p];


				for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
					//从购买的下一个开始
					if(i_brand_e != 0 && (p.p_buyimformation[i_brand_e-1].brand_id == p.p_buyimformation[i_brand_e].brand_id) && p.p_buyimformation[i_brand_e].type == 1){
						i_brand_b = i_brand_e+1;
						continue;
					}

					if (i_brand_b >0 && p.p_buyimformation[i_brand_b-1].brand_id == p.p_buyimformation[i_brand_b].brand_id &&
							p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
													//如果一个人在几天内查看突破天际的话就说明他要购买
													double num_over_check_av = 0;
													for (int i_brand_e_d = i_brand_b,i_brand_b_d = i_brand_b; i_brand_e_d < i_brand_e; i_brand_e_d++) {
														double num_one_day = 0;
																if (compare(p.p_buyimformation[i_brand_b_d], p.p_buyimformation[i_brand_e_d]) != 0) {
																	//单天查看次数
																	num_one_day = i_brand_e_d-i_brand_b_d;
																	i_brand_b_d = i_brand_e_d;
																	i_brand_e_d--;
																}

																	num_over_check_av += num_one_day/4.;



													}
													if(num_over_check_av >= 1 ){
														arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = p.p_buyimformation[i_brand_b].brand_id;
														leng_s_a_b_l[i_a_p]++;
													}
													i_brand_b = i_brand_e;
													i_brand_e--;
												}

					else if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
								//如果一个人在几天内查看突破天际的话就说明他要购买

								double num_over_check_av = 0;
								for (int i_brand_e_d = i_brand_b,i_brand_b_d = i_brand_b; i_brand_e_d < i_brand_e; i_brand_e_d++) {
									double num_one_day = 0;
											if (compare(p.p_buyimformation[i_brand_b_d], p.p_buyimformation[i_brand_e_d]) != 0) {
												//单天查看次数
												num_one_day = i_brand_e_d-i_brand_b_d;
												i_brand_b_d = i_brand_e_d;
												i_brand_e_d--;
											}
											if(arry_p_b_hbt[4][i_a_p] != 0 && arry_p_b_hbt[4][i_a_p]/arry_p_b_hbt[3][i_a_p] < 2 && num_one_day/arry_p_b_hbt[3][i_a_p] > 0.8 )
												num_over_check_av ++;
											else if((arry_p_b_hbt[4][i_a_p] == 0 || arry_p_b_hbt[4][i_a_p]/arry_p_b_hbt[3][i_a_p] > 2) && num_one_day >= 4 )
												num_over_check_av ++;


								}
								if(num_over_check_av >= 2 || (i_brand_e - i_brand_b) > 10){//&& day_gap(f_m_b,f_m_e,p.p_buyimformation[i_brand_e-1].visit_datetime_month,p.p_buyimformation[i_brand_e-1].visit_datetime_day) < 50){
									arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = p.p_buyimformation[i_brand_b].brand_id;
									leng_s_a_b_l[i_a_p]++;
								}

								i_brand_b = i_brand_e;
								i_brand_e--;
							}
				}



		}


		//查看次数}=====================================================================================






		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

			person &p = arry_person[i_a_p];


		//检查一下i_a_p用户在第四个月是否买东西了
		int i_find_u = 0;
		for(;i_find_u<len_arry2;i_find_u++){
			if(arry_person[i_a_p].get_person_id() == arry2[i_find_u][0])
				break;
		}

		//如果买了
		if(i_find_u != len_arry2){
			cout<<"周期:"<<arry_p_b_hbt[0][i_a_p]<<"\t标准差"<<arry_p_b_hbt[1][i_a_p]<<"\t平均次数:"<<arry_p_b_hbt[3][i_a_p]<<"\t标准差:"<<arry_p_b_hbt[4][i_a_p]<<endl;
		for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			//cout<<p.p_buyimformation[i_brand_b].brand_id<<"\t"<<p.p_buyimformation[i_brand_e].brand_id<<endl;
					if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
						bool ifbuy = false;
						bool ifinbuylist = false;
							for(int j = 1;j<len_arry2_ev[i_find_u];j++){
								if(p.p_buyimformation[i_brand_b].brand_id == arry2[i_find_u][j]){
									ifbuy = true;
									break;
								}
							}
							for(int j=0;j<leng_s_a_b_l[i_a_p];j++){
								if(p.p_buyimformation[i_brand_b].brand_id == arry_buy_list[i_a_p][j]){
									ifinbuylist = true;
									break;
								}
							}

						if(!ifbuy || ifinbuylist){
							i_brand_b = i_brand_e;
							i_brand_e--;
							continue;
						}


						for (int i = i_brand_b; i < i_brand_e; i++) {
							cout<<arry_person[i_a_p].get_person_id()<<"\t"<<p.p_buyimformation[i].brand_id<<"\t"<<p.p_buyimformation[i].type<<"\t"<<p.p_buyimformation[i].visit_datetime_month<<"."<<p.p_buyimformation[i].visit_datetime_day<<endl;
						}
						double d_g = day_gap(p.p_buyimformation[i_brand_e-1].visit_datetime_month,p.p_buyimformation[i_brand_e-1].visit_datetime_day
								,p.p_buyimformation[i_brand_b].visit_datetime_month,p.p_buyimformation[i_brand_b].visit_datetime_day);
						double day_gap_value2 = 0;
						for(int i_check_d = i_brand_b+1;i_check_d<i_brand_e;i_check_d++){
							if(compare(p.p_buyimformation[i_check_d],p.p_buyimformation[i_check_d-1]) != 0){
								day_gap_value2++;
							}
						}
						cout<<"查看天数1:"<<d_g<<"\t天数2："<<day_gap_value2<<"\t次数:"<<double(i_brand_e-i_brand_b)<<"\t1:"<<double(i_brand_e-i_brand_b)/d_g
								<<"\t2:"<<double(i_brand_e-i_brand_b)/day_gap_value2<<"\tP_b="<<P_buy[i_a_p]
						<<endl;
						cout<<"-------------------------------------------------"<<endl;


						i_brand_b = i_brand_e;
						i_brand_e--;
						cout<<"======================================================================="<<endl;
						//system("pause");
					}

				}
	}


		}


/*
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 =0;

		chose_kind_from_day(0,16,7,15,arry_person_input,leng_s_arry_person_input,arry1,len_arry1,len_arry1_ev,2);
*/

		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 =0;


		//将buy_list转换为arry1形式储存{-----------------------------------------------------------------------
		//现判断一下len_arry1的大小
		for(int i=0;i<leng_s_arry_person;i++){
			if(leng_s_a_b_l[i]>0){
				len_arry1++;
			}
		}
		arry1 = new int *[len_arry1];
		len_arry1_ev = new int [len_arry1];
		int i_arry_buy_list = 0;
		for(int i=0;i<len_arry1;i++){
			len_arry1_ev[i] = 0;
			for(;i_arry_buy_list<leng_s_arry_person;i_arry_buy_list++){
				if(leng_s_a_b_l[i_arry_buy_list] > 0){
					len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list]+1;
					arry1[i] = new int [leng_s_a_b_l[i_arry_buy_list]+1];
					arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

					for(int j=0;j<leng_s_a_b_l[i_arry_buy_list];j++){
						arry1[i][j+1] = arry_buy_list[i_arry_buy_list][j];
					}
					i_arry_buy_list++;
					break;
				}
			}
		}

		//转化end}=========================================================================================

		check_arry_re(arry1,len_arry1,len_arry1_ev);
		ofstream fout("solution4.txt");
		for(int i=0;i<len_arry1;i++){
			fout<<arry1[i][0]<<"\t";
			for(int j=0;j<len_arry1_ev[i]-1;j++){
				fout<<arry1[i][j]<<",";
			}
			fout<<arry1[i][len_arry1_ev[i]-1]<<endl;
		}
//f_m_b,f_d_b,f_m_e,f_d_e

			double * value_score = NULL;
			value_score = score(arry1,len_arry1_ev,len_arry1,arry2,len_arry2_ev,len_arry2);
			cout<<value_score[0]*100.0<<"%\t"<<value_score[1]*100.0<<"%\t"<<value_score[2]*100.0<<"%"<<endl;

}

void solution5(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
			int f_m_e=9,int f_d_e=15){


	const double C1 = 0.0;//如果两个用户间没有交集取0.5代替
	const double C2 = 0.1;//



	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
	}

	int ** arry_cnnct;
	int *len_arry_cnnct_ev;
	int *buy_num;

	findconnect(arry_person, leng_s_arry_person, arry_cnnct, len_arry_cnnct_ev,
			buy_num);

	//
	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	int *leng_a_b_l = new int[leng_s_arry_person];

	for(int i=0;i<leng_s_arry_person;i++){
		leng_a_b_l[i] = 1000;
		leng_s_a_b_l[i] = 0;
		arry_buy_list[i] = new int [leng_a_b_l[i]];
	}

	int ** arry2;
	int * len_arry2_ev = NULL;
	int len_arry2 = 0;

	chose_kind_from_day(f_m_b, f_d_b, f_m_e, f_d_e, arry_person_input,
			leng_s_arry_person_input, arry2, len_arry2, len_arry2_ev, 1);

	int * userid = new int [len_arry_cnnct_ev[0]+1];
	int len_userid = len_arry_cnnct_ev[0]+1;
	userid[0] = 0;
	for(int i=1;i<len_arry_cnnct_ev[0]+1;i++){
		userid[i] = arry_cnnct[0][i-1];
	}
	int * buyid = new int [2000];
	int len_buyid = 0;

	for(int i=0;i<leng_s_arry_person;i++){
		delete [] arry_cnnct[i];
	}
	delete [] arry_cnnct;
	delete [] len_arry_cnnct_ev;

	double ** arry_buy_cnnct_m = new double *[len_userid];
	for(int i=0;i<len_userid;i++){
		arry_buy_cnnct_m[i] = new double [2000];
	}

	for(int i=0;i<len_userid;i++){
		BuyImformation *p = arry_person[userid[i]].p_buyimformation;
		for(int j=0;j<arry_person[userid[i]].leng_s_p_buyim;j++){
			if(p[j].type == 1){


			int k=0;
			for(;k<len_buyid;k++){
				if(p[j].brand_id == buyid[k]){
					arry_buy_cnnct_m[i][k] = 1;
					break;
				}
			}
			if(k == len_buyid){
				buyid[len_buyid] = p[j].brand_id;
				arry_buy_cnnct_m[i][len_buyid] = 1;
				len_buyid++;
			}
			}
		}
	}
	int **num_cover = new int *[len_userid];
	for(int i=0;i<len_userid;i++){
		num_cover[i] = new int [len_userid];
	}

	for(int i=0;i<len_userid;i++){
		for(int j=i+1;j<len_userid;j++){
			int num_cover_l = 0;
			for(int k=0;k<len_buyid;k++){
				if(arry_buy_cnnct_m[i][k] + arry_buy_cnnct_m[j][k] == 2)
					num_cover_l ++;
			}
			num_cover[i][j] = num_cover_l;
			num_cover[j][i] = num_cover_l;
		}
	}
	//对于每一个人筛选
#pragma omp parallel for
	for(int i_a=0;i_a<len_userid;i_a++){

		//对于每一件商品
		for(int i_br = 0;i_br <len_buyid;i_br++){

			if(arry_buy_cnnct_m[i_a][i_br] == 1)
				continue;
			double rio = 0 ;
			for(int i_b=0;i_b<len_userid;i_b++){
				if(i_a == i_b)
					continue;
				//cout<<num_cover[i_a][i_b]<<endl;
				if(arry_buy_cnnct_m[i_b][i_br] == 1)
					rio+= pow(double(num_cover[i_a][i_b]),2)/double(buy_num[i_b]);

			}
			rio *= 0.1*double(buy_num[i_a]);
			if(rio >= 0.5){
				int i_a_p = userid[i_a];

				arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = buyid[i_br];
				leng_s_a_b_l[i_a_p]++;
			}
		}

	}

	for(int i=0;i<len_userid;i++){
		delete [] num_cover[i];

	}
	delete [] num_cover;

	for(int i=0;i<len_userid;i++){
		delete [] arry_buy_cnnct_m[i];
	}
	delete [] arry_buy_cnnct_m;
	delete []userid;
	delete [] buyid;




	int ** arry1;
	int * len_arry1_ev = NULL;
	int len_arry1 =0;


	//将buy_list转换为arry1形式储存{-----------------------------------------------------------------------
	//现判断一下len_arry1的大小
	for(int i=0;i<leng_s_arry_person;i++){
		if(leng_s_a_b_l[i]>0){
			len_arry1++;
		}
	}
	arry1 = new int *[len_arry1];
	len_arry1_ev = new int [len_arry1];
	int i_arry_buy_list = 0;
	for(int i=0;i<len_arry1;i++){
		len_arry1_ev[i] = 0;
		for(;i_arry_buy_list<leng_s_arry_person;i_arry_buy_list++){
			if(leng_s_a_b_l[i_arry_buy_list] > 0){
				int len_local = leng_s_a_b_l[i_arry_buy_list];
				len_arry1_ev[i] = len_local+1;
				arry1[i] = new int [len_local+1];
				arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

				for(int j=0;j<leng_s_a_b_l[i_arry_buy_list];j++){
					arry1[i][j+1] = arry_buy_list[i_arry_buy_list][j];
				}
				i_arry_buy_list++;
				break;
			}
		}
	}

	//转化end}=========================================================================================

	check_arry_re(arry1,len_arry1,len_arry1_ev);


	double * value_score = NULL;
				value_score = score(arry1,len_arry1_ev,len_arry1,arry2,len_arry2_ev,len_arry2);
				cout<<value_score[0]*100.0<<"%\t"<<value_score[1]*100.0<<"%\t"<<value_score[2]*100.0<<"%"<<endl;


}

void findconnect(person * arry_person_input,int len_arry_person_input,int ** & arry_person_connect,int * & len_a_p_c,int *&buy_num){

	person * arry_person = new person[len_arry_person_input];
	int len_arry_person = len_arry_person_input;
	for (int i = 0; i < len_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}



	arry_person_connect = new int *[len_arry_person];
	buy_num = new int [len_arry_person];
	int **arry_connect_local = new int *[len_arry_person];
	len_a_p_c = new int [len_arry_person];
	int * len_a_p_c_l = new int [len_arry_person];
	int * len_a_p_c_ln = new int [len_arry_person];
	for(int i=0;i<len_arry_person;i++){
		arry_person_connect[i] = new int [len_arry_person];
		arry_connect_local[i] = new int [len_arry_person];
		len_a_p_c[i] = 0;
		len_a_p_c_l[i] = 0;
		len_a_p_c_ln[i] = 0;
	}

	int num_save_s = 0;
	int num_find_total = 0;//已经找到的

	//先找到第一个人作为开始

				arry_connect_local[0][0] = 0;
				len_a_p_c_l[0] = 1;
				num_save_s = 1;
				num_find_total++;
				if(arry_person[0].leng_s_p_buyim != 0)
					arry_person[0].leng_s_p_buyim *= -1;
				else
					arry_person[0].leng_s_p_buyim = -1;


	//开始查找
	do{
		if(num_find_total == len_arry_person)
			break;

		if(len_a_p_c_l[num_save_s-1] == len_a_p_c_ln[num_save_s-1]){
			//说明一个组的已经找完，开始下一个组
			for(int i=0;i<len_arry_person;i++){
				if(arry_person[i].leng_s_p_buyim>=0){
					arry_connect_local[num_save_s][0] = i;
					len_a_p_c_l[num_save_s] = 1;
					num_save_s++;
					num_find_total++;
					if(arry_person[i].leng_s_p_buyim != 0)
						arry_person[i].leng_s_p_buyim *= -1;
					else
						arry_person[i].leng_s_p_buyim = -1;
					break;
				}
			}
		}

		//查找与checkid对应的人有联系的人
					int checkid = arry_connect_local[num_save_s-1][len_a_p_c_ln[num_save_s-1]];
					BuyImformation * p_r = arry_person[checkid].p_buyimformation;
					int buycheckid[500];
					int len_buycheckid = 0;
								for(int j=0;j<abs(arry_person[checkid].leng_s_p_buyim);j++){
									if(p_r[j].type != 1)
										continue;
									buycheckid[len_buycheckid] = p_r[j].brand_id;
									len_buycheckid++;
								}
					buy_num[checkid] = len_buycheckid;
		for(int i=0;i<abs(len_arry_person);i++){

			//首先判断一下该user是否已加入小组
			if(arry_person[i].leng_s_p_buyim < 0 )
				continue;

			//该用户没有加入，判断一下是否需要加入
			bool ifshoudjoin = false;
			BuyImformation * p_f = arry_person[i].p_buyimformation;
			for(int j=0;j<abs(arry_person[i].leng_s_p_buyim);j++){
				if(p_f[j].type != 1)
					continue;
				for(int k=0;k<len_buycheckid;k++){
					if(p_f[j].brand_id == buycheckid[k]){
						cout<<buycheckid[k]<<endl;
						ifshoudjoin =true;
						break;
					}

				}
				if(ifshoudjoin)
					break;
			}
			if(ifshoudjoin){
				arry_connect_local[num_save_s-1][len_a_p_c_l[num_save_s-1]] = i;
				len_a_p_c_l[num_save_s-1]++;
				if(arry_person[i].leng_s_p_buyim != 0)
				arry_person[i].leng_s_p_buyim *=-1;
				else
					arry_person[i].leng_s_p_buyim = -1;
				num_find_total++;
			}
		}
		len_a_p_c_ln[num_save_s-1]++;
	}while(1);


	//将arry_connect_local转化为分用户储存方便查看
	for(int i=0;i<num_save_s;i++){
		for(int j=0;j<len_a_p_c_l[i];j++){
			for(int k=0;k<len_a_p_c_l[i];k++){
				int ci = arry_connect_local[i][j];
				if(ci == arry_connect_local[i][k])
					continue;

				arry_person_connect[ci][len_a_p_c[ci]] = arry_connect_local[i][k];
				len_a_p_c[ci]++;
			}
		}
	}


	delete [] arry_person;
	delete [] len_a_p_c_l;
	delete [] len_a_p_c_ln;
	for(int i=0;i<len_arry_person;i++){

		delete [] arry_connect_local[i];

	}
	delete [] arry_connect_local;

}



//对BuyImformation进行排序，以brand_id递增的方式排序，相同id内以日期递增的方式排序
void BubbleSort(BuyImformation * pData, int Count) {
	BuyImformation iTemp;
	for (int i = 0; i < Count; i++) {
		for (int j = Count - 1; j > i; j--) {
			if (pData[j].brand_id < pData[j - 1].brand_id) {
				iTemp = pData[j - 1];
				pData[j - 1] = pData[j];
				pData[j] = iTemp;

			} else if (pData[j].brand_id == pData[j - 1].brand_id) {
				//日期小的在前面
				if (pData[j].visit_datetime_month
						< pData[j - 1].visit_datetime_month
						|| (pData[j].visit_datetime_month
								== pData[j - 1].visit_datetime_month
								&& pData[j].visit_datetime_day
										< pData[j - 1].visit_datetime_day)) {
					iTemp = pData[j - 1];
					pData[j - 1] = pData[j];
					pData[j] = iTemp;
				}
			}
		}
	}
}



//比较两个日期大小，1,-1,0对应大于等于，小于
int compare(BuyImformation a, BuyImformation b) {
	//1,-1,0对应大于等于，小于
	int a_d = a.visit_datetime_day + a.visit_datetime_month * 100;
	int b_d = b.visit_datetime_day + b.visit_datetime_month * 100;

	a_d -= b_d;
	if (a_d > 0) {
		return 1;
	} else if (a_d == 0) {
		return 0;
	} else if (a_d < 0) {
		return -1;
	} else {
		return -2;
	}
}





double* score(int ** arry1, int *len_arry1_ev, int len_arry1, int ** arry2, int *len_arry2_ev, int len_arry2)
//返回一个3位数组的指针，第一位是命中率，第二位是召回率，第三位是分数，都是double型
{


	int hit = 0,hitu=0;
	//hit存储品牌命中数，hitu存储命中用户数

	int pBrand = 0, bBrand = 0;
	//pBrand存储预测品牌总数，bBrand存储实际品牌总数

	int* hituser1 = new int[len_arry1];
	//hituser存储命中用户在1中的序号，不是id

	int* hituser2 = new int[len_arry1];
	//hituser存储命中用户在2中的序号，不是id

	double* prst=new double[3];

	for (int i = 0; i < len_arry1; i++)
	//遍历1
	{
		for (int j = 0; j < len_arry2; j++)
		//每个1与所有2比较
		{
			if (arry1[i][0]==arry2[j][0])
			{
				hituser1[hitu] = i;
				hituser2[hitu] = j;
				hitu++;
			}
		}
	}
	for (int i = 0; i < hitu; i++)
	//遍历所有命中用户，此时i代表hituser1/2中的序号，hituser1/2[i]代表user id在1，2中的序号
	{
		for (int j = 1; j < len_arry1_ev[hituser1[i]]; j++)
		//遍历某命中用户的预测品牌
		{

			for (int k = 1; k < len_arry2_ev[hituser2[i]]; k++)
			//比较预测品牌和所有实际品牌
			{

				if (arry1[hituser1[i]][j] == arry2[hituser2[i]][k])
				{
					hit++;
				}
			}
		}
	}

	for (int i = 0; i < len_arry1; i++)
	//计算命中数
	{
		pBrand += len_arry1_ev[i]-1;
	}

	for (int i = 0; i < len_arry2; i++)
	//计算召回数
	{
		bBrand += len_arry2_ev[i]-1;
	}

	prst[0] = (double(hit) / double(pBrand));
	//命中率
	prst[1] = (double(hit) / double(bBrand));
	//召回率

	cout<<"hit="<<hit<<"\tpBrand="<<pBrand<<"\tbBrand="<<bBrand<<endl;
	prst[2] = (2.0 * prst[0] * prst[1] / (prst[0] + prst[1]));
	//得分

	delete [] hituser1;
	delete [] hituser2;

	return prst;
}
