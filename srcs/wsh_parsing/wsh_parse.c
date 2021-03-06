/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wsh_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 16:25:23 by mashad            #+#    #+#             */
/*   Updated: 2021/03/12 15:20:17 by oel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		wsh_tokenizer(char cmd[][1024], char *string, int pipe)
{
	char    *token;
	int		i;

	i = 0;
	if (!(token = (char *)malloc(sizeof(char) * 1024)))
		return (ERROR);
	while (wsh_scan_commands(token, (const char *) string, pipe))
		ft_strlcpy((char *)cmd[i++], (const char *)token, ft_strlen(token)+1);
	ft_strlcpy((char *)cmd[i++], (const char *)token, ft_strlen(token)+1);
	cmd[i][0]= '\0';
	wsh_free((void *) token);
	return (1);
}

void			*wsh_fillargs(t_wsh_tokens *wsh_token, char wsh_args[][1024], int *position)
{
	int		counter;

	counter = 0;
	if (wsh_args[*position][0] != '-')
		return (NULL);
	if (!(wsh_token->wsh_arg = (char **)malloc(sizeof(char *) * 1024)))
		return (NULL);
	while (wsh_args[*position][0] == '-' && wsh_args[*position][0] != '\0')
		wsh_token->wsh_arg[counter++] = ft_strdup(wsh_args[(*position)++]);
	if (wsh_args[*position][0] == '-' && wsh_args[*position][0] != '\0')
		wsh_token->wsh_arg[counter++] = ft_strdup(wsh_args[(*position)++]);
	wsh_token->wsh_arg[counter] = 0;
	return (NULL);
}

void			*wsh_fillparams(t_wsh_tokens *wsh_token, char wsh_params[][1024], int *position)
{
	int		counter;

	counter = 0;
	if (wsh_params[*position][0] == '\0' || wsh_params[*position][0] == '-')
		return (NULL);
	if (!(wsh_token->wsh_param = (char **)malloc(sizeof(char *) * 1024)))
		return (NULL);
	while (wsh_params[*position][0] != '\0')
		wsh_token->wsh_param[counter++] = ft_strdup(wsh_params[(*position)++]);
	if (wsh_params[*position] && wsh_params[*position][0] != '\0')
		wsh_token->wsh_param[counter++] = ft_strdup(wsh_params[(*position)++]);
	wsh_token->wsh_param[counter] = 0;
	return (NULL);
}

int	g_i;
int g_j;

int				ft_ispipe(char token[][1024], int ret)
{
	while (token[g_i][0] != '\0')
		g_i++;
	if (g_i == g_j)
		return (ret);
	g_j++;
	return (666);
}

int				ft_isspecial(char *c, int c_pos)
{
	if (c[c_pos] == '_' || c[c_pos] == '#' || c[c_pos] == '$' || c[c_pos] == '-')
		return (1);
	return (0);
}

void			wsh_replacevar(char **envs, char pipe[1024], int c_pos)
{
	char	var[1024];
	char	*c_var;
	int		c_i;
	int		c_j;
	int		c_k;

	c_j = c_pos++;
	c_i = 0;
	c_k = 0;
	if (pipe[c_pos] == '\0')
		return ;
	if (ft_isspecial(pipe, c_pos))
		var[c_i++] = pipe[c_pos++];
	else
		while (pipe[c_pos] != '\0' && (ft_isalnum(pipe[c_pos]) || pipe[c_pos] == '_'))
			var[c_i++] = pipe[c_pos++];
	var[c_i] = EOL;
	c_var = ft_strdup(wsh_get_envar((char *)var, envs));
	c_i = 0;
	while (pipe[c_i] != EOL && pipe[c_i] != VAR)
		var[c_k++] = pipe[c_i++];
	c_i = 0;
	while (c_var && c_var[c_i] != EOL)
		var[c_k++] = c_var[c_i++];
	while (pipe[c_pos] != EOL)
		var[c_k++] = pipe[c_pos++];
	var[c_k] = EOL;
	ft_strlcpy(pipe, var, ft_strlen(var) + 1);
	return ;
}

int			wsh_quotesremove(char c, int c_sq , int c_dq, int c_p)
{
	if ((c == SQUOTE && c_dq == 1) ||(c == DQUOTE && c_sq == 1))
		return (1);
	if ((c == SQUOTE && c_p == 1) ||(c == DQUOTE && c_p == 1))
		return (1);
	if (c_p == 0 && c != SQUOTE && c != DQUOTE)
		return (1);
	return (0);
}


void			wsh_escape(char **envs, char pipe[1024])
{
	int		c_i;
	int		c_j;
	int		c_sq;
	int		c_dq;
	int		c_p;
	char	newpipe[1024];

	c_sq = 0;
	c_dq = 0;
	c_p = 0;
	c_i = 0;
	c_j = 0;
	while (pipe[c_i] != '\0')
	{
		if (c_p == 0 && c_dq == 0 && c_sq == 0 && pipe[c_i] == SQUOTE)
			c_sq = 1;
		else if (c_sq == 1 && pipe[c_i] == SQUOTE)
			c_sq = 0;
		if (c_p == 0 && c_sq == 0 && c_dq == 0 && pipe[c_i] == DQUOTE)
			c_dq = 1;
		else if (c_dq == 1 && pipe[c_i] == DQUOTE)
			c_dq = 0;
		if (c_p == 0 && c_sq == 0 && pipe[c_i] == VAR && pipe[c_i + 1] != '\0')
		{
			wsh_replacevar(envs, pipe, c_i);
			c_i = 0;
			c_j = 0;
		}
		if (c_p == 0 && c_sq == 0 && pipe[c_i] == ESC)
			c_p = 1;
		if (wsh_quotesremove(pipe[c_i], c_sq, c_dq, c_p))
			newpipe[c_j++] = pipe[c_i];
		if (c_p == 1 && c_sq == 0 && pipe[c_i - 1] == ESC)
			c_p = 0;
		c_i++;
	}
	newpipe[c_j] = EOL;
	ft_strlcpy(pipe, newpipe, ft_strlen(newpipe) + 1);
	return ;
}

t_wsh_tokens	*wsh_fillCommands(char **envs, t_wsh_tokens *wsh_token, char pipe[][1024])
{
	int		counter;
	int		i;
	char	redirec[1024][1024];
	char	foreach[1024][1024];
	int		j;
	int		c_w;
	int		c_k;

	i = 0;
	j = 0;
	c_k = 0;
	c_w = 0;
	g_i = 0;
	g_j = 1;
	while (i < 1024)
		ft_memset(foreach[i++], '\0', 1024);
	counter = 0;
	while (pipe[counter][0] != '\0')
	{
		i = 0;
		c_k = 0;
		wsh_escape(envs, pipe[counter]);
		if (wsh_tokenizer(redirec, pipe[counter], 3) == ERROR)
			return (NULL);	
		if (redirec[0])
		{
			if (wsh_tokenizer(foreach, redirec[0], 2) == ERROR)
				return (NULL);
			wsh_token->wsh_command = ft_strdup(foreach[i++]);
			if (ft_isbuiltin(wsh_token->wsh_command))
				wsh_token->type = BUILTIN;
			wsh_fillargs(wsh_token, foreach, &i);
			wsh_fillparams(wsh_token, foreach, &i);
			c_k++;
		}
		while (redirec[c_k][0] != '\0')
		{
			i = 0;
			if (!(wsh_token->wsh_redi = wsh_redi_init()))
					return (NULL);
			if (wsh_tokenizer(foreach, redirec[c_k], 2) == ERROR)
				return (NULL);
			wsh_token->wsh_redi->filename= ft_strdup(foreach[i++]);
			wsh_token->type = REDIREC;
			if (redirec[++c_k][0] != '\0')
			{
				if (!(wsh_token->wsh_redi->next = wsh_redi_init()))
					return (NULL);
				wsh_token->wsh_redi = wsh_token->wsh_redi->next;
			}
			c_k++;
		}
		if (pipe[++counter][0] != '\0')
		{
			if (!(wsh_token->next = wsh_token_init()))
				return (NULL);
			wsh_token->std_out = ft_ispipe(pipe, 1);
			wsh_token = wsh_token->next;
		}
	}
	return (wsh_token);
}

t_wsh_tokens	*wsh_parse(char **envs, char *cmd)
{
	char			array[1024][1024];
	char			pipe[1024][1024];
	t_wsh_tokens	*wsh_token;
	t_wsh_tokens	*wsh_token_first;
	int				i;
	int				j;

	i = 0;
	j = 0;
	wsh_token = wsh_token_init();
	if (wsh_tokenizer(array, cmd, 0) == ERROR)
		return (NULL);
	wsh_token_first = wsh_token;
	while (array[i][0] != '\0')
	{
		if (wsh_tokenizer(pipe, array[i], 1) == ERROR)
			return (NULL);
		wsh_token->std_in = 0;
		wsh_token = wsh_fillCommands(envs, wsh_token, pipe);
		if (array[++i][0] != '\0')
		{
			if (!(wsh_token->next = wsh_token_init()))
				return (NULL);
			wsh_token = wsh_token->next;
		}
	}
	return (wsh_token_first);
}
