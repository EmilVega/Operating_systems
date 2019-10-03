#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* retorna "a - b" en segundos */
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}


void main()
{
	const char *proc[3];
	proc[0]="./proceso1.sh";
	proc[1]="./proceso2.sh";
	proc[2]="./proceso3.sh";

	const char *prBg[3];
	prBg[0]="./proceso1.sh &";
	prBg[1]="./proceso2.sh &";
	prBg[2]="./proceso3.sh &";

	const char *prStp[3];
	prStp[0]="killall -STOP proceso1.sh";
	prStp[1]="killall -STOP proceso2.sh";
	prStp[2]="killall -STOP proceso3.sh";

	const char *prCont[3];
	prCont[0]="killall -CONT proceso1.sh";
	prCont[1]="killall -CONT proceso2.sh";
	prCont[2]="killall -CONT proceso3.sh";

	int c[3];
	c[0]=0;
	c[1]=0;
	c[2]=0;

	double t[3];
	struct timeval start, end;
	for (int i=0;i<3;i++)
	{
		gettimeofday(&start, NULL);
		system(proc[i]);
		gettimeofday(&end, NULL);
		t[i] = timeval_diff(&end, &start);	
	}
	double totTime = 0.0;
	printf("\nProcess\t\t|\tTime taken\n");
	for (int i=0;i<3;i++)
	{
		printf("%s\t|\t%f\n",proc[i],t[i]);
		totTime=totTime+t[i];	
	}
	printf("\nThe total time taken: %f\n",totTime);

	/* Applying the Round Robin scheduling for these processes */
	
	printf("\nNow we are going to apply Round Robin scheduling \n");
	double t_quantum;
	printf("Enter Time Quantum:\t");
  	scanf("%lf",&t_quantum);
	struct timeval st[4], ed[4];
	struct timeval wtst[3], wted[3];
	double ct[3], wait_t[3]={0.0,0.0,0.0};
	int p=0;
	for(int i=0;i<3;i++)
	{
		gettimeofday(&wtst[i], NULL);
	}
	gettimeofday(&st[3], NULL);
	while(t[0]>0.1 || t[1]>=0.1 || t[2]>=0.1)
	{
			
		if(t[p]>=0.1)
		{			
			gettimeofday(&start, NULL);
			if (c[p]==0)
			{
				gettimeofday(&wted[p], NULL);
				wait_t[p]=wait_t[p]+timeval_diff(&wted[p], &wtst[p]);
				gettimeofday(&st[p], NULL);
				system(prBg[p]);
				printf("%s started \n", proc[p]);
				c[p]=1;
			}else{
				gettimeofday(&wted[p], NULL);
				wait_t[p]=wait_t[p]+timeval_diff(&wted[p], &wtst[p]);
				printf("%s resumed \n", proc[p]);
				system(prCont[p]);
			}
			double count = 0.0;
			while ((t_quantum - count)>=0.1 && t[p]>0.1)
			{
				gettimeofday(&end, NULL);
				count=timeval_diff(&end, &start);
				if((t[p]-count)<0.1)
				{
					t[p]=0;
				}
			}
			if (t[p]!=0)
			{
				t[p]=t[p]-t_quantum;
			}
			if (t[p]>0.1){
				gettimeofday(&wtst[p], NULL);
				system(prStp[p]);
				printf("%s stopped \n", proc[p]);
			}else{
				gettimeofday(&ed[p], NULL);
				printf("%s terminated \n", proc[p]);
				ct[p]=timeval_diff(&ed[p], &st[p]);
			}		
		}
		p=p+1;
		if ((p==3) && (t[0]>0.1 || t[1]>0.1 || t[2]>0.1))
		{
			p=0;
		}
	}
	gettimeofday(&ed[3], NULL);
	totTime=timeval_diff(&ed[3], &st[3]);
	printf("\nProcess\t\t|\tTime taken\t|\tWaiting time\n");
	for(int i=0;i<3;i++)
	{
		printf("%s\t|\t%f\t|\t%f\n",proc[i],ct[i],wait_t[i]);
	}
	printf("\nThe total time taken with Round Robin: %f\n",totTime);
}
