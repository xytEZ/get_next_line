#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "get_next_line.h"

extern ssize_t	my_putchar(int, char);
extern ssize_t	my_putstr(const char *, size_t, int);
extern size_t	my_strlen(const char *, size_t);

static t_wrap	g_wrap = { 0, NULL };

static int	push_back_in_env(const char *s, size_t size, int i)
{
  t_env		*new;
  t_env		*tmp;

  if (!(new = malloc(sizeof(*new)))
      || !(new->s = malloc(sizeof(*new->s) * (size + 1))))
    return 0;
  while (i < TO_INT(size))
    new->s[i++] = *s++;
  new->s[i] = '\0';
  new->size = size;
  new->is_newline = (g_wrap.quota_newline != 0) ? 1 : 0;
  if (g_wrap.quota_newline != 0)
    g_wrap.quota_newline = 0;
  new->next = NULL;
  if (!g_wrap.env)
    g_wrap.env = new;
  else
    {
      tmp = g_wrap.env;
      while (tmp->next)
	tmp = tmp->next;
      tmp->next = new;
    }
  return 1;
}

static char	*concat_strs_until_newline(t_env *tmp,
					   size_t size,
					   int i,
					   int j)
{
  char		*s;

  tmp = g_wrap.env;
  while (tmp && tmp->is_newline != 1)
    {
      size += tmp->size;
      tmp = tmp->next;
    }
  if (!(s = malloc(sizeof(*s) * (size + 1))))
    return NULL;
  tmp = g_wrap.env;
  while (g_wrap.env && g_wrap.env->is_newline != 1)
    {
      tmp = g_wrap.env;
      g_wrap.env = g_wrap.env->next;
      j = 0;
      while (tmp->s[j])
	s[i++] = tmp->s[j++];
      free(tmp->s);
      free(tmp);
    }
  s[i] = '\0';
  if (g_wrap.env)
    g_wrap.env->is_newline = 0;
  return s;
}

static int	get_presence_newline()
{
  const t_env	*tmp;

  tmp = g_wrap.env;
  while (tmp)
    {
      if (tmp->is_newline == 1)
	return 1;
      tmp = tmp->next;
    }
  return 0;
}

static short	g_presence_newline = 0;

static int	cut_str_and_store_substr(const char *s, int i, int j, int k)
{
  char		*s2;
  size_t	linesize;

  while (s[i] != '\n' && s[i])
    ++i;
  if ((linesize = i - j) > 0)
    {
      if (!(s2 = malloc(sizeof(*s2) * (linesize + 1))))
	return 0;
      while (j < i)
	s2[k++] = s[j++];
      s2[k] = '\0';
      if (!push_back_in_env(s2, linesize, 0))
	return 0;
      free(s2);
    }
  if (s[i])
    {
      g_presence_newline = 1;
      if (linesize > 0)
	g_wrap.quota_newline = 1;
      if (!cut_str_and_store_substr(s, ++i, ++j, 0))
	return 0;
    }
  return 1;
}

char		*get_next_line(const int fd)
{
  char		*s;
  ssize_t	bytes_read;

  if (!g_wrap.env || !get_presence_newline())
    {
      if (!(s = malloc(sizeof(*s) * MAX_READ)))
	return 0;
      g_presence_newline = 0;
      while (!g_presence_newline && (bytes_read = read(fd, s, MAX_READ)) > 0)
	{
	  s[bytes_read] = '\0';
	  if (!cut_str_and_store_substr(s, 0, 0, 0))
	    return 0;
	}
      g_wrap.quota_newline = 0;
      free(s);
    }
  return (g_wrap.env) ? concat_strs_until_newline(NULL, 0, 0, 0) : NULL;
}
