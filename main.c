#include <stdio.h>
#include <errno.h>  //errorno
#include <unistd.h> //getcwd
#include <fcntl.h>  //open
#include <dirent.h>
#include <string.h>
#include <ctype.h> //trim

#define INSIZE 100
#define OUTSIZE 512

char *trimwhitespace(char *str) 
{ // 앞뒤 공백 제거
  char *end;

  // 앞의 공백제거
  while((isspace(*str))) str++;

  if(*str == 0)  // 스트링 전체가 공백문자면
    return str;

  // 뒤의 공백문자 제거
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // 맨 뒤에 새로 null을 붙여줌
  *(end+1) = 0;

  return str;
}

void mypwd(char output[])
{ //pwd를 output에 저장
    if (getcwd(output, 128) == NULL)
    {
        fprintf(stderr, "PWD error %d\n", errno);
    }
}
void mycd(char *path)
{ //path로 이동
    if (chdir(path) == -1)
    {
        fprintf(stderr, "CD error %d\n", errno);
    }
}
void myhistory(char output[])
{ //history에서 읽어서 output에 저장
    int fd;
    char buffer[100];
    if ((fd = open("history", O_RDONLY | O_CREAT, 0644)) == -1)
        fprintf(stderr, "History error %d\n", errno);
    // while((read(fd,buffer,100))>0){ //나중에 동기화
    // }
    read(fd, buffer, 100);
    output = buffer;
}
void myright(char *name, char output[])
{ //name 파일 만들어서 output의 내용을 저장
    int fd;
    if ((fd = open(name, O_RDWR | O_TRUNC | O_CREAT, 0644)) == -1)
        fprintf(stderr, "Myright error %d\n", errno);
    if (write(fd, output, strlen(output)) > 0)
    {
        // printf("%d\n",strlen(output));
    }
}
void myls(char *path, char output[])
{ //path의 디렉토리 내용을 output에 저장
    struct dirent *d;
    DIR *dp;
    char *name = path;
    if (name == NULL)
        name = ".";
    if ((dp = opendir(name)) == NULL)
        fprintf(stderr, "Ls error %d\n", errno);
    while ((d = readdir(dp)))
    {
        if (d->d_ino != 0)
        {
            // printf("%s\n",d->d_name);
            char *file = strcat(d->d_name, "\n");
            strcat(output, file);
        }
    }
}
void mygrep(char *word, char output[])
{ //output을 한줄씩 잘라서 word가 포함된 줄만 프린트
    char *ptr = strtok(output, "\n");
    while (ptr != NULL)
    {
        if (strstr(ptr, word) != NULL)
        {
            printf("%s\n", ptr);
        }
        ptr = strtok(NULL, "\n");
    }
}
void execCMD(char* cmd){ // ; 포함된거는 처리 안됨
    char temp[20];
    strcpy(temp,cmd);
    char* cmdArr[10];
    char *ptr = strtok(temp," "); // cmd = cmdArr[0] 
    int i =0;
    while (ptr != NULL)
    {
        cmdArr[i] = ptr;
        i++;
        ptr = strtok(NULL," ");
    }
    for(int j=0;j<i;j++){
        printf("%s\n", cmdArr[j]);
    }

}

int seperate(char* first){ // ls, cd, pwd, ... ex> ls -al 중에 ls만 넘어옴
    if(strstr(first,"ls") != NULL){
        return 1;
    }
    if(strstr(first,"cd") != NULL){
        return 2;
    }
    if(strstr(first,"pwd") != NULL){
        return 3;
    }else{
        return -1;
    }
}

int main()
{
    char output[OUTSIZE];
    char input[INSIZE];
    char c;
    int cI = 0;
    

    while ((c = getchar()) != EOF)
    {
        if (c != 10)
        { //10 : '\n'
            input[cI++] = c;
        }
        if (c == 10)
        {
            if (strchr(input, ';') != NULL) // ; 있을때
            {
                char *ptr = strtok(input, ";"); // ; 자르기
                while (ptr != NULL)  //ptr : 하나의 cmd
                {
                    
                    execCMD(trimwhitespace(ptr));
                    printf("%s\n",ptr);
                    ptr = strtok(NULL, ";"); 
                }
            }else{   // ; 가 없을때
                execCMD(trimwhitespace(input));
                
            }
            for(int j=0;j<cI;j++){  //input 초기화
                input[j] = '\0';
            }
            cI=0;
              
        }
    }
}