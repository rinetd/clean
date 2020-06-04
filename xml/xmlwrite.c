#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<mxml.h>
int main()
{
    //文件头
    mxml_node_t *root=mxmlNewXML("1.0");
    //根标签 china 
    mxml_node_t*china =mxmlNewElement(root,"china");
    //字标签 城市
    mxml_node_t*city=mxmlNewElement(china,"city");
    mxml_node_t*info=mxmlNewElement(city,"name");
    //给标签赋值
    mxmlNewText(info,0,"北京");
    mxmlElementSetAttr(info,"isbig","Yes");
    //面积
    info=mxmlNewElement(city,"area");
    mxmlNewText(info,0,"16410平方公里");
    //人口
    info=mxmlNewElement(city,"population");
    mxmlNewText(info,0,"2171万人");
    info=mxmlNewElement(city,"GDP");
    mxmlNewText(info,0,"21410亿");

    //东京
    //字标签 城市
    city=mxmlNewElement(china,"city");
    info=mxmlNewElement(city,"name");
    //给标签赋值
    mxmlNewText(info,0,"东京");
    mxmlElementSetAttr(info,"isbig","Yes");
    //面积
    info=mxmlNewElement(city,"area");
    mxmlNewText(info,0,"2188平方公里");
    //人口
    info=mxmlNewElement(city,"population");
    mxmlNewText(info,0,"3670万人");
    info=mxmlNewElement(city,"GDP");
    mxmlNewText(info,0,"31700亿元");
    

    //数据保存到磁盘文件
    FILE *fp=fopen("china.xml","w");
    mxmlSaveFile(root,fp,MXML_NO_CALLBACK);
    fclose(fp);
    mxmlDelete(root);
    //删除释放
    //printf("Hello world\n");
    return 0;
}