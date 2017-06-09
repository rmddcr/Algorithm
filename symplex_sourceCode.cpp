/*
****compiler data*****

Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/5/lto-wrapper
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 5.4.0-6ubuntu1~16.04.4' --with-bugurl=file:///usr/share/doc/gcc-5/README.Bugs --enable-languages=c,ada,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-5 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-libmpx --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-5-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-5-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-5-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4) 

****test cases****

Maximize p = 6x + 5y + 4z  subject to
2x + y + z <= 180 ,
x + 3y + 2z <= 300,
2x + y + 2z <= 240,


Maximize p = x + 2y - z  subject to
2x +  y + z <= 14 ,
4x + 2y + 3z <= 28,
2x + 5y + 5z <= 30,

Maximize p = 6x + 5y + 4z  subject to
2x + y + z <= 180 ,
x + 3y + 2z <= 300,
2x + y + 2z <= 240,
2x+6y+4z<=600


in hear x,y,z,w,>0
****How to use****

How to use: 
define M as number of constraints,
define N as number of variables
build an MxN 2D array as the constraint matrix
build a array of size M for constraints}--b
build a coefiecnt array for objective function of size N}--c[]
build a solution array of size N to recive solutions }--A[]
and pass these variables to simplex function matching it's 
solutions will be recived from solution array of size N you passed to the function

egm = 3
n = 3 
A = | 2 | 1 | 1 |
    | 1 | 3 | 2 | 
    | 2 | 1 | 2 |

b = | 180 |
    | 300 |
    | 240 |

c = | 6 |
    | 5 |
    | 4 |

x = array of size N

****Limitations****

 all variables shoud be positive eg: x, y, z, w >= 0 
 if there is compleate negative colomns in the  array then this will not work
eg:
A = | 2 | 1 | -1 |
    | 1 | 3 | -2 | 
    | 2 | 1 | -2 |

 there could be floating point errors due to division of values by the compiler dueto fix no of presition points

 i had given iteration limit to stop non stop recurrsion of the code 
 ***important***
 *if you want to run this more than 20 times please change it as necessory


*/
#include <iostream>
using namespace std;

#define N 3 //define N as number of variables
#define M 4 //define M as number of constraints,
// change hear to change the matrix A (m*n) where n is no of colums m is no of rows 


float* simplexAlgo(float A[M][N], float b[], float c[], float x[], int m, int n,float *max);
void printTable(float table[M+1][M+N+2], int m, int n);



int main(){//example
	float A[M][N];//constraint matrix
	float b[M];// Ax = b are the constraints 
	float c[N];//coefiecients of the objective function

/*

	//test passed
	c[0] = 6; c[1] = 5; c[2] = 4;//m=3,n=3 objective function P = 6x + 5y + 4z
	A[0][0] = 2; A[0][1] = 1; A[0][2] = 1; b[0] = 180;//constrain 1  2x + y + z <= 180 
	A[1][0] = 1; A[1][1] = 3; A[1][2] = 2; b[1] = 300;//constrain 1  x + 3y + 2z <= 300 
	A[2][0] = 0; A[2][1] = 1; A[2][2] = 2; b[2] = 240;//constrain 1  2x + y + 2z <= 240 


	
	//test passed
    c[0] = 1; c[1] = 2; c[2] = -1; // m=3,n=3 objective function p = x + 2y + -z 
	A[0][0] = 2; A[0][1] = 1; A[0][2] = 1; b[0] = 14;//2x + y +   z <=  14
	A[1][0] = 4; A[1][1] = 2; A[1][2] = 3; b[1] = 28;//4x + 2y + 3z <= 28,
	A[2][0] = 2; A[2][1] = 5; A[2][2] = 5; b[2] = 30;//2x + 5y + 5z <= 30, 

	


	//test passed
	c[0] = 6; c[1] = 3; c[2] = 1; c[3]=4; //m=3,n=4 objective function p = 6x + 3y + z + 4w   
	A[0][0] = 1; A[0][1] = 1; A[0][2] =  1;  A[0][3] =-1;b[0] = 40;//   x   +y   +z   -w  <= 40
	A[1][0] = 2; A[1][1] = 1; A[1][2] =  8;  A[1][3] =-1;b[1] = 100;// 2x   +y  +8z   -w  <= 100
	A[2][0] = 3; A[2][1] = 4; A[2][2] = -1;  A[2][3] =-1;b[2] = 80;//  3x  +4y   -z   -w  <= 80 
	//no optimization solution

*/
	

	//test passed
     c[0] = 6; c[1] = 5; c[2] = 4;//m=4,n=3 objective function p = 6x + 5y + 4z
	A[0][0] = 2; A[0][1] = 1; A[0][2] = 1; b[0] = 180;// 2x +  y +  z <=  180 
	A[1][0] = 1; A[1][1] = 2; A[1][2] = 2; b[1] = 300;//  x + 2y + 2z <=  300,
	A[2][0] = 2; A[2][1] = 1; A[2][2] = 2; b[2] = 240;// 2x +  y + 2z <=  240, 
	A[3][0] = 2; A[3][1] = 6; A[3][2] = 4; b[3] = 600;// 2x + 6y + 4z <=  600
	
	
	
	float x[N];//solution set 
	float MAX;
	
	simplexAlgo(A, b, c, x, M, N, &MAX);//return the pointer of the answere array
	// solutions are in the x[N] array
	
	cout << "##Solution Are :" << endl;
	for(int i = 0; i < N ; i++){// to printout solutions
		cout << "X" << i << " = " << x[i] << endl;// x1 = x , x2 = y , x3 = z
	}
	cout<< "Max = "<<MAX<<endl;
	cout << "-------------------------------" << endl;
	return 0;
}


float* simplexAlgo(float A[M][N], float b[], float c[], float x[], int m, int n,float *max){
	int tableHight = m + 1; //no of constraints plus one for objective
	int tableWith = n + m + 2; //no of variables + no of slack variables +1(for colomn P) +1(forcolomn b) 
	float table[M+1][M+N+2]; //declare table with relevent size


	

	//filling the table with matrix A[M][N]
	for (int i = 0 ; i < m; i++ ){	//move from row to row
		for (int j = 0 ; j < n; j++ ){  //iterate through the row
				table[i][j] = A[i][j];
		}
	}
	

	// filling the matrix with the identity matrix m*n

	for (int i = 0 ; i < tableHight; i++ ){	//move from row to row
		for (int j = n ; j < tableWith -1; j++ ){  //iterate through the row
				if(i==j-n){
					table[i][j] = 1;
				}
				else {
					table[i][j] = 0;
				}
		}
	}

	

	//filling the column with b[]
	for (int i = 0 ; i < m; i++ ){
		table[i][tableWith-1] = b[i]; 
	}

	

	//filling the last row with the array c[]

	for(int j= 0;j<n;j++){
		table[tableHight-1][j]= - c[j];
	}
	

	//filling zero in the leftbottom corner
	
	table[tableHight-1][tableWith-1]=0;

	
	cout<<endl;
	cout<<"Created Table :"<<endl;
	printTable(table, tableHight, tableWith);

bool foundNegative;
int colomn, row, iteration;
float minimum,minimumRatio, divisor, multiplier;
	cout<<endl;
	cout << "### Starting Simplex algorithm" << endl;
	cout << "-------------------------------" << endl;
	
	//run this process atleast once  
	
iteration = 1;
cout<<endl;
cout<<endl;
cout<<"Table # 0"<<endl;
cout<<"---------"<<endl;
printTable(table, tableHight, tableWith);
cout << "-------------------------------" << endl;
	do{

		foundNegative = false;
		minimum = 0;
		for(int j = 0; j < tableWith - 1; j++){     //looking for minimum negative value in the bottom line till column p
			if(table[tableHight-1][j] < minimum){
				colomn = j;
				minimum = table[tableHight-1][j];		//find the pivot column and check for negative value
				foundNegative = true;
			}
		}


		bool isPositivePievat[tableHight-1];
		for(int i = 0 ; i <tableHight-1;i++){
			if(table[i][colomn]>0) isPositivePievat[i]=true; //creat an array which has negative positive nature of a row
			else isPositivePievat[i]=false;
		}
		

		bool solvable ;
		solvable = false; 
		for(int i = 0 ; i <tableHight-1;i++){
			if(isPositivePievat[i]){solvable  = true;}//check for solvability

		}

		//finding the first positive pivot position

		int k = 0 ;
		while(k < tableHight-1){
			if(isPositivePievat){
				break;
			}else 
			k++ ;}



		minimumRatio = (table[k][tableWith-1] / table[k][colomn]);
		row = k;

		if(foundNegative){//do this only if you find a negative value in indicator row

			if(! solvable){
			 goto ending;


		}
			
			for(int i = k; i < tableHight - 1; i++){	//looking for minimum ratio row

				if(isPositivePievat[i]){//if pivot is positive

					if((table[i][tableWith-1] / table[i][colomn]) < minimumRatio){
						minimumRatio = (table[i][tableWith-1] / table[i][colomn]);
						row = i;							//find the pivot point column
					}
				}
			
			}
			divisor = table[row][colomn];  // pivot point coefficient
			cout <<"pivot = "<<divisor<<"|";
			cout <<"r = "<<row<<" , ";
			cout <<"c = "<<colomn<<endl;
			cout<<endl;

			

			for(int j = 0; j < tableWith; j++){    //normalisiong the pivot row making pivot coefficient = 1
				
				table[row][j] = table[row][j] / divisor; 
			}
			

			for(int i = 0; i < tableHight; i++){      //subtracting all rows by selected row to make the pivot column zero
				if(i == row){                         // leave this row alone
				}
				else{multiplier = -(table[i][colomn]);		//finding the relevent multiplyer
					 for(int j = 0; j < tableWith; j++){
						 table[i][j] = (table[i][j] +(table[row][j] * multiplier));  //nomalization
					}
				}
			}
		}
	
	cout << endl;
	if (foundNegative) 
		{
			cout << "Table #" << iteration<< endl;
			cout << "----------"<<endl;
			printTable(table, tableHight, tableWith);
		}

	else {cout << "Final compleated table : " << endl;printTable(table, tableHight, tableWith);}
	cout << "-------------------------------" << endl;
	iteration ++ ;
	if (iteration ==20) foundNegative = false; //to break the looping to infinity 
	
	}while(foundNegative);
	//Simplex algo has been executed at this point need to get the solutiond
	cout << "-------------------------------" << endl;




	bool isValueZero;
	int pointer;
	float colomnSum;
	for(int j = 0; j < n; j++){//extracting solution by traversing through colomns 
		isValueZero = false;
		colomnSum=0.0;
		for(int i = 0; i < tableHight; i++){//traversing rows
				 
				 colomnSum=colomnSum+table[i][j];
				 
				 if(table[i][j]==1.0){pointer = i;//find the position of 1
				// cout<<"pointer i j "<<pointer<<" "<<j<<endl;
				}
				 
				 
				 if(table[i][j] != 0 || table[i][j]==1.0) {
				 	isValueZero = true;//filter out if the column consist of 1 and 0
				 	
				 }

			}
		
		if(isValueZero &&colomnSum != 1.0){ // check for duplicates of 1 is not there
			x[j] = 0;   
		}          // if there is/are values in other places except i=j 
		else {
		x[j] = table[pointer][tableWith-1];    // if there only one 1 and all others are zeros result is zero
		//cout<<table[pointer][tableWith-1]<<endl;
	}
}



*max=table[tableHight-1][tableWith-1];

return x;
cout << "**************************************"<<endl;
ending:

cout << "This equation Cannot be Solved........" <<endl;
cout << "Could not find a valid pivot.........."<<endl;
cout<<endl;
for(int i = 0;i<n;i++){
x[i]=0.0;
}

*max=0.0;

return x;

}



void printTable(float table[M+1][M+N+2], int m, int n){//print print print!!!
	for(int i = 0; i < m; i++){
		cout<<"|";
		for(int j = 0; j < n; j++){
			cout << table[i][j] << " | ";
		}
		cout << endl;
	}
}