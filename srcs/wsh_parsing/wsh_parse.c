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

void			wsh_replacevar(t_wsh_list *wsh_list, char pipe[1024], int c_pos)
{
	char	var[1024];
	char	*c_var;
	int		c_i;

	c_i = 0;
	while (pipe[c_pos] != '\0')
	{
		if (ft_isalpha(pipe[c_pos]))
			var[c_i++] = pipe[c_pos];
		else
			break;
		c_pos++;
	}
	c_var = ft_strdup(wsh_get_envar((char *)var, wsh_list->wsh_envs));
	return ;
}

void			wsh_escape(t_wsh_list *wsh_list, char pipe[1024])
{
	int		c_i;
	int		c_sq;
	int		c_p;

	c_sq = 0;
	c_p = 0;
	c_i = 0;
	while (pipe[c_i] != '\0')
	{
		if (c_p == 0 && c_sq == 0 && pipe[c_i] == SQUOTE)
			c_sq = 1;
		else if (c_sq == 1 && pipe[c_i] == SQUOTE)
			c_sq = 0;
		if (c_p == 0 && c_sq == 0 && pipe[c_i] == VAR)
			wsh_replacevar(pipe, c_i);
		if (c_p == 0 && c_sq == 0 && pipe[c_i] == ESC)
			c_p = 1;
		else if (c_p == 1 && pipe[c_i] == ESC)
			c_p = 0;
		c_i++;
	}
	return ;
}

t_wsh_tokens	*wsh_fillCommands(t_wsh_tokens *wsh_token, char pipe[][1024])
{
	int		counter;
	int		i;
	char	foreach[1024][1024];
	int		j;

	i = 0;
	j = 0;
	g_i = 0;
	g_j = 1;
	while (i < 1024)
		ft_memset(foreach[i++], '\0', 1024);
	counter = 0;
	while (pipe[counter][0] != '\0')
	{
		i = 0;
		wsh_escape(wsh_list, pipe[counter]);
		if (wsh_tokenizer(foreach, pipe[counter], 2) == ERROR)
			return (NULL);
		wsh_token->wsh_command = ft_strdup(foreach[i++]);
		if (ft_isbuiltin(wsh_token->wsh_command))
			wsh_token->type = BUILTIN;
		wsh_fillargs(wsh_token, foreach, &i);
		wsh_fillparams(wsh_token, foreach, &i);
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

t_wsh_tokens	*wsh_parse(char *cmd)
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
		wsh_token = wsh_fillCommands(wsh_token, pipe);
		if (array[++i][0] != '\0')
		{
			if (!(wsh_token->next = wsh_token_init()))
				return (NULL);
			wsh_token = wsh_token->next;
		}
	}
	return (wsh_token_first);
}
