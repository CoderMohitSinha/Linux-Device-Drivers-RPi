#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main(){

    int fd;
    char option;

    printf("welcome to the demo of character device driver....\n");

    fd = open("/dev/my_device", O_RDWR);
    if(fd < 0){
        printf("cannot open the device file....\n");
    }

    while(1){
        printf("******please enter your options*******\n");
        printf("            1. write            \n");
        printf("            2. read            \n");
        printf("            3. exit            \n");

        scanf("%c", &option);
        printf("your selected option is = %c\n", option);

        switch (option)
        {
        case '1':
            printf("enter the string to write into the driver:\n");
            scanf("%[^\t\n]s", write_buf);
            printf("data writting....\n");
            write(fd, write_buf, strlen(write_buf)+1);
            printf("writting done....\n");
            break;

       case '2':
            printf("reading the data....\n");
            read(fd, read_buf, 1024);
            printf("reading done....\n");
            printf("date = %s\n\n", read_buf);


        case '3':
            close(fd);
            exit(1);
        default:
            printf("enter the valid options = %c\n", option);
            break;
        }
    }

    close(fd);

}