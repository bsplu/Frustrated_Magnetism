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




class BuyImformation {
public:
	int brand_id;
	int type; //�����0������1���ղأ�2�����ﳵ��3
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

	//�����ַ
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

	//�Ż�BuyImformation����ʹ�䴢�泤�ȵ������볤�ȣ��Խ�ʡ�ڴ�
	void optimize_p_buy();

	//����Ʒ����BuyImformation�����еĵ�ַλ�ã���0��ʼ������ɾ��
	void delete_one_p_buy(int i_brand){
		//i_brandΪ����λ��
		for (int i = i_brand; i < leng_s_p_buyim - 1; i++) {
			this->p_buyimformation[i] = this->p_buyimformation[i + 1];
		}
		leng_s_p_buyim--;


		this->optimize_p_buy();



	}

	//ɾ����d_month,d_day���ڿ�ʼ֮������ݣ��ú�����ҪĿ�����ñ������ݽ���Ԥ��ʱ����ʹ��
	void delete_by_day(int d_month, int d_day);

	person & operator=(person a) {
		p_id = a.p_id;
		leng_p_buyim = a.leng_p_buyim;
		leng_s_p_buyim = a.leng_s_p_buyim;
		if (a.leng_p_buyim > 0) {
			delete[] p_buyimformation;
			p_buyimformation = NULL;
			p_buyimformation = new BuyImformation[a.leng_p_buyim];
			for (int i = 0; i < leng_s_p_buyim; i++) {
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
/*
person* person:: delete_one_p_buy(int i_brand) {
	//i_brandΪ����λ��
	for (int i = i_brand; i < leng_s_p_buyim - 1; i++) {
		this->p_buyimformation[i] = this->p_buyimformation[i + 1];
	}
	leng_s_p_buyim--;


	optimize_p_buy();
	for(int ii=0;ii<leng_s_p_buyim;ii++){
		cout<<p_buyimformation[ii].brand_id<<endl;
	}

	return this;
}
*/
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








int main() {
	void read_txt(person* &arry_person, int & leng_s_arry_person);
	void solution1(person * arry_person_input, int leng_s_arry_person_input);
	void solution2(person * arry_person_input, int leng_s_arry_person_input);

	srand((unsigned) time(NULL));
	person * arry_person = NULL;
	int leng_s_arry_person = 0;

	//cout<<leng_s_arry_person<<endl;

	read_txt(arry_person, leng_s_arry_person);

	//solution2(arry_person, leng_s_arry_person);



	person & p_local = arry_person[0];
	BubbleSort(p_local.p_buyimformation,arry_person[0].leng_s_p_buyim);
	for(int i=0;i<arry_person[0].leng_s_p_buyim;i++){
		cout<<i<<"\t"<<p_local.p_buyimformation[i].brand_id<<"\t"<<p_local.p_buyimformation[i].visit_datetime_month<<"."<<p_local.p_buyimformation[i].visit_datetime_day<<endl;
	}

	cout<<"======================================================"<<endl;
	arry_person[0].delete_by_day(7,15);
	BubbleSort(p_local.p_buyimformation,arry_person[0].leng_s_p_buyim);
	for(int i=0;i<arry_person[0].leng_s_p_buyim;i++){
		cout<<i<<"\t"<<p_local.p_buyimformation[i].brand_id<<"\t"<<p_local.p_buyimformation[i].visit_datetime_month<<"."<<p_local.p_buyimformation[i].visit_datetime_day<<endl;
	}


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
	int leng_arry_person = 1000; //���鳤��

	leng_s_arry_person = 0; //�Ѵ������
	//cout<<"==================="<<endl;
	person * arry_person = new person[leng_arry_person];

	while (!feof(fp)) {

		int p_id_pre_local, brand_id_local, type_local,
				visit_datetime_day_local, visit_datetime_month_local;

		fscanf(fp, "%d,%d,%d,%d��%d��\n", &p_id_pre_local, &brand_id_local,
				&type_local, &visit_datetime_month_local,
				&visit_datetime_day_local);

		i_line++;

		if (p_id_pre == p_id_pre_local) {
			//������ǰ�û�

			arry_person[leng_s_arry_person - 1].push_p_buy(brand_id_local,
					type_local, visit_datetime_month_local,
					visit_datetime_day_local);

		} else {
			//��ʼ���û�

			//�ж��Ƿ���Ҫ����{-----------------------------------------------------
			if (leng_s_arry_person == leng_arry_person) {
				//���鳤�Ȳ�����Ҫ����
				//������ʱ������и���
				person * arry_person_local = new person[leng_arry_person];
				for (int i = 0; i < leng_arry_person; i++) {
					arry_person_local[i] = arry_person[i];
				}

				//��������Ϊԭ��2��

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
			//�ж�����end}=========================================================

			//���û���ʼ��{---------------------------------------------------------

			p_id_pre = p_id_pre_local;
			arry_person[leng_s_arry_person].change_person_id(p_id_pre);

			arry_person[leng_s_arry_person].allocate_p_buy(100);

			arry_person[leng_s_arry_person].p_buyimformation[0].initialize(
					brand_id_local, type_local, visit_datetime_month_local,
					visit_datetime_day_local);

			leng_s_arry_person++;

			//��ʼ������}===========================================================

			//����һ���û��ڴ�����Ż�{-------------------------------------------------
			//��p_buyimformation��û��ʹ�õĶ��������ɾ����ʹ��leng=leng_s

			if (leng_s_arry_person > 1) {

				//����û���ĳһ��day1��������Ʒa����day1����鿴a����Ϣɾ��

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
								&& p_buyim_local.p_buyimformation[i].type == 0) {
							//����ͬ�칺��鿴������������ɾ��

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
							//һ���ж�ι���һ����ƷҲ��ɾ����

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
			//�Ż�}===============================================================

		}
		//cout<<arry_person[0].p_buyimformation[0].brand_id<<endl;
	}

	//��ȡ���
	//cout<<"��ȡ���"<<endl;
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

//������������֮����������
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

//�㷨1
/*
 * by YHH
 * ������ȫ�����˼��
 *
 * �������ڹ�����Ʒa���20���ڲ鿴�������ף�Ԥ�ⰴ�ոù˿������鿴��Ʒ������1/2�ĸ���������֮���Ƿ���
 * ��ѡԤ�⹺����������������ĸ��ʽ��������ѡ
 */

void solution1(person * arry_person_input, int leng_s_arry_person_input) {
	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}

	//������Ʒa���20���ڲ鿴��¼ɾ��{----------------------------------------------------------------------

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
					//����20���ڲ鿴

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
	//20��end}===========================================================================================

	FILE *fp = fopen("solution1.txt", "w");

	double P_person = randoms();	//Ԥ������������
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
				//����
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

		//���ɾ��buylist���ظ�����
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
		//���
		fprintf(fp, "%d\t", arry_person[i_s].get_person_id());
		for (int i = 0; i < leng_s_buylist - 1; i++) {
			fprintf(fp, "%d,", Buylist[i]);
		}
		fprintf(fp, "%d\n", Buylist[leng_s_buylist - 1]);
	}

}
//�㷨2
/*

 * ���ü�Ȩƽ��
 * ����������
 * 1.���򿼲��ڣ��������ڹ�����Ʒ��ǰ�ڲ鿴���������磺������P������Ʒa֮ǰ��ǰb�쿪ʼ�鿴
 * 2.���򣨸ţ��ʣ������߶����й��ɵĹ�����Ʒ������/�鿴��Ʒ������
 */

void solution2(person * arry_person_input, int leng_s_arry_person_input) {
	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
	}

	//������Ϊһ����������һ����Ʒ������ʱ���ڲ鿴��Ʒ����Ϣ�ǶԹ�����Ϣû�а����ģ�Ӧ��ɾ��
	/*���Գ��û��Ĺ���ϰ��:
	 * 1.���򿼲��ڣ�һ���û��ڹ���ǰ��һ�����ǰ�����쿪ʼ����
	 * 2.����Ƶ�ʣ�ǿ�ȣ��û��ڿ���һ����Ʒʱ�����¶೤ʱ������һ����Ʒ���Լ��û��ڿ���ʱ���쿼�����
	 *
	 * ����̻�������
	 * 1.���ڿ�������d
	 * 2.����ǿ��ƽ��ֵ�������ܴ���/��������
	 * 3.����ǿ�ȷ��Ϊ����ǿ��ƽ��ֵ�ķ����Ӧ���쿼��ǿ��
	 * 4.����ƽ��ֵ������ʱ��๺��ʱ���ƽ��ֵ
	 */

	/*
	 * 2014/3/18
	 * �ָı��뷨����������ֵ���̻�
	 * �������ں͹��������ڲ鿴��������������������ֵ�ķ�����������̬�ֲ������㹺�����
	 */

	int **arry_buy_list = new int *[leng_s_arry_person];
	int *leng_s_a_b_l=new int [leng_s_arry_person];
	int *leng_a_b_l = new int [leng_s_arry_person];


	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		leng_s_a_b_l[i_a_p] = 0;
		leng_a_b_l[i_a_p] = 200;
		arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
		person &p = arry_person[i_a_p];
		//�Ȱ�id�Ÿ���
		BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);
		//test{
		//��һ������һ������������һ�εĿ�����

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

					//�ֱ�����Ϊ�������ξ��й�������
					int T = double(
							day_gap(buy[leng_s_buy - 1].visit_datetime_month,
									buy[leng_s_buy - 1].visit_datetime_day,
									buy[0].visit_datetime_month,
									buy[0].visit_datetime_day))
							/ double(leng_s_buy);
					//��һ����һ�ι����Ƿ���Ԥ��������
					if (day_gap(8, 16, buy[leng_s_buy - 1].visit_datetime_month,
							buy[leng_s_buy - 1].visit_datetime_day) <= T
							&& day_gap(9, 15,
									buy[leng_s_buy - 1].visit_datetime_month,
									buy[leng_s_buy - 1].visit_datetime_day)
									> T) {
						//��Ԥ�����䣬��ӵ������б���
						arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
								buy[0].brand_id;
						leng_s_a_b_l[i_a_p]++;
						//ɾ������Ʒ�Ƶ�ȫ����Ϣ
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

	const int num_kind_arry_pbhbt = 5;
	double *arry_p_b_hbt[num_kind_arry_pbhbt];
	for(int i=0;i<num_kind_arry_pbhbt;i++){
		arry_p_b_hbt[i] = new double [leng_s_arry_person];
	}
	//[0]λ���湺������ƽ��ֵ��[1]λ����������׼�[2]λ���湺�������[3]�鿴����ƽ��ֵ��4Ϊ���׼�
	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {


		for (int i = 0; i < num_kind_arry_pbhbt; i++) {
			arry_p_b_hbt[i][i_a_p] = 0;

		}

		person & p_bimf_l = arry_person[i_a_p];

		for (int i_b = 0; i_b < arry_person[i_a_p].leng_s_p_buyim; i_b++) {
			if (p_bimf_l.p_buyimformation[i_b].type == 1) {
				BuyImformation b_local;
				b_local = p_bimf_l.p_buyimformation[i_b];
				//���֮ǰ�Ƿ����ͬ����Ʒ

				int i_b_b = i_b - 1;	//���к�պ���һ��brand_id����ʼλ��
				int i_b_b_l = 0;
				for (;
						(p_bimf_l.p_buyimformation[i_b_b].brand_id == b_local.brand_id
								&& i_b_b >= 0); i_b_b--) {
					if (p_bimf_l.p_buyimformation[i_b_b].type == 1) {

						i_b_b_l = i_b_b;
					}
				}
				i_b_b_l = i_b_b_l == 0 ? i_b_b + 1 : i_b_b_l + 1;

				//��ʼ��¼ƽ��ֵ
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
				double num_check_rel = double(i_b - i_b_b_l)
						/ double(day_gap_value);
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
	//Ԥ�⽫�Ṻ�����Ʒ
	//f(x)=1/(sqrt(2*Pi)*sigma)*exp(-1*(x-miu)^2/(2*sigma^2))
	//��Ʒ�Ǵ�8��15�ſ�ʼ��Ԥ��
	//x�������ǿ�ʼ�鿴������

	double total = 0;
	double acc = 0;
	for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

		person & p = arry_person[i_a_p];

		int i_brand_e = 0;
		int i_brand_b = 0;
		BuyImformation buy_local;
		bool ifbuy = false;
		for (; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {


			//������Ϊ��4���µ�ʱ���ڣ����һ����Ʒ���ʱ��֮���ǲ����������Ʒ�ģ����������Ʒ���һ���µ���һ�β鿴����Ϣ���
			if(p.p_buyimformation[i_brand_e].type == 1 && p.p_buyimformation[i_brand_b].brand_id == p.p_buyimformation[i_brand_e].brand_id){
				ifbuy = true;
				buy_local = p.p_buyimformation[i_brand_e];
			}

			if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {

				if(ifbuy){
					//ɾ������Ʒ�����һ�����ڼ�����ǰ��ȫ����Ϣ
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
					//��Ʒ������鿴�����ҵ������ҽ��鿴����ͳ�Ƴ���
					//���ڰ�ʱ�����򣬵�һ�����ǿ�ʼ����
					if(compare(p.p_buyimformation[i],p.p_buyimformation[i-1]) == 0){
						num_check--;
					}
				}
				//���㹺�����{---------------------------------------------------------------

				double d_x = double(day_gap(8,16,p.p_buyimformation[i_brand_b].visit_datetime_month,p.p_buyimformation[i_brand_b].visit_datetime_day));
				const double c = 0.159155;
				if(arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[4][i_a_p] > 0.0000001){
					total ++;
					//cout<<arry_p_b_hbt[i_a_p][1]<<"\t"<<arry_p_b_hbt[i_a_p][4]<<endl;
				double P = exp(-0.5*((d_x-arry_p_b_hbt[0][i_a_p])*(d_x-arry_p_b_hbt[0][i_a_p])
						/(arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[1][i_a_p])+(num_check-arry_p_b_hbt[3][i_a_p])*(num_check-arry_p_b_hbt[3][i_a_p])
						/(arry_p_b_hbt[4][i_a_p]*arry_p_b_hbt[4][i_a_p])));

				P /= (arry_p_b_hbt[1][i_a_p]*arry_p_b_hbt[4][i_a_p]);
				P *= c;

				if(randoms()<500*P){
					acc++;
					//����
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
					cout<<i_a_p<<"\t"<<P<<endl;
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
	cout<<"������"<<acc/total<<endl;
}


//��BuyImformation����������brand_id�����ķ�ʽ������ͬid�������ڵ����ķ�ʽ����
void BubbleSort(BuyImformation * pData, int Count) {
	BuyImformation iTemp;
	for (int i = 0; i < Count; i++) {
		for (int j = Count - 1; j > i; j--) {
			if (pData[j].brand_id < pData[j - 1].brand_id) {
				iTemp = pData[j - 1];
				pData[j - 1] = pData[j];
				pData[j] = iTemp;

			} else if (pData[j].brand_id == pData[j - 1].brand_id) {
				//����С����ǰ��
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

//�Ƚ��������ڴ�С��1,-1,0��Ӧ���ڵ��ڣ�С��
int compare(BuyImformation a, BuyImformation b) {
	//1,-1,0��Ӧ���ڵ��ڣ�С��
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

void score(void)
{

}
