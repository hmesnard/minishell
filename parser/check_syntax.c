#include "parser.h"

int	semicolons_valid(char *cmd)
{
	int		no_semicolon;
	int		i;
	t_pars	p;

	i = -1;
	no_semicolon = 1;
	init_pars_struct(&p, 1, 0);
	while (cmd[++i])
	{
		set_quotes(i, cmd, &p);
		if (no_semicolon == 1 && is_unesc_char(&cmd[i], i) && cmd[i] == ';'
			&& p.in_d_quotes == 0 && p.in_s_quotes == 0)
			return (0);
		if (!is_space(cmd[i]) && cmd[i] != ';')
			no_semicolon = 0;
		if (is_unesc_char(&cmd[i], i) && cmd[i] == ';')
			no_semicolon = 1;
		if (cmd[i] == '|' || cmd[i] == '>' || cmd[i] == '<')
			no_semicolon = 1;
	}
	return (1);
}

int	pipe_is_last_char(char *cmd, int i)
{
	while (cmd[i])
	{
		if (!is_space(cmd[i]))
			return (0);
		i++;
	}
	return (1);
}

int	pipes_valid(char *cmd)
{
	int		no_pipe;
	int		i;
	t_pars	p;

	i = -1;
	no_pipe = 1;
	init_pars_struct(&p, 1, 0);
	while (cmd[++i])
	{
		set_quotes(i, cmd, &p);
		if (no_pipe == 1 && cmd[i] == '|' && is_unesc_char(&cmd[i], i)
			&& p.in_d_quotes == 0 && p.in_s_quotes == 0)
			return (0);
		if (!is_space(cmd[i]) && cmd[i] != '|')
			no_pipe = 0;
		if (cmd[i] == '|' && is_unesc_char(&cmd[i], i))
		{
			if (pipe_is_last_char(cmd, i + 1))
				return (0);
			no_pipe = 1;
		}
		if (cmd[i] == ';' || cmd[i] == '>' || cmd[i] == '<')
			no_pipe = 1;
	}
	return (1);
}

int	are_quotes_closed(char *cmd)
{
	int		i;
	t_pars	p;

	i = -1;
	init_pars_struct(&p, 1, 0);
	while (cmd[++i])
		set_quotes(i, cmd, &p);
	if (p.in_s_quotes != 0)
	{
		set_exit_status("unexpected EOF while looking for matching `''", 2);
		return (0);
	}
	if (p.in_d_quotes != 0)
	{
		set_exit_status("unexpected EOF while looking for matching `\"'", 2);
		return (0);
	}
	return (1);
}

int	check_syntax(char *cmd)
{
	if (!are_quotes_closed(cmd))
		return (0);
	if (!semicolons_valid(cmd))
	{
		set_exit_status("syntax error near unexpected token `;'", 2);
		return (0);
	}
	if (!pipes_valid(cmd))
	{
		set_exit_status("syntax error near unexpected token `|'", 2);
		return (0);
	}
	return (1);
}
