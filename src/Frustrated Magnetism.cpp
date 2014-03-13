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
	int brand_id;
	int type;
	int visit_datetime_month;
	int visit_datetime_day;
};

class person {
private:
	int p_id;

public:
	BuyImformation * p_buyimformation;
	int leng_p_buyim;

	int get_person_id() {
		return p_id;
	}

	~person() {
		delete[] p_buyimformation;
	}

};

int main() {
	cout << (101 & 011) << endl; // prints !!!Hello World!!!
	return 0;
}

void read_txt() {
	FILE* fp;

	if ((fp = fopen("t_alibaba_data.csv", "r")) < 0) {
		printf("open the file is error!\n");
		exit(0);
	}

	int i_line = 0;
	int p_id_pre = -1;
	while (!feof(fp)) {

		int p_id_pre_local, brand_id_local, type_local,
				visit_datetime_day_local, visit_datetime_month_local;

		fscanf(fp, "%d,%d,%d,%d日%d月", p_id_pre_local, brand_id_local,
				type_local, visit_datetime_month_local,
				visit_datetime_day_local);

		i_line++;

		if (p_id_pre == p_id_pre_local) {
			//继续当前用户

		} else {
			//开始新用户

		}

	}
}

