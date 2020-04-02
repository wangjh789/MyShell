#include<stdio.h>
#include<unistd.h> //getcwd

void mypwd(char output[]){  
	getcwd (output, 128); 
}
void mycd(char* path){ //filepath
    if(chdir(path) == -1){
        printf("failed");
    }
}
void myls(){

}

int main(){
    char output[100];

    mypwd(output);
    printf("%s\n",output);
    mycd("testDir");
    mypwd(output);
    printf("%s\n",output);

}