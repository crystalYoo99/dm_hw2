#define _CRT_SECURE_NO WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
    char element[10];//or, not, and, a1, a2, 
    struct node * prev;
    struct node * next[100];
    int childCount;
    int order;
};
typedef struct node node;

node * makeChild(node *current, char newElement[][10], int index, int childorder);
node* changeCurrentToPrev(node *currentNode, int index);
node* changeCurrentToNext(node *currentNode, int sibling);
int makeTree(node *current, char input[][10], int size, int count);

void nnf(node *current, int start);
void findNot(node *node);
void insertNot(node*insertnode, int order);
void deleteNot(node *delnode);

void dnf(node* current, int start);
int checkANDOR(node*current);
void writingResult(FILE *file, node *current, int start);

int main(int argc, char* argv[]){
    if(argc<1){
		exit(-1);
	}
    //입력 파일 받기.
    node*head=(node*)malloc(sizeof(node));
    head->childCount=0;
    FILE *inputfile =fopen(argv[1], "r");
    FILE *ouputfile=fopen("output.txt", "w");
    char line[1000];
    char input[1000][10]={0};
    int count=0;
    if(inputfile == NULL){
        printf("please enter the file name");
		exit(-1);
    }
	fgets(line, 1000, inputfile);
    printf("input : %s\n", line);

    //token
    for(int i=0; i<strlen(line); i++){
        if(line[i]=='('){
            strcpy(input[count],"(");
            count++;
        }else if(line[i]==')'){
            strcpy(input[count], ")");
            count++;
        }else if(line[i]=='n'){
            strcpy(input[count], "not");
            i+=2;
            count++;
        }else if(line[i]=='o'){
            strcpy(input[count], "or");
            i+=1;
            count++;
        }else if(line[i]=='a'&&line[i+1]=='n'&&line[i+2]=='d'){
            strcpy(input[count], "and");
            i+=2;
            count++;
        }else if(line[i]=='a'){
            input[count][0]=line[i];
            input[count][1]=line[i+1];
            i+=1;
            count++;
        }
    }

    makeTree(head, input,count, 0);  //hanna완료, do not change
    nnf(head,1);  //hanna완료, do not change
    dnf(head,1);
    writingResult(ouputfile, head,1);    //hanna완료, do not change

    return 0;
}

int makeTree(node*current, char input[][10], int size, int count){
	if(count>=size){
		return 0;
	}
	if(strcmp("(", *(input+count))==0){
		count++;
		current->next[current->childCount]=makeChild(current,input,count,current->childCount);
		current->childCount++;
		current=changeCurrentToNext(current, current->childCount);
	}else if(strcmp(")", *(input+count))==0){
		current=changeCurrentToPrev(current,1);
	}else{   // and, or, not, a1, a2, ..
		current->next[current->childCount]=makeChild(current, input,count, current->childCount);
		current->childCount++;
	}
	return makeTree(current, input, size, count+1);
}

node * makeChild( node *current, char newElement[][10], int index, int childorder){
    node*newNode = (node*)malloc(sizeof(node));
    strcpy(newNode->element, *(newElement+index));
    newNode->prev=current;
    newNode->childCount=0;
    newNode->order=childorder;
    for(int i=0; i<100; i++){
        newNode->next[i]=NULL;
    }
    return newNode;
}

node* changeCurrentToPrev(node *currentNode, int index){
    node*current=currentNode;
    int count=0;
    if(index==0){
        return current;
    }
    while(count!=index && current !=NULL){
        count++;
        current=current->prev;  //실질적으로 tree정의에서는 이 명령어 하나만 있어도 됨.
    }
    return current;
}

node* changeCurrentToNext(node *currentNode, int child){ //lavel 올릴 때
    node*current=currentNode;
    current=currentNode->next[child-1];
    return current;
}

void nnf(node *current, int start){
    if(start==1){
        current=current->next[0];
    }
	//if(current->element[1]>=48&&current->element[1]<=57){
		//printf("%s\n", current->element);
    //}
    for(int i=0; i<current->childCount; i++){
        //not일때
        if(strcmp("not",current->element)==0&&!(current->next[0]->element[1]>=48&&current->next[0]->element[1]<=57)){
            findNot(current->next[0]);
            current=current->next[0];
            deleteNot(current->prev);
            nnf(current,0);
        }
        else{
            nnf(current->next[i],0);
        }
    }
}

void findNot(node *node){
    //다음원소가 a1,a2,...일 때 아무것도 필요없음.
    //다음원소가 and, or일 때 element바꾸고 그 다음 것에 not insert
    if(strcmp(node->element, "and")==0){
        strcpy(node->element,"or");
        //다음다음에 not insert
        for(int i=0; i<node->childCount; i++)
            insertNot(node, i);
    }else if(strcmp(node->element, "or")==0){
        strcpy(node->element,"and");
        //다음다음에 not insert
        for(int i=0; i<node->childCount; i++)
            insertNot(node, i);
    }
    //다음원소가 not일 때 다음원소도 지우기.
    if(strcmp(node->element, "not")==0){
        deleteNot(node);
    }

}

void insertNot(node*insertnode, int order){
    node*newNode = (node*)malloc(sizeof(node));
    strcpy(newNode->element, "not");
    newNode->prev=insertnode;
    newNode->childCount=1;
    newNode->order=order;
    newNode->next[0]=insertnode->next[order];
    for(int i=1; i<100; i++){
        newNode->next[i]=NULL;
    }
    insertnode->next[order]->order=0;
	insertnode->next[order]->prev=newNode;
    insertnode->next[order]=newNode;
}

void deleteNot(node *delnode){ //우선 nnf를 염두로 함.
    node *delprev=delnode->prev;
    node *delnext=delnode->next[0];
    delnext->prev=delnode->prev;
    delprev->next[delnode->order]=delnode->next[0];//delete는 not일 때만 하고, not은 child가 하나밖에 없다.
    free(delnode);
}

void dnf(node *current, int start){
    int count=0;
    //분배법칙
    if(start==1){
        current=current->next[0];
        if(strcmp(current->element, "and")==0 && checkANDOR(current)>1){
            printf("It can't be SOP");
        }
    }
    if(strcmp(current->element, "and")==0){
        for(int i=0; i<current->childCount; i++){
            count=checkANDOR(current);
            if(strcmp(current->next[i]->element, "or")==0){
                current->prev->next[current->order]=current->next[i];
                current->next[i]->prev=current->prev;
                for(int j=0; j<current->next[i]->childCount; j++){
                    current->next[i]->next[j]=current;
                    current->next[i]=current->next[i]->next[j];
                    current->next[i]->next[j]->order=i;
                }
            }
            if(strcmp(current->next[i]->element, "and")==0){
                current->prev->next[current->order]=current->next[i];
                current->next[i]->prev=current->prev;
                if(strcmp(current->next[i]->element, "or")==0){

                }
            }
        }
    }
    //결과값에 부정
}

int checkANDOR(node*current){
    int count=0;
    for(int i=0; i<current->childCount;i++){
        if(strcmp(current->next[i]->element, "and")==0||strcmp(current->next[i]->element, "or")==0){
            count++;
        }
    }
    return count;
}

void writingResult(FILE* file, node*current,int start){
    if(start==1){
        current=current->next[0];
    }
    if(strcmp(current->element, "not")==0){
        fprintf(file, "-");
    }
    if(current->element[1]>=48&&current->element[1]<=57){
        fprintf(file, "%c",current->element[1]);
    }
    for(int i=0; i<current->childCount; i++){
		writingResult(file, current->next[i],0);
        if(i!=(current->childCount-1)){
            if(strcmp(current->element, "and")==0){
                fprintf(file, "\n");
            }else if(strcmp(current->element, "or")==0){
                fprintf(file, " ");
            }
        }
    }
}
