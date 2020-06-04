#include <stdio.h>
#include <string.h>
#include <dirent.h>

void split(char *src,const char *separator,char **dest,int *num) {
	/*
		src 源字符串的首地址(buf的地址) 
		separator 指定的分割字符
		dest 接收子字符串的数组
		num 分割后子字符串的个数
	*/
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止 
        return;
     if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止 
        return;
     pNext = (char *)strtok(src,separator); //必须使用(char *)进行强制类型转换(虽然不写有的编译器中不会出现指针错误)
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  //必须使用(char *)进行强制类型转换
    }  
    *num = count;
} 	

void parentdir(char *path)
{
    int i = 0;
    int k = 0;
    for (i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            k = i;
        }
    }
    for (i = k; i >= 0; i--)
    {
        if (path[i] == '/')
        {
            path[i] = 0;
        }
        else
        {
            break;
        }
    }
}

// typedef	struct NNIE_JPEG
// {
// 	void (*callback)(char * str)
// };

typedef void (*callback)(char * str);
int dir_read(char *path)
{
    DIR             *dir = NULL;
    struct dirent   *dp = NULL;

    if ((dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "Open dir error...\n");
        return 1;
    }
    while ((dp = readdir(dir)) != NULL)
    {
        if (dp->d_name[0] == '.') // . .. .svn
        {
            continue;
        }
        else if (dp->d_type == DT_REG) // file
        {
			int num = 0;
            // filter_elf(path, dp->d_name);
			// printf("%s\n",path);
			// printf("%s\n",dp->d_name);
			// 文件执行
			if(strstr(dp->d_name,".bgr")){

				// for(int i = 0;i < num; i ++) {
				// printf("%s\n",revbuf[i]);
                // 合并路径和文件名
                char * s = (char * )malloc(strlen(dp->d_name)+strlen(path)+2);
                sprintf(s,"%s/%s",path,dp->d_name);
                printf("%s\n",s);

                // 文件名解析
				char *b = strdup(dp->d_name);   // 复制d_name字符串
				split(b,"-.",revbuf,&num);
				if (num >3)
				{
					printf("n: %d\n",num);
					printf("w: %d\n",atoi(revbuf[num-3]));
					printf("h: %d\n",atoi(revbuf[num-2]));
				}
                free(s);
                free(b);
				
			}
				
		
			

        }
        else if (dp->d_type == DT_LNK) // link file
        {
            //printf("DT_LNK:%s/%s\n", path, dp->d_name);
        }
        else if (dp->d_type == DT_DIR) // dir
        {
            strcat(path, "/");
            strcat(path, dp->d_name);
            dir_read(path);
            parentdir(path);
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc,char* argv[])
{

	dir_read(argv[1]);
}


 
 
// int main(){
// 	int i;
// 	char buf[]="www.baidu.com,www.taobao.com,www.csdn.com,www.python.org";
// 	//用来接收返回数据的数组。这里的数组元素只要设置的比分割后的子字符串个数大就好了。
// 	char *revbuf[8] = {0}; //存放分割后的子字符串 
	
// 	//分割后子字符串的个数
// 	int num = 0;
	
// 	split(buf,",",revbuf,&num); //调用函数进行分割 
	
	
// 	//输出返回的每个内容 
// 	for(i = 0;i < num; i ++) {
// 		//lr_output_message("%s\n",revbuf[i]);
// 		printf("%s\n",revbuf[i]);
// 	}
 
 
// 	return 0;
// }