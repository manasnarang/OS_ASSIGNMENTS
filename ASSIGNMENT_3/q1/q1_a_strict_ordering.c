#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//MIGHT PRINT 1 FORK AT A TIME INSTEAD OF BOTH
//CHECK STRICT ORDERING AGAIN BEFORE SUBMITTING

//Strict ordering of resource requests

int forks[5];

//functions to return left and right forks

void lock_fork(int fork_num,int philosopher){
	while(forks[fork_num]!=-1);		//waiting for lock to be released
	forks[fork_num]=philosopher;
	printf("Philosopher %d has picked up fork %d\n",philosopher,fork_num);
}

void release_fork(int fork_num, int philosopher){
	printf("Philosopher %d has put down fork %d \n",philosopher,fork_num);
	forks[fork_num]=-1;
}

int left(int philosopher){
	return philosopher;
}

int right(int philosopher){
	return (philosopher+1)%5;
}

//function to pick up the forks
void get_forks(int philosopher){
	if(philosopher==4){				//Different order for philosopher no. 4 to avoid deadlock
		lock_fork(right(philosopher),philosopher);
		// printf("Philosopher %d has picked up fork %d\n",philosopher,right(philosopher));
		lock_fork(left(philosopher),philosopher);
		// printf("Philosopher %d has picked up fork %d\n",philosopher,left(philosopher));
	}
	else{
		lock_fork(left(philosopher),philosopher);
		// printf("Philosopher %d has picked up fork %d\n",philosopher,left(philosopher));
		lock_fork(right(philosopher),philosopher);
		// printf("Philosopher %d has picked up fork %d\n",philosopher,right(philosopher));
	}
}

//function to put down the forks(release the semaphore locks)
void put_forks(int philosopher){
	//printing before releasing to avoid inconsistent order of outputs
	// printf("Philosopher %d has put down fork %d \n",philosopher,left(philosopher));
	release_fork(left(philosopher),philosopher);
	// printf("Philosopher %d has put down fork %d \n",philosopher,left(philosopher));
	// printf("Philosopher %d has put down fork %d \n",philosopher,right(philosopher));
	release_fork(right(philosopher),philosopher);
	// printf("Philosopher %d has put down fork %d \n",philosopher,right(philosopher));
	// printf("Philosopher %d has put down the forks %d and %d\n",philosopher,left(philosopher),right(philosopher));
}

//function to simulate thinking
void think(int philosopher){
	printf("Philosopher %d is thinking\n",philosopher);
}

//function to simulate eating
void eat(int philosopher){
	get_forks(philosopher);
	// printf("Philosopher %d has picked up forks %d and %d\n",philosopher,left(philosopher),right(philosopher));
	sleep(1);
	put_forks(philosopher);
	// printf("Philosopher %d has put down forks %d and %d\n",philosopher,left(philosopher),right(philosopher));
}


//function to simulate philosopher
void *philosopher_simulation(void *n){
	// printf("Working in function\n");
	int phil_number=(int)n;
	// printf("Working in function after typecast\n");
	think(phil_number);
	eat(phil_number);
}

int main(){
	
	pthread_t philosophers[5];		//array of threads for 5 philosophers

	
	//initialise the forks
	for(int i=0;i<5;i++){
		forks[i]=-1;		//initially all forks are free(locks are released)
	}

	printf("Dinner started\n");

	
	//creating threads
	for(int i=0;i<5;i++){
		pthread_create(&philosophers[i],NULL,philosopher_simulation,(void *)i);
		// printf("Working till here\n");
	}
	
	// printf("Working till here\n");
	//using pthread_join to ensure threads terminate
	for(int i=0;i<5;i++){
		pthread_join(philosophers[i],NULL);
	}

	printf("Dinner has finished and all philosophers ate without any deadlock\n");	
	return 0;
}