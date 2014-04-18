//============================================================================
// Name        : Frustrated.cpp
// Author      : BSheng  &  YHH  & LH
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <cstdio>
#include <cwchar>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#pragma warning(disable:4996) //全部关掉

using namespace std;

class BuyImformation;
class person;

int compare(BuyImformation a, BuyImformation b);
void BubbleSort(BuyImformation * pData, int Count);
int compare(BuyImformation a, BuyImformation b);
double* score(int ** arry1, int *len_arry1_ev, int len_arry1, int ** arry2,
		int *len_arry2_ev, int len_arry2);
void findconnect(person * arry_person_input, int len_arry_person_input,
		int ** & arry_person_connect, int * & len_a_p_c, int *&buy_num);
void findPbuy(person * arry_person_input, int len_arry_person_input,
		double **&Pbuy, int &len_Pbuy);
void findPbuyagain(person * arry_person_input, int len_arry_person_input,
		double **&Pbuy, int &len_Pbuy);
double Sq(double q, int n, double a1) {
	return a1 * (1. - pow(q, n)) / (1. - q);
}

void findprivilege(person * arry_person,int len_arry_person,double **arry_Pbuy,int len_Pbuy,int f_m_b,int f_d_b ,int *& arry_pri_brand,int & len_arry_pri_brand);
void findbuyinBuylist(person* arry_person,int len_arry_person
		,int **&buylist,int *&len_buylist_ev,int &len_buylist
		,double **Pbuy,int len_Pbuy
		,int f_m_b,int f_d_b);
void brandcheckout(person* arry_person,int len_arry_person,int brand_id,int * brand_im,const int num_day_total);
void brandchecked(person* arry_person,int len_arry_person,int *& arry_brand,int &len_arry_brand);

void findpeak(int* brd_arr, int len);
void findthreshold(person personi,
		int f_m_b,int f_d_b,int f_m_e,int f_d_e);
void findthreshold2(person personi,
		int f_m_b,int f_d_b,int f_m_e,int f_d_e
		,int *& buylist,int& len_buylist);
double fit(double *x,double *y,int num_n);

class BuyImformation {
public:
	int brand_id;
	int type; //点击：0；购买：1；收藏：2；购物车：3
	int visit_datetime_month;
	int visit_datetime_day;
	int num_check_buy;
	int num_buys;

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
		this->num_check_buy = a.num_check_buy;
		this->num_buys = a.num_buys;

		return *this;
	}
	BuyImformation(void) {
		this->brand_id = -1;
		this->type = -1;
		this->visit_datetime_day = -1;
		this->visit_datetime_month = -1;
		this->num_check_buy = -1;
		this->num_buys = -1;
	}

	BuyImformation(const BuyImformation & a) {
		this->brand_id = a.brand_id;
		this->type = a.type;
		this->visit_datetime_day = a.visit_datetime_day;
		this->visit_datetime_month = a.visit_datetime_month;
		this->num_check_buy = a.num_check_buy;
		this->num_buys = a.num_buys;
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
	void delete_one_p_buy(int i_brand) {
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
		} else {
			cout << "operat erro" << endl;
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

		else {
			cout << a.leng_p_buyim << endl;
			cout << "operat erro" << endl;
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

void person::delete_by_day(int d_month, int d_day) {

	BuyImformation d_b;
	d_b.visit_datetime_month = d_month;
	d_b.visit_datetime_day = d_day;

	for (int i = 0; i < leng_s_p_buyim; i++) {

		if (compare(p_buyimformation[i], d_b) >= 0) {
			delete_one_p_buy(i);
			i--;
		}
	}
}

//将b_month,b_day开始的购买的商品id返回出来
void chose_kind_from_day(int b_month, int b_day, int e_month, int e_day,
		person * arry_in, int len_arry_in, int ** &arry_buy_list,
		int &len_arry_buy_list, int *&len_arry_buy_list_ev, int kind) {

	if (kind != 1 && kind != 2 && kind != 3 && kind != 0) {
		cout << "kind erro" << endl;
		return;
	}

	len_arry_buy_list = 0;
	int *len_a_b_l_re = new int[len_arry_in]; //储存arry_buy_list实际每一行的声明长度
	for (int i = 0; i < len_arry_in; i++) {
		len_a_b_l_re[i] = 100;
	}

	arry_buy_list = new int *[len_arry_in];
	len_arry_buy_list_ev = new int[len_arry_in];
	for (int i = 0; i < len_arry_in; i++) {
		arry_buy_list[i] = new int[len_a_b_l_re[i]];
		len_arry_buy_list_ev[i] = 0;
	}

	for (int i = 0; i < len_arry_in; i++) {
		for (int j = 0; j < arry_in[i].leng_p_buyim; j++) {
			BuyImformation com_local_b;
			com_local_b.visit_datetime_month = b_month;
			com_local_b.visit_datetime_day = b_day;
			BuyImformation com_local_e;
			com_local_e.visit_datetime_month = e_month;
			com_local_e.visit_datetime_day = e_day;
			//如果日期大于选择开始日期，并且type==1则加入购买名单
			if (compare(com_local_b, arry_in[i].p_buyimformation[j]) < 0
					&& compare(com_local_e, arry_in[i].p_buyimformation[j]) > 0
					&& arry_in[i].p_buyimformation[j].type == kind) {
				//如果len_arry_buy_list_ev[len_arry_buy_list]为0,说明还没有储存user——id，先储存user_id
				if (len_arry_buy_list_ev[len_arry_buy_list] == 0) {
					arry_buy_list[len_arry_buy_list][0] =
							arry_in[i].get_person_id();
					arry_buy_list[len_arry_buy_list][1] =
							arry_in[i].p_buyimformation[j].brand_id;
					len_arry_buy_list_ev[len_arry_buy_list] = 2;
				} else {
					//检查该id之前是否添加过{----------------------------------------------------------------------
					int k = 0;
					for (; k < len_arry_buy_list_ev[len_arry_buy_list]; k++) {
						if (arry_buy_list[len_arry_buy_list][k]
								== arry_in[i].p_buyimformation[j].brand_id) {
							break;
						}
					}
					//end}==================================================================================
					if (k == len_arry_buy_list_ev[len_arry_buy_list]) {
						//如果相等说明没有被储存
						arry_buy_list[len_arry_buy_list][len_arry_buy_list_ev[len_arry_buy_list]] =
								arry_in[i].p_buyimformation[j].brand_id;
						len_arry_buy_list_ev[len_arry_buy_list]++;

						//判断是否需要扩容{-----------------------------------------------------------------------
						if (len_arry_buy_list_ev[len_arry_buy_list]
								== len_a_b_l_re[len_arry_buy_list]) {
							int * cp_buy =
									new int[len_a_b_l_re[len_arry_buy_list]];
							for (int i_cp = 0;
									i_cp < len_a_b_l_re[len_arry_buy_list];
									i_cp++) {
								cp_buy[i_cp] =
										arry_buy_list[len_arry_buy_list][i_cp];
							}
							delete[] arry_buy_list[len_arry_buy_list];
							arry_buy_list[len_arry_buy_list] = NULL;
							arry_buy_list[len_arry_buy_list] =
									new int[len_a_b_l_re[len_arry_buy_list] * 2];
							for (int i_cp = 0;
									i_cp < len_a_b_l_re[len_arry_buy_list];
									i_cp++) {
								arry_buy_list[len_arry_buy_list][i_cp] =
										cp_buy[i_cp];
							}
							len_a_b_l_re[len_arry_buy_list] *= 2;
							delete[] cp_buy;
						}
						//扩容end}==============================================================================
					}
				}
			}
		}

		if (len_arry_buy_list_ev[len_arry_buy_list] != 0) {
			len_arry_buy_list++;
		}
	}

	delete[] len_a_b_l_re;
}

//检查arry中是否有重复的商品，有的话剔除
void check_arry_re(int ** &arry_buy_list, int &len_arry_buy_list,
		int *&len_arry_buy_list_ev) {
	for (int i = 0; i < len_arry_buy_list; i++) {
		int i_ev = 2;
		for (; i_ev < len_arry_buy_list_ev[i]; i_ev++) {
			int j_ev = 1;
			for (; j_ev < i_ev; j_ev++) {
				if (abs(arry_buy_list[i][i_ev])
						== abs(arry_buy_list[i][j_ev])) {
					if (arry_buy_list[i][i_ev] > 0) {
						arry_buy_list[i][i_ev] = 0;
					} else if (arry_buy_list[i][j_ev] > 0) {
						arry_buy_list[i][j_ev] = 0;
					} else
						arry_buy_list[i][i_ev] = 0;
				}

			}
		}

		for (i_ev = 0; i_ev < len_arry_buy_list_ev[i]; i_ev++) {
			if (arry_buy_list[i][i_ev] == 0 || arry_buy_list[i][i_ev] < 0) {
				for (int j = i_ev; j < len_arry_buy_list_ev[i] - 1; j++) {
					arry_buy_list[i][j] = arry_buy_list[i][j + 1];
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
	void solution2(person * arry_person_input, int leng_s_arry_person_input,
			int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15);
	void solution3(person * arry_person_input, int leng_s_arry_person_input,
			int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15);
	void solution4(person * arry_person_input, int leng_s_arry_person_input,
			int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15);
	void solution5(person * arry_person_input, int leng_s_arry_person_input,
			int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15);
	void solution_test(person * arry_person_input, int leng_s_arry_person_input,
			int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15);
	void solution_test2(person * arry_person_input,
			int leng_s_arry_person_input, int f_m_b = 8, int f_d_b = 16,
			int f_m_e = 9, int f_d_e = 15);
	srand((unsigned) time(NULL));
	person * arry_person = NULL;
	int leng_s_arry_person = 0;

	//cout<<leng_s_arry_person<<endl;

	read_txt(arry_person, leng_s_arry_person);

	//solution4(arry_person, leng_s_arry_person,7,16,8,15);
	solution5(arry_person, leng_s_arry_person,8, 16, 9, 15);

	return 0;
}

double randoms(double b = 0.0, double e = 1.0) {

	return (e - b) * double(rand()) / double(RAND_MAX) + b;
}

void read_txt(person* &arry_person_inpute, int & leng_s_arry_person) {
	cout << "读文本中" << endl;
	FILE* fp;
	if ((fp = fopen("t_alibaba_data.csv", "r"))<0) { 
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

				person & p_buyim_local = arry_person[leng_s_arry_person - 2];
				int i_brand_id = 0;

				do {
					int buyday(0), buymonth(0), buybrand_id(0);

					for (;
							i_brand_id
									< arry_person[leng_s_arry_person - 2].leng_s_p_buyim;
							i_brand_id++) {
						if (p_buyim_local.p_buyimformation[i_brand_id].type
								== 1) {
							buybrand_id =
									p_buyim_local.p_buyimformation[i_brand_id].brand_id;
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
						if (p_buyim_local.p_buyimformation[i].brand_id
								== buybrand_id
								&& p_buyim_local.p_buyimformation[i].visit_datetime_month
										== buymonth
								&& p_buyim_local.p_buyimformation[i].visit_datetime_day
										== buyday
								&& (p_buyim_local.p_buyimformation[i].type == 0
										|| p_buyim_local.p_buyimformation[i].type
												== 3)) {

							//满足同天购买 查看加购物车条件，将该条删除

							for (int j = 0;
									j
											< arry_person[leng_s_arry_person - 2].leng_s_p_buyim;
									j++) {

								if (p_buyim_local.p_buyimformation[j].brand_id
										== buybrand_id
										&& p_buyim_local.p_buyimformation[j].visit_datetime_month
												== buymonth
										&& p_buyim_local.p_buyimformation[j].visit_datetime_day
												== buyday
										&& (p_buyim_local.p_buyimformation[j].type
												== 1)) {
									if (p_buyim_local.p_buyimformation[j].num_check_buy
											< 0) {
										p_buyim_local.p_buyimformation[j].num_check_buy =
												1;
									} else
										p_buyim_local.p_buyimformation[j].num_check_buy++;
								}

							}

							arry_person[leng_s_arry_person - 2].delete_one_p_buy(
									i);

							if (i < i_brand_id) {
								i_brand_id--;

							}

							i--;

						} else if (p_buyim_local.p_buyimformation[i].brand_id
								== buybrand_id
								&& p_buyim_local.p_buyimformation[i].visit_datetime_month
										== buymonth
								&& p_buyim_local.p_buyimformation[i].visit_datetime_day
										== buyday
								&& p_buyim_local.p_buyimformation[i].type == 1
								&& i != i_brand_id) {

							//一天中多次购买一样商品也会删除掉
							for (int j = 0;
									j
											< arry_person[leng_s_arry_person - 2].leng_s_p_buyim;
									j++) {

								if (p_buyim_local.p_buyimformation[j].brand_id
										== buybrand_id
										&& p_buyim_local.p_buyimformation[j].visit_datetime_month
												== buymonth
										&& p_buyim_local.p_buyimformation[j].visit_datetime_day
												== buyday
										&& (p_buyim_local.p_buyimformation[j].type
												== 1)) {
									if (p_buyim_local.p_buyimformation[j].num_buys
											< 0) {
										p_buyim_local.p_buyimformation[j].num_buys =
												1;
									} else
										p_buyim_local.p_buyimformation[j].num_buys++;
								}

							}
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
	cout << "读取完成" << endl;
}
int day_month(int month){

	switch (month) {
		case 4:
			return 30;
			break;
		case 5:
			return 31;
			break;
		case 6:
			return 30;
			break;
		case 7:
			return 31;
			break;
		case 8:
			return 31;
			break;
		case 9:
			return 30;
		default:
			cout<<"no month"<<endl;
			exit(0);
			break;
	}

	return 0;
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

	int num_day = 0;
	num_day += day_month(m_s) - d_s;
	for(int i=m_s+1;i<m_b;i++){
		num_day += day_month(i);
	}
	num_day+= d_b;
	return sign *num_day;

}

int day_gap(BuyImformation a1,BuyImformation a2){
	return day_gap(a1.visit_datetime_month,a1.visit_datetime_day,a2.visit_datetime_month,a2.visit_datetime_day);
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
					buybrand_id =
							p_buyim_local.p_buyimformation[i_brand_id].brand_id;
					buymonth =
							p_buyim_local.p_buyimformation[i_brand_id].visit_datetime_month;
					buyday =
							p_buyim_local.p_buyimformation[i_brand_id].visit_datetime_day;
					break;
				}
			}
			if (i_brand_id == arry_person[i_s].leng_s_p_buyim)
				break;
			for (int i = 0; i < arry_person[i_s].leng_s_p_buyim; i++) {
				int d_gap = day_gap(
						p_buyim_local.p_buyimformation[i].visit_datetime_month,
						p_buyim_local.p_buyimformation[i].visit_datetime_day,
						buymonth, buyday);
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
			if (randoms() > P_buy
					&& p_s_local.p_buyimformation[i_pbi].type != 1) {
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
			Buylist[leng_s_buylist] =
					p_s_local.p_buyimformation[i_pbi].brand_id;
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

void solution2(person * arry_person_input, int leng_s_arry_person_input,
		int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15) {

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
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
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	int *leng_a_b_l = new int[leng_s_arry_person];

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = 200;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);
		//{----------------------------------------------------------------------------------------
		//看一个人买一个东西后又买一次的可能性

		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {

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
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = buy[0].brand_id;
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
	for (int i = 0; i < num_kind_arry_pbhbt; i++) {
		arry_p_b_hbt[i] = new double[leng_s_arry_person];
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
						(p_bimf_l.p_buyimformation[i_b_b].brand_id
								== b_local.brand_id && i_b_b >= 0); i_b_b--) {
					if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

						i_b_b_l = i_b_b;
					}
				}
				i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

				//开始记录平均值
				int day_gap_value = day_gap(
						p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
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

				double num_check_rel = (double(i_b) - double(i_b_b_l))
						/ double(day_gap_value);
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
			if (p.p_buyimformation[i_brand_e].type == 1
					&& p.p_buyimformation[i_brand_b].brand_id
							== p.p_buyimformation[i_brand_e].brand_id) {
				ifbuy = true;
				buy_local = p.p_buyimformation[i_brand_e];
			}

			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {

				if (ifbuy) {
					//删除该物品购买后一个月内及购买前的全部信息
					int i = 0;
					int gap_local = i_brand_e - i_brand_b;
					for (; i < gap_local; i++) {
						if (day_gap(
								p.p_buyimformation[i + i_brand_b].visit_datetime_month,
								p.p_buyimformation[i + i_brand_b].visit_datetime_day,
								buy_local.visit_datetime_month,
								buy_local.visit_datetime_day) < 30) {
							arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							p.p_buyimformation =
									arry_person[i_a_p].p_buyimformation;
						} else {
							i++;
							break;
						}
					}
					i_brand_e -= i;

					ifbuy = false;
				}

				double num_check = double(i_brand_e - i_brand_b);
				for (int i = i_brand_b + 1; i < i_brand_e; i++) {
					//将品牌最早查看日期找到，并且将查看次数统计出来
					//由于按时间排序，第一个就是开始日期
					if (compare(p.p_buyimformation[i],
							p.p_buyimformation[i - 1]) == 0) {
						num_check--;
					}
				}
				//计算购买概率{---------------------------------------------------------------

				double d_x =
						double(
								day_gap(f_m_b, f_d_b,
										p.p_buyimformation[i_brand_b].visit_datetime_month,
										p.p_buyimformation[i_brand_b].visit_datetime_day));
				const double c = 0.159155;
				if (arry_p_b_hbt[1][i_a_p] * arry_p_b_hbt[4][i_a_p]
						> 0.0000001) {
					total++;
					//cout<<arry_p_b_hbt[i_a_p][1]<<"\t"<<arry_p_b_hbt[i_a_p][4]<<endl;

					double P =
							exp(
									-0.5
											* ((d_x - arry_p_b_hbt[0][i_a_p])
													* (d_x
															- arry_p_b_hbt[0][i_a_p])
													/ (arry_p_b_hbt[1][i_a_p]
															* arry_p_b_hbt[1][i_a_p])
													+ (num_check
															- arry_p_b_hbt[3][i_a_p])
															* (num_check
																	- arry_p_b_hbt[3][i_a_p])
															/ (arry_p_b_hbt[4][i_a_p]
																	* arry_p_b_hbt[4][i_a_p])));

					P /= (arry_p_b_hbt[1][i_a_p] * arry_p_b_hbt[4][i_a_p]);
					P *= c;

					if (false && randoms() < P) {
						acc++;
						//扩充
						if (leng_s_a_b_l[i_a_p] == leng_a_b_l[i_a_p]) {
							int * arry_b_l_copy = new int[leng_a_b_l[i_a_p]];
							for (int i = 0; i < leng_a_b_l[i_a_p]; i++) {
								arry_b_l_copy[i] = arry_buy_list[i_a_p][i];
							}
							delete[] arry_buy_list[i_a_p];
							arry_buy_list[i_a_p] = NULL;
							arry_buy_list[i_a_p] =
									new int[2 * leng_a_b_l[i_a_p]];
							for (int i = 0; i < leng_a_b_l[i_a_p]; i++) {
								arry_buy_list[i_a_p][i] = arry_b_l_copy[i];
							}
							leng_a_b_l[i_a_p] *= 2;
							delete[] arry_b_l_copy;
						}

						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								p.p_buyimformation[i_brand_b].brand_id;
						//cout<<p[i_brand_b].brand_id<<endl;
						leng_s_a_b_l[i_a_p]++;

					} else {
						//cout<<i_a_p<<"\t"<<P<<endl;
					}
				} else {
					//cout<<i_a_p<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"!"<<endl;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

		}
		if (leng_s_a_b_l[i_a_p] > 0) {
			fout << arry_person[i_a_p].get_person_id() << "\t";
			for (int i = 0; i < leng_s_a_b_l[i_a_p]; i++) {
				if (i < leng_s_a_b_l[i_a_p] - 1) {
					fout << arry_buy_list[i_a_p][i] << ",";
				} else {
					fout << arry_buy_list[i_a_p][i] << endl;
				}

			}
		}

	}
	cout << "接受率" << acc / total << endl;

	if (!(f_m_b == 8 && f_d_b == 16)) {
		//启动本地预判分系统
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 = 0;
		//现判断一下len_arry1的大小
		for (int i = 0; i < leng_s_arry_person; i++) {
			if (leng_s_a_b_l[i] > 0) {
				len_arry1++;
			}
		}
		arry1 = new int *[len_arry1];
		len_arry1_ev = new int[len_arry1];
		int i_arry_buy_list = 0;
		for (int i = 0; i < len_arry1; i++) {
			len_arry1_ev[i] = 0;
			for (; i_arry_buy_list < leng_s_arry_person; i_arry_buy_list++) {
				if (leng_s_a_b_l[i_arry_buy_list] > 0) {
					len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list] + 1;
					arry1[i] = new int[leng_s_a_b_l[i_arry_buy_list] + 1];
					arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

					for (int j = 0; j < leng_s_a_b_l[i_arry_buy_list]; j++) {
						arry1[i][j + 1] = arry_buy_list[i_arry_buy_list][j];
					}
					i_arry_buy_list++;
					break;
				}
			}
		}

		int ** arry2;
		int * len_arry2_ev = NULL;
		int len_arry2 = 0;

		chose_kind_from_day(f_m_b, f_d_b, f_m_e, f_d_e, arry_person_input,
				leng_s_arry_person_input, arry2, len_arry2, len_arry2_ev, 1);

		double * value_score = NULL;
		value_score = score(arry1, len_arry1_ev, len_arry1, arry2, len_arry2_ev,
				len_arry2);
		cout << value_score[0] * 100.0 << "%\t" << value_score[1] * 100.0
				<< "%\t" << value_score[2] * 100.0 << "%" << endl;

	}
}

void solution3(person * arry_person_input, int leng_s_arry_person_input,
		int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15) {

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
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
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	int *leng_a_b_l = new int[leng_s_arry_person];

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
			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {

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
											p.p_buyimformation[i].visit_datetime_day))
									<= 2)
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
					if (day_gap(f_m_b, f_d_b,
							buy[leng_s_buy - 1].visit_datetime_month,
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
	for (int i = 0; i < num_kind_arry_pbhbt; i++) {
		arry_p_b_hbt[i] = new double[leng_s_arry_person];
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
						(p_bimf_l.p_buyimformation[i_b_b].brand_id
								== b_local.brand_id && i_b_b >= 0); i_b_b--) {
					if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

						i_b_b_l = i_b_b;
					}
				}
				i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

				//开始记录平均值
				int day_gap_value = day_gap(
						p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
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

				double num_check_rel = (double(i_b) - double(i_b_b_l))
						/ double(day_gap_value);
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
			if (p.p_buyimformation[i_brand_e].type == 1
					&& p.p_buyimformation[i_brand_b].brand_id
							== p.p_buyimformation[i_brand_e].brand_id) {
				ifbuy = true;
				buy_local = p.p_buyimformation[i_brand_e];
			}

			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {

				if (ifbuy) {
					//删除该物品购买后一个月内及购买前的全部信息
					int i = 0;
					int gap_local = i_brand_e - i_brand_b;
					for (; i < gap_local; i++) {
						if (day_gap(
								p.p_buyimformation[i + i_brand_b].visit_datetime_month,
								p.p_buyimformation[i + i_brand_b].visit_datetime_day,
								buy_local.visit_datetime_month,
								buy_local.visit_datetime_day) < 30) {
							arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							p.p_buyimformation =
									arry_person[i_a_p].p_buyimformation;
						} else {
							i++;
							break;
						}
					}
					i_brand_e -= i;

					ifbuy = false;
				}

				double num_check = double(i_brand_e - i_brand_b);
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
				double d_x =
						double(
								day_gap(f_m_b, f_d_b,
										p.p_buyimformation[i_brand_b].visit_datetime_month,
										p.p_buyimformation[i_brand_b].visit_datetime_day));
				const double c = 0.159155;
				if (arry_p_b_hbt[1][i_a_p] * arry_p_b_hbt[4][i_a_p]
						> 0.0000001) {
					total++;
					//cout<<arry_p_b_hbt[i_a_p][1]<<"\t"<<arry_p_b_hbt[i_a_p][4]<<endl;

					double P =
							exp(
									-0.5
											* ((d_x - arry_p_b_hbt[0][i_a_p])
													* (d_x
															- arry_p_b_hbt[0][i_a_p])
													/ (arry_p_b_hbt[1][i_a_p]
															* arry_p_b_hbt[1][i_a_p])
													+ (num_check
															- arry_p_b_hbt[3][i_a_p])
															* (num_check
																	- arry_p_b_hbt[3][i_a_p])
															/ (arry_p_b_hbt[4][i_a_p]
																	* arry_p_b_hbt[4][i_a_p])));

					P /= (arry_p_b_hbt[1][i_a_p] * arry_p_b_hbt[4][i_a_p]);
					P *= c;

					//double P = arry_p_b_hbt[4][i_a_p]/arry_p_b_hbt[3][i_a_p];

					//if(P<1.2 && arry_p_b_hbt[3][i_a_p]*d_x < num_check*10./8.){
					if (randoms() < P) {
						acc++;
						//扩充
						if (leng_s_a_b_l[i_a_p] == leng_a_b_l[i_a_p]) {
							int * arry_b_l_copy = new int[leng_a_b_l[i_a_p]];
							for (int i = 0; i < leng_a_b_l[i_a_p]; i++) {
								arry_b_l_copy[i] = arry_buy_list[i_a_p][i];
							}
							delete[] arry_buy_list[i_a_p];
							arry_buy_list[i_a_p] = NULL;
							arry_buy_list[i_a_p] =
									new int[2 * leng_a_b_l[i_a_p]];
							for (int i = 0; i < leng_a_b_l[i_a_p]; i++) {
								arry_buy_list[i_a_p][i] = arry_b_l_copy[i];
							}
							leng_a_b_l[i_a_p] *= 2;
							delete[] arry_b_l_copy;
						}

						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								p.p_buyimformation[i_brand_b].brand_id;
						//cout<<p[i_brand_b].brand_id<<endl;
						leng_s_a_b_l[i_a_p]++;

					} else {
						//cout<<i_a_p<<"\t"<<P<<endl;
					}
				} else {
					//cout<<i_a_p<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"\t"<<arry_p_b_hbt[2][i_a_p]<<"!"<<endl;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

		}
		if (leng_s_a_b_l[i_a_p] > 0) {
			fout << arry_person[i_a_p].get_person_id() << "\t";
			for (int i = 0; i < leng_s_a_b_l[i_a_p]; i++) {
				if (i < leng_s_a_b_l[i_a_p] - 1) {
					fout << arry_buy_list[i_a_p][i] << ",";
				} else {
					fout << arry_buy_list[i_a_p][i] << endl;
				}

			}
		}

	}
	cout << "接受率" << acc / total << endl;

	if (!(f_m_b == 8 && f_d_b == 16)) {
		//启动本地预判分系统
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 = 0;
		//现判断一下len_arry1的大小
		for (int i = 0; i < leng_s_arry_person; i++) {
			if (leng_s_a_b_l[i] > 0) {
				len_arry1++;
			}
		}
		arry1 = new int *[len_arry1];
		len_arry1_ev = new int[len_arry1];
		int i_arry_buy_list = 0;
		for (int i = 0; i < len_arry1; i++) {
			len_arry1_ev[i] = 0;
			for (; i_arry_buy_list < leng_s_arry_person; i_arry_buy_list++) {
				if (leng_s_a_b_l[i_arry_buy_list] > 0) {
					len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list] + 1;
					arry1[i] = new int[leng_s_a_b_l[i_arry_buy_list] + 1];
					arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

					for (int j = 0; j < leng_s_a_b_l[i_arry_buy_list]; j++) {
						arry1[i][j + 1] = arry_buy_list[i_arry_buy_list][j];
					}
					i_arry_buy_list++;
					break;
				}
			}
		}

		int ** arry2;
		int * len_arry2_ev = NULL;
		int len_arry2 = 0;

		chose_kind_from_day(f_m_b, f_d_b, f_m_e, f_d_e, arry_person_input,
				leng_s_arry_person_input, arry2, len_arry2, len_arry2_ev, 1);

		double * value_score = NULL;
		value_score = score(arry1, len_arry1_ev, len_arry1, arry2, len_arry2_ev,
				len_arry2);
		cout << value_score[0] * 100.0 << "%\t" << value_score[1] * 100.0
				<< "%\t" << value_score[2] * 100.0 << "%" << endl;

	}
}

void solution4(person * arry_person_input, int leng_s_arry_person_input,
		int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15) {

	for(int i_a_p=0;i_a_p<leng_s_arry_person_input;i_a_p++){
		//先按id排个序
		person &p = arry_person_input[i_a_p];
		BubbleSort(p.p_buyimformation, arry_person_input[i_a_p].leng_s_p_buyim);
	}

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
	}

	double **Pbuy;
	int len_Pbuy = 0;
	findPbuy(arry_person, leng_s_arry_person, Pbuy, len_Pbuy);
	//cout<<Pbuy[2][52]<<"\t"<<Pbuy[1][52]<<endl;


	 double **Pbuy_again;
	 int len_Pbuy_again = 0;
	 findPbuyagain(arry_person,leng_s_arry_person,Pbuy_again,len_Pbuy_again);


	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	int *leng_a_b_l = new int[leng_s_arry_person];

	double ** P_buy_p = new double *[2];	//一个人的购买率：买过的品牌数/看过的品牌数
	for (int i = 0; i < 2; i++) {
		P_buy_p[i] = new double[leng_s_arry_person];
	}

	/*
	int * arry_pri_brand=NULL;
	int len_arry_pri_brand = 0;
	findprivilege(arry_person,leng_s_arry_person,Pbuy,len_Pbuy,f_m_b,f_d_b,arry_pri_brand,len_arry_pri_brand);
*/

	int ** arry2;
	int * len_arry2_ev = NULL;
	int len_arry2 = 0;

	chose_kind_from_day(f_m_b, f_d_b, f_m_e, f_d_e, arry_person_input,
			leng_s_arry_person_input, arry2, len_arry2, len_arry2_ev, 1);

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = 3000;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);

		//{----------------------------------------------------------------------------------------
		//看一个人买一个东西后又买一次的可能性

		double buy_num_total = 0;			//一共买过的次数
		double buy_num_again = 0;			//回头次数
		double check_num_total = 0;			//一共看过的商品数

		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {

				double T = 0;
				BuyImformation buy;
				int leng_s_buy = 0;
				int num_buy_f = 0;//分开的购买次数
				for (int i = i_brand_b; i < i_brand_e; i++) {
					if (p.p_buyimformation[i].type == 1) {

						if (leng_s_buy == 0) {

							buy = p.p_buyimformation[i];
							leng_s_buy+=2+p.p_buyimformation[i].num_buys;
							num_buy_f++;
						} else {


							buy = p.p_buyimformation[i];
							leng_s_buy+=2+p.p_buyimformation[i].num_buys;
							num_buy_f++;
							T += day_gap(
									buy.visit_datetime_month,
									buy.visit_datetime_day,
									p.p_buyimformation[i].visit_datetime_month,
									p.p_buyimformation[i].visit_datetime_day);
							//}
						}
					}
				}

				if (leng_s_buy >= 1)
					buy_num_total++;

				check_num_total++;

				if (num_buy_f > 1 || leng_s_buy >= 3 ) {//|| leng_s_buy > 3 num_buy_f > 1
					buy_num_again++;

					//在预测区间，添加到购买列表中
					//ev
//					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = buy.brand_id;
//					leng_s_a_b_l[i_a_p]++;

					//删除掉该品牌的全部信息
					for (int i = i_brand_b; i < i_brand_e; i++) {
						arry_person[i_a_p].delete_one_p_buy(i_brand_b);
					}
					i_brand_e = i_brand_b;

				}

				i_brand_b = i_brand_e;
				i_brand_e--;

			}
			P_buy_p[0][i_a_p] = buy_num_total / check_num_total;
			P_buy_p[1][i_a_p] = buy_num_total;
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
	for (int i = 0; i < num_kind_arry_pbhbt; i++) {
		arry_p_b_hbt[i] = new double[leng_s_arry_person];
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
						(i_b_b >= 0
								&& p_bimf_l.p_buyimformation[i_b_b].brand_id
										== b_local.brand_id); i_b_b--) {
					if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

						i_b_b_l = i_b_b;
					}
				}

				i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

				//开始记录平均值
				//day_gap_value1描述查看天数
				double day_gap_value1 = day_gap(
						p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
						p_bimf_l.p_buyimformation[i_b].visit_datetime_day,
						p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_month,
						p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_day);

				//int day_gap_value2描述有几天查看
				double day_gap_value2 = 0;
				for (int i_check_d = i_b_b_l + 1; i_check_d < i_b;
						i_check_d++) {
					if (compare(p_bimf_l.p_buyimformation[i_check_d],
							p_bimf_l.p_buyimformation[i_check_d - 1]) != 0) {
						day_gap_value2++;
					}
				}
				if (day_gap_value1 < 0)
					exit(0);
				if (day_gap_value1 == 0)
					continue;
				//cout<<day_gap_value<<endl;

				arry_p_b_hbt[0][i_a_p] += double(day_gap_value1);
				arry_p_b_hbt[1][i_a_p] += double(
						day_gap_value1 * day_gap_value1);
				arry_p_b_hbt[2][i_a_p] += 1.0;

				double num_check_rel =
						day_gap_value2 > 0 ?
								(double(i_b) - double(i_b_b_l))
										/ double(day_gap_value2) :
								0;
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
		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			/*
			 if(i_brand_e != 0 && (p_bimf_l.p_buyimformation[i_brand_e-1].brand_id == p_bimf_l.p_buyimformation[i_brand_e].brand_id) &&p_bimf_l.p_buyimformation[i_brand_e].type == 1){
			 i_brand_b = i_brand_e+1;
			 continue;
			 }
			 */
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
				if (num_check_rel != 0)
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

		if (arry_person[i_a_p].get_person_id() == 11340250) {

			for (int i = 0; i < arry_person[i_a_p].leng_s_p_buyim; i++) {
				cout << i << "\t" << p.p_buyimformation[i].brand_id << "\t"
						<< p.p_buyimformation[i].type<<"\t"<< p.p_buyimformation[i].visit_datetime_month
						<<"."<<p.p_buyimformation[i].visit_datetime_day<< endl;
			}
		}


		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {

			if (i_brand_b > 0
					&& p.p_buyimformation[i_brand_b - 1].brand_id
							== p.p_buyimformation[i_brand_b].brand_id
					&& p.p_buyimformation[i_brand_b].brand_id
							!= p.p_buyimformation[i_brand_e].brand_id) {
//对应之前有购买
				double daygap = day_gap(f_m_b, f_d_b,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_month,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_day);
				double daygap2 = day_gap(
						p.p_buyimformation[i_brand_e - 1].visit_datetime_month,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_day,
						p.p_buyimformation[i_brand_b - 1].visit_datetime_month,
						p.p_buyimformation[i_brand_b - 1].visit_datetime_day);
				/*
				 //如果一个人在几天内查看突破天际的话就说明他要购买

				 double num_over_check_av = 0;
				 for (int i_brand_e_d = i_brand_b, i_brand_b_d = i_brand_b;
				 i_brand_e_d < i_brand_e; i_brand_e_d++) {
				 double num_one_day = 0;
				 if (compare(p.p_buyimformation[i_brand_b_d],
				 p.p_buyimformation[i_brand_e_d]) != 0) {
				 double daygapvalue = day_gap(f_m_b,f_d_b,p.p_buyimformation[i_brand_b_d].visit_datetime_month,p.p_buyimformation[i_brand_b_d].visit_datetime_day);
				 double q = 1;
				 if(daygapvalue <5){
				 q = 2.0;
				 }else if(daygapvalue < 15){
				 q = 1.2;
				 }else if(daygapvalue <30 ){
				 q = 1.1;
				 }else if(daygapvalue < 50)
				 q = 0.9;
				 else
				 q = 0.8;
				 //单天查看次数
				 num_one_day = (i_brand_e_d - i_brand_b_d)*q;
				 if (arry_p_b_hbt[4][i_a_p] != 0
				 && arry_p_b_hbt[4][i_a_p] / arry_p_b_hbt[3][i_a_p]
				 < 5
				 && num_one_day / arry_p_b_hbt[3][i_a_p] > 0.2)
				 num_over_check_av++;
				 else if ((arry_p_b_hbt[4][i_a_p] == 0) && num_one_day >= 6)
				 num_over_check_av++;

				 i_brand_b_d = i_brand_e_d;
				 i_brand_e_d--;
				 }


				 }
				 */
				int i_Pbuy = 0;
				for (; i_Pbuy < len_Pbuy; i_Pbuy++) {
					if (p.p_buyimformation[i_brand_b].brand_id
							== Pbuy[0][i_Pbuy]) {
						break;
					}
				}



				if (
						Pbuy[2][i_Pbuy] >= 10
						&& Pbuy[1][i_Pbuy] >= 0.07
						&&
						daygap < 30) {

					//ev
//					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
//							p.p_buyimformation[i_brand_b].brand_id;
//					leng_s_a_b_l[i_a_p]++;

				}



				i_brand_b = i_brand_e;
				i_brand_e--;
			}
//对应商品只有查看
			else if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {
				//如果一个人在几天内查看突破天际的话就说明他要购买

				double daygap = day_gap(f_m_b, f_d_b,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_month,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_day);

				double num_over_check_av = 0;


				double **arry_check = new double *[2];//第一位存储距fday的距离，第二位储存当天的查看次数,
				int  num_check_day = 0;//为储存位数，同时代表了一个人一共查看了多少天
				arry_check[0] = new double [i_brand_e-i_brand_b];
				arry_check[1] = new double [i_brand_e-i_brand_b];
				for (int i_brand_e_d = i_brand_b, i_brand_b_d = i_brand_b;
						i_brand_e_d < i_brand_e; i_brand_e_d++) {
					double num_one_day = 0;
					if (compare(p.p_buyimformation[i_brand_b_d],

							p.p_buyimformation[i_brand_e_d]) != 0  || i_brand_e_d == i_brand_e-1) {

						if (i_brand_e_d != i_brand_e - 1) {
							double daygapvalue =
									day_gap(f_m_b, f_d_b,
											p.p_buyimformation[i_brand_b_d].visit_datetime_month,
											p.p_buyimformation[i_brand_b_d].visit_datetime_day);
							arry_check[0][num_check_day] = daygapvalue;
							arry_check[1][num_check_day] = i_brand_e_d-i_brand_b_d;
							num_check_day++;

						}else{
							//
							if(compare(p.p_buyimformation[i_brand_b_d],
														p.p_buyimformation[i_brand_e_d]) == 0){
								//最后一个与之前为同一天
								double daygapvalue =
										day_gap(f_m_b, f_d_b,
												p.p_buyimformation[i_brand_b_d].visit_datetime_month,
												p.p_buyimformation[i_brand_b_d].visit_datetime_day);
								arry_check[0][num_check_day] = daygapvalue;
								arry_check[1][num_check_day] = i_brand_e_d-i_brand_b_d+1;
								num_check_day++;
							}else{
								double daygapvalue =
										day_gap(f_m_b, f_d_b,
												p.p_buyimformation[i_brand_b_d].visit_datetime_month,
												p.p_buyimformation[i_brand_b_d].visit_datetime_day);
								arry_check[0][num_check_day] = daygapvalue;
								arry_check[1][num_check_day] = i_brand_e_d-i_brand_b_d;
								num_check_day++;


								daygapvalue =
										day_gap(f_m_b, f_d_b,
												p.p_buyimformation[i_brand_e_d].visit_datetime_month,
												p.p_buyimformation[i_brand_e_d].visit_datetime_day);
								arry_check[0][num_check_day] = daygapvalue;
								arry_check[1][num_check_day] = 1;
								num_check_day++;
							}
						}



						i_brand_b_d = i_brand_e_d;
						if(i_brand_e_d != i_brand_e-1)
						i_brand_e_d--;
					}


				}

				//将一个人对商品的查看信息存在数组中，
				//处理办法：1.以最后一天作为时间原点，离最后一天时间越远的查看系数越小
				//		2.还要乘以查看间隔时间天数作为权重，比如一个人隔5天后有查看一件商品，说明很有可能购买
				//		3.最后乘以最后一天距预测时间的权重，比如一个商品最后在4月看的，7月很有可能不买


				//计算购买阈值估计{--------------------------------------------------------------

//				master{------------------------------------------------------------
//				int num_gap = 0;
//				double daybig = int(double(num_check_day) / 5.) + 1;
//				num_over_check_av += pow(arry_check[1][0], 2.5)
//						/ pow(1. * (num_check_day / daybig), 1.2)
//						/ (exp((arry_check[0][0])-5)+10.);
//
//				for (int i = 1; i < num_check_day; i++) {
//					if((arry_check[0][0] - arry_check[0][i])> 10){
//						num_gap++;
//					}
//
//					num_over_check_av +=
//							pow(
//							(arry_check[0][0] - arry_check[0][i]), 3)
//
//							* pow(arry_check[1][i], 2.5)
//							/ pow(1. * ((num_check_day - i) / daybig), 1.2)
//							/ (exp((arry_check[0][i])-5)+10.);
//
//
//				}
//
//				num_over_check_av = num_over_check_av
//						* pow(num_check_day, 3)
//						/ pow(
//								arry_check[0][0]
//										- arry_check[0][num_check_day - 1] + 1,
//								3)/pow(num_check_day,1)
//
//
//						;
//
//				num_over_check_av = num_over_check_av / 1.;
				//master}===========================================================================
				//master}===========================================================================
				//branch1{------------------------------------------------------------------------
				if(num_check_day >4){
				int num_gap = 0;
				double daybig = int(double(num_check_day) / 5.) + 1;
				double moveday = 5.;
				double cosdaygap = day_gap(f_m_b,f_d_e,4,15)+moveday;

				num_over_check_av += pow(arry_check[1][0], 2.5)

						*pow(cos((arry_check[0][0]+moveday)/cosdaygap*3.14159/2.),7);

				for (int i = 1; i < num_check_day; i++) {
					if((arry_check[0][0] - arry_check[0][i])> 10){
						num_gap++;
					}

					num_over_check_av +=
							pow(
							(arry_check[0][0] - arry_check[0][i]), 3)

							* pow(arry_check[1][i], 2.5)

							*pow(cos((arry_check[0][i]+moveday)/cosdaygap*3.14159/2.),7);


				}

				num_over_check_av = num_over_check_av
						//* pow(num_check_day, 1)
						/ pow(
								arry_check[0][0]
										- arry_check[0][num_check_day - 1] + 1,
								3)


						;

				num_over_check_av = num_over_check_av / pow(2.,3)*2.;
				//num_over_check_av = 0;
				}else


				if(num_check_day >3){

					int num_gap = 0;
				double daybig = int(double(num_check_day) / 5.) + 1;
				double moveday = 5.;
				double cosdaygap = day_gap(f_m_b,f_d_e,4,15)+moveday;

				num_over_check_av += pow(arry_check[1][0], 2.5)
						/ pow(1. * (num_check_day / daybig), 2.)
						*pow(cos((arry_check[0][0]+moveday)/cosdaygap*3.14159/2.),7);

				for (int i = 1; i < num_check_day; i++) {
					if((arry_check[0][0] - arry_check[0][i])> 10){
						num_gap++;
					}

					num_over_check_av +=
							pow(
							(arry_check[0][0] - arry_check[0][i]), 3)

							* pow(arry_check[1][i], 2.5)
							/ pow(1. * ((num_check_day - i) / daybig), 2.)
							*pow(cos((arry_check[0][i]+moveday)/cosdaygap*3.14159/2.),7);


				}

				num_over_check_av = num_over_check_av
						//* pow(num_check_day, 1)
						/ pow(
								arry_check[0][0]
										- arry_check[0][num_check_day - 1] + 1,
								3)


						;

				num_over_check_av = num_over_check_av / pow(2.,3)*2.;
				//num_over_check_av = 0;
				}else{
					int num_gap = 0;
					double daybig = int(double(num_check_day) / 5.) + 1;
					double moveday = 5.;
					double cosdaygap = day_gap(f_m_b,f_d_e,4,15)+moveday;

					num_over_check_av += pow(arry_check[1][0], 2.5)
							/ pow(1. * (num_check_day / daybig), 1.2)
							*pow(cos((arry_check[0][0]+moveday)/cosdaygap*3.14159/2.),7);

					for (int i = 1; i < num_check_day; i++) {
						if((arry_check[0][0] - arry_check[0][i])> 10){
							num_gap++;
						}

						num_over_check_av +=
								pow(
								(arry_check[0][0] - arry_check[0][i]), 3)

								* pow(arry_check[1][i], 2.5)
								/ pow(1. * ((num_check_day - i) / daybig), 1.2)
								*pow(cos((arry_check[0][i]+moveday)/cosdaygap*3.14159/2.),7);


					}

					num_over_check_av = num_over_check_av
							* pow(num_check_day, 3)
							/ pow(
									arry_check[0][0]
											- arry_check[0][num_check_day - 1] + 1,
									3)/pow(num_check_day,1)


							;

					num_over_check_av = num_over_check_av / pow(2.,6)*1.1;
				}
				//branch1}=======================================================================


				//一个人如果一个月都没买，认为其以后也不会买
				/*
				if(arry_check[0][0]-arry_check[0][num_check_day-1]< 30 && arry_check[0][0]-arry_check[0][num_check_day-1] > 15)
					num_over_check_av = 0;
				*/
				/*
				if (num_over_check_av >= 10000){
					cout<<num_over_check_av<<endl;
				}
				*/
				//估计}======================================================================

				//branch{-----------------------------------------------------------------------
				if (num_over_check_av >= 1 ) {

					//ev
//					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
//							p.p_buyimformation[i_brand_b].brand_id;
//					leng_s_a_b_l[i_a_p]++;

				}
				//branch}=============================================================
				//master{--------------------------------------------------------------------------
				/*
				if (num_over_check_av >= 1 && (i_brand_e - i_brand_b) >= 2
						&& daygap < 6) {
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				} else if (num_over_check_av >= 1
						&& (i_brand_e - i_brand_b) >= 3 && daygap < 10) {//&& day_gap(f_m_b,f_m_e,p.p_buyimformation[i_brand_e-1].visit_datetime_month,p.p_buyimformation[i_brand_e-1].visit_datetime_day) < 50){
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				} else if (num_over_check_av >= 1
						&& (i_brand_e - i_brand_b) >= 8 && daygap < 50) {
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				} else if (num_over_check_av >= 1
						&& (i_brand_e - i_brand_b) >= 15 && daygap < 75) {
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				} else if (num_over_check_av >= 1
						&& (i_brand_e - i_brand_b) >= 25 && daygap < 90) {
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				}
				*/
				//master}=========================================================================
				//一个人查看一直很平均，可能也会购买{--------------------------------------------------


				//}========================================================================
				i_brand_b = i_brand_e;
				i_brand_e--;
			}

			//从购买的下一个开始
			if (p.p_buyimformation[i_brand_e].type == 1) {
				//有查看但是以购买作为分割
				//最后一次对商品的行为是购买
				if (i_brand_e == arry_person[i_a_p].leng_s_p_buyim - 1
						|| p.p_buyimformation[i_brand_e].brand_id
								!= p.p_buyimformation[i_brand_e + 1].brand_id) {

					double daygap = day_gap(f_m_b, f_d_b,
							p.p_buyimformation[i_brand_e].visit_datetime_month,
							p.p_buyimformation[i_brand_e].visit_datetime_day);

					int i_Pbuy = 0;
					for (; i_Pbuy < len_Pbuy; i_Pbuy++) {
						if (p.p_buyimformation[i_brand_e].brand_id
								== Pbuy[0][i_Pbuy]) {
							break;
						}
					}



					int i_Pbuy_ag = 0;
					for (; i_Pbuy_ag < len_Pbuy_again; i_Pbuy_ag++) {
						if (p.p_buyimformation[i_brand_e].brand_id
								== Pbuy_again[0][i_Pbuy_ag]) {
							break;
						}
					}



					bool ifjoinbuylist = false;
					if (Pbuy[2][i_Pbuy] >= 0 && Pbuy[2][i_Pbuy] < 10) {
						ifjoinbuylist = true;
					}

					if (Pbuy_again[2][i_Pbuy_ag] >= 5 ) {
						//最后一次的行为是购买{----------------------------------------------------------

						try {
							if (p.p_buyimformation[i_brand_e - 1].brand_id
									!= p.p_buyimformation[i_brand_e].brand_id ) {

								//branch buyagain{---------------------------------------

								//ev
//								arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
//										p.p_buyimformation[i_brand_e].brand_id;
//								leng_s_a_b_l[i_a_p]++;
								//}====================================================
								/*master
								 if (!(Pbuy[2][i_Pbuy] >= 0
								 && Pbuy[2][i_Pbuy] < 10)) {

								 arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								 p.p_buyimformation[i_brand_e].brand_id;
								 leng_s_a_b_l[i_a_p]++;
								 } else if (Pbuy[2][i_Pbuy] >= 0
								 && Pbuy[2][i_Pbuy] < 10
								 && Pbuy[1][i_Pbuy] > 0.4) {
								 arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								 p.p_buyimformation[i_brand_e].brand_id;
								 leng_s_a_b_l[i_a_p]++;
								 }
								 */
							}
						} catch (...) {

						}

						//最后一次的行为是购买}==========================================================

						//只有一次行为是购买{--------------------------------------------------------------

						try {
							if (p.p_buyimformation[i_brand_e - 1].brand_id
									== p.p_buyimformation[i_brand_e].brand_id ) {
//ev
//								arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
//										p.p_buyimformation[i_brand_e].brand_id;
//								leng_s_a_b_l[i_a_p]++;

							}
						} catch (...) {
//ev
//							arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
//									p.p_buyimformation[i_brand_e].brand_id;
//							leng_s_a_b_l[i_a_p]++;

						}
					}
					//只有最后一次行为是购买}===============================================================

				}

				i_brand_b = i_brand_e + 1;
				continue;
			}
		}

		findthreshold2(arry_person[i_a_p],f_m_b,f_d_b,f_m_e,f_d_e,arry_buy_list[i_a_p],leng_s_a_b_l[i_a_p]);
	}

	//查看次数}=====================================================================================








	//刨除掉购买率极低的商品{---------------------------------------------------------------------

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {
		for (int i_a_p_b = 0; i_a_p_b < leng_s_a_b_l[i_a_p]; i_a_p_b++) {

			for (int j = 0; j < len_Pbuy; j++) {
				//master{---------------------------------------------------------

				if (arry_buy_list[i_a_p][i_a_p_b] == Pbuy[0][j]) {

					if (Pbuy[2][j] >= 60 && Pbuy[1][j] < 0.03) {
						arry_buy_list[i_a_p][i_a_p_b] = 0;
						break;
					} else if (Pbuy[2][j] >= 50 && Pbuy[2][j] < 60
							&& Pbuy[1][j] < 0.07) {
						arry_buy_list[i_a_p][i_a_p_b] = 0;
						break;
					} else if (Pbuy[2][j] >= 40 && Pbuy[2][j] < 50
							&& Pbuy[1][j] < 0.1) {
						arry_buy_list[i_a_p][i_a_p_b] = 0;
						break;
					}
				}

				//master}==========================================================
				//branch{------------------------------------------------------------

				if (arry_buy_list[i_a_p][i_a_p_b] == Pbuy[0][j]) {


//					if (Pbuy[2][j] >= 100 && Pbuy[1][j]*Pbuy[2][j] <= 10) {
//						arry_buy_list[i_a_p][i_a_p_b] = 0;
//						break;
//					}

//					else if (Pbuy[2][j] >= 50 && Pbuy[2][j] < 60
//							&& Pbuy[1][j]*Pbuy[2][j] <= 4) {
//						arry_buy_list[i_a_p][i_a_p_b] = 0;
//						break;
//					} else if (Pbuy[2][j] >= 40 && Pbuy[2][j] < 50
//							&& Pbuy[1][j]*Pbuy[2][j] <= 2) {
//						arry_buy_list[i_a_p][i_a_p_b] = 0;
//						break;
//					}

				}

				//branch}============================================================
			}
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!21110

//			if(
//					arry_buy_list[i_a_p][i_a_p_b] == 21146
//				||	arry_buy_list[i_a_p][i_a_p_b] == 	21110
//				||	arry_buy_list[i_a_p][i_a_p_b] == 25687
//				||	arry_buy_list[i_a_p][i_a_p_b] == 5470
//				||	arry_buy_list[i_a_p][i_a_p_b] == 17214

//				){
//				arry_buy_list[i_a_p][i_a_p_b] = 0;
//			}

			/*
			 * 21146只会在7.8号购买
			 * 21110基本不会购买
			 * 1180在8月查看才会购买
			 * 25687不会购买
			 * 6571000,5470会买两次，其他人不买
			 * 17214在8-9月不会买8216500，8845250，5034000
			 *
			 */
		}

	}

	//刨除end}===============================================================================



	ofstream fout_bl("buylist.txt");

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		person &p = arry_person[i_a_p];

		//检查一下i_a_p用户在第四个月是否买东西了
		int i_find_u = 0;
		for (; i_find_u < len_arry2; i_find_u++) {
			if (arry_person[i_a_p].get_person_id() == arry2[i_find_u][0])
				break;
		}

		//如果买了
		if (i_find_u != len_arry2) {
			//cout<<"周期:"<<arry_p_b_hbt[0][i_a_p]<<"\t标准差"<<arry_p_b_hbt[1][i_a_p]<<"\t平均次数:"<<arry_p_b_hbt[3][i_a_p]<<"\t标准差:"<<arry_p_b_hbt[4][i_a_p]<<endl;
			for (int i_brand_e = 0, i_brand_b = 0;
					i_brand_e < arry_person[i_a_p].leng_s_p_buyim;
					i_brand_e++) {
				//cout<<p.p_buyimformation[i_brand_b].brand_id<<"\t"<<p.p_buyimformation[i_brand_e].brand_id<<endl;
				if (p.p_buyimformation[i_brand_b].brand_id
						!= p.p_buyimformation[i_brand_e].brand_id) {
					bool ifbuy = false;
					bool ifinbuylist = false;
					for (int j = 1; j < len_arry2_ev[i_find_u]; j++) {
						if (p.p_buyimformation[i_brand_b].brand_id
								== arry2[i_find_u][j]) {
							ifbuy = true;
							break;
						}
					}
					for (int j = 0; j < leng_s_a_b_l[i_a_p]; j++) {
						if (p.p_buyimformation[i_brand_b].brand_id
								== arry_buy_list[i_a_p][j]) {
							ifinbuylist = true;
							break;
						}
					}

					if (!ifbuy || ifinbuylist) {
						i_brand_b = i_brand_e;
						i_brand_e--;
						continue;
					}

					for (int i = i_brand_b; i < i_brand_e; i++) {
						fout_bl << i_a_p << "\t"
								<< arry_person[i_a_p].get_person_id() << "\t"
								<< p.p_buyimformation[i].brand_id << "\t"
								<< p.p_buyimformation[i].type << "\t"
								<< p.p_buyimformation[i].visit_datetime_month
								<< "."
								<< p.p_buyimformation[i].visit_datetime_day
								<< "\t" << p.p_buyimformation[i].num_check_buy
								<< "\t" << p.p_buyimformation[i].num_buys
								<< endl;
					}
					double d_g =
							day_gap(
									p.p_buyimformation[i_brand_e - 1].visit_datetime_month,
									p.p_buyimformation[i_brand_e - 1].visit_datetime_day,
									p.p_buyimformation[i_brand_b].visit_datetime_month,
									p.p_buyimformation[i_brand_b].visit_datetime_day);
					double day_gap_value2 = 0;
					for (int i_check_d = i_brand_b + 1; i_check_d < i_brand_e;
							i_check_d++) {
						if (compare(p.p_buyimformation[i_check_d],
								p.p_buyimformation[i_check_d - 1]) != 0) {
							day_gap_value2++;
						}
					}

					fout_bl << "-------------------------------------------------"
							<< endl;

					i_brand_b = i_brand_e;
					i_brand_e--;
					fout_bl
							<< "======================================================================="
							<< endl;
					//system("pause");
				}

			}
		}

	}


fout_bl.close();




	cout<<"\n\n没有购买\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;

	ofstream fout_nob("nobuy.txt");

	//输出没有购买，但是我提交的商品
	for(int i_a_p=0;i_a_p<leng_s_arry_person;i_a_p++){
		person &p = arry_person[i_a_p];
		//判断一下，用户i_a_p是否购买东西了
		int i_find = 0;
		for(int i_find=0;i_find<len_arry2;i_find++){
			if(arry_person[i_a_p].get_person_id() == arry2[i_find][0])
				break;
		}
		if (i_find == len_arry2) {

			for (int i_list = 0; i_list < leng_s_a_b_l[i_a_p]; i_list++) {
				for (int i_b = 0; i_b < arry_person[i_a_p].leng_s_p_buyim;
						i_b++) {
					if(arry_buy_list[i_a_p][i_list] ==  arry_person[i_a_p].p_buyimformation[i_b].brand_id){
						fout_nob << i_a_p << "\t"
								<< arry_person[i_a_p].get_person_id() << "\t"
								<< p.p_buyimformation[i_b].brand_id << "\t"
								<< p.p_buyimformation[i_b].type << "\t"
								<< p.p_buyimformation[i_b].visit_datetime_month
								<< "."
								<< p.p_buyimformation[i_b].visit_datetime_day
								<< "\t" << p.p_buyimformation[i_b].num_check_buy
								<< "\t" << p.p_buyimformation[i_b].num_buys
								<< endl;
					}

				}
			}
			fout_nob<<"-------------------------------------------------------------------------"<<endl;
		}
		else{
			for (int i_list = 0; i_list <  leng_s_a_b_l[i_a_p]; i_list++) {
				for (int i_b = 0; i_b < arry_person[i_a_p].leng_s_p_buyim;
						i_b++) {
					if(arry_buy_list[i_a_p][i_list] ==  arry_person[i_a_p].p_buyimformation[i_b].brand_id){
						bool ifbuybrand = false;
						for(int i=1;i<len_arry2_ev[i_find];i++){
							if(arry_buy_list[i_a_p][i_list] == arry2[i_find][i]){
								ifbuybrand = true;
							break;
							}
						}
						if(!ifbuybrand){
							fout_nob << i_a_p << "\t"
								<< arry_person[i_a_p].get_person_id() << "\t"
								<< p.p_buyimformation[i_b].brand_id << "\t"
								<< p.p_buyimformation[i_b].type << "\t"
								<< p.p_buyimformation[i_b].visit_datetime_month
								<< "."
								<< p.p_buyimformation[i_b].visit_datetime_day
								<< "\t" << p.p_buyimformation[i_b].num_check_buy
								<< "\t" << p.p_buyimformation[i_b].num_buys
								<< endl;
						}
					}

				}
			}
			fout_nob<<"-------------------------------------------------------------------------"<<endl;
		}


		fout_nob<<"================================================================================"<<endl;
	}
	fout_nob.close();
	//刨除掉购买率极低的人{------------------------------------------------------------------------
	/*
	 for(int i_a_p=0;i_a_p<leng_s_arry_person;i_a_p++){
	 if(P_buy_p[1][i_a_p] > 5 && P_buy_p[0][i_a_p]<= 0.07){
	 //先试试全部删除掉
	 for(int i=0;i<leng_s_a_b_l[i_a_p];i++){
	 if(randoms() >= 2.*P_buy_p[0][i_a_p])
	 arry_buy_list[i_a_p][i] = 0;
	 }
	 }
	 }
	 */
	//刨除end}===============================================================================

	int ** arry1;
	int * len_arry1_ev = NULL;
	int len_arry1 = 0;

	//将buy_list转换为arry1形式储存{-----------------------------------------------------------------------
	//现判断一下len_arry1的大小

	for (int i = 0; i < leng_s_arry_person; i++) {
		if (leng_s_a_b_l[i] > 0) {
			len_arry1++;
		}
	}

	arry1 = new int *[900];
	len_arry1_ev = new int[900];
	int i_arry_buy_list = 0;
	for(int i=0;i<900;i++){


		arry1[i] = new int[300];
	}


	for (int i = 0; i < len_arry1; i++) {

		for (; i_arry_buy_list < leng_s_arry_person; i_arry_buy_list++) {

			if (leng_s_a_b_l[i_arry_buy_list] > 0) {
				len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list] + 1;
				arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

				for (int j = 0; j < leng_s_a_b_l[i_arry_buy_list]; j++) {
					arry1[i][j + 1] = arry_buy_list[i_arry_buy_list][j];
				}
				i_arry_buy_list++;
				break;
			}
		}
	}


	//转化end}=========================================================================================

	check_arry_re(arry1, len_arry1, len_arry1_ev);


	double * value_score = NULL;
	value_score = score(arry1, len_arry1_ev, len_arry1, arry2, len_arry2_ev,
			len_arry2);
	cout << value_score[0] * 100.0 << "%\t" << value_score[1] * 100.0 << "%\t"
			<< value_score[2] * 100.0 << "%" << endl;


	//findbuyinBuylist(arry_person,leng_s_arry_person,arry1,len_arry1_ev,len_arry1,Pbuy,len_Pbuy,f_m_b,f_d_b);



	value_score = NULL;
	value_score = score(arry1, len_arry1_ev, len_arry1, arry2, len_arry2_ev,
			len_arry2);
	cout << value_score[0] * 100.0 << "%\t" << value_score[1] * 100.0 << "%\t"
			<< value_score[2] * 100.0 << "%" << endl;


	ofstream fout("solution4.txt");
	for (int i = 0; i < len_arry1; i++) {
		fout << arry1[i][0] << "\t";
		for (int j = 1; j < len_arry1_ev[i] - 1; j++) {
			fout << arry1[i][j] << ",";
		}
		fout << arry1[i][len_arry1_ev[i] - 1] << endl;
	}

}

void solution5(person * arry_person_input, int leng_s_arry_person_input,
		int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15) {



	for(int i_a_p=0;i_a_p<leng_s_arry_person_input;i_a_p++){
		//先按id排个序
		person &p = arry_person_input[i_a_p];
		BubbleSort(p.p_buyimformation, arry_person_input[i_a_p].leng_s_p_buyim);
	}

	/*
	ofstream fout_da("alibaba_data_bubble.txt");
	for(int i_a_p=0;i_a_p<leng_s_arry_person_input;i_a_p++){
		for(int i_a_b=0;i_a_b<arry_person_input[i_a_p].leng_s_p_buyim;i_a_b++){
			BuyImformation &p = arry_person_input[i_a_p].p_buyimformation[i_a_b];
			fout_da<<arry_person_input[i_a_p].get_person_id()<<","<<p.brand_id<<","<<p.type<<","
					<<p.visit_datetime_month<<"月"<<p.visit_datetime_day<<"日"<<endl;
		}
	}
	*/


	//将arry_personcopy一个副本
	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
	}


	//将所有brand名查找出来{---------------------------------------------------------------
	int *arry_brand_id=NULL;
	int len_arry_brand_id(0);
	brandchecked(arry_person,leng_s_arry_person,arry_brand_id,len_arry_brand_id);
	//查找完成}==========================================================================

	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	for(int i_a_p =0;i_a_p<leng_s_arry_person;i_a_p++){
		arry_buy_list[i_a_p] = new int [3000];
		leng_s_a_b_l[i_a_p] = 0;
	}
	//将每一个商品的购买周期和习惯统计出来{-----------------------------------------------------
	/*for (int i_a_b_id = 0; i_a_b_id < len_arry_brand_id; i_a_b_id++) {
		int num_day_total = day_gap(f_m_b, f_d_b, 4, 15)+1;
		int *brand_im=new int [num_day_total];
		int brand_id_pr = arry_brand_id[i_a_b_id];
		brandcheckout(arry_person, leng_s_arry_person, brand_id_pr, brand_im,
				num_day_total);

		delete [] brand_im;
	}*/
	//------------------------------------------------------
//
//	int i_a_b_id = 0;
//	int num_day_total = day_gap(f_m_b, f_d_b, 4, 15) + 1;
//	int *brand_im = new int[num_day_total];
//	int brand_id_pr = arry_brand_id[i_a_b_id];
//
//	brand_id_pr = 7552;
//	cout << brand_id_pr << endl;
//	brandcheckout(arry_person, leng_s_arry_person, brand_id_pr, brand_im,
//		num_day_total);
//
//	findpeak(brand_im, num_day_total);
//
//	delete[] brand_im;

	//------------------------------------------------------


	for(int i_a_p = 0;i_a_p<leng_s_arry_person;i_a_p++){

		cout<<arry_person[i_a_p].get_person_id()<<endl;
			findthreshold(arry_person[i_a_p],7,16,8,15);

	}



	for(int i_a_p =0;i_a_p<leng_s_arry_person;i_a_p++){
		delete []arry_buy_list[i_a_p];
	}
	delete []arry_buy_list;
	delete []leng_s_a_b_l;

}

void solution_test(person * arry_person_input, int leng_s_arry_person_input,
		int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15) {

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
	}
	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	int *leng_a_b_l = new int[leng_s_arry_person];

	int ** arry2;
	int * len_arry2_ev = NULL;
	int len_arry2 = 0;

	chose_kind_from_day(f_m_b, f_d_b, f_m_e, f_d_e, arry_person_input,
			leng_s_arry_person_input, arry2, len_arry2, len_arry2_ev, 1);

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = 3000;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);

		//{----------------------------------------------------------------------------------------
		//看一个人买一个东西后又买一次的可能性

		double buy_num_total = 0;						//一共买过的次数
		double buy_num_again = 0;						//回头次数
		double check_num_total = 0;						//一共看过的商品数

		bool buytotay = false;

		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {

				double T = 0;
				BuyImformation buy[100];
				int leng_s_buy = 0;
				for (int i = i_brand_b; i < i_brand_e; i++) {
					if (p.p_buyimformation[i].type == 1) {
						for (int i_c = i_brand_b; i_c < i; i_c++) {
							if (compare(p.p_buyimformation[i_c],
									p.p_buyimformation[i]) < 0
									&& p.p_buyimformation[i_c].type == 0) {
								buytotay |= true;
							}
						}

						if (leng_s_buy == 0) {
							buy[leng_s_buy] = p.p_buyimformation[i];
							leng_s_buy++;
						} else {

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

				if (leng_s_buy >= 1)
					buy_num_total++;

				check_num_total++;

				if (leng_s_buy > 1) {
					buy_num_again++;

					//在预测区间，添加到购买列表中

					//arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] = buy[0].brand_id;
					//leng_s_a_b_l[i_a_p]++;

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

	}

	//直接检查查看次数{---------------------------------------------------------------------------------

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		person &p = arry_person[i_a_p];
		if (arry_person[i_a_p].get_person_id() == 4475750) {
			for (int i = 0; i < arry_person[i_a_p].leng_s_p_buyim; i++) {
				cout << i << "\t" << p.p_buyimformation[i].brand_id << "\t"
						<< p.p_buyimformation[i].type << endl;
			}
		}
		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {

			if ((i_brand_b > 0
					&& p.p_buyimformation[i_brand_b - 1].brand_id
							== p.p_buyimformation[i_brand_b].brand_id
					&& p.p_buyimformation[i_brand_b].brand_id
							!= p.p_buyimformation[i_brand_e].brand_id)) {

				if (false) {
					//cout<<Pbuy[2][i_Pbuy]<<"\t"<<Pbuy[1][i_Pbuy]<<endl;
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

			else if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {
				//如果一个人在几天内查看突破天际的话就说明他要购买

				double daygap = day_gap(f_m_b, f_d_b,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_month,
						p.p_buyimformation[i_brand_e - 1].visit_datetime_day);

				double num_over_check_av = 0;

				for (int i_brand_e_d = i_brand_b, i_brand_b_d = i_brand_b;
						i_brand_e_d < i_brand_e; i_brand_e_d++) {
					double num_one_day = 0;
					if (compare(p.p_buyimformation[i_brand_b_d],
							p.p_buyimformation[i_brand_e_d]) != 0) {

						double daygapvalue =
								day_gap(f_m_b, f_d_b,
										p.p_buyimformation[i_brand_b_d].visit_datetime_month,
										p.p_buyimformation[i_brand_b_d].visit_datetime_day);

						i_brand_b_d = i_brand_e_d;
						i_brand_e_d--;
					}

				}

				if (false) {
					arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
							p.p_buyimformation[i_brand_b].brand_id;
					leng_s_a_b_l[i_a_p]++;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

			//从购买的下一个开始
			if (p.p_buyimformation[i_brand_e].type == 1) {
				//有查看但是以购买作为分割
				//最后一次对商品的行为是购买
				if (i_brand_e == arry_person[i_a_p].leng_s_p_buyim - 1
						|| p.p_buyimformation[i_brand_e].brand_id
								!= p.p_buyimformation[i_brand_e + 1].brand_id) {

					try {
						if (p.p_buyimformation[i_brand_e - 1].brand_id
								== p.p_buyimformation[i_brand_e].brand_id) {
							arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
									p.p_buyimformation[i_brand_e].brand_id;
							leng_s_a_b_l[i_a_p]++;
						}
					} catch (...) {

						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								p.p_buyimformation[i_brand_e].brand_id;
						leng_s_a_b_l[i_a_p]++;

					}

				}

				i_brand_b = i_brand_e + 1;
				continue;
			}
		}

	}

	int ** arry1;
	int * len_arry1_ev = NULL;
	int len_arry1 = 0;

	//将buy_list转换为arry1形式储存{-----------------------------------------------------------------------
	//现判断一下len_arry1的大小
	for (int i = 0; i < leng_s_arry_person; i++) {
		if (leng_s_a_b_l[i] > 0) {
			len_arry1++;
		}
	}
	arry1 = new int *[len_arry1];
	len_arry1_ev = new int[len_arry1];
	int i_arry_buy_list = 0;
	for (int i = 0; i < len_arry1; i++) {
		len_arry1_ev[i] = 0;
		for (; i_arry_buy_list < leng_s_arry_person; i_arry_buy_list++) {
			len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list] + 1;
			arry1[i] = new int[leng_s_a_b_l[i_arry_buy_list] + 1];
			if (leng_s_a_b_l[i_arry_buy_list] > 0) {

				arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

				for (int j = 0; j < leng_s_a_b_l[i_arry_buy_list]; j++) {
					arry1[i][j + 1] = arry_buy_list[i_arry_buy_list][j];
				}
				i_arry_buy_list++;
				break;
			}
		}
	}

	//转化end}=========================================================================================

	check_arry_re(arry1, len_arry1, len_arry1_ev);
	ofstream fout("solution4.txt");
	for (int i = 0; i < len_arry1; i++) {
		fout << arry1[i][0] << "\t";
		for (int j = 0; j < len_arry1_ev[i] - 1; j++) {
			fout << arry1[i][j] << ",";
		}
		fout << arry1[i][len_arry1_ev[i] - 1] << endl;
	}
	//f_m_b,f_d_b,f_m_e,f_d_e

	double * value_score = NULL;
	value_score = score(arry1, len_arry1_ev, len_arry1, arry2, len_arry2_ev,
			len_arry2);
	cout << value_score[0] * 100.0 << "%\t" << value_score[1] * 100.0 << "%\t"
			<< value_score[2] * 100.0 << "%" << endl;

}

void solution_test2(person * arry_person_input, int leng_s_arry_person_input,
		int f_m_b = 8, int f_d_b = 16, int f_m_e = 9, int f_d_e = 15) {
	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b, f_d_b);
	}
	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l = new int[leng_s_arry_person];
	int *leng_a_b_l = new int[leng_s_arry_person];

	int ** arry2;
	int * len_arry2_ev = NULL;
	int len_arry2 = 0;

	chose_kind_from_day(f_m_b, f_d_b, f_m_e, f_d_e, arry_person_input,
			leng_s_arry_person_input, arry2, len_arry2, len_arry2_ev, 1);


	double ** Pbuyagain;
	int len_buyagain;
	findPbuyagain(arry_person,leng_s_arry_person,Pbuyagain,len_buyagain);

	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = arry_person[i_a_p].leng_s_p_buyim;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);

		for (int i = 0; i < arry_person[i_a_p].leng_s_p_buyim; i++) {
			arry_buy_list[i_a_p][i] = p.p_buyimformation[i].brand_id;
		}
		leng_s_a_b_l[i_a_p] = arry_person[i_a_p].leng_s_p_buyim;
	}

	int ** arry1;
	int * len_arry1_ev = NULL;
	int len_arry1 = 0;

	//将buy_list转换为arry1形式储存{-----------------------------------------------------------------------
	//现判断一下len_arry1的大小
	for (int i = 0; i < leng_s_arry_person; i++) {
		if (leng_s_a_b_l[i] > 0) {
			len_arry1++;
		}
	}
	arry1 = new int *[len_arry1];
	len_arry1_ev = new int[len_arry1];
	int i_arry_buy_list = 0;
	for (int i = 0; i < len_arry1; i++) {
		len_arry1_ev[i] = 0;
		for (; i_arry_buy_list < leng_s_arry_person; i_arry_buy_list++) {
			len_arry1_ev[i] = leng_s_a_b_l[i_arry_buy_list] + 1;
			arry1[i] = new int[leng_s_a_b_l[i_arry_buy_list] + 1];
			if (leng_s_a_b_l[i_arry_buy_list] > 0) {

				arry1[i][0] = arry_person[i_arry_buy_list].get_person_id();

				for (int j = 0; j < leng_s_a_b_l[i_arry_buy_list]; j++) {
					arry1[i][j + 1] = arry_buy_list[i_arry_buy_list][j];
				}
				i_arry_buy_list++;
				break;
			}
		}
	}

	//转化end}=========================================================================================

	check_arry_re(arry1, len_arry1, len_arry1_ev);
	ofstream fout("solution4.txt");
	for (int i = 0; i < len_arry1; i++) {
		fout << arry1[i][0] << "\t";
		for (int j = 1; j < len_arry1_ev[i] - 1; j++) {
			fout << arry1[i][j] << ",";
		}
		fout << arry1[i][len_arry1_ev[i] - 1] << endl;
	}
	//f_m_b,f_d_b,f_m_e,f_d_e

	double * value_score = NULL;
	value_score = score(arry1, len_arry1_ev, len_arry1, arry2, len_arry2_ev,
			len_arry2);
	cout << value_score[0] * 100.0 << "%\t" << value_score[1] * 100.0 << "%\t"
			<< value_score[2] * 100.0 << "%" << endl;
}

void findPbuy(person * arry_person_input, int len_arry_person_input,
		double **&Pbuy, int &len_Pbuy) {

	cout<<"findPbuy----->------->------>--------->"<<endl;
	Pbuy = new double *[3];
	for (int i = 0; i < 3; i++) {
		Pbuy[i] = new double[10000];
	}
	len_Pbuy = 0;

	person * arry_person = new person[len_arry_person_input];
	int len_arry_person = len_arry_person_input;
	for (int i = 0; i < len_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}

	for (int i_a_p = 0; i_a_p < len_arry_person; i_a_p++) {

		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);
		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {
				//查看是否已添加过
				int i_buy = 0;
				for (; i_buy < len_Pbuy; i_buy++) {
					if (Pbuy[0][i_buy]
							== p.p_buyimformation[i_brand_b].brand_id) {
						break;
					}
				}

				if (i_buy == len_Pbuy) {
					//没有添加过
					Pbuy[0][len_Pbuy] = p.p_buyimformation[i_brand_b].brand_id;
					len_Pbuy++;
					Pbuy[1][len_Pbuy] = 0;
					Pbuy[2][len_Pbuy] = 0;
				}

				bool ifcheck = false;
				bool ifbuy = false;
				for (int i = i_brand_b; i < i_brand_e; i++) {
					if (p.p_buyimformation[i].type == 1) {
						ifbuy = true;
					}
					if (p.p_buyimformation[i].type == 0) {
						ifcheck = true;
					}
					if (ifbuy && ifcheck) {
						break;
					}
				}
				if (ifcheck)
					Pbuy[2][i_buy]++;
				if (ifbuy)
					Pbuy[1][i_buy]++;

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

		}
	}

	for (int i = 0; i < len_Pbuy; i++) {
		if(Pbuy[2][i] >20 && Pbuy[1][i] == 0  ){
			cout<<Pbuy[0][i]<<"\t"<<Pbuy[1][i]<<"\t"<<Pbuy[2][i]<<endl;

		}
		Pbuy[1][i] /= Pbuy[2][i];



	}

	delete[] arry_person;
	cout<<"len_Pbuy="<<len_Pbuy<<endl;
	cout<<"findPbuy----->------->------>--------->"<<endl;
}

void findPbuyagain(person * arry_person_input, int len_arry_person_input,
		double **&Pbuy, int &len_Pbuy) {

	cout<<"findPbuy----->------->------>--------->"<<endl;
	Pbuy = new double *[3];
	for (int i = 0; i < 3; i++) {
		Pbuy[i] = new double[3000];
	}
	len_Pbuy = 0;

	person * arry_person = new person[len_arry_person_input];
	int len_arry_person = len_arry_person_input;
	for (int i = 0; i < len_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}

	for (int i_a_p = 0; i_a_p < len_arry_person; i_a_p++) {

		person &p = arry_person[i_a_p];
		//先按id排个序
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);
		for (int i_brand_e = 0, i_brand_b = 0;
				i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
			if (p.p_buyimformation[i_brand_b].brand_id
					!= p.p_buyimformation[i_brand_e].brand_id) {
				//查看是否已添加过
				int i_buy = 0;
				for (; i_buy < len_Pbuy; i_buy++) {
					if (Pbuy[0][i_buy]
							== p.p_buyimformation[i_brand_b].brand_id) {
						break;
					}
				}

				int num_buy = 0;
				for (int i = i_brand_b; i < i_brand_e; i++) {
					if (p.p_buyimformation[i].type == 1) {
						num_buy++;
					}

				}
				if (i_buy == len_Pbuy && num_buy >= 1) {
					//没有添加过
					Pbuy[0][len_Pbuy] = p.p_buyimformation[i_brand_b].brand_id;
					len_Pbuy++;

					Pbuy[2][len_Pbuy] = 1;
					if (num_buy >= 2)
						Pbuy[1][i_buy] = 1;
					else
						Pbuy[1][i_buy] = 0;
				} else {
					if (num_buy >= 1)
						Pbuy[2][i_buy]++;
					if (num_buy >= 2)
						Pbuy[1][i_buy]++;
				}

				i_brand_b = i_brand_e;
				i_brand_e--;
			}

		}
	}

	for (int i = 0; i < len_Pbuy; i++) {

		Pbuy[1][i] /= Pbuy[2][i];
	}

	delete[] arry_person;
	cout<<"findPbuy----->------->------>--------->"<<endl;
}

void findconnect(person * arry_person_input, int len_arry_person_input,
		int ** & arry_person_connect, int * & len_a_p_c, int *&buy_num) {

	person * arry_person = new person[len_arry_person_input];
	int len_arry_person = len_arry_person_input;
	for (int i = 0; i < len_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}

	arry_person_connect = new int *[len_arry_person];
	buy_num = new int[len_arry_person];
	int **arry_connect_local = new int *[len_arry_person];
	len_a_p_c = new int[len_arry_person];
	int * len_a_p_c_l = new int[len_arry_person];
	int * len_a_p_c_ln = new int[len_arry_person];
	for (int i = 0; i < len_arry_person; i++) {
		arry_person_connect[i] = new int[len_arry_person];
		arry_connect_local[i] = new int[len_arry_person];
		len_a_p_c[i] = 0;
		len_a_p_c_l[i] = 0;
		len_a_p_c_ln[i] = 0;
	}

	int num_save_s = 0;
	int num_find_total = 0;	//已经找到的

	//先找到第一个人作为开始

	arry_connect_local[0][0] = 0;
	len_a_p_c_l[0] = 1;
	num_save_s = 1;
	num_find_total++;
	if (arry_person[0].leng_s_p_buyim != 0)
		arry_person[0].leng_s_p_buyim *= -1;
	else
		arry_person[0].leng_s_p_buyim = -1;

	//开始查找
	do {
		if (num_find_total == len_arry_person)
			break;

		if (len_a_p_c_l[num_save_s - 1] == len_a_p_c_ln[num_save_s - 1]) {
			//说明一个组的已经找完，开始下一个组
			for (int i = 0; i < len_arry_person; i++) {
				if (arry_person[i].leng_s_p_buyim >= 0) {
					arry_connect_local[num_save_s][0] = i;
					len_a_p_c_l[num_save_s] = 1;
					num_save_s++;
					num_find_total++;
					if (arry_person[i].leng_s_p_buyim != 0)
						arry_person[i].leng_s_p_buyim *= -1;
					else
						arry_person[i].leng_s_p_buyim = -1;
					break;
				}
			}
		}

		//查找与checkid对应的人有联系的人
		int checkid = arry_connect_local[num_save_s - 1][len_a_p_c_ln[num_save_s
				- 1]];
		BuyImformation * p_r = arry_person[checkid].p_buyimformation;
		int buycheckid[500];
		int len_buycheckid = 0;
		for (int j = 0; j < abs(arry_person[checkid].leng_s_p_buyim); j++) {
			if (p_r[j].type != 1)
				continue;
			buycheckid[len_buycheckid] = p_r[j].brand_id;
			len_buycheckid++;
		}
		buy_num[checkid] = len_buycheckid;
		for (int i = 0; i < abs(len_arry_person); i++) {

			//首先判断一下该user是否已加入小组
			if (arry_person[i].leng_s_p_buyim < 0)
				continue;

			//该用户没有加入，判断一下是否需要加入
			bool ifshoudjoin = false;
			BuyImformation * p_f = arry_person[i].p_buyimformation;
			for (int j = 0; j < abs(arry_person[i].leng_s_p_buyim); j++) {
				if (p_f[j].type != 1)
					continue;
				for (int k = 0; k < len_buycheckid; k++) {
					if (p_f[j].brand_id == buycheckid[k]) {

						ifshoudjoin = true;
						break;
					}

				}
				if (ifshoudjoin)
					break;
			}
			if (ifshoudjoin) {
				arry_connect_local[num_save_s - 1][len_a_p_c_l[num_save_s - 1]] =
						i;
				len_a_p_c_l[num_save_s - 1]++;
				if (arry_person[i].leng_s_p_buyim != 0)
					arry_person[i].leng_s_p_buyim *= -1;
				else
					arry_person[i].leng_s_p_buyim = -1;
				num_find_total++;
			}
		}
		len_a_p_c_ln[num_save_s - 1]++;
	} while (1);

	//将arry_connect_local转化为分用户储存方便查看
	for (int i = 0; i < num_save_s; i++) {
		for (int j = 0; j < len_a_p_c_l[i]; j++) {
			for (int k = 0; k < len_a_p_c_l[i]; k++) {
				int ci = arry_connect_local[i][j];
				if (ci == arry_connect_local[i][k])
					continue;

				arry_person_connect[ci][len_a_p_c[ci]] =
						arry_connect_local[i][k];
				len_a_p_c[ci]++;
			}
		}
	}

	delete[] arry_person;
	delete[] len_a_p_c_l;
	delete[] len_a_p_c_ln;
	for (int i = 0; i < len_arry_person; i++) {

		delete[] arry_connect_local[i];

	}
	delete[] arry_connect_local;

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

double* score(int ** arry1, int *len_arry1_ev, int len_arry1, int ** arry2,
		int *len_arry2_ev, int len_arry2)
//返回一个3位数组的指针，第一位是命中率，第二位是召回率，第三位是分数，都是double型
		{

	int hit = 0, hitu = 0;
	//hit存储品牌命中数，hitu存储命中用户数

	int pBrand = 0, bBrand = 0;
	//pBrand存储预测品牌总数，bBrand存储实际品牌总数

	int* hituser1 = new int[len_arry1];
	//hituser存储命中用户在1中的序号，不是id

	int* hituser2 = new int[len_arry1];
	//hituser存储命中用户在2中的序号，不是id

	double* prst = new double[3];

	for (int i = 0; i < len_arry1; i++)
	//遍历1
			{
		for (int j = 0; j < len_arry2; j++)
		//每个1与所有2比较
				{
			if (arry1[i][0] == arry2[j][0]) {
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

				if (arry1[hituser1[i]][j] == arry2[hituser2[i]][k]) {

					hit++;
				}
			}
		}
	}

	for (int i = 0; i < len_arry1; i++)
	//计算命中数
			{
		pBrand += len_arry1_ev[i] - 1;
	}

	for (int i = 0; i < len_arry2; i++)
	//计算召回数
			{
		bBrand += len_arry2_ev[i] - 1;
	}

	prst[0] = (double(hit) / double(pBrand));
	//命中率
	prst[1] = (double(hit) / double(bBrand));
	//召回率

	cout << "" << hit << "\t" << pBrand << "\tbBrand=" << bBrand
			<< endl;
	prst[2] = (2.0 * prst[0] * prst[1] / (prst[0] + prst[1]));
	//得分

	delete[] hituser1;
	delete[] hituser2;

	return prst;
}

void findprivilege(person * arry_person,int len_arry_person,double **arry_Pbuy,int len_Pbuy,int f_m_b,int f_d_b ,int *& arry_pri_brand,int & len_arry_pri_brand){
	cout<<"寻找打折优惠商品--->---------->---------->-------->------->"<<endl;

	len_arry_pri_brand = 0;
	arry_pri_brand = new int [1000];

	arry_pri_brand = new int [len_Pbuy];
	//起始日期是4月15
	int **arry_check_day = new int* [2];
	int num_total_day_gap = day_gap(f_m_b,f_d_b,4,15);
	arry_check_day[0] = new int [num_total_day_gap];
	arry_check_day[1] = new int [num_total_day_gap];
	int len_arry_check_day = 0;

	for(int i_finding_brand = 0;i_finding_brand<len_Pbuy;i_finding_brand++){
		int brand_pre = arry_Pbuy[0][i_finding_brand];

		//将同一个商品的查看信息统计出来
		for(int i_a_p=0;i_a_p<len_arry_person;i_a_p++){
			BuyImformation * p = arry_person[i_a_p].p_buyimformation;
			for(int i_a_b=0;i_a_b<arry_person[i_a_p].leng_s_p_buyim;i_a_b++){
				if(brand_pre == p[i_a_b].brand_id ){
					int i_a_c=0;
					int daygap_local_f = day_gap(p[i_a_b].visit_datetime_month,p[i_a_b].visit_datetime_day,4,15);
					for(;i_a_c<len_arry_check_day;i_a_c++){

						if(arry_check_day[0][i_a_c] == daygap_local_f){
							arry_check_day[1][i_a_c]++;
							break;
						}
					}
					if(i_a_c == len_arry_check_day){
						arry_check_day[0][len_arry_check_day] = daygap_local_f;
						arry_check_day[1][len_arry_check_day] = 1;
						len_arry_check_day++;
					}
				}
			}
		}

		//检查查看高峰
		int num_check_total = 0;


		int num_check_1 = 0;
		int num_check_2 = 0;
		for(int i=0;i<len_arry_check_day;i++){
			num_check_total+=arry_check_day[1][i];
			if(arry_check_day[0][i] <= 15){
				num_check_1++;
			}
			if(arry_check_day[0][i] <= 40){
				num_check_2++;
			}
		}

		if(double(num_check_1)/15. > 0.5*double(num_check_total)/double(num_total_day_gap)){
			arry_pri_brand[len_arry_pri_brand] = brand_pre;
			cout<<len_arry_pri_brand+1<<"\t"<<arry_pri_brand[len_arry_pri_brand]<<endl;
			len_arry_pri_brand++;
			if(len_arry_pri_brand >=1000){
				cout<<"erro:no space for arry_pri_brand"<<endl;
				exit(0);
			}
		}
	}


	delete [] arry_check_day[0];
	delete [] arry_check_day[1];
	cout<<"寻找打折优惠商品--->---------->---------->-------->------->"<<endl;
}

void findbuyinBuylist(person* arry_person,int len_arry_person
		,int **&buylist,int *&len_buylist_ev,int &len_buylist
		,double **Pbuy,int len_Pbuy
		,int f_m_b,int f_d_b){

	cout<<"查找预测中购买较多的商品--------->--------------->--------------->"<<endl;
	int **brandlist = new int *[2];
	brandlist[0]= new int [len_Pbuy];
	brandlist[1]= new int [len_Pbuy];

	for(int i=0;i<len_Pbuy;i++){
		brandlist[0][i] = Pbuy[0][i];
		brandlist[1][i] = 0;
	}
	cout<<"\t统计中"<<endl;
	for(int i_a_p=0;i_a_p<len_buylist;i_a_p++){
		for(int i_b_l=1;i_b_l<len_buylist_ev[i_a_p];i_b_l++){
			for(int i=0;i<len_Pbuy;i++){
				if(brandlist[0][i] == buylist[i_a_p][i_b_l]){
					brandlist[1][i]++;
					break;
				}
			}
		}
	}
	int num_buy = 0;
	for(int i=0;i<len_Pbuy;i++){
		if(brandlist[1][i]>0){
			num_buy++;
		}
	}

	cout<<"\t统计完成"<<endl;



	cout<<"\t查找概率较大者"<<endl;
	cout<<"\t总预测购买数num_buy="<<num_buy<<endl;
	ofstream fout_num_pr("num_pre.txt");

	for(int i=0;i<len_Pbuy;i++){
		if(brandlist[1][i]>0){
			fout_num_pr<<brandlist[0][i]<<"\t"<<brandlist[1][i]<<endl;
		}
	}
	fout_num_pr.close();
	cout<<"\t输入最小值:"<<endl;
	int num_min_pre = 0;
	//cin>>num_min_pre;
	num_min_pre=23;

	cout<<"\t大于最小值"<<num_min_pre<<"的商品有"<<endl;
	for(int i=0;i<len_Pbuy;i++){
		if(brandlist[1][i]>=num_min_pre){
			cout<<brandlist[0][i]<<"\t"<<brandlist[1][i]<<endl;
		}
	}
	//system("pause");


	cout<<"\t查找完成"<<endl;




	cout<<"将商品添加中"<<endl;
	for(int i_b_p=0;i_b_p<len_Pbuy;i_b_p++){
		if(brandlist[1][i_b_p]<num_min_pre)
			continue;


		int brand_p = brandlist[0][i_b_p];
		for(int i_a_p=0;i_a_p<len_arry_person;i_a_p++){
			for(int i_p_b=0;i_p_b<arry_person[i_a_p].leng_s_p_buyim;i_p_b++){
				if(arry_person[i_a_p].p_buyimformation[i_p_b].brand_id == brand_p){

					//如果查看日期比较近就认为很有可能购买。
					int i_check_last = i_p_b;
					int i_buy_last = -1;
					for(;i_check_last<arry_person[i_a_p].leng_s_p_buyim;i_check_last++){

						if(arry_person[i_a_p].p_buyimformation[i_check_last].brand_id != brand_p){
							i_check_last--;
							break;
						}
						if(arry_person[i_a_p].p_buyimformation[i_check_last].type == 1){
							i_buy_last = i_check_last;
						}

					}

					if(
							i_buy_last == -1
									&& day_gap(
								f_m_b, f_d_b,arry_person[i_a_p].p_buyimformation[i_check_last].visit_datetime_month,
									arry_person[i_a_p].p_buyimformation[i_check_last].visit_datetime_day)>0
					 ) {
//						if(i_buy_last == -1){
//							cout<<i_a_p<<"\t"<<arry_person[i_a_p].get_person_id()<<"\t"<<brand_p<<endl;
//						}
						int i = 0;
						for (; i < len_buylist; i++) {
							if (buylist[i][0]
									== arry_person[i_a_p].get_person_id()) {

								buylist[i][len_buylist_ev[i]] = brand_p;
								len_buylist_ev[i]++;
								break;
							}
						}
						if (i == len_buylist) {
							len_buylist_ev[len_buylist] = 1;
							buylist[len_buylist][1] = brand_p;
							buylist[len_buylist][0] =
									arry_person[i_a_p].get_person_id();

							len_buylist++;
						}
					}
					break;
				}
			}
		}
	}

	cout<<"添加完成"<<endl;

	check_arry_re(buylist, len_buylist, len_buylist_ev);

	delete []brandlist[0];
	delete []brandlist[1];
	cout<<"查找预测中购买较多的商品--------->--------------->--------------->"<<endl;
}

void brandcheckout(person* arry_person,int len_arry_person,int brand_id,int * brand_im,const int num_day_total){

	//cout<<"将单个brand查看信息统计出来------------------------------------->"<<endl;
	//先将brand_im初始化一下
	for(int i=0;i<num_day_total;i++){
		brand_im[i] = 0;
	}

	for(int i_a_p=0;i_a_p<len_arry_person;i_a_p++){
		BuyImformation * p = arry_person[i_a_p].p_buyimformation;
		for(int i_a_b=0;i_a_b<arry_person[i_a_p].leng_s_p_buyim;i_a_b++){
			if(p[i_a_b].brand_id == brand_id){
				cout<<p[i_a_b].visit_datetime_month<<"\t"<<p[i_a_b].visit_datetime_day<<endl;
				int daygap = day_gap(p[i_a_b].visit_datetime_month,p[i_a_b].visit_datetime_day,4,15);
				cout<<daygap<<endl;
				brand_im[daygap]+= p[i_a_b].type == 1?10000:1;
			}
		}
	}
	//cout<<"单个brand查找完成---------------------------------------------<"<<endl;
}

//返回所有brand名
void brandchecked(person* arry_person,int len_arry_person,int *& arry_brand,int &len_arry_brand){

	cout<<"将brind名找出来-------------------------------------->"<<endl;
	arry_brand = new int [10000];
	len_arry_brand = 0;

	for(int i_a_p=0;i_a_p<len_arry_person;i_a_p++){
		BuyImformation * p = arry_person[i_a_p].p_buyimformation;
		for(int i_a_b=0;i_a_b<arry_person[i_a_p].leng_s_p_buyim;i_a_b++){
			bool TFjoin = false;
			int brind_p = p[i_a_b].brand_id;
			for(int iab=0;iab<len_arry_brand;iab++){
				if(brind_p == arry_brand[iab]){
					TFjoin = true;
					break;
				}
			}
			if(!TFjoin){
				arry_brand[len_arry_brand] = brind_p;
				len_arry_brand++;
			}
		}
	}

	cout<<"查找完成---------------------------------------------<"<<endl;
}

void findpeak(int* brd_arr, int len)
{
	ofstream save_file("findpeak.txt", ios::app);//在文档后追加
	for (int i = 0; i < len; i++)
	{
		save_file << brd_arr[i] << endl;
	}
	save_file << "------------------" << endl;
}


//需找一个人的查看阈值
void findthreshold(person personi,
		int f_m_b,int f_d_b,int f_m_e,int f_d_e){

	ofstream save_fileb("findthresholdb.txt",ios::app);//在文档后追加
	ofstream save_filen("findthresholdn.txt",ios::app);
	save_fileb<<personi.get_person_id()<<"------------------>"<<endl;
	save_filen<<personi.get_person_id()<<"------------------>"<<endl;
	int brand_id_p = personi.p_buyimformation[0].brand_id;
	int i_from_brand = 0;
	int i_to_brand = 0;

	do{
	for(;i_to_brand<personi.leng_s_p_buyim;i_to_brand++){
		//cout<<i_to_brand<<"\t"<<personi.p_buyimformation[i_to_brand].type<<endl;
		if(personi.p_buyimformation[i_to_brand].brand_id != brand_id_p){
			break;
		}
	}

	int i_buy = i_from_brand;
	for(;i_buy<i_to_brand;i_buy++){
//		cout<<brand_id_p<<"\t"<<personi.p_buyimformation[i_buy].type<<"\t"<<
//				personi.p_buyimformation[i_buy].visit_datetime_month<<"."<<
//				personi.p_buyimformation[i_buy].visit_datetime_day<<endl;
		if(personi.p_buyimformation[i_buy].type == 1){
			break;
		}
	}
//cout<<"-----------------------------------------------------"<<endl;

	if(i_buy == i_to_brand){
		//没有购买


		if(i_buy - i_from_brand > 0){
			save_filen<<personi.p_buyimformation[i_from_brand].brand_id<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>"<<endl;
		int daygap_f = -1;
		int num_check = 0;
		for(int i=i_from_brand;i<i_buy;i++){
			if(day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15) > daygap_f){
				if(num_check != 0){
				save_filen<<daygap_f<<"\t"<<num_check<<endl;

				}
				daygap_f= day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15);

				num_check = 1;

			}else{
				num_check += 1;
			}


		}
		save_filen<<daygap_f<<"\t"<<num_check<<endl;


		}


		//save_file<< brand_id_p<<"\t" << i_to_brand	- i_from_brand <<endl;
	}else{
		//-------------------------------------------------
		BuyImformation begin_day,end_day;
		begin_day.visit_datetime_month= f_m_b;
		begin_day.visit_datetime_day = f_d_b;
		end_day.visit_datetime_month= f_m_e;
				end_day.visit_datetime_day = f_d_e;

				if(compare(personi.p_buyimformation[i_buy],begin_day) >=0 &&
						        compare(end_day,personi.p_buyimformation[i_buy]) >= 0) {
		if(i_buy - i_from_brand > 0){
			save_fileb<<personi.p_buyimformation[i_from_brand].brand_id<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>"<<endl;
		int daygap_f = -1;
		int num_check = 0;
		for(int i=i_from_brand;i<i_buy;i++){
			if(day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15) > daygap_f){
				if(num_check != 0){
				save_fileb<<daygap_f<<"\t"<<num_check<<endl;

				}
				daygap_f= day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15);

				num_check = 1;

			}else{
				num_check += 1;
			}


		}
		save_fileb<<daygap_f<<"\t"<<num_check<<endl;


		}

	}
		//-------------------------------------------------
		//save_file<<  brand_id_p<<"\t"<< -1*(i_buy - i_from_brand) <<endl;
	}
	brand_id_p = personi.p_buyimformation[i_to_brand].brand_id;
	i_from_brand = i_to_brand;
	}while(i_from_brand<personi.leng_s_p_buyim);

	save_fileb << "<------------------" << endl;
	save_filen << "<------------------" << endl;
}

//需找一个人的查看阈值
void findthreshold2(person personi,
		int f_m_b,int f_d_b,int f_m_e,int f_d_e
		,int *& buylist,int& len_buylist){

	ofstream save_fileb("findthresholdb.txt",ios::app);//在文档后追加
	ofstream save_filen("findthresholdn.txt",ios::app);
	save_fileb<<personi.get_person_id()<<"------------------>"<<endl;

	int brand_id_p = personi.p_buyimformation[0].brand_id;
	int i_from_brand = 0;
	int i_to_brand = 0;

	do{
	for(;i_to_brand<personi.leng_s_p_buyim;i_to_brand++){
		//cout<<i_to_brand<<"\t"<<personi.p_buyimformation[i_to_brand].type<<endl;
		if(personi.p_buyimformation[i_to_brand].brand_id != brand_id_p){
			break;
		}
	}

	int i_buy = i_from_brand;
	for(;i_buy<i_to_brand;i_buy++){

		if(personi.p_buyimformation[i_buy].type == 1){
			break;
		}
	}

	double arry_check[2][70];
	int len_arry_check = 0;

	if(i_buy == i_to_brand){
		//没有购买
		len_arry_check = 0;

		if(i_buy - i_from_brand > 0){

			save_filen<<personi.p_buyimformation[i_from_brand].brand_id<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>"<<endl;
		int daygap_f = -1;
		int num_check = 0;
		for(int i=i_from_brand;i<i_buy;i++){
			if(day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15) > daygap_f){
				if(num_check != 0){
					save_filen<<daygap_f<<"\t"<<num_check<<endl;
				arry_check[0][len_arry_check] = double(daygap_f);
				arry_check[1][len_arry_check] = double(num_check);
				len_arry_check++;
				}
				daygap_f= day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15);

				num_check += 1;

			}else{
				num_check += 1;
			}


		}
		save_filen<<daygap_f<<"\t"<<num_check<<endl;
				arry_check[0][len_arry_check] = double(daygap_f);
				arry_check[1][len_arry_check] = double(num_check);
				len_arry_check++;


		}
		double r_check = 0;
		r_check=fit(arry_check[0],arry_check[1],len_arry_check);
		save_filen<<"r="<<r_check<<endl;
		bool TFfit = false;
		if(r_check <0.7 || (r_check >0.87 && r_check < 0.94) || r_check > 0.98){
			TFfit =true;
		}
		if(len_arry_check>=3 && TFfit && arry_check[0][len_arry_check-1]>60){
			buylist[len_buylist] = brand_id_p;
			len_buylist++;
		}

		//save_file<< brand_id_p<<"\t" << i_to_brand	- i_from_brand <<endl;
	}else{
		/*
		len_arry_check = 0;
		//-------------------------------------------------
		BuyImformation begin_day,end_day;
		begin_day.visit_datetime_month= f_m_b;
		begin_day.visit_datetime_day = f_d_b;
		end_day.visit_datetime_month= f_m_e;
				end_day.visit_datetime_day = f_d_e;

				if(compare(personi.p_buyimformation[i_buy],begin_day) >=0 &&
						        compare(end_day,personi.p_buyimformation[i_buy]) >= 0) {
		if(i_buy - i_from_brand > 0){
			save_fileb<<personi.p_buyimformation[i_from_brand].brand_id<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>"<<endl;
		int daygap_f = -1;
		int num_check = 0;
		for(int i=i_from_brand;i<i_buy;i++){
			if(day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15) > daygap_f){
				if(num_check != 0){
				save_fileb<<daygap_f<<"\t"<<num_check<<endl;
				arry_check[0][len_arry_check] = double(daygap_f);
								arry_check[1][len_arry_check] = double(num_check);
								len_arry_check++;

				}
				daygap_f= day_gap(personi.p_buyimformation[i].visit_datetime_month,personi.p_buyimformation[i].visit_datetime_day,4,15);

				num_check += 1;

			}else{
				num_check += 1;
			}


		}
		save_fileb<<daygap_f<<"\t"<<num_check<<endl;
		arry_check[0][len_arry_check] = double(daygap_f);
		arry_check[1][len_arry_check] = double(num_check);
		len_arry_check++;


		}
		double r_check = 0;
		r_check=fit(arry_check[0],arry_check[1],len_arry_check);
		save_fileb<<"r="<<r_check<<endl;
	}
		//-------------------------------------------------
		//save_file<<  brand_id_p<<"\t"<< -1*(i_buy - i_from_brand) <<endl;
	*/}
	brand_id_p = personi.p_buyimformation[i_to_brand].brand_id;
	i_from_brand = i_to_brand;
	}while(i_from_brand<personi.leng_s_p_buyim);

	save_fileb << "<------------------" << endl;
	save_filen << "<------------------" << endl;
}


double fit(double *x,double *y,int num_n){

	/*
	 * 原理：
	 * 使用最小二乘法进行线性拟合
	 * 设y=ax+b
	 * 设A=Sigma（x_i^2),B=Sigma(x_i),C=Sigma(y_i*x_i),D=Sigam(y_i)
	 *
	 * 得：
	 * 	a=(Cn-BD)/(An-BB)
	 * 	b=(AD-CB)/(An-BB)
	 */

	double r = 0;
	double a,b;

	double A(0),B(0),C(0),D(0);

	for(int i=0;i<num_n;i++){
		A += x[i]*x[i];
		B += x[i];
		C += y[i]*x[i];
		D += y[i];
	}

	double n = double(num_n);
	a = (C*n-B*D)/(A*n-B*B);
	b = (A*D-C*B)/(A*n-B*B);

	double sum_xy = 0.;
	double sum_x = 0.;
	double sum_y = 0;
	double sum_x2 = 0;
	double sum_y2 = 0;
	for(int i=0;i<num_n;i++){
		sum_xy += x[i]*y[i];
		sum_x += x[i];
		sum_y += y[i];
		sum_x2 += x[i]*x[i];
		sum_y2 += y[i]*y[i];
	}

	r = (double(num_n)*sum_xy-sum_x*sum_y)
			/sqrt((double(num_n)*sum_x2-sum_x*sum_x)*(double(num_n)*sum_y2-sum_y*sum_y));

	return r;
}


