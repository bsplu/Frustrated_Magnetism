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
	int type;//�����0������1���ղأ�2�����ﳵ��3
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

	person(){
		p_id = 0;
		leng_s_p_buyim = 0;
		leng_p_buyim = 0;
		p_buyimformation = NULL;
	}

	int get_person_id() {
		return p_id;
	}
	void change_person_id(int p_id_input){
		p_id = p_id_input;
	}

	void allocate_p_buy(int leng){
		if(p_buyimformation != NULL){
			delete [] p_buyimformation;
			p_buyimformation = NULL;
		}
		p_buyimformation = new BuyImformation [leng];
		leng_p_buyim = leng;
		leng_s_p_buyim = 0;
	}

	void push_p_buy(int brand_id_input,int type_input,
			int visit_datetime_month_input,int visit_datetime_day_input){
		if(leng_s_p_buyim == leng_p_buyim && leng_s_p_buyim > 0){
			BuyImformation * p_buyimformation_local = new BuyImformation [leng_p_buyim];
			for(int i=0;i<leng_p_buyim;i++){
				p_buyimformation_local[i] = p_buyimformation[i];
			}
			delete [] p_buyimformation;
			p_buyimformation = NULL;
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
			p_buyimformation = NULL;
			p_buyimformation = new BuyImformation [leng_s_p_buyim];
			for(int i=0;i<leng_s_p_buyim;i++){
				p_buyimformation[i] = p_buyimformation_local[i];
			}
			leng_p_buyim = leng_s_p_buyim;
			delete []p_buyimformation_local;
			p_buyimformation_local = NULL;
		}
	}

	void delete_one_p_buy(int i_brand){
		//i_brandΪ����λ��
		for(int i=i_brand;i<leng_s_p_buyim-1;i++){
			p_buyimformation[i] = p_buyimformation[i+1];
		}
		leng_s_p_buyim--;
	}

	person & operator=(person a){
		p_id = a.p_id;
		leng_p_buyim = a.leng_p_buyim;
		leng_s_p_buyim = a.leng_s_p_buyim;
		if(a.leng_p_buyim >0){
		delete [] p_buyimformation;
		p_buyimformation = NULL;
		p_buyimformation = new BuyImformation [a.leng_p_buyim];
		for(int i=0;i<leng_s_p_buyim;i++){
			p_buyimformation[i] = a.p_buyimformation[i];
		}
		}
		return *this;
	}

	~person() {
		delete[] p_buyimformation;
		p_buyimformation = NULL;
	}

};

int main() {
	void read_txt(person* &arry_person, int & leng_s_arry_person);


	person * arry_person = NULL;
	int leng_s_arry_person=0;


	//cout<<leng_s_arry_person<<endl;

	read_txt( arry_person,leng_s_arry_person);


	//���ʵ����û�����
	BuyImformation *p = arry_person[0].p_buyimformation;

	for(int i=0;i<arry_person[0].leng_s_p_buyim;i++){
		cout<<p[i].brand_id<<"\t"<<p[i].type<<"\t"<<p[i].visit_datetime_month<<"��"<<p[i].visit_datetime_day<<"��"<<endl;
	}

	return 0;
}

void read_txt(person* &arry_person_inpute, int & leng_s_arry_person) {

	FILE* fp;

	if ((fp = fopen("t_alibaba_data.csv", "r")) < 0) {
		printf("open the file is error!\n");
		exit(0);
	}

	int i_line = 0;
	int p_id_pre = -1;
	int leng_arry_person = 1000;//���鳤��

	leng_s_arry_person = 0;//�Ѵ������
	//cout<<"==================="<<endl;
	person * arry_person = new person [leng_arry_person];

	while (!feof(fp)) {

		int p_id_pre_local, brand_id_local, type_local,
				visit_datetime_day_local, visit_datetime_month_local;

		fscanf(fp, "%d,%d,%d,%d��%d��\n", &p_id_pre_local, &brand_id_local,
				&type_local, &visit_datetime_month_local,
				&visit_datetime_day_local);

		i_line++;

		if (p_id_pre == p_id_pre_local) {
			//������ǰ�û�

			arry_person[leng_s_arry_person-1].push_p_buy(brand_id_local,
				type_local, visit_datetime_month_local,
				visit_datetime_day_local);


		} else {
			//��ʼ���û�

			//�ж��Ƿ���Ҫ����{-----------------------------------------------------
			if(leng_s_arry_person == leng_arry_person){
				//���鳤�Ȳ�����Ҫ����
				//������ʱ������и���
				person * arry_person_local = new person [leng_arry_person];
				for(int i=0;i<leng_arry_person;i++){
					arry_person_local[i] = arry_person[i];
				}

				//��������Ϊԭ��2��

				delete [] arry_person;
				arry_person = NULL;
				arry_person = new person [leng_arry_person*2];

				for(int i=0;i<leng_arry_person;i++){
					arry_person[i] = arry_person_local[i];
				}
				leng_arry_person *= 2;

				delete [] arry_person_local;
				arry_person_local = NULL;
			}
			//�ж�����end}=========================================================

			//���û���ʼ��{---------------------------------------------------------

			p_id_pre = p_id_pre_local;
			arry_person[leng_s_arry_person].change_person_id(p_id_pre);

			arry_person[leng_s_arry_person].allocate_p_buy(100);


			arry_person[leng_s_arry_person].p_buyimformation[0].initialize(
					brand_id_local,type_local, visit_datetime_month_local,
					visit_datetime_day_local);

			leng_s_arry_person++;
			//��ʼ������}===========================================================

			//����һ���û��ڴ�����Ż�{-------------------------------------------------
			//��p_buyimformation��û��ʹ�õĶ��������ɾ����ʹ��leng=leng_s
			if(leng_s_arry_person != 1){

				//����û���ĳһ��day1��������Ʒa����day1����鿴a����Ϣɾ��

				BuyImformation *p_buyim_local = arry_person[leng_s_arry_person-2].p_buyimformation;
				int i_brand_id = 0;
				do{
					int buyday(0),buymonth(0),buybrand_id(0);
				for(;i_brand_id<arry_person[leng_s_arry_person-2].leng_s_p_buyim;i_brand_id++){
					if(p_buyim_local[i_brand_id].type == 1){
						buybrand_id = p_buyim_local[i_brand_id].brand_id;
						buymonth = p_buyim_local[i_brand_id].visit_datetime_month;
						buyday = p_buyim_local[i_brand_id].visit_datetime_day;
						break;
					}
				}

				if(i_brand_id == arry_person[leng_s_arry_person-2].leng_s_p_buyim)
					break;

				for(int i=0;i<arry_person[leng_s_arry_person-2].leng_s_p_buyim;i++){
					if(p_buyim_local[i].brand_id == buybrand_id && p_buyim_local[i].visit_datetime_month == buymonth
							&& p_buyim_local[i].visit_datetime_day == buyday && p_buyim_local[i].type == 0){
						//����ͬ�칺��鿴������������ɾ��

						arry_person[leng_s_arry_person-2].delete_one_p_buy(i);
						i--;
						if(i<i_brand_id){
							i_brand_id--;
						}
					}else if(p_buyim_local[i].brand_id == buybrand_id && p_buyim_local[i].visit_datetime_month == buymonth
							&& p_buyim_local[i].visit_datetime_day == buyday && p_buyim_local[i].type == 1 && i != i_brand_id){
						//һ���ж�ι���һ����ƷҲ��ɾ����

						arry_person[leng_s_arry_person-2].delete_one_p_buy(i);
						i--;
						if(i<i_brand_id){
							i_brand_id--;
						}

					}
				}
				i_brand_id++;
				}while(1);



				arry_person[leng_s_arry_person-2].optimize_p_buy();
			}
			//�Ż�}===============================================================

		}
		//cout<<arry_person[0].p_buyimformation[0].brand_id<<endl;
	}

	//��ȡ���
	//cout<<"��ȡ���"<<endl;
	//cout<<arry_person[0].p_buyimformation[0].brand_id<<endl;

	if(arry_person_inpute == NULL){
		arry_person_inpute = new person [leng_s_arry_person];
	}else{
		delete [] arry_person_inpute;
		arry_person_inpute = NULL;
		arry_person_inpute = new person [leng_s_arry_person];
	}

	for(int i=0;i<leng_s_arry_person;i++){
		arry_person_inpute[i] = arry_person[i];
	}
}

