#include "mxml/config.h"
#include "mxml/mxml.h"


// #include <sys/socket.h>
// #include <sys/ioctl.h>
// #include <linux/if.h>
// #include <netdb.h>
#include <stdio.h>
#include <string.h>



/*
 * Globals...
 */


int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line args */
     char *argv[])			/* I - Command-line args */
{
  int			i;		/* Looping var */
  FILE			*fp;		/* File to read */
  int			fd;		/* File descriptor */
  mxml_index_t		*ind;		/* XML index */
  char			buffer[16384];	/* Save string */
  wchar_t   pwcs[32768];
  memset(pwcs, 0, 32768);
  size_t len;


  static const char	*types[] =	/* Strings for node types */
			{
			  "MXML_ELEMENT",
			  "MXML_INTEGER",
			  "MXML_OPAQUE",
			  "MXML_REAL",
			  "MXML_TEXT"
			};


  mxml_node_t *xml;    /* <?xml ... ?> */
  mxml_node_t *data;   /* <data> */
  mxml_node_t *node;   /* <node> */
  mxml_node_t *dict;   /* <node> */
  mxml_node_t *dict2;   /* <node> */
  mxml_node_t *group;  /* <group> */

  data = mxmlNewElement(NULL, "array");

  dict = mxmlNewElement(data, "dict");
  node = mxmlNewElement(dict, "key");
  mxmlNewText(node, 0, "IOMatch");
  dict2 = mxmlNewElement(dict, "dict");
  node = mxmlNewElement(dict2, "key");
  mxmlNewText(node, 0, "BSD Name");
  node = mxmlNewElement(dict2, "string");
  mxmlNewText(node, 0, "en0");
  node = mxmlNewElement(dict2, "key");
  mxmlNewText(node, 0, "IOProviderClass");
  node = mxmlNewElement(dict2, "string");
  mxmlNewText(node, 0, "IONetworkInterface");
  node = mxmlNewElement(dict, "key");
  mxmlNewText(node, 0, "BLMACAddress");
  node = mxmlNewElement(dict, "data");
  // mxmlNewText(node, 0, getmac());
  /* dict2 = mxmlNewElement(dict, "dict"); */
  /* node = mxmlNewElement(dict2, "key"); */
  /* mxmlNewText(node, 0, "IOEFIDevicePathType"); */
  /* node = mxmlNewElement(dict2, "string"); */
  /* mxmlNewText(node, 0, "MessagingIPv4"); */
  /* node = mxmlNewElement(dict2, "key"); */
  /* mxmlNewText(node, 0, "RemoteIpAddress"); */
  /* node = mxmlNewElement(dict2, "string"); */
  /* mxmlNewText(node, 0, "192.168.5.1"); */


  // mxmlSaveString(data, buffer, sizeof(buffer), MXML_NO_CALLBACK);
  // writevar((unsigned char*) buffer);
  fp = fopen("filename.xml", "w");
  fwrite(buffer, strlen(buffer), 1, fp);
  /* len = mbstowcs( pwcs, buffer, 32768); */
  /* fwrite(pwcs, len, 1, fp); */
  fclose(fp);
}
