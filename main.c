#include<stdio.h>
#include<unistd.h> //getcwd

void mypwd(){ 
    char buff[100];
    getcwd(buff,100);
    printf("%s\n",buff);
}
void mycd(char* path){ //filepath
    if(chdir(path) == -1){
        printf("failed");
    }
}

int main(){
    mycd("testDir");
    mypwd();
}