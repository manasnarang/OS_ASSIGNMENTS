#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

//MIGHT PRINT 1 FORK AT A TIME INSTEAD OF BOTH TOGETHER

//NO STRICT ORDER
sem_t forks[5];

//functions to return left and right forks
int left(int philosopher){
	return philosopher;
}

int right(int philosopher){
	return (philosopher+1)%5;
}


//function to pick up the forks(acquire semaphore locks)
void get_forks(int philosopher){
	// if(philosopher==4){					//different order of picking forks for philosopher no. 4 to avoid deadlock
	// 	sem_wait(&forks[right(philosopher)]);
	// 	printf("Philosopher %d has picked up fork %d\n",philosopher,right(philosopher));
	// 	sem_wait(&forks[left(philosopher)]);
	// 	printf("Philosopher %d has picked up fork %d\n",philosopher,left(philosopher));
	// }
	// else{
		sem_wait(&forks[left(philosopher)]);
		printf("Philosopher %d has picked up fork %d\n",philosopher,left(philosopher));
		sem_wait(&forks[right(philosopher)]);
		printf("Philosopher %d has picked up fork %d\n",philosopher,right(philosopher));
	// }
}


//function to put down the forks(release the semaphore locks)
void put_forks(int philosopher){
	//printing before sem_post to avoid inconsistent order of outputs
	printf("Philosopher %d has put down fork %d \n",philosopher,left(philosopher));
	sem_post(&forks[left(philosopher)]);
	printf("Philosopher %d has put down fork %d \n",philosopher,right(philosopher));
	sem_post(&forks[right(philosopher)]);
	// printf("Philosopher %d has put down the forks %d and %d\n",philosopher,left(philosopher),right(philosopher));
}


//function to simulate thinking
void think(int philosopher){
	printf("Philosopher %d is thinking\n",philosopher);
}

//function simulating eating
void eat(int philosopher){
	get_forks(philosopher);
	// printf("Philosopher %d is eating\n",philosopher);

	sleep(1);	//sleep to give some time for eating(might remove later)

	put_forks(philosopher);
	// printf("Philosopher %d has finished eating\n",philosopher);
}



//function that simulates the philosopher
void *philosopher_simulation(void *n){
	int phil_number=(int)n;
	think(phil_number);
	eat(phil_number);
}

int main(){
	
	pthread_t philosophers[5];		//array of threads for 5 philosophers

	
	//initialise the semaphores
	for(int i=0;i<5;i++){
		sem_init(&forks[i],0,1);
	}

	printf("Dinner started\n");

	
	//creating threads
	for(int i=0;i<5;i++){
		pthread_create(&philosophers[i],NULL,philosopher_simulation,(void *)i);
	}
	
	
	//using pthread_join to ensure threads terminate
	for(int i=0;i<5;i++){
		pthread_join(philosophers[i],NULL);
	}

	printf("Dinner has finished and all philosophers ate without any deadlock\n");
	return 0;
}