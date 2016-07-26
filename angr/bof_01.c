/*
** bof_01.c for exploitation course
**
** Made by Julien Bachmann
**
** Started on  Mon Apr 16 21:28:05 2012 Julien Bachmann
** Last update Thu Dec 11 08:53:17 2014 Julien Bachmann
*/

// Note: -fno-stack-protector -z execstack

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void     vulnerable(void)
{
  char          buffer[128];

  read(0, buffer, 150);
  printf("user input: %s\n", buffer);
  return;
}

int			main(int argc, char** argv)
{
  	vulnerable();
  	return 0;
}
