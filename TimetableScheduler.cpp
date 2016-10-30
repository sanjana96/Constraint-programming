#include <iostream>
#include <vector> 
#include <queue>
#include <map> 
#include <set>
#include <utility> //Pair
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <bitset>
#include <limits>
#define pb push_back
#define mp make_pair
using namespace std;

//Subtraction for a particular {x,y,z} max times
#define PRcontteacher 5	//Twice
#define PRcont 8	//Twice
#define PRmaxwst 10	//once then keep doing every time maxhrs is exceeded
#define PRdefault 100	//Assigned initially
#define PRteachernon 20	//once
#define PRfixed 25	//once


int noteachers;int secs;
int spd;int nw;int maxwst;

//spd-slots per day, nw-no of working days
int slots;

//3d array with value-0 unassigned, 1-assigned, 2-blocked
bool g[/*noteachers*/100][/*secs*/1000][/*spd*nw*/1000];
int wst[/*noteachers*/100][/*nw*/10];
//wst-working hrs per day for each teacher

int score[/*noteachers*/100][/*secs*/1000][/*spd*nw*/1000];

vector<int> order;



//After each assignment this function is called
void block(int t,int sec,int slot){

    //Avoid collision, same teacher no other section same slot 
	for(int i=0;i<secs;i++)
		score[t][i][slot]=-1;

	//Avoid collision, same section no other teacher same slot 
	for(int i=0;i<noteachers;i++)
		score[i][sec][slot]=-1;

	//No continuous block on either sided
	if(slot%spd!=0){
		for(int i=0;i<noteachers;i++){
			if(score[i][sec][slot-1]!=-1)
				score[i][sec][slot-1]+=PRcont;
		}
		for(int i=0;i<secs;i++){
			score[t][i][slot-1]-=PRcontteacher;
		}
		score[t][sec][slot-1]-=2*PRcont;
	}
	if((slot+1)%spd!=0){
		for(int i=0;i<noteachers;i++){
			if(score[i][sec][slot+1]!=-1)
				score[i][sec][slot+1]+=PRcont;
		}
		for(int i=0;i<secs;i++){
			score[t][i][slot+1]-=PRcontteacher;
		}
		score[t][sec][slot+1]-=2*PRcont;
	}

	//update working hrs of teacher for that day
	wst[t][slot/spd]++;
	//Block rest of the day if teachers max working hrs for that day is reached
	if(wst[t][slot/spd]>=maxwst){
		int day=slot/spd;
		for(int i=0;i<secs;i++){
			for(int j=day*spd;j<day*spd+spd;j++){
				for(int k=0;k<noteachers;k++){
					if(score[k][i][j]!=-1)
						score[k][i][j]+=PRmaxwst;
				}
				score[t][i][j] -=2*PRmaxwst;
			}
		}
	}
}

int firstTwoNonBlocked(int t,int sec){
	int i=0;
	int len=order.size();
	int ans=-1;int maxscore=0;
	for(;i<len;i++){
		int curr=order[i];
		if((curr+1<len) && curr/spd==(curr+1)/spd && score[t][sec][curr]>0 && score[t][sec][curr+1]>0 && score[t][sec][curr]+score[t][sec][curr+1]>maxscore){
//			return curr;
			ans=curr;
			maxscore=score[t][sec][curr]+score[t][sec][curr+1];
		}
		if((curr-1>=0) && curr/spd==(curr-1)/spd && score[t][sec][curr]>0 && score[t][sec][curr-1]>0 && score[t][sec][curr]+score[t][sec][curr-1]>maxscore){
//			return curr-1;
			ans=curr-1;
			maxscore=score[t][sec][curr]+score[t][sec][curr-1];
		}
//		cout<<score[t][sec][curr]<<endl;
	}
//	cout<<t<<" "<<sec<<" "<<ans<<endl;
	return ans;
}
//Finds the first non blocked and not assigned slot
//Search method: check first slot of each day,second slot of each day, so on.. in order to avoid free hrs in the first few hrs
//Returns -1 if no such slot can be found
int firstNonBlocked(int t,int sec){
	int i=0;
	int len=order.size();
	int ans=-1;int maxscore=0;
	for(;i<len;i++){
		int curr=order[i];
		if(score[t][sec][curr]>maxscore){
			ans=curr;
			maxscore=score[t][sec][curr];
		}
	}
//	cout<<t<<" "<<sec<<" "<<ans<<" "<<score[t][sec][ans]<<endl;
	return ans;
}

//Prints Timetable
void printTT(){
//	cout<<g[0][2][5]<<endl;
	for(int i=0;i<secs;i++){
		cout<<"Timetable for section: "<<i+1<<endl;
		for(int j=0;j<slots;j++){
			if(j%spd==0)cout<<endl<<"Day "<<j/spd+1<<": ";
			bool printed=false;
			for(int k=0;k<noteachers;k++){
				if(g[k][i][j]){
					cout<<k<<" ";printed=true;
				}
			}
			if(!printed)cout<<"- ";
		}
		cout<<endl<<endl;
	}
}
int main(){
	//set default unassigned
	memset(g,0,sizeof(g));

	memset(wst,0,sizeof(wst));

	cout<<"No of teachers: ";
	cin>>noteachers;
	cout<<"No of sections: ";
	cin>>secs;
	cout<<"No of slots per day: ";
	cin>>spd;
	cout<<"No of working days per week: ";
	cin>>nw;
	slots=spd*nw;
	cout<<"Maximum working slots per day for each teacher: ";
	cin>>maxwst;

	//Initialize score matrix to a default Priority
	for(int i=0;i<noteachers;i++){
		for(int j=0;j<secs;j++){
			for(int k=0;k<slots;k++){
				score[i][j][k]=PRdefault;
			}
		}
	}
	
	//Pair<a,b> => Teacher a should take section b
	vector<pair<int,int> > al;

	cout<<"Order of filling of slots(from index 0): "<<endl;
	for(int i=0;i<slots;i++){
		int temp;cin>>temp;order.pb(temp);
	}

	cout<<"No of teacher section pairs: ";
	int n;cin>>n;
	cout<<"Enter all pairs(teacher,section,numberPerWeek): "<<endl;
	for(int i=0;i<n;i++){
		int t,s,npw;cin>>t>>s>>npw;
		for(int j=0;j<npw;j++){
			al.pb({t,s});
		}
	}
	cout<<"No of teacher section continuous slots required";
	int contno;cin>>contno;
	cout<<"Enter (teacher,section,numberPerWeek) ofcontinuous slots needed:"<<endl;
	for(int i=0;i<contno;i++){
		int tcont,tsec,tnpw;cin>>tcont>>tsec>>tnpw;
		for(int j=0;j<tnpw;j++){
			al.pb({tcont,tsec});
		}
	}
	

	cout<<"No of teacher slot nonavailability pairs: ";
	int nonavailabilityno;cin>>nonavailabilityno;
	cout<<"Enter all pairs(teacher,slot)"<<endl;
	vector<int,int> nonavail;
	for(int i=0;i<nonavailabilityno;i++){
		int teachernon,slotnon;cin>>teachernon>>slotnon;
		nonavail.pb({teachernon,slotnon});
	}
	for(pair<int,int> temp1:nonavail){
		//Increase score for all teacher,secs for that slot		
		for(int x=0;x<noteachers;x++){
			for(int y=0;y<secs;y++){
				if(nonavail.find(temp1)==nonavail.end())
					score[x][y][slotnon]+=PRteachernon;
			}
		}
		//decrease for all secs for that teacher and slot
		for(int j=0;j<secs;j++){
			score[teachernon][j][slotnon]-=PRteachernon;
		}
	}

	//Fixed slots:
	//tecaher0 takes sec1 on slot5 simlarly 	
	cout<<"No of fixed teacher section slots: ";
	int fixedslotsno;cin>>fixedslotsno;
	cout<<"Enter all fixed slots(teacher,section,slot):"<<endl;
	for(int i=0;i<fixedslotsno;i++){
		int fteacher,fsec,fslot;cin>>fteacher>>fsec>>fslot;
//		block(fteacher,fsec,fslot);
		for(int j=0;j<secs;j++){
			score[fteacher][j][fslot]-=PRfixed;
		}
		for(int j=0;j<noteachers;j++){
			score[j][fsec][fslot]-=PRfixed;
		}
		score[fteacher][fsec][fslot]+=3*PRfixed;
	}
	
//	cout<<g[0][][]<<endl;
/*	for(int j=0;j<secs;j++)
					for(int k=0;k<slots;k++){
						cout<<score[0][j][k]<<" ";
				}
				cout<<endl;*/

	while(!al.empty()){
		//Consider a pair of a teacher and section and allot it 
		pair<int,int> temp = al.back();al.pop_back();
		cout<<"HERE "<<temp.first<<" "<<temp.second;
		if(contno>0){
			int slot = firstTwoNonBlocked(temp.first,temp.second);
		//	cout<<"Here "<<slot<<" "<<temp.first<<" "<<temp.second<<endl;
			//If no slot was found!
			if(slot==-1){
				cout<<"Not possible"<<temp.first<<" "<<temp.second<<endl;
				break;
			}
			//Assign
			g[temp.first][temp.second][slot]=true;
			g[temp.first][temp.second][slot+1]=true;
			//Block slots for considering collisions and other constraints 
			block(temp.first,temp.second,slot);
			block(temp.first,temp.second,slot+1);
			contno--;
		}
		else{
			int slot = firstNonBlocked(temp.first,temp.second);
//			cout<<"Here "<<slot<<" "<<temp.first<<" "<<temp.second<<endl;
//			cout<<g[temp.first][temp.second][slot]<<endl;

			//If no slot was found!
			if(slot==-1){
				cout<<"Not possible"<<endl;break;
			}
			//Assign
			g[temp.first][temp.second][slot]=true;
			//Block slots for considering collisions and other constraints 
			block(temp.first,temp.second,slot);
		}
	}
	printTT();
} 
