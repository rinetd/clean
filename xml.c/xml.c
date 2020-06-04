#include <stdio.h>
 
//author: dengzhaoqun
//date: 2012/03/05
 
static FILE *out = NULL;
static int tabs = 0;
 
void set_out_fp(FILE *fp)
{
	out = fp;
}
 
void put(char *str)
{
	fprintf(out, "%s", str);
}
 
void put_head(char *head)
{
	put("<?");
	put(head);
	put("?>\n");
}
 
void out_tabs()
{
	int i;
	for(i=0; i < tabs; i++)
	{
		put("\t");
	}
}
 
void tag_start(char *tag)
{
	out_tabs();
	put("<");
	put(tag);
	put(">\n");
	tabs = tabs + 1;
}
 
void tag_end(char *tag)
{
	tabs = tabs - 1;
	out_tabs();
	put("</");
	put(tag);
	put(">\n");
}
 
void tag_content(char *tag, char *content)
{
	out_tabs();
	put("<");
	put(tag);
	put(">");
	put(content);
    put("</"); put(tag);put(">\n");

}
void tag_name(char *tag, char *name)
{
	out_tabs();
	put("<");
	put(tag);
	put("\t");
	put("name=\"");
	put(name);
	put("\"");
	put(">\n");
    tabs = tabs + 1;

}

void tag_value(char *tag, char *value)
{
	out_tabs();
	put("<");
	put(tag);
	put("\t");
	put("\"value\"=\"");
	put(value);
	put("\"");
	put("\t/>\n");
}
 
void tag_value_num(char *tag, long value)
{
	out_tabs();
	put("<");
	put(tag);
	put("\t");
	put("\"value\"=");
	fprintf(out, "%d", value);
	put("\t/>\n");
}

void tag_result_filename_flag(char *tag, char * filename, char flag)
{

	out_tabs();
	put("<");
	put(tag);
	put(" ");
	put("filename=");
    fprintf(out, "\"%s\"", filename);
    fprintf(out, " flag=");
    if (flag)
    {
	    put("\"True\"");
    }else
    {
         put("\"False\"");
    }

	put("/>\n");
        	tabs = tabs + 1;

}
FILE *fp=NULL;
int xml_head(char * filename){
	fp = fopen( "a.xml","w");
	set_out_fp(fp);
 
	put_head("xml version='1.0' encoding=\"utf-8\"");
	tag_start("Annotation");

}

int xml_body(){
	tag_result_filename_flag("result","DaoXianYiWu0020",1);
                tag_start("size");
                    tag_content("width","1233");
                    tag_content("height","1233");
                    tag_content("depth","3");
                tag_end("size");

                tag_name("object","diaoche");
                    tag_start("bndbox");
                    // tag_value_num("网卡", 1);
                    tag_content("xmin","1233");
                    tag_content("ymin","1233");
                    tag_content("xmax","3");
                    tag_content("ymax","3");
                    tag_end("bndbox");
                tag_end("object");
        tag_end("result");
}
int xml_footer(char * filename){

	tag_end("Annotation");
 
	fclose(fp);

}



int main()
{	
	FILE *fp = fopen( "a.xml","w");
	set_out_fp(fp);
 
	put_head("xml version='1.0' encoding=\"utf-8\"");
	tag_start("Annotation");


        tag_result_filename_flag("result","DaoXianYiWu0020",1);
                tag_start("size");
                    tag_content("width","1233");
                    tag_content("height","1233");
                    tag_content("depth","3");
                tag_end("size");

                tag_name("object","diaoche");
                    tag_start("bndbox");
                    // tag_value_num("网卡", 1);
                    tag_content("xmin","1233");
                    tag_content("ymin","1233");
                    tag_content("xmax","3");
                    tag_content("ymax","3");
                    tag_end("bndbox");
                tag_end("object");
        tag_end("result");



	tag_end("Annotation");
 
	fclose(fp);
 
	return 0;
}