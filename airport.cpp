
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<math.h>
#include <iomanip>
using namespace std;

#define Capacity 10000

int number[2]={0,1};
int rear[6]={0,2,2,2,2,0};
int crash=0;
int emergency=0;
int randland,randtake;
void allocate(int **r,int IDnumber)
{
	//landing
	if(IDnumber%2==1)
	{
	
		if	(r[2][0]+r[2][1]<=r[3][0]+r[3][1] && r[2][0]+r[2][1]<=r[4][0]+r[4][1])
		{
			rear[2]++;
			if(r[2][0]<=r[2][1]) {r[2][rear[2]]=IDnumber;r[2][0]++;}
			else {r[2][rear[2]]=-1*IDnumber; r[2][1]++;}		
		}
		else if(r[3][0]+r[3][1]<=r[2][0]+r[2][1] && r[3][0]+r[3][1]<=r[4][0]+r[4][1])
		{
			rear[3]++;
			if(r[3][0]<=r[3][1]) {r[3][rear[3]]=IDnumber;r[3][0]++;}
			else {r[3][rear[3]]=-1*IDnumber; r[3][1]++;}		
		}
		else
		{
			rear[4]++;
			if(r[4][0]<=r[4][1]) {r[4][rear[4]]=IDnumber;r[4][0]++;}
			else {r[4][rear[4]]=-1*IDnumber; r[4][1]++;}	//cin Idnumber into a landing queue,add queue size
		}	
	}
	//takeoff/
	else  
	{
		
		
		if(r[1][2]<=r[3][2] && r[1][2]<=r[2][2] && r[1][2]<=r[1][2])
		{
			rear[1]++;
			r[1][rear[1]]=IDnumber;r[1][2]++;
		}
		
		else if(r[4][2]<=r[3][2] && r[4][2]<=r[2][2] && r[4][2]<=r[1][2])
		{
			rear[4]++;
			r[4][rear[4]]=IDnumber;r[4][2]++;
		}
		
		else if(r[3][2]<=r[4][2] && r[3][2]<=r[2][2] && r[3][2]<=r[1][2])
		{
			rear[3]++;
			r[3][rear[3]]=IDnumber;r[3][2]++;
		}
		
		else if(r[2][2]<=r[4][2] && r[2][2]<=r[3][2] && r[2][2]<=r[1][2])
		{
			rear[2]++;
			r[2][rear[2]]=IDnumber;r[2][2]++;
		}
		
	}			
}


void newplane(int **r,bool *ID,int *Flevel,int *number)//產生新飛機並分配到跑道裡 
{
	cout<<"Step 1:\n"<<"landing plane: ";
	for(int i=0;i<randland;i++)
	{
		ID[number[1]]=1;
		
		allocate(r,number[1]);	
		Flevel[number[1]]= rand()%9 +1;
		cout<<" ("<<number[1]<<", "<<Flevel[number[1]]<<"),";
		number[1]+=2;
	}
	cout<<'\n'<<'\n';
	for(int i=1;i<=4;i++)
	{
		cout<<"Runway"<<i<<"(-1)\nL1:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]>0 && r[i][m]%2==1) cout<<" ("<<r[i][m]<<", "<<Flevel[r[i][m]]<<"),";
		cout<<"\nL2:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]<0) cout<<" ("<<-1*r[i][m]<<", "<<Flevel[-1*r[i][m]]<<"),";
		cout<<"\nT:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]%2==0) cout<<" ("<<r[i][m]<<"),";
		cout<<'\n'<<'\n';
	}
	
	cout<<"Step 2:\ntakeoff plane:";
	for(int i=0;i<randtake;i++) 
	{
		ID[number[0]]=1;
		allocate(r,number[0]);
		cout<<"("<<number[0]<<"), ";
		Flevel[number[0]]=-1;	
		number[0]+=2;
	}
	cout<<'\n'<<'\n';
	for(int i=1;i<=4;i++)
	{
		cout<<"Runway"<<i<<"(-1)\nL1:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]>0 && r[i][m]%2==1) cout<<" ("<<r[i][m]<<", "<<Flevel[r[i][m]]<<"),";
		cout<<"\nL2:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]<0) cout<<" ("<<-1*r[i][m]<<", "<<Flevel[-1*r[i][m]]<<"),";
		cout<<"\nT:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]%2==0) cout<<" ("<<r[i][m]<<"),";
		cout<<'\n'<<'\n'<<'\n';
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
int  pop(int **r,bool *ID,int element,int row,int *rear)//element是在array中的位置 
{
	ID[abs(r[row][element])]=0;
	int target=r[row][element];//error
	if(element>=3)
	{
		if(target<0) r[row][1]--;
		else if(target%2==1) r[row][0]--;
		else r[row][2]--;
	
	 	
	}
	for(int i=element;i<rear[row];i++) r[row][i]=r[row][i+1];//move
		{
			rear[row]--;
			
		}
	
	
	return target;
}


void chooseway(int **r,bool *ID,int *Flevel,int *rear)
{
	//landing
	for(int i=0;i<5;i++) r[0][i]=0;//record if runway is full
	int nofuel=0;
	int warning =0;
	int record[100]={0};//暫存nofuel id 
	int out[100]={0};//record out id
	for(int i=1;i<=4;i++)
	{
		for(int j=3;j<=rear[i];j++)//check how many plane no fuel
		{
			if(abs(r[i][j])%2==1 && Flevel[abs(r[i][j])]<=2) warning++;
			if(abs(r[i][j])%2==1 && Flevel[abs(r[i][j])]==0)
			{
				record[nofuel]=10*j+i;
				nofuel++;
			}
		}
	}
	
	emergency+=nofuel;
	
	cout<<"Step 3:\nemergency plane:";
	for(int i=0;i<nofuel;i++)
	{
		cout<<abs(r[record[i]%10][record[i]/10])<<", ";
	}
	cout<<'\n'<<'\n';
	
	
	if(nofuel==1) 		
	{
		int a=record[0];
		ID[abs(r[a%10][a/10])]=0;
		out[1]=pop(r,ID,a/10,a%10,rear);
		r[0][1]=1;
	
		
	}
	if(nofuel==2)
	{
		
		int a=record[0];
		int b=record[1];
		
		ID[abs(r[a%10][a/10])]=0;
		ID[abs(r[b%10][b/10])]=0;
		
		out[1]=pop(r,ID,a/10,a%10,rear);
		
		
		if(a%10==b%10) b-=10;
		
		int outt=pop(r,ID,b/10,b%10,rear);
		
			
		if(r[2][0]+r[2][1]<=r[3][0]+r[3][1] && r[2][0]+r[2][1]<=r[4][0]+r[4][1])		
			{r[0][1]=r[0][2]=1;
			 out[2]=outt;
		
			}
		
		else if(r[3][0]+r[3][1]<=r[2][0]+r[2][1] && r[3][0]+r[3][1]<=r[4][0]+r[4][1])		
			{r[0][1]=r[0][3]=1;
			out[3]=outt;
		
			}
		
		else 
			{ r[0][1]=r[0][4]=1;
			  out[4]=outt;
			
			}
			
	}
	
	if(nofuel==3)
	{
		int a=record[0];
		
		int b=record[1];
		int c=record[2];
		ID[abs(r[a%10][a/10])]=0;
		ID[abs(r[b%10][b/10])]=0;
		ID[abs(r[c%10][c/10])]=0;
		int out1=pop(r,ID,a/10,a%10,rear);
		if(a%10==b%10) b-=10;
		int out2=pop(r,ID,b/10,b%10,rear);
		if(a%10==c%10) c-=20;
		else if(b%10==c%10) c-=10;//修正c/10 
		int out3=pop(r,ID,c/10,c%10,rear);
		out[1]=out1;
		
		if(r[4][0]+r[4][1]>=r[2][0]+r[2][1] && r[4][0]+r[4][1]>=r[3][0]+r[3][1])		
		{
			r[0][2]=r[0][1]=r[0][3]=1;
			out[2]=out2;
			out[3]=out3;
		
		}	
		
		else if(r[3][0]+r[3][1]>=r[2][0]+r[2][1] && r[3][0]+r[3][1]>=r[4][0]+r[4][1])		
			{
				r[0][1]=r[0][2]=r[0][4]=1;
				out[2]=out2;
				out[4]=out3;
			
			}
		
		else {
			r[0][1]=r[0][3]=r[0][4]=1;	
			out[3]=out2;
			out[4]=out3;
		
		}
	}
	if(nofuel==4)
	{
		int a=record[0];
		int b=record[1];
		int c=record[2];
		int d=record[3];
		ID[abs(r[a%10][a/10])]=0;
		ID[abs(r[b%10][b/10])]=0;
		ID[abs(r[c%10][c/10])]=0;
		ID[abs(r[d%10][d/10])]=0;
		
		if(a%10==b%10&&b%10==c%10&&c%10==d%10)
		{
			b-=10;
			c-=20;
			d-=30;
		}
		else if(a%10==b%10&&b%10==c%10&&c%10!=d%10)
		{
			b-=10;
			c-=20;
		}
		else if(a%10==b%10&&b%10!=c%10&&c%10==d%10)
		{
			b-=10;
			d-=10;
		}
		else if(a%10==b%10&&b%10!=c%10&&c%10!=d%10)
		{
			b-=10;
		}
		else if(a%10!=b%10&&b%10==c%10&&c%10==d%10)
		{
			c-=10;
			d-=20;
		}
		else if(a%10!=b%10&&b%10!=c%10&&c%10==d%10)
		{
			d-=10;
		}
		else if(a%10!=b%10&&b%10==c%10&&c%10!=d%10)
		{
			c-=10;
		}
		
		int out1=pop(r,ID,a/10,a%10,rear);
		 
		int out2=pop(r,ID,b/10,b%10,rear);
		int out3=pop(r,ID,c/10,c%10,rear);
		int out4=pop(r,ID,d/10,d%10,rear);
		out[1]=out1;
		out[2]=out2;
		out[3]=out3;
		out[4]=out4;
		
		r[0][1]=r[0][2]=r[0][3]=r[0][4]=1;
	}
	for(int i=1;i<=4;i++)
	{
		cout<<"Runway"<<i<<"(";
		if(out[i]!=0) cout<<abs(out[i]);
		else cout<<"-1";
		cout<<")\nL1:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]>0 && r[i][m]%2==1) cout<<" ("<<r[i][m]<<", "<<Flevel[r[i][m]]<<"),";
		cout<<"\nL2:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]<0) cout<<" ("<<-1*r[i][m]<<", "<<Flevel[-1*r[i][m]]<<"),";
		cout<<"\nT:";
		for(int m=3;m<=rear[i];m++) if(r[i][m]%2==0) cout<<" ("<<r[i][m]<<"),";
		cout<<'\n'<<'\n';
	}
	
	if(nofuel>=5)	
	{
	cout<<"crash\n";
		/*for(int i=0;i<nofuel;i++) 
		{
			ID[abs(r[record[i]%10][record[i]/10])]=0;
			out[i+1]=abs(pop(r,ID,record[i]/10,record[i]%10,rear));
			r[0][1]=r[0][2]=r[0][3]=r[0][4]=1;
		
		}	
		*/
		crash+=(nofuel-4);
		exit (0);
	}
	
	if(warning>=4)
	{
		for(int i=1;i<=4;i++)
		
		{
			if(r[0][i-1]==0)
			{
				for(int j=3;j<=rear[i];j++)
				{
					if(r[i][j]%2!=0) out[i]=abs(pop(r,ID,j,i,rear));
				}
			}
		}
	}
	//input last plane
	cout<<"Step 4:\n\n\n";	
	for(int k=1;k<=4;k++)
	{
		cout<<"Runway"<<k<<"(";
		if(r[0][k]==1) cout<<abs(out[k]);
		else if(r[k][0]+r[k][1]+r[k][2]==0) cout<<"-1";
		else if(r[0][k]==0&&(r[k][0]+r[k][1]+r[k][2]>0))
		{
			cout<<abs(pop(r,ID,3,k,rear));	
			out[k]=r[k][3];
		}
		
		cout<<")\n";
		cout<<"L1:";
		for(int m=3;m<=rear[k];m++) if(r[k][m]>0 && r[k][m]%2==1) cout<<" ("<<r[k][m]<<", "<<Flevel[r[k][m]]<<"),";
		cout<<"\nL2:";
		for(int m=3;m<=rear[k];m++) if(r[k][m]<0) cout<<" ("<<-1*r[k][m]<<", "<<Flevel[-1*r[k][m]]<<"),";
		cout<<"\nT:";
		for(int m=3;m<=rear[k];m++) if(r[k][m]%2==0) cout<<" ("<<r[k][m]<<"),";
		cout<<'\n'<<'\n';
	}
}


int main ()
{
	srand(time(NULL));
	
	int T;
	int Flevel[Capacity]={0};
	bool ID[Capacity]={0};//record the ID if is still in queue or poped
	int Time[Capacity]={0};//Time[1~N]=wating/landing  time
	int runway[5][Capacity]={0};//runway[跑道][1~N]=ID 
	int *r[5]={runway[0],runway[1],runway[2],runway[3],runway[4]};
	//runway[0][p] records if runway p has choosen a plane to land or take off
	//runway[1~4][0] records left landing queue size
	//runway[1~4][1] records right landing queue size
	//runway[1~4][2] records takeoff queue size
	//runway[1~4][3~] records ID in this runway
	cout<<"How Many Time Unit You Want To Simulate:";
	cin>>T;
	for(int k=1;k<=T;k++)
	{		
		
		
		randland=rand()%5;
		randtake=rand()%4;
		
		if(k!=1 )
		{ 
			for(int i=2;i<=4;i++)
				{
				for(int j=3;j<=rear[i];j++)
				{
					if(ID[abs(r[i][j])]==1&&r[i][j]%2!=0)  Flevel[abs(r[i][j])]--;
				}
				}
			for(int i=0;i<=max(number[0],number[1]);i++) 
			{
				if(ID[i]==1) Time[i]++;	
			}
		}
		
		cout<<"Simulation Time: "<<k<<"\n\n\n";
		for(int i=1;i<=4;i++)
		{
			cout<<"Runway"<<i<<"(-1)\nL1:";
			for(int m=3;m<=rear[i];m++) if(r[i][m]>0 && r[i][m]%2==1) cout<<" ("<<r[i][m]<<", "<<Flevel[r[i][m]]<<"),";
			cout<<"\nL2:";
			for(int m=3;m<=rear[i];m++) if(r[i][m]<0) cout<<" ("<<-1*r[i][m]<<", "<<Flevel[-1*r[i][m]]<<"),";
			cout<<"\nT:";
			for(int m=3;m<=rear[i];m++) if(r[i][m]%2==0) cout<<" ("<<r[i][m]<<"),";
			cout<<'\n'<<'\n'<<'\n';
		}
		newplane(r,ID,Flevel,number);
		
		chooseway(r,ID,Flevel,rear);
		
		}
		
		int totallandtime=0,totaltaketime=0;
		long long totalfuel=0;
	
		for(int i=0;i<=number[0];i+=2) 
		{
			totallandtime+=Time[i];
		
		}
			
		for(int i=1;i<=number[1];i+=2) 
		{
			totaltaketime+=Time[i];
			
		}
		
		for(int i=1;i<=number[1];i+=2) 	totalfuel+=Flevel[i];	
		
		double avgland=0.0;
		double avgtake=0.0;
		double avgfuel=0.0;
		avgland=(double)(totallandtime)/(double)((number[1]+1)/2);
		avgtake=(double)(totaltaketime)/(double)((number[0]+1)/2);
		avgfuel=(double)(totalfuel)/(double)((number[1]+1)/2);
		cout<<"How Many Time Unit You Want To Simulate:"<<T<<'\n';
		cout<<"average landing waiting time: "<<fixed<<setprecision(3)<<avgland<<"(s)\naverage takeoff waiting time: "<<avgtake<<"(s)"<<'\n';	
		cout<<"average saved fuel: "<<fixed<<setprecision(3)<<avgfuel<<"(s)"<<'\n';
		cout<<"total plane in emergency: "<<emergency<<'\n';
		cout<<"total plane crashed: "<<crash<<'\n';
		return 0;	
}


