#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*huilin tai 260962725 */

struct ACCOUNT {
 int accountNumber;
 double startingBalance;
 double endingBalance;
 struct ACCOUNT *next;
};
//create ACCOUNT * node
struct ACCOUNT * createNodeFromLine(char * line)
{
	struct ACCOUNT * node;
	node = (struct ACCOUNT *) malloc(sizeof(struct ACCOUNT));
	memset (node,0,sizeof(struct ACCOUNT));
	if (line)
	{
		//split each field
	sscanf(line, "%d,%lf,%lf",&node->accountNumber,&node->startingBalance,&node->endingBalance);
	}
return node;
}
 
struct ACCOUNT* ListInsert(struct ACCOUNT * head, struct ACCOUNT * node)
{
       struct ACCOUNT * ptr=(struct ACCOUNT *) malloc(sizeof(struct ACCOUNT));
       struct ACCOUNT * pre=(struct ACCOUNT *) malloc(sizeof(struct ACCOUNT));
       	ptr=head;
	pre=head; 
if (head == NULL)
{ node->next = NULL;
       head = node; 
 }
else {	
if (ptr->accountNumber < node->accountNumber) {
		while (ptr->next && ptr->accountNumber <=  node->accountNumber) {
        	pre=ptr;
        	ptr=ptr->next; }
		if(ptr->next && pre->accountNumber == node->accountNumber){
			printf("Duplicate account number [account, start, end]: %d %.2f %.2f\n",node->accountNumber,node->startingBalance,node->endingBalance);
        	}
		else if(ptr->next == NULL && ptr->accountNumber < node->accountNumber) {
		node->next =NULL ;
		ptr->next = node;
		}
		else if(ptr->next == NULL && ptr->accountNumber == node->accountNumber) {
                printf("Duplicate account number [account, start, end]: %d %.2f %.2f\n",node->accountNumber,node->startingBalance,node->endingBalance);
		}

		else {
 node->next = ptr;
    pre->next=node; }
		}	
else if (ptr->accountNumber == node->accountNumber ){
	if (ptr->accountNumber!=0) {
	printf("Duplicate account number [account, start, end]: %d %.2f %.2f\n",node->accountNumber,node->startingBalance,node->endingBalance);
                 }
else if (ptr->accountNumber > node->accountNumber ) {
	node->next = ptr;
        head=node;
	} 
	}
return head;	
}
}

// free the list
void ListDestroy(struct ACCOUNT * head)
{
    if (head)
    {
        ListDestroy(head->next);
        free(head);
    }
}


int main(int argc, char *argv[])
{
	const double EPS =1e-6;
	const char * input_filename;
	const char * output_filename;
  	FILE * fin, *fout;
    	char line[1000];
    	char header[1000];
	char holder[1000];
	char lined[1000];
	//if not passed 2 arguments
	if (argc != 3)
	{
	printf("Wrong number of arguments!\n");
	printf("./tv STATE TRANSACTIONS\n./tv state.csv transaction.csv\n./tv ../state.csv /data/log/transaction.csv\n");
	exit(1);
	}
	input_filename = argv[1];
	output_filename = argv[2];
        fin = fopen(input_filename,"rt");
//if the program cannot open the input
if (fin==NULL)
{
	printf("Unable to open filename %s\n",input_filename);
	 printf("./tv STATE TRANSACTIONS\n./tv state.csv transaction.csv\n./tv ../state.csv /data/log/transaction.csv\n");
	exit (2);
}


    struct ACCOUNT * head, *node;
    head =createNodeFromLine(NULL);
    //////////////////////////////////////////
    //    open input  file and read  data   //
    //////////////////////////////////////////
while (fgets(line, 1000, fin))
 {
      if (strlen(line) > 1)
        {	
            node = createNodeFromLine(line);
          // printf("%d\n",node->accountNumber);
	    head=ListInsert(head, node);  
	}
    }
//printf("%d,%d,%d,%d",head->accountNumber,head->next->accountNumber,head->next->next-> accountNumber,head->next->next->next->accountNumber);
fclose(fin);
    //////////////////////////////////////////
    //    open output file and write data   //
    //////////////////////////////////////////
fout = fopen(output_filename, "rt");
    if (fout==NULL)
    {
        printf("Unable to open filename %s\n", output_filename);
       printf("./tv STATE TRANSACTIONS \n./tv state.csv transaction.csv\n./tv ../state.csv /data/log/transaction.csv\n");
        exit (2);
    }
 
	int accountNumberd;
        char mode;
	float absoluteValueAmount;
	

if (head->next) {
	int t = 0;
	while (fgets(lined, 1000, fout)) {	
       		if (strlen(lined) > 1)
        	{
			if(t==0) {
       			 t=t+1;
       			continue;}
			else {
	 		node = head->next; 
			sscanf(lined, "%d,%c,%f",&accountNumberd,&mode,&absoluteValueAmount);
 			t=t+1;
				while (node != NULL && node->accountNumber != accountNumberd) {
					node = node->next;
					}			
				if (node==NULL) {
					printf("Account not found (account, mode, amount): %d %c %.2f\n",accountNumberd,mode,absoluteValueAmount);
						}
				else
					{
						if(mode == 'd') {
						node->startingBalance = (node->startingBalance + absoluteValueAmount);
						}
	
						if (mode == 'w') {
							if ( node->startingBalance >= absoluteValueAmount) 
							{
	 							node->startingBalance = (node->startingBalance - absoluteValueAmount);
							}
							else 
							{			
		 					printf("“Balance below zero error (account, mode, transaction, startingBalance before): %d %c %.2f %.2f\n",accountNumberd,mode,absoluteValueAmount,node->startingBalance);
							node->startingBalance = 0;
							}
			
						}			
    					}				
			}	
    		}	
}	
}



if (!head->next)
    {
	    int q=0;
	 while (fgets(lined, 1000, fout))
    {
         if (strlen(lined) > 1 )
        {
		if(q==0) {
              q=q+1;
       continue;}
 q=q+1;
	}
    }
            if (q>1){
        printf("File %s is empty. Unable to validate %s\n",input_filename,output_filename);
        exit (3);
    }
    }


fclose(fout);   
if (head->next) {
  node=head->next;
 while (node != NULL) {
	 if(fabs(node->startingBalance - node->endingBalance) > EPS)
  {
	  printf("End of day balances do not agree (account, starting, ending): %d %.2f %.2f\n",node->accountNumber,node->startingBalance,node->endingBalance);
  }
	  node = node->next;
 }
}
ListDestroy(head);

}


