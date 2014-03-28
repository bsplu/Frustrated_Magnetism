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



//��b_month,b_day��ʼ�Ĺ������Ʒid���س���
void chose_kind_from_day(int b_month, int b_day,int e_month,int e_day,person * arry_in,int len_arry_in,
		int ** &arry_buy_list,int &len_arry_buy_list,int *&len_arry_buy_list_ev,int kind){

	if(kind !=1 && kind != 2 && kind !=3 && kind !=0){
		cout<<"kind erro"<<endl;
		return ;
	}

	len_arry_buy_list = 0;
	int *len_a_b_l_re = new int [len_arry_in];//����arry_buy_listʵ��ÿһ�е���������
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
			//������ڴ���ѡ��ʼ���ڣ�����type==1����빺������
			if(compare(com_local_b,arry_in[i].p_buyimformation[j]) <0 && compare(com_local_e,arry_in[i].p_buyimformation[j]) >0 && arry_in[i].p_buyimformation[j].type == kind){
				//���len_arry_buy_list_ev[len_arry_buy_list]Ϊ0,˵����û�д���user����id���ȴ���user_id
				if(len_arry_buy_list_ev[len_arry_buy_list] == 0){
					arry_buy_list[len_arry_buy_list][0] = arry_in[i].get_person_id();
					arry_buy_list[len_arry_buy_list][1] = arry_in[i].p_buyimformation[j].brand_id;
					len_arry_buy_list_ev[len_arry_buy_list] = 2;
				}else{
					//����id֮ǰ�Ƿ���ӹ�{----------------------------------------------------------------------
					int k=0;
					for(;k<len_arry_buy_list_ev[len_arry_buy_list];k++){
						if(arry_buy_list[len_arry_buy_list][k] == arry_in[i].p_buyimformation[j].brand_id){
							break;
						}
					}
					//end}==================================================================================
					if(k == len_arry_buy_list_ev[len_arry_buy_list]){
						//������˵��û�б�����
					arry_buy_list[len_arry_buy_list][len_arry_buy_list_ev[len_arry_buy_list]] = arry_in[i].p_buyimformation[j].brand_id;
					len_arry_buy_list_ev[len_arry_buy_list] ++;

					//�ж��Ƿ���Ҫ����{-----------------------------------------------------------------------
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
					//����end}==============================================================================
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

//���arry���Ƿ����ظ�����Ʒ���еĻ��޳�
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
								&& (p_buyim_local.p_buyimformation[i].type == 0 || p_buyim_local.p_buyimformation[i].type == 3 )  ) {
							//����ͬ�칺�� �鿴�ӹ��ﳵ������������ɾ��

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

void solution2(person * arry_person_input, int leng_s_arry_person_input,int f_m_b=8,int f_d_b=16,
		int f_m_e=9,int f_d_e=15) {

	person * arry_person = new person[leng_s_arry_person_input];
	int leng_s_arry_person = leng_s_arry_person_input;
	for (int i = 0; i < leng_s_arry_person; i++) {
		arry_person[i] = arry_person_input[i];
		arry_person[i].delete_by_day(f_m_b,f_d_b);
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
		//{----------------------------------------------------------------------------------------
		//��һ������һ������������һ�εĿ�����

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

				i_brand_b = i_brand_e;
				i_brand_e--;

			}

		}

		//�ڶ��жϽ׶Σ����һ����


		//�����ظ�ģ��end}=================================================================================


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
	cout<<"������"<<acc/total<<endl;

	if(!(f_m_b == 8 && f_d_b == 16)){
		//��������Ԥ�з�ϵͳ
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 =0;
		//���ж�һ��len_arry1�Ĵ�С
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
		//{----------------------------------------------------------------------------------------
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
					if (day_gap(f_m_b, f_d_b, buy[leng_s_buy - 1].visit_datetime_month,
							buy[leng_s_buy - 1].visit_datetime_day) <= T
							&& day_gap(f_m_e, f_d_e,
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
     //�����ظ�ģ��end}=================================================================================

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
				/*
				for(int i=i_brand_b+1;i<i_brand_e;i++){
					//��Ʒ������鿴�����ҵ������ҽ��鿴����ͳ�Ƴ���
					//���ڰ�ʱ�����򣬵�һ�����ǿ�ʼ����
					if(compare(p.p_buyimformation[i],p.p_buyimformation[i-1]) == 0){
						num_check--;
					}
				}
				*/
				//���㹺�����{---------------------------------------------------------------

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
	cout<<"������"<<acc/total<<endl;

	if(!(f_m_b == 8 && f_d_b == 16)){
		//��������Ԥ�з�ϵͳ
		int ** arry1;
		int * len_arry1_ev = NULL;
		int len_arry1 =0;
		//���ж�һ��len_arry1�Ĵ�С
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

		double * P_buy = new double [leng_s_arry_person];//һ���˵Ĺ����ʣ������Ʒ����/������Ʒ����

		int ** arry2;
					int * len_arry2_ev = NULL;
					int len_arry2 =0;

					chose_kind_from_day(f_m_b,f_d_b,f_m_e,f_d_e,arry_person_input,leng_s_arry_person_input,arry2,len_arry2,len_arry2_ev,1);


		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

			leng_s_a_b_l[i_a_p] = 0;
			leng_a_b_l[i_a_p] = 1000;
			arry_buy_list[i_a_p] = new int[leng_a_b_l[i_a_p]];
			person &p = arry_person[i_a_p];
			//�Ȱ�id�Ÿ���
			BubbleSort(p.p_buyimformation, arry_person[i_a_p].leng_s_p_buyim);


			//{----------------------------------------------------------------------------------------
			//��һ������һ������������һ�εĿ�����


			double buy_num_total = 0;//һ������Ĵ���
			double buy_num_again = 0;//��ͷ����
			double check_num_total = 0;//һ����������Ʒ��

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

					i_brand_b = i_brand_e;
					i_brand_e--;

				}
				P_buy[i_a_p] = buy_num_total/check_num_total;
			}


			//�ڶ��жϽ׶Σ��ж�һ�����������Ʒ��ڶ��εĿ�����
			/*
			double P_again = (buy_num_total >2)? buy_num_again/buy_num_total:0;//��ͷ����
			//���һ���˻�ͷ�ʱȽϸߣ�������Ϊ���������Ʒ������
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
			//�����ظ�ģ��end}=================================================================================
/*
			//�ղ�ģ��{--------------------------------------------------------------------------------------
			//�鿴�Ƿ��ղ���Ʒ����Ϊ�ղ���Ʒ�Ṻ��
			for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
				if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
					//��ѭ����ÿһ��Ʒ�ƣ�i_brand_b����ʼֵ��i_brand_e�ǽ���ֵ��������i_brand_e)

					//���һ��Ʒ�Ʒ����˹�����Ϊ�����ǽ���Ϊ�䲻���ٹ���{------------------------------------------
					bool ifbuy_local = false;
					for (int i = i_brand_b; i < i_brand_e; i++) {
						if (p.p_buyimformation[i].type == 1) {
							ifbuy_local = true;
							break;
						}
					}
					if(ifbuy_local)
						continue;
					//�жϹ������}======================================================================

					BuyImformation buy;


					for (int i = i_brand_b; i < i_brand_e; i++) {
						if (p.p_buyimformation[i].type == 2) {

								buy = p.p_buyimformation[i];
								break;
						}
					}

					if (buy.type == 2) {

							//��Ԥ�����䣬��ӵ������б���
							arry_buy_list[i_a_p][leng_s_a_b_l[i_a_p]] =
									buy.brand_id;
							leng_s_a_b_l[i_a_p]++;

							//ɾ������Ʒ�Ƶ�ȫ����Ϣ
							for (int i = i_brand_b; i < i_brand_e; i++) {
								arry_person[i_a_p].delete_one_p_buy(i_brand_b);
							}
							i_brand_e = i_brand_b;


					}

					i_brand_b = i_brand_e;
					i_brand_e--;

				}

			}

		//�ղ�ģ��}=======================================================================================
*/




		}




		const int num_kind_arry_pbhbt = 6;
		double *arry_p_b_hbt[num_kind_arry_pbhbt];
		for(int i=0;i<num_kind_arry_pbhbt;i++){
			arry_p_b_hbt[i] = new double [leng_s_arry_person];
		}
		//[0]λ���湺������ƽ��ֵ��[1]λ����������׼�[2]λ���湺�������[3]�鿴����ƽ��ֵ��4Ϊ���׼�5Ϊ����
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
					//day_gap_value1�����鿴����
					double day_gap_value1 = day_gap(p_bimf_l.p_buyimformation[i_b].visit_datetime_month,
							p_bimf_l.p_buyimformation[i_b].visit_datetime_day,
							p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_month,
							p_bimf_l.p_buyimformation[i_b_b_l].visit_datetime_day);

					//int day_gap_value2�����м���鿴
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
				double day_gap_value2 = 0;					//ʵ�ʲ鿴����
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



		//������ģ��{---------------------------------------------------------------------------------------
		/*
		 * ���ù������жϵĽ������  3%
		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

			if(false){
				person &p = arry_person[i_a_p];
				for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
					//�ӹ������һ����ʼ
					if(p.p_buyimformation[i_brand_e].type == 1){
						i_brand_b = i_brand_e+1;
						continue;
					}

							if (p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
								double day_gap_value2 = 0;//ʵ�ʲ鿴����
								for(int i_check_d = i_brand_b+1;i_check_d<i_brand_e;i_check_d++){
									if(compare(p.p_buyimformation[i_check_d],p.p_buyimformation[i_check_d-1]) != 0){
										day_gap_value2++;
									}
								}
								if(day_gap_value2 != 0 && double(i_brand_e-i_brand_b)/day_gap_value2 > 3.0/sqrt(P_buy[i_a_p])*arry_p_b_hbt[3][i_a_p]
								                                                                                           &&
								 day_gap(f_m_b,f_d_b,p.p_buyimformation[i_brand_b].visit_datetime_month,p.p_buyimformation[i_brand_b].visit_datetime_day)< arry_p_b_hbt[0][i_a_p] + 0.5*arry_p_b_hbt[1][i_a_p])
								{	//��ӵ������б���
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
		//������ģ��}=======================================================================================

		//ֱ�Ӽ��鿴����{---------------------------------------------------------------------------------

		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {
			person &p = arry_person[i_a_p];


				for (int i_brand_e = 0,i_brand_b = 0; i_brand_e < arry_person[i_a_p].leng_s_p_buyim; i_brand_e++) {
					//�ӹ������һ����ʼ
					if(i_brand_e != 0 && (p.p_buyimformation[i_brand_e-1].brand_id == p.p_buyimformation[i_brand_e].brand_id) && p.p_buyimformation[i_brand_e].type == 1){
						i_brand_b = i_brand_e+1;
						continue;
					}

					if (i_brand_b >0 && p.p_buyimformation[i_brand_b-1].brand_id == p.p_buyimformation[i_brand_b].brand_id &&
							p.p_buyimformation[i_brand_b].brand_id != p.p_buyimformation[i_brand_e].brand_id) {
													//���һ�����ڼ����ڲ鿴ͻ����ʵĻ���˵����Ҫ����
													double num_over_check_av = 0;
													for (int i_brand_e_d = i_brand_b,i_brand_b_d = i_brand_b; i_brand_e_d < i_brand_e; i_brand_e_d++) {
														double num_one_day = 0;
																if (compare(p.p_buyimformation[i_brand_b_d], p.p_buyimformation[i_brand_e_d]) != 0) {
																	//����鿴����
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
								//���һ�����ڼ����ڲ鿴ͻ����ʵĻ���˵����Ҫ����

								double num_over_check_av = 0;
								for (int i_brand_e_d = i_brand_b,i_brand_b_d = i_brand_b; i_brand_e_d < i_brand_e; i_brand_e_d++) {
									double num_one_day = 0;
											if (compare(p.p_buyimformation[i_brand_b_d], p.p_buyimformation[i_brand_e_d]) != 0) {
												//����鿴����
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


		//�鿴����}=====================================================================================






		for (int i_a_p = 0; i_a_p < leng_s_arry_person; i_a_p++) {

			person &p = arry_person[i_a_p];


		//���һ��i_a_p�û��ڵ��ĸ����Ƿ�������
		int i_find_u = 0;
		for(;i_find_u<len_arry2;i_find_u++){
			if(arry_person[i_a_p].get_person_id() == arry2[i_find_u][0])
				break;
		}

		//�������
		if(i_find_u != len_arry2){
			cout<<"����:"<<arry_p_b_hbt[0][i_a_p]<<"\t��׼��"<<arry_p_b_hbt[1][i_a_p]<<"\tƽ������:"<<arry_p_b_hbt[3][i_a_p]<<"\t��׼��:"<<arry_p_b_hbt[4][i_a_p]<<endl;
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
						cout<<"�鿴����1:"<<d_g<<"\t����2��"<<day_gap_value2<<"\t����:"<<double(i_brand_e-i_brand_b)<<"\t1:"<<double(i_brand_e-i_brand_b)/d_g
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


		//��buy_listת��Ϊarry1��ʽ����{-----------------------------------------------------------------------
		//���ж�һ��len_arry1�Ĵ�С
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

		//ת��end}=========================================================================================

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


	const double C1 = 0.0;//��������û���û�н���ȡ0.5����
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
	//����ÿһ����ɸѡ
#pragma omp parallel for
	for(int i_a=0;i_a<len_userid;i_a++){

		//����ÿһ����Ʒ
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


	//��buy_listת��Ϊarry1��ʽ����{-----------------------------------------------------------------------
	//���ж�һ��len_arry1�Ĵ�С
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

	//ת��end}=========================================================================================

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
	int num_find_total = 0;//�Ѿ��ҵ���

	//���ҵ���һ������Ϊ��ʼ

				arry_connect_local[0][0] = 0;
				len_a_p_c_l[0] = 1;
				num_save_s = 1;
				num_find_total++;
				if(arry_person[0].leng_s_p_buyim != 0)
					arry_person[0].leng_s_p_buyim *= -1;
				else
					arry_person[0].leng_s_p_buyim = -1;


	//��ʼ����
	do{
		if(num_find_total == len_arry_person)
			break;

		if(len_a_p_c_l[num_save_s-1] == len_a_p_c_ln[num_save_s-1]){
			//˵��һ������Ѿ����꣬��ʼ��һ����
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

		//������checkid��Ӧ��������ϵ����
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

			//�����ж�һ�¸�user�Ƿ��Ѽ���С��
			if(arry_person[i].leng_s_p_buyim < 0 )
				continue;

			//���û�û�м��룬�ж�һ���Ƿ���Ҫ����
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


	//��arry_connect_localת��Ϊ���û����淽��鿴
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





double* score(int ** arry1, int *len_arry1_ev, int len_arry1, int ** arry2, int *len_arry2_ev, int len_arry2)
//����һ��3λ�����ָ�룬��һλ�������ʣ��ڶ�λ���ٻ��ʣ�����λ�Ƿ���������double��
{


	int hit = 0,hitu=0;
	//hit�洢Ʒ����������hitu�洢�����û���

	int pBrand = 0, bBrand = 0;
	//pBrand�洢Ԥ��Ʒ��������bBrand�洢ʵ��Ʒ������

	int* hituser1 = new int[len_arry1];
	//hituser�洢�����û���1�е���ţ�����id

	int* hituser2 = new int[len_arry1];
	//hituser�洢�����û���2�е���ţ�����id

	double* prst=new double[3];

	for (int i = 0; i < len_arry1; i++)
	//����1
	{
		for (int j = 0; j < len_arry2; j++)
		//ÿ��1������2�Ƚ�
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
	//�������������û�����ʱi����hituser1/2�е���ţ�hituser1/2[i]����user id��1��2�е����
	{
		for (int j = 1; j < len_arry1_ev[hituser1[i]]; j++)
		//����ĳ�����û���Ԥ��Ʒ��
		{

			for (int k = 1; k < len_arry2_ev[hituser2[i]]; k++)
			//�Ƚ�Ԥ��Ʒ�ƺ�����ʵ��Ʒ��
			{

				if (arry1[hituser1[i]][j] == arry2[hituser2[i]][k])
				{
					hit++;
				}
			}
		}
	}

	for (int i = 0; i < len_arry1; i++)
	//����������
	{
		pBrand += len_arry1_ev[i]-1;
	}

	for (int i = 0; i < len_arry2; i++)
	//�����ٻ���
	{
		bBrand += len_arry2_ev[i]-1;
	}

	prst[0] = (double(hit) / double(pBrand));
	//������
	prst[1] = (double(hit) / double(bBrand));
	//�ٻ���

	cout<<"hit="<<hit<<"\tpBrand="<<pBrand<<"\tbBrand="<<bBrand<<endl;
	prst[2] = (2.0 * prst[0] * prst[1] / (prst[0] + prst[1]));
	//�÷�

	delete [] hituser1;
	delete [] hituser2;

	return prst;
}
