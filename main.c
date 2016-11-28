#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "get_next_line.h"

extern ssize_t		my_putchar(int, char);
extern ssize_t		my_putstr(const char *, size_t, int);
extern size_t		my_strlen(const char *, size_t);

int	main()
{
  char	*s;

  while ((s = get_next_line(0)))
    {
      my_putstr(s, my_strlen(s, 0), 1);
      my_putchar(1, '\n');
      free(s);
    }
  return 1;
}
