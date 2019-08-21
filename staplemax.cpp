#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define SALESPPL  20
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//global of all the input files in an arr
const string files[SALESPPL] = { "/home/fac/lillethd/cpsc3500/projects/p3a/sales1.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales2.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales3.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales4.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales5.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales6.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales7.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales8.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales9.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales10.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales11.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales12.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales13.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales14.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales15.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales16.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales17.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales18.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales19.txt",
							"/home/fac/lillethd/cpsc3500/projects/p3a/sales20.txt"
	};

	
//globals of inital inventory nums
const int PENMAX = 200;
const int PAPERMAX = 100;
const int TONERMAX = 35;
const int LAPTOPMAX = 20;

struct store{
	int pen;
	int paper;
	int toner;
	int laptop;
	int count;
	int fileID;
};


void *inventoryThread(void *threadPtr)
{
	store *ptr = (store*) threadPtr;
	int rml, rmu; //returns from mutex lock & unlock
	
	string line;
	string fileName;
	
	fileName = files[ptr->fileID];
	ifstream infile;
	infile.open(fileName);
	while (getline(infile, line)){
		//decrement for each sale, restock if the decrement completly decrements inventory
		rml = pthread_mutex_lock(&mutex);
		//error check on lock
		if (rml != 0){
			cout << "Error locking " << endl;
			return 0;
		}
		if (line == "pen") {
			ptr->pen--;
			if (ptr->pen == 0)
				ptr->pen = PENMAX;
		}
		else if (line == "paper"){
			ptr->paper--;
			if (ptr->paper == 0)
				ptr->paper = PAPERMAX;
		}
		else if (line == "toner"){
			ptr->toner--;
			if (ptr->toner == 0)
				ptr->toner = TONERMAX;
		}
		else if (line == "laptop"){
			ptr->laptop--;
			if (ptr->laptop == 0)
				ptr->laptop = LAPTOPMAX;
		}
		ptr->count++; //this will be the total number of sales for ea text file
		rmu = pthread_mutex_unlock(&mutex);
		//error check on unlock
		if (rmu != 0){
			cout << "Error unlocking " << endl;
			return 0;
		}

	}
	infile.close();
	
	pthread_exit(NULL);
}

int main()
{

	pthread_t threads [SALESPPL]; //arr of threads
	store shelf;
	store *ptr = &shelf;
	ptr->pen = PENMAX;
	ptr->paper = PAPERMAX;
	ptr->toner = TONERMAX;
	ptr->laptop = LAPTOPMAX;
	int rc, rj, rml, rmu; //for return vals/ error checking
	
	
	for (int i  = 0; i < SALESPPL; i++){
		//create thread
		ptr->fileID = i;
		rc = pthread_create(&threads[i], NULL, inventoryThread, ptr);
		//return if rc is error val
		if (rc != 0) {
			cout << "Error creating thread " << i << endl;
			return 0;
		}
		
		//join threads
		rj = pthread_join(threads[i], NULL);
		//error check on return val
		if (rj != 0){
			cout << "Error joining thread " << i << endl;
			return 0;
		}

	}
	
	rml = pthread_mutex_lock(&mutex);
	//error check on lock
	if (rml != 0){
		cout << "Error locking " << endl;
		return 0;
	}
	
	cout << "Final Pen Inventory: " << ptr->pen << endl;
	cout << "Final Paper Inventory: " << ptr->paper << endl;
	cout << "Final Toner Inventory: " << ptr->toner << endl;
	cout << "Final Laptop Inventory: " << ptr->laptop << endl;
	cout << "Total Sales: " << ptr->count << endl;
	
	rmu = pthread_mutex_unlock(&mutex);
	//error check on unlock
	if (rmu != 0){
			cout << "Error unlocking " << endl;
			return 0;
	}
	
	return 0;
}