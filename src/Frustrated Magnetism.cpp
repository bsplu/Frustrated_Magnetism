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
	void solution1(person * arry_person_input,int leng_s_arry_person_input);

	srand((unsigned) time(NULL));
	person * arry_person = NULL;
	int leng_s_arry_person=0;


	//cout<<leng_s_arry_person<<endl;

	read_txt( arry_person,leng_s_arry_person);

	solution1(arry_person,leng_s_arry_person);

	return 0;
}

double randoms(double b=0.0,double e=1.0){

	return (e-b)*double(rand())/double(RAND_MAX)+b;
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

						if(i<i_brand_id){
							i_brand_id--;
						}
						i--;
					}else if(p_buyim_local[i].brand_id == buybrand_id && p_buyim_local[i].visit_datetime_month == buymonth
							&& p_buyim_local[i].visit_datetime_day == buyday && p_buyim_local[i].type == 1 && i != i_brand_id){
						//һ���ж�ι���һ����ƷҲ��ɾ����

						arry_person[leng_s_arry_person-2].delete_one_p_buy(i);

						if(i<i_brand_id){
							i_brand_id--;
						}
						i--;

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


//������������֮����������
int day_gap(int month1,int day1,int month2,int day2){



	int m_b = month1;
	int m_s = month2;
	int d_b = day1;
	int d_s = day2;
	int sign = 1;

	if(month2 > month1 ){
		m_b = month2;
		d_b = day2;
		m_s = month1;
		d_s = day1;
		sign = -1;
	}else if(month1 == month2){
		return (day1 - day2);
	}

	return sign*((m_b-m_s)*30-d_b+d_s);

}

//�㷨1
/*
 * by YHH
 * ������ȫ�����˼��
 *
 * �������ڹ�����Ʒa���20���ڲ鿴�������ף�Ԥ�ⰴ�ոù˿������鿴��Ʒ������1/2�ĸ���������֮���Ƿ���
 * ��ѡԤ�⹺����������������ĸ��ʽ��������ѡ
 */

void solution1(person * arry_person_input,int leng_s_arry_person_input){
	person * arry_person = new person [leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for(int i=0;i<leng_s_arry_person;i++){
		arry_person[i] = arry_person_input[i];
	}

	//������Ʒa���20���ڲ鿴��¼ɾ��{----------------------------------------------------------------------

	for(int i_s=0;i_s<leng_s_arry_person;i_s++){
		int i_brand_id = 0;
		BuyImformation *p_buyim_local = arry_person[i_s].p_buyimformation;
		do{
			int buyday(0),buymonth(0),buybrand_id(0);
			for(;i_brand_id<arry_person[i_s].leng_s_p_buyim;i_brand_id++){
									if(p_buyim_local[i_brand_id].type == 1){
										buybrand_id = p_buyim_local[i_brand_id].brand_id;
										buymonth = p_buyim_local[i_brand_id].visit_datetime_month;
										buyday = p_buyim_local[i_brand_id].visit_datetime_day;
										break;
									}
								}
			if(i_brand_id == arry_person[i_s].leng_s_p_buyim)
				break;
			for(int i=0;i<arry_person[i_s].leng_s_p_buyim;i++){
				int d_gap = day_gap(p_buyim_local[i].visit_datetime_month,p_buyim_local[i].visit_datetime_day,
						buymonth,buyday);
				if(!(d_gap>0 && d_gap <20))
					continue;

				if(p_buyim_local[i].brand_id == buybrand_id && p_buyim_local[i].type == 0){
					//����20���ڲ鿴

					arry_person[i_s].delete_one_p_buy(i);

					if(i<i_brand_id){
						i_brand_id--;
					}
					i--;
				}
			}
			i_brand_id++;
		}while(1);
	}
	//20��end}===========================================================================================

	FILE *fp = fopen("solution1.txt","w");

	double P_person = randoms();//Ԥ������������
	for(int i_s=0;i_s<leng_s_arry_person;i_s++){
		if(randoms()>P_person)
			continue;

		double P_buy = randoms();
		int leng_buylist = 100;
		int leng_s_buylist = 0;
		int *Buylist = new int [leng_buylist];


		BuyImformation *p_s_local = arry_person[i_s].p_buyimformation;
		for(int i_pbi=0;i_pbi<arry_person[i_s].leng_s_p_buyim;i_pbi++){
			//P_buy = randoms();
			if(randoms()>P_buy && p_s_local[i_pbi].type != 1){
				continue;
			}

			if(leng_s_buylist == leng_buylist){
				//����
				int *Buylist_local = new int [leng_buylist];
				for(int i=0;i<leng_buylist;i++){
					Buylist_local[i] = Buylist[i];
				}
				delete [] Buylist;
				Buylist = NULL;
				Buylist = new int [leng_buylist*2];
				for(int i=0;i<leng_buylist;i++){
					Buylist[i] = Buylist_local[i];
				}
				leng_buylist*=2;
				delete [] Buylist_local;
				Buylist_local = NULL;
			}
			Buylist[leng_s_buylist] = p_s_local[i_pbi].brand_id;
			leng_s_buylist++;
	}

		//���ɾ��buylist���ظ�����
		for(int i=0;i<leng_s_buylist;i++){
			for(int j=0;j<leng_s_buylist;j++){
				if(i == j)
					continue;
				if(Buylist[i] == Buylist[j]){
					for(int k=j;k<leng_s_buylist-1;k++){
						Buylist[k] = Buylist[k+1];
					}

					if(j<i)
						i--;
					j--;
					leng_s_buylist--;
				}
			}
		}
		//���
		fprintf(fp,"%d\t",arry_person[i_s].get_person_id());
		for(int i=0;i<leng_s_buylist-1;i++){
				fprintf(fp,"%d,",Buylist[i]);
		}
		fprintf(fp,"%d\n",Buylist[leng_s_buylist-1]);
	}

}
//�㷨2
/*
 * by BSheng
 * ���ü�Ȩƽ��
 * ����������
 * 1.���򿼲��ڣ��������ڹ�����Ʒ��ǰ�ڲ鿴���������磺������P������Ʒa֮ǰ��ǰb�쿪ʼ�鿴
 * 2.���򣨸ţ��ʣ������߶����й��ɵĹ�����Ʒ������/�鿴��Ʒ������
 */

void solution2(person * arry_person_input,int leng_s_arry_person_input){
	person * arry_person = new person [leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for(int i=0;i<leng_s_arry_person;i++){
		arry_person[i] = arry_person_input[i];
	}


	//������Ϊһ����������һ����Ʒ������ʱ���ڲ鿴��Ʒ����Ϣ�ǶԹ�����Ϣû�а����ģ�Ӧ��ɾ��
	/*���Գ��û��Ĺ���ϰ��:
	 * 1.���򿼲��ڣ�һ���û��ڹ���ǰ��һ�����ǰ�����쿪ʼ����
	 * 2.����Ƶ�ʣ��û��ڿ���һ����Ʒʱ�����¶೤ʱ������һ����Ʒ
	 */


}


