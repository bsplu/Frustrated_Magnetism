//============================================================================
// Name        : Frustrated.cpp
// Author      : BSheng  &  YHH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include <time.h>
#include<fstream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
#include <sstream>
#include <omp.h>

using namespace std;

class BuyImformation {
public:
	int brand_id;
	int type;
	int visit_datetime_month;
	int visit_datetime_day;

	void initialize(int brand_id_input,int type_input,
			int visit_datetime_month_input,int visit_datetime_day_input){
		brand_id = brand_id_input;
		type = type_input;
		visit_datetime_month = visit_datetime_month_input;
		visit_datetime_day = visit_datetime_day_input;
	}

	BuyImformation & operator=(BuyImformation a){
		this->brand_id = a.brand_id;
		this->type = a.type;
		this->visit_datetime_day = a.visit_datetime_day;
		this->visit_datetime_month = a.visit_datetime_month;

		return *this;
	}
};

class person {
private:
	int p_id;

public:
	BuyImformation * p_buyimformation;
	int leng_s_p_buyim;
	int leng_p_buyim;

	int get_person_id() {
		return p_id;
	}
	void change_person_id(int p_id_input){
		p_id = p_id_input;
	}

	void allocate_p_buy(int leng){
		delete [] p_buyimformation;
		p_buyimformation = new BuyImformation [leng];
		leng_p_buyim = leng;
		leng_s_p_buyim = 0;
	}

	void push_p_buy(int brand_id_input,int type_input,
			int visit_datetime_month_input,int visit_datetime_day_input){
		if(leng_s_p_buyim == leng_p_buyim){
			BuyImformation * p_buyimformation_local = new BuyImformation [leng_p_buyim];
			for(int i=0;i<leng_p_buyim;i++){
				p_buyimformation_local[i] = p_buyimformation[i];
			}
			delete [] p_buyimformation;
			p_buyimformation = new BuyImformation [2*leng_p_buyim];
			for(int i=0;i<leng_p_buyim;i++){
				p_buyimformation[i] = p_buyimformation_local[i];
			}
			leng_p_buyim *= 2;
		}

			p_buyimformation[leng_s_p_buyim].initialize(brand_id_input,
					type_input,visit_datetime_month_input,visit_datetime_day_input);
			leng_s_p_buyim++;

	}

	void optimize_p_buy(){
		if(leng_s_p_buyim != leng_p_buyim){
			BuyImformation * p_buyimformation_local = new BuyImformation [leng_s_p_buyim];
			for(int i=0;i<leng_s_p_buyim;i++){
				p_buyimformation_local[i] = p_buyimformation[i];
			}
			delete [] p_buyimformation;
			p_buyimformation = new BuyImformation [leng_s_p_buyim];
			for(int i=0;i<leng_s_p_buyim;i++){
				p_buyimformation[i] = p_buyimformation_local[i];
			}
			leng_p_buyim = leng_s_p_buyim;
		}
	}



	person & operator=(person a){
		p_id = a.p_id;
		leng_p_buyim = a.leng_p_buyim;
		leng_s_p_buyim = a.leng_s_p_buyim;
		if(a.leng_p_buyim >0){
		delete [] p_buyimformation;
		p_buyimformation = new BuyImformation [a.leng_p_buyim];
		for(int i=0;i<leng_s_p_buyim;i++){
			p_buyimformation[i] = a.p_buyimformation[i];
		}
		}
		return *this;
	}

	~person() {
		delete[] p_buyimformation;
	}

};

int main() {
	void read_txt(person * arry_person,int & leng_s_arry_person);


	person * arry_person;
	int leng_s_arry_person;

	read_txt(arry_person,leng_s_arry_person);


	cout<<leng_s_arry_person<<endl;


	return 0;
}

void read_txt(person * arry_person,int & leng_s_arry_person) {
	FILE* fp;

	if ((fp = fopen("t_alibaba_data.csv", "r")) < 0) {
		printf("open the file is error!\n");
		exit(0);
	}

	int i_line = 0;
	int p_id_pre = -1;
	int leng_arry_person = 1000;//数组长度
	leng_s_arry_person = 0;//已储存个数
	arry_person = new person [leng_arry_person];

	while (!feof(fp)) {

		int p_id_pre_local, brand_id_local, type_local,
				visit_datetime_day_local, visit_datetime_month_local;

		fscanf(fp, "%d,%d,%d,%d日%d月", &p_id_pre_local, &brand_id_local,
				&type_local, &visit_datetime_month_local,
				&visit_datetime_day_local);

		i_line++;

		if (p_id_pre == p_id_pre_local) {
			//继续当前用户

			arry_person[leng_s_arry_person-1].push_p_buy(brand_id_local,
				type_local, visit_datetime_month_local,
				visit_datetime_day_local);


		} else {
			//开始新用户

			//判断是否需要扩容{-----------------------------------------------------
			if(leng_s_arry_person == leng_arry_person){
				//数组长度不足需要扩容

				//建立临时数组进行复制
				person * arry_person_local = new person [leng_arry_person];
				for(int i=0;i<leng_arry_person;i++){
					arry_person_local[i] = arry_person[i];
				}

				//将长度扩为原先2倍

				delete [] arry_person;
				arry_person = new person [leng_arry_person*2];

				for(int i=0;i<leng_arry_person;i++){
					arry_person[i] = arry_person_local[i];
				}
				leng_arry_person *= 2;

				delete [] arry_person_local;
			}
			//判断扩容end}=========================================================

			//新用户初始化{---------------------------------------------------------
			p_id_pre = p_id_pre_local;
			arry_person[leng_s_arry_person].change_person_id(p_id_pre);
			arry_person[leng_s_arry_person].allocate_p_buy(100);
			arry_person[leng_s_arry_person].p_buyimformation[0].initialize(
					brand_id_local,type_local, visit_datetime_month_local,
					visit_datetime_day_local);

			leng_s_arry_person++;
			//初始化结束}===========================================================

			//将上一个用户内存进行优化{-------------------------------------------------
			//将p_buyimformation中没有使用的多余的数组删掉，使得leng=leng_s
			if(leng_s_arry_person != 1)
				arry_person[leng_s_arry_person-2].optimize_p_buy();
			//优化}===============================================================

		}
	}

	//读取完毕

}

