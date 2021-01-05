/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mashad <mashad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 15:49:50 by mashad            #+#    #+#             */
/*   Updated: 2021/01/05 16:23:52 by mashad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** WESH SHELL MAIN LOOP
*/

int				wsh_loop(t_wsh_list *wsh_list)
{
	while (1)
	{
		wsh_list->string = wsh_read(&wsh_list->garbage_flag);
		if (wsh_list->garbage_flag != ERROR)
			wsh_list->ast_parsed = wsh_parse(wsh_list->string);
		if (wsh_list->garbage_flag != ERROR)
			wsh_exec(wsh_list->ast_parsed);
		if (wsh_garbageCollector(wsh_list) == ERROR)
			return (ERROR);
	}
	return (EXIT);
}

/*
** WESH SHELL STRUCT INIT
*/

t_wsh_list		wsh_init()
{
	t_wsh_list wsh_list;

	wsh_list.string = NULL;
	wsh_list.ast_parsed = NULL;
	wsh_list.garbage_flag = INIT;
	return (wsh_list);
}

int				main(int argc, char **argv, char **env)
{
	t_wsh_list wsh_list;

	(void)argv;
	(void)env;
	wsh_list = wsh_init();
	if ( argc != 1)
		return (ERROR);
	if (wsh_loop(&wsh_list) == ERROR)
		return (ERROR);
	wsh_garbageCollector(&wsh_list);
	return (EXIT);
}