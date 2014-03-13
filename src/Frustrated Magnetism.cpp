//============================================================================
// Name        : Frustrated.cpp
// Author      : BSheng  &¡¡YHH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

class BuyImformation{
	int brand_id;
	int type;
	int visit_datetime_month;
	int visit_datetime_day;
};

class person{
private:
	int p_id;


public:
	BuyImformation * p_buyimformation;
	int leng_p_buyim;

	int get_person_id(){
		return p_id;
	}

	~person(){
		delete []p_buyimformation;
	}

};

int main() {
	cout << (101 & 011) << endl; // prints !!!Hello World!!!
	return 0;
}

void read_txt(){

}

