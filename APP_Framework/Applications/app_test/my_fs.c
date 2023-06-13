/*
* Copyright (c) 2020 AIIT XUOS Lab
* XiUOS is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*        http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*/

/**
* @file:    test_fs.c
* @brief:   a application of file_system function
* @version: 1.1
* @author:  AIIT XUOS Lab
* @date:    2022/11/17
*/
#include <transform.h>
#include <my_board.h>
#include <stdio.h>
#include <string.h>

#define MAX_READ_LENGTH 311296
#define STORE_PATH "/IMAGE.BMP"

uint8_t filewords[MAX_READ_LENGTH];

// sd card here is loaded as "/"
void TestMyFs(void)
{
    //open the file in sdcard
    int fd = open(FPATH,O_RDWR);
    fd = fread();
    if(fd<0){
        printf("fs fd open error:%d\n",fd);
        return;
    }
    printf("open finish\n");
    memset(filewords,0,MAX_READ_LENGTH);

    //read and write then close file
    int err_flag = read(fd,filewords,MAX_READ_LENGTH);
    if(err_flag<0){
        printf("read failed,error:%d\n",err_flag);
        return;
    }
    printf("read finish\n");
    err_flag = close(fd);
    if(err_flag<0){
        printf("close failed,error %d\n",err_flag);
        return ;
    }
    
    // for( int i = 0; i < 10000; ++i ) {
    //     printf("%u", filewords[i]);
    // }
    
    fd = open(STORE_PATH,O_RDWR | O_CREAT);
    err_flag = write(fd,filewords,strlen(filewords));
    if(err_flag<0){
        printf("write failed,error:%d\n",err_flag);
        return;
    }
    printf("write finish\n");
    err_flag = close(fd);
    if(err_flag<0){
        printf("close failed,error %d\n",err_flag);
        return ;
    }  

    return;
}

PRIV_SHELL_CMD_FUNCTION(TestMyFs, a sd or usb filesystem test sample, PRIV_SHELL_CMD_MAIN_ATTR);