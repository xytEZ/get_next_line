#include <unistd.h>

ssize_t		my_putchar(int fd, char c)
{
  return (write(fd, &c, 1));
}

ssize_t		my_putstr(const char *s, size_t size, int fd)
{
  return (write(fd, s, size));
}

size_t		my_strlen(const char *s, size_t size)
{
  return (s && *s ? my_strlen(++s, ++size) : size);
}
