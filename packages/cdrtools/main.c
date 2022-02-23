#include <string.h>

extern int cdrecord_main(int argc, char** argv);
extern int readcd_main(int argc, char** argv);

int main(int argc, char** argv)
{
  char* s;
  char* name;

  name = argv[0];
  if (*name == '-')
    name++;
  s = name;
  while (*s)
    if (*(s++) == '/')
      name = s;
  if (!memcmp(name,"cdrecord",sizeof("cdrecord")))
    return cdrecord_main(argc,argv);
  if (!memcmp(name,"readcd",sizeof("readcd")))
    return readcd_main(argc,argv);
  return 127;
}
