#include "minishell.h"

int		wsh_removevar(char **wsh_envs, int c_p)
{
	while (wsh_envs[c_p] != NULL)
	{
		wsh_free((void *)wsh_envs[c_p]);
		wsh_envs[c_p] = NULL;
		if (wsh_envs[c_p + 1] != NULL)
			wsh_envs[c_p] = ft_strdup(wsh_envs[c_p + 1]);
		c_p++;
	}
	return (0);
}

int		wsh_searchenv(char **wsh_envs, char *var)
{
	int		c_i;

	c_i = 0;
	while (wsh_envs && wsh_envs[c_i] != NULL)
	{
		
		if (ft_strncmp(wsh_envs[c_i], var, ft_strlen(var)) == 0)
			return (c_i);
		c_i++;
	}
	return (0);
}

void	wsh_unset(t_wsh_tokens *wsh_token, t_wsh_list *wsh_list)
{
	int		c_i;
	int		c_j;
	int		c_p;

	c_i = 0;
	c_j = 0;
	c_p = 0;
	while (wsh_token->wsh_param && wsh_token->wsh_param[c_i] != NULL)
	{
		if ((c_p = wsh_searchenv(wsh_list->wsh_envs, wsh_token->wsh_param[c_i])))
			wsh_removevar(wsh_list->wsh_envs, c_p);
		c_i++;
	}
	if (wsh_token->std_out == 1)
		return ;
	else
		exit(0);
}