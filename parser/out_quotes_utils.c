#include "parser.h"

int	must_append(int i, char *cmd, t_pars *p)
{
	if (p->not_append == 1)
		return (0);
	if (p->prev_var_w_space == 1)
	{
		p->prev_var_w_space = 0;
		return (0);
	}
	if (i != 0 && !is_r_space(&cmd[i - 1], i - 1)
		&& !is_r_resvd_char(&cmd[i - 1], i - 1, 0))
		return (1);
	return (0);
}

char	*make_word(char	*word, t_pars *p, t_package *s)
{
	char		*new_word;
	int			j;
	static int	i;

	while (is_space(word[i]) && word[i])
		i++;
	j = i;
	while (!is_space(word[i]) && word[i])
		i++;
	if (j == i)
	{
		if (is_space(word[i - 1]))
			p->prev_var_w_space = 1;
		i = 0;
		return (NULL);
	}
	new_word = ft_strdup_len(word + j, i - j);
	if (!new_word)
	{
		free(word);
		error_free_pars(s->cmd, s->b_list, s->p_begin);
	}
	return (new_word);
}

char 	*get_redir_word(t_pars *p, t_package *s, char *word, char *new_word)
{
	while (is_space(s->cmd[p->i]))
		p->i += 1;
	p->r = 1;
	if (s->cmd[p->i] == '\'')
		word = arg_simple_quotes(p, s);
	else if (s->cmd[p->i] == '"')
		word = arg_double_quotes(p, s);
	else
		word = get_next_word(p, s);
	while (!is_r_space(&s->cmd[p->i], p->i)
		&& !is_r_resvd_char(&s->cmd[p->i], p->i, 0) && s->cmd[p->i])
	{
		if (s->cmd[p->i] == '\'')
			word = ft_strjoin_w_ns(word, arg_simple_quotes(p, s));
		else if (s->cmd[p->i] == '"')
			word = ft_strjoin_w_ns(word, arg_double_quotes(p, s));
		else
		{
			new_word = get_next_word(p, s);
			word = ft_strjoin_w_ns(word, new_word);
		}
	}
	p->r = 0;
	return (word);
}
