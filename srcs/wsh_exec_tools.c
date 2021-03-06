#include "minishell.h"

int         wsh_tab_length(char **tab)
{
    int i;

    i = 0;
    if (tab == NULL)
        return (0);
    while (tab[i++]);
    return (i);
}

char        *wsh_get_envar(char *s, char **env)
{
    int     i;
    char    *var;

    i = 0;
    var = NULL;
    if (ft_strncmp(s, "-", 1) == 0)
        return ("himBH");
    else if (s[0] == '#')
		return ("0");
    while (env[i])
    {
        var = ft_substr(env[i], 0, wsh_findeq(env[i]));
        if (ft_strncmp(var, s, ft_strlen(s)+1) == 0)
            return (env[i] + (ft_strlen(s) + 1));
        wsh_free((void *)var);
        i++;
    }
    return ("\0");
}