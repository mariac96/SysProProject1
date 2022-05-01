#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "skiplist.h"

int readfile(char* filename,int length,int length2,int length3,datenode**head);
int vaccineStatusBloom(char *id, char *virus,int size);
int exitAll(int length,int length2,int length3,datenode **head);
int vaccineStatusVirus(char*id,char*virus,int length3);
int  vaccineStatus(char*id,int length3);
int printNotVaccinated(char*virus,int size);
#endif
