#include<iostream>
using namespace std;

string name_user[2];
double CNIC,birth_date;

int main()
{
	cout<<"Enter your Name: "<<endl;
	getline(cin, name_user[0]);
	cout<<"Enter your Father name: "<<endl;
	getline(cin, name_user[1]);	
	cout<<"Enter your CNIC: "<<endl;
	cin>>CNIC;
	cout<<"Enter your date of birth Without Spaces: "<<endl;
	cin>>birth_date;
	cout<<"\n\nAccount Created Sucessfully"<<endl;
	cout<<name_user[0]<<endl<<name_user[1]<<endl<<CNIC<<endl<<birth_date;

	return 0;
}