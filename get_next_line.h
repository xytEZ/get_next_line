#ifndef GET_NEXT_LINE_H_
# define GET_NEXT_LINE_H_

# define MAX_READ	8
# define TO_INT(x)	(int)(x)

typedef struct	s_env
{
  char		*s;
  size_t	size;
  short		is_newline;
  struct s_env	*next;
}		t_env;

typedef struct	s_wrap
{
  int		quota_newline;
  t_env		*env;
}		t_wrap;

char		*get_next_line(const int);

#endif	/* !GET_NEXT_LINE_H_ */
