/*
 * kernel/command/cmd-help.c
 *
 * Copyright(c) 2007-2014 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <types.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <xboot/list.h>
#include <xboot/initcall.h>
#include <command/command.h>


#if	defined(CONFIG_COMMAND_HELP) && (CONFIG_COMMAND_HELP > 0)

extern struct command_list * command_list;

/**
 * the entry of help command.
 */
static int help(int argc, char ** argv)
{
	struct command_list * list;
	struct list_head * pos;
	struct command_t ** cmd_array;
	struct command_t * cmd;
	s32_t i = 0, j, k = 0, cmd_num, swaps;

	if(argc == 1)
	{
		i = 0;
		cmd_num = command_number();
		cmd_array = malloc(sizeof(struct command_t *) * cmd_num);

		if(!cmd_array)
		{
			printf("malloc command array fail for sort command list\r\n");
			return -1;
		}

		for(pos = (&command_list->entry)->next; pos != (&command_list->entry); pos = pos->next)
		{
			list = list_entry(pos, struct command_list, entry);
			cmd_array[i++] = list->cmd;
			j = strlen(list->cmd->name);
			if(j > k)	k = j;
		}

		/* sort command list (trivial bubble sort) */
		for (i = cmd_num - 1; i > 0; --i)
		{
			swaps = 0;
			for(j=0; j<i; ++j)
			{
				if (strcmp(cmd_array[j]->name, cmd_array[j + 1]->name) > 0)
				{
					cmd = cmd_array[j];
					cmd_array[j] = cmd_array[j + 1];
					cmd_array[j + 1] = cmd;
					++swaps;
				}
			}
			if(!swaps)
				break;
		}

		/* display online help (desc) */
		for (i=0; i<cmd_num; i++)
		{
			printf(" %s%*s - %s",cmd_array[i]->name, k-strlen(cmd_array[i]->name), "", cmd_array[i]->desc);
		}
		free(cmd_array);
	}
	else
	{
		for (i=1; i<argc; ++i)
		{
			if( (cmd = command_search((char*)argv[i])) != NULL )
			{
				printf("%s - %s" "usage: %s" "help:\r\n" "%s", cmd->name, cmd->desc, cmd->usage, cmd->help);
			}
			else
			{
				printf("unknown command '%s' - try 'help' without arguments for list of all known commands\r\n", argv[i]);
			}
		}
	}

	return 0;
}

static struct command_t help_cmd = {
	.name		= "help",
	.func		= help,
	.desc		= "display online help\r\n",
	.usage		= "help [command ...]\r\n",
	.help		= "    show help information (for 'command')\r\n"
				  "    'help' display online help for the monitor commands.\r\n"
				  "    to get detailed help information for specific commands you can type 'help' with one or more command names as arguments.\r\n"
};

static __init void help_cmd_init(void)
{
	if(command_register(&help_cmd))
		LOG("Register command 'help'");
	else
		LOG("Failed to register command 'help'");
}

static __exit void help_cmd_exit(void)
{
	if(command_unregister(&help_cmd))
		LOG("Unegister command 'help'");
	else
		LOG("Failed to unregister command 'help'");
}

command_initcall(help_cmd_init);
command_exitcall(help_cmd_exit);

#endif