#include <iostream>

#include <mutex>

#include <condition_variable>

#include <stdlib.h>

#include <pthread.h>

#define INFI 256

struct Args

{

	int id;

};



class Node // Combinning Tree Node Class

{

public:

	enum CStatus {IDLE, FIRST, SECOND, RESULT, ROOT};
	bool locked;
	CStatus cStatus;
	int firstValue, secondValue;
	int result;
	Node *Parent;

	//for sync of precombine on this node

	pthread_mutex_t precombinelock;

	//for sync of combine on this node

	pthread_mutex_t combinelock;

	//for sync of op on this node

	pthread_mutex_t oplock;

	//for sync of distribute on this node

	pthread_mutex_t distributelock;

	//for protect the condition veriable

	pthread_mutex_t cond_mutex;

	//condition veriable for waiting on this node

	pthread_cond_t cond;



	Node() {

		cStatus = ROOT;
		locked = false;
		Parent = NULL;
		firstValue = 0;
		secondValue = 0;
		result = 0;

		pthread_mutex_init(&precombinelock, NULL);

		pthread_mutex_init(&combinelock, NULL);

		pthread_mutex_init(&oplock, NULL);

		pthread_mutex_init(&distributelock, NULL);

		pthread_mutex_init(&cond_mutex, NULL);

		pthread_cond_init(&cond, NULL);

	}



	Node(Node *myParent) {

		Parent = myParent;
		cStatus = IDLE;
		locked = false;
		firstValue = 0;
		secondValue = 0;
		result = 0;

		pthread_mutex_init(&precombinelock, NULL);

		pthread_mutex_init(&combinelock, NULL);

		pthread_mutex_init(&oplock, NULL);

		pthread_mutex_init(&distributelock, NULL);

		pthread_mutex_init(&cond_mutex, NULL);

		pthread_cond_init(&cond, NULL);

	}



	bool precombine();
	int combine(int combined);
	int op(int combined);
	void distribute(int prior);	
	void precombine_lock();
	void precombine_unlock();
	void combine_lock();
	void combine_unlock();
	void op_lock();
	void op_unlock();
	void distribute_lock();
	void distribute_unlock();
	void wait();
	void notify_all();


};



class CombiningTree

{

public:

	Node *nodes[INFI]; //head of internal nodes 

	Node *leaf[INFI];  //head of leaf nodes 

public:

	CombiningTree(int width) {

		nodes[0] = new Node();

		for(int i = 1; i < width; i++) {

			nodes[i] = new Node(nodes[(i-1)/2]);

		}

		for (int i = 0; i < (width + 1)/2; i++) {

			leaf[i] = new Node(nodes[width - i -2]);

		}

	}

	int getAndIncrement(int id);

};
