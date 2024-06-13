#include "construct_SAIS.hpp"
#include "modified_construct_SAIS.hpp"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

void print_sa(vector<int> v){
	cout << '[';
	for(uint64_t i = 0; i < v.size(); ++i)
		if(i<v.size()-1)
			cout << v[i] << ' ';
		else
			cout << v[i];
	cout  << ']' << endl;
}

int main() {
	
	cout << "Testprogramm für collection:" << endl << endl;
	
	// 0 is the sentinel $ bzw. #
	string text1_1  = "232123321323210"; //string collection:  ABA $   ABBA $   BABA $   #
	string text1_2 = "454145542545430"; //string collection:  ABA $_1 ABBA $_2 BABA $_3 #
	string text2 = "122141421414411310"; //string:  immisismisissiipi$ 
	string text3_1 = "233242132424124210"; //string collection:  immisi $   misis $   isi $   # 
	string text3_2 = "455464154646246430"; //string collection:  immisi $_1 misis $_2 isi $_3 # 
	//ACABACBABCCBA $1 CBAABCAACBACBBA $2 BACABACBABCBAB $3 CBACBAABABCBAB $4 #
	string text4_1 = "2423243234432143223422432433213242324323432314324322323432310";
	string text4_2 = "5756576567765176556755765766526575657656765637657655656765640";
	
	// make string to int vector:	
	vector<int> text_vec1_1(text1_1.length(),-1);
	vector<int> text_vec1_2(text1_2.length(),-1);
	vector<int> text_vec2(text2.length(),-1);
	vector<int> text_vec3_1(text3_1.length(),-1);
	vector<int> text_vec3_2(text3_2.length(),-1);
	vector<int> text_vec4_1(text4_1.length(),-1);
	vector<int> text_vec4_2(text4_2.length(),-1);
	for(int i = 0; i < text1_1.length(); i++) text_vec1_1[i] = (int) text1_1[i] - (int) '0';
	for(int i = 0; i < text1_2.length(); i++) text_vec1_2[i] = (int) text1_2[i] - (int) '0';
	for(int i = 0; i < text2.length(); i++) text_vec2[i] = (int) text2[i] - (int) '0';
	for(int i = 0; i < text3_1.length(); i++) text_vec3_1[i] = (int) text3_1[i] - (int) '0';
	for(int i = 0; i < text3_2.length(); i++) text_vec3_2[i] = (int) text3_2[i] - (int) '0';
	for(int i = 0; i < text4_1.length(); i++) text_vec4_1[i] = (int) text4_1[i] - (int) '0';
	for(int i = 0; i < text4_2.length(); i++) text_vec4_2[i] = (int) text4_2[i] - (int) '0';


	vector<int> sa1_1= construct(text_vec1_1, 0);
	cout << "############" << endl;
	cout << "SA of text 1" << endl;
	print_sa(sa1_1);
	cout << endl;

	vector<int> sa1_2 = construct(text_vec1_2, 0);
	cout << "############" << endl;
	cout << "SA of text 2" << endl;
	print_sa(sa1_2);
	cout << endl;

	vector<int> sa1_3 = modified_construct(text_vec1_1, 0);
	cout << "############" << endl;
	cout << "SA of text 3" << endl;
	print_sa(sa1_3);
	cout << "(has to mach text 2: " << equal(sa1_2.begin(), sa1_2.end(), sa1_3.begin()) << ")" << endl; // compare if results equal
	cout << endl;

	vector<int> sa3_2 = construct(text_vec3_2, 0);
	cout << "############" << endl;
	cout << "SA of text 4" << endl;
	print_sa(sa3_2);
	cout << endl;

	vector<int> sa3_1 = modified_construct(text_vec3_1, 0);
	cout << "############" << endl;
	cout << "SA of text 5" << endl;
	print_sa(sa3_1);
	cout << "(has to mach text 4: " << equal(sa3_2.begin(), sa3_2.end(), sa3_1.begin()) << ")" << endl; // compare if results equal
	cout << endl;

	vector<int> sa4_2 = construct(text_vec4_2, 0);
	cout << "############" << endl;
	cout << "SA of text 6" << endl;
	print_sa(sa4_2);
	cout << endl;

	vector<int> sa4_1 = modified_construct(text_vec4_1, 0);
	cout << "############" << endl;
	cout << "SA of text 7" << endl;
	print_sa(sa4_1);
	cout << "(has to mach text 6: " << equal(sa3_2.begin(), sa3_2.end(), sa3_1.begin()) << ")" << endl; // compare if results equal
	cout << endl;

	return 0;
}
