#include <stdio.h>
#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>

extern void config();
int init(){
    static int flag =0;
    if (flag)
    {
          printf("init already finished!\n%s,%d",__func__,__LINE__);
        /* code */
        return 1;
    }

    printf("init\n");
    flag =1;
    

}
int main()
{
    init();
	config();
    init();
    init();
    init();

	return 0;
}