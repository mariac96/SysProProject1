#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hashing.h"
#include "functions.h"
#include <ctype.h>

int main(int argc, char *argv[]){
  int i=1,recordsfile=-1,lines=0,flag;
  int length,length2,length3,age;
  FILE *fp;
  char *buffer=NULL,*buffer2=NULL,*word,id[15],virus[25],first[15],last[15],country[50],answer[10],date[20],*day,*month,*year;
  size_t size=0;
  datenode*head=NULL; //list fot the dates
  srand(time(0));
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  if(argc!=5){
    fprintf(stderr, "Not enough arguments\n" );
    return -1;
  }
  while (i<argc )
  {
      if(strcmp(argv[i],"-b")==0){
          bloomsize = atoi(argv[i+1]);
      }
      else if (strcmp(argv[i],"-c")==0){
          recordsfile=i+1;
      }
      i++;

  }


  fp = fopen(argv[recordsfile],"r");
  if(fp==NULL){
    fprintf(stderr, "Error opening file\n");
    return -1;
  }

  while(getline(&buffer,&size,fp)!=-1){
    lines++; //count the lines in the files
  }
  free(buffer);
  fclose(fp);

  //initialize citizen hashtable, country hashtable and virus hashtable
  length = initialize(lines,0);
  length2= initialize(200,1);
  length3= initialize(30,2);

  readfile(argv[recordsfile],length,length2,length3,&head);

//waiting for commands
  while(1){
    flag=0;
    getline(&buffer2,&size,stdin);
    word = strtok(buffer2, " ");

    if(strcmp(word,"/exit\n")==0){
      free(buffer2);
      exitAll(length,length2,length3,&head);

    }else if(strcmp(word,"/vaccineStatusBloom")==0){
      if((word = strtok(NULL, " "))!=NULL){
        strcpy(id,word);
        if((word = strtok(NULL, " "))!=NULL){
          strcpy(virus,word);
          virus[strlen(virus)-1]='\0';
          vaccineStatusBloom(id,virus,length3);
        }
      }
    }else if(strcmp(word,"/vaccineStatus")==0){
      if((word = strtok(NULL, " "))!=NULL){
        strcpy(id,word);
        if((word = strtok(NULL, " "))!=NULL){
          strcpy(virus,word);
          virus[strlen(virus)-1]='\0';
          vaccineStatusVirus(id,virus,length3);
        }else{
          id[strlen(id)-1]='\0';
          vaccineStatus(id,length3);
        }
      }
    }else if(strcmp(word,"/insertCitizenRecord")==0 || strcmp(word,"/vaccinateNow")==0){
      if(strcmp(word,"/vaccinateNow")==0){
        flag=1;
      }
      word = strtok(NULL, " ");

      for(i=0;i<strlen(word);i++){
        if(isdigit(word[i])==0){
            printf("ERROR IN RECORD %s\n",word);
            continue;
        }
      }
      strcpy(id,word);
      word = strtok(NULL, " ");
      strcpy(first,word);
      word = strtok(NULL, " ");
      strcpy(last,word);
      word = strtok(NULL, " ");
      strcpy(country,word);
      word = strtok(NULL, " ");
      age = atoi(word);
      if(age>120 || age<0){
        printf("ERROR IN RECORD %s\n",id);
        continue;
      }
      word = strtok(NULL, " ");
      strcpy(virus,word);
      if(flag!=1){
        word = strtok(NULL, " ");
        strcpy(answer,word);
        word = strtok(NULL, " ");
        if(word ==NULL ){
          answer[strlen(answer)-1]= '\0';  //withouth the /n
        }

        if(strcmp(answer,"YES")==0 ){
          if(word==NULL|| strcmp(word,"\n")==0){
            printf("ERROR IN RECORD %s\n",id);
            continue;

          }else{
            word[strlen(word)-1]= '\0';
            strcpy(date,word);

            day = strtok(word,"-");
            if(atoi(day)>30 || atoi(day)<1){
              printf("ERROR IN RECORD %s\n",id);
              continue;
            }
            month = strtok(NULL,"-");
            if(atoi(month)<1 || atoi(month)>12){
                printf("ERROR IN RECORD %s\n",id);
                continue;
              }
              year = strtok(NULL,"-");
            if(atoi(year)>2021 || atoi(year)<1901){
              printf("ERROR IN RECORD %s\n",id);
              continue;
            }
        }

          insert(length,length2,length3,id,first,last,age,country,virus,date,&head,1);
        }else if(strcmp(answer,"NO")==0){
          if(word!=NULL){
              printf("ERROR IN RECORD %s\n",id);
              continue;
            }
          insert(length,length2,length3,id,first,last,age,country,virus,NULL,&head,1);
        }else{ //if the answer is neither YES nor NO
          printf("ERROR IN RECORD %s\n",id);
          continue;
        }
      }else{
        strftime(date, sizeof(date), "%d-%m-%Y", t); //date for vaccinateNow
        virus[strlen(virus)-1]= '\0';

        insert(length,length2,length3,id,first,last,age,country,virus,date,&head,1);
      }

    }else if(strcmp(word,"/list-nonVaccinated-Persons")==0){
        word = strtok(NULL, " ");
        strcpy(virus,word);
        virus[strlen(virus)-1]= '\0';
        printNotVaccinated(virus,length3);
    }
  }
}
