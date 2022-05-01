#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "citizens.h"
#include "hashing.h"
#include <ctype.h>
#include "skiplist.h"
#include "virus.h"

//reading and inserting the records in the file filename
int readfile(char* filename,int length,int length2,int length3,datenode**head){
  citizen *temp;
  int age,i;
  FILE *fp;
  char *buffer=NULL,*word,*last,*first,*country,*virus,*date,*answer,*id;
  char *day,*month,*year;
  size_t size=0;
  int flag,flag2,flag3;

   fp = fopen(filename,"r");
   if(fp==NULL){
     fprintf(stderr, "Error opening file\n");
     return -1;
   }

    while(getline(&buffer,&size,fp)!=-1){  //reading lines untile the end of file
      flag2=0;
      flag3=0;
      word = strtok(buffer, " ");

      for(i=0;i<strlen(word);i++){
        if(isdigit(word[i])==0){  //id must be integer
            printf("ERROR IN RECORD %s\n",word);
            flag3=1;
        }
      }
      id = (char*)malloc(sizeof(char) * (strlen(word) + 1));
      if(id==NULL){
        fprintf(stderr, "Error allocating memory\n" );
        return -1;
      }
      strcpy(id,word);

      word = strtok(NULL, " ");
      first = (char*)malloc(sizeof(char) * (strlen(word) + 1));
      if(first==NULL){
        fprintf(stderr, "Error allocating memory\n" );
        return -1;
      }
      strcpy(first,word);

      word = strtok(NULL, " ");
      last = (char*)malloc(sizeof(char) * (strlen(word) + 1));
      if(last==NULL){
        fprintf(stderr, "Error allocating memory\n" );
        return -1;
      }
      strcpy(last,word);

      word = strtok(NULL, " ");
      country = (char*)malloc(sizeof(char) * (strlen(word) + 1));
      if(country==NULL){
        fprintf(stderr, "Error allocating memory\n" );
        return -1;
      }
      strcpy(country,word);


    word = strtok(NULL, " ");
    for(i=0;i<strlen(word);i++){
      if(isdigit(word[i])==0){ //age must be integer
          printf("ERROR IN RECORD %s\n",id);
          flag3=1;
      }
    }
    if(flag3==0){
      age = atoi(word);
      if(age>120 || age<0){
        printf("ERROR IN RECORD %s\n",id);
        flag3=1;
      }
    }

    word = strtok(NULL, " ");
    virus = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    if(virus==NULL){
      fprintf(stderr, "Error allocating memory\n" );
      return -1;
    }
    strcpy(virus,word);
    flag=0;
    for (i=0; i <strlen(virus);  i++) { //check that the virus name is correct
      if(isalnum(virus[i])==0){

         if( virus[i]=='-'){

            if (flag ==0){//only one -
              flag =2;
            }else{
             printf("ERROR IN RECORD %s\n",id);
             flag3=1;
           }

         }else{
           printf("ERROR IN RECORD %s\n",id);
           flag3=1;
         }
      }
    }

    word = strtok(NULL, " ");
    answer = (char*)malloc(sizeof(char) * (strlen(word) + 1));
    if(answer==NULL){
      fprintf(stderr, "Error allocating memory\n" );
      return -1;
    }
    strcpy(answer,word);

    word = strtok(NULL, " ");
    if(word ==NULL ){
      answer[strlen(answer)-1]= '\0';  //withouth the /n
    }

    if(strcmp(answer,"YES")==0 ){
      if(word==NULL|| strcmp(word,"\n")==0){ //if there is no date
        printf("ERROR IN RECORD %s\n",id);
        flag3=1;

      }else{
        date = (char*)malloc(sizeof(char) * (strlen(word) + 1));
        if(date==NULL){
          fprintf(stderr, "Error allocating memory\n" );
          return -1;
        }
        flag2=2;
        word[strlen(word)-1]= '\0';
        strcpy(date,word);

        day = strtok(word,"-");
        if(atoi(day)>30 || atoi(day)<1){ //check that date is correct
          printf("ERROR IN RECORD %s\n",id);
          flag3=1;
        }
        month = strtok(NULL,"-");
        if(atoi(month)<1 || atoi(month)>12){
            printf("ERROR IN RECORD %s\n",id);
            flag3=1;
        }
        year = strtok(NULL,"-");
        if(atoi(year)>2021 || atoi(year)<1901){
          printf("ERROR IN RECORD %s\n",id);
          flag3=1;
        }
      }

    }else if(strcmp(answer,"NO")==0){ //if no then record must not have a date
      if(word!=NULL){
          printf("ERROR IN RECORD %s\n",id);
          flag3=1;
      }

    }else{ //if the answer is neither YES nor NO
      printf("ERROR IN RECORD %s\n",id);
      flag3=1;
    }


     printf("Record %s %s %s %d %s %s %s \n",id,first,last,age,country,virus,answer );

  if(flag3==0){ //if the record is valid
    if(flag2==0){ //if there is no date set it to null
      date = NULL;
    }
    temp=insert(length,length2,length3,id,first,last,age,country,virus,date,head,0);
  }
    free(id);
    free(first);
    free(last);
    free(virus);
    free(country);
   if(flag2>0){
      free(date);
    }
    free(answer);
  }
  free(buffer);
  fclose(fp);
  return 0;
}



//checking bloom filter for virus and id
int vaccineStatusBloom(char *id, char *virus,int size){
  int k;
  virusnode*temp;

  k = stringhashfunction(virus,size);

  temp = findVirus(virushash[k],virus);

  if(temp!=NULL){

    if(checkinBloom(id,temp)==0){
      printf("MAYBE\n" );
      return 0;
    }
  }
  printf("NOT VACCINATED\n" );
  return -1;
}

//free memory and exit
int exitAll(int length,int length2,int length3,datenode **head){
  deleteHashtable(length,0);
  deleteHashtable(length2,1);
  deleteHashtable(length3,2);
  deleteDateList(head);
  exit(0);
}

//check vaccinated skip list for id
int vaccineStatusVirus(char*id,char*virus,int length3){
  int k;
  virusnode*temp;
  skipnode *temp2;

  k = stringhashfunction(virus,length3);
  if((temp = findVirus(virushash[k],virus))!=NULL){
    if((temp2=findSkipList(temp->vacc,id))!=NULL){
      printf("VACCINATED ON %s\n",temp2->date->date );
      return 0;
    }
  }
  printf("NOT VACCINATED\n");
  return 1;

}

//check vaccinated skip list and not vaccinated skip list
int  vaccineStatus(char*id,int length3){
  int i;
  virusnode*current;
  skipnode *temp2;

  for(i=0; i<length3;i++){
    current = virushash[i];
    while (current != NULL){
      if((temp2=findSkipList(current->vacc,id))!=NULL){
        printf("%s YES %s\n",current->name,temp2->date->date );
      }else if((temp2=findSkipList(current->notvacc,id))!=NULL){
        printf("%s NO\n",current->name);
      }
      current = current->next;
    }
  }
  return 1;
}

//print citizens in the not vaccinated skip list of virus 
int printNotVaccinated(char*virus,int size){
  int k;
  virusnode *temp3=NULL;
  k = stringhashfunction (virus,size);
  temp3 = findVirus(virushash[k],virus);

  if(temp3==NULL){
    printf("ERROR VIRUS %s DOES NOT EXIST\n",virus );
    return 0;
  }
  printSkipList(&temp3->notvacc);
  return 1;
}
