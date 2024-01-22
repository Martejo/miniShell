#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "libft/libft.h"

static void	exec_cmd(char **cmd)
{
	__pid_t	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	}
	else
	{
		if (execve(cmd[0], cmd, NULL) == -1)
			perror("Shell");
		exit(EXIT_FAILURE);
	}
}

static void free_array(char ***array)
{
    if (!*array)
        return;

    int i = 0;
    while ((*array)[i] != NULL)
    {
        free((*array)[i]);
        (*array)[i] = NULL;
        i++;
    }

    free(*array);
    *array = NULL;
}

int	main()
{
	char	*buffer;
	char	*pwd;
	char	**cmd;
	pwd = getcwd(NULL, 0);
	pwd = ft_strjoin(pwd, " $> ");


	while (1)
	{
		buffer = readline(pwd);
		
		if (buffer == NULL || strcmp(buffer, "exit") == 0)
			break;
		cmd = ft_split(buffer, ' ');
		exec_cmd(cmd);
		if (cmd)
			free_array(&cmd);
		add_history(buffer);
		free(buffer);
	}
	if (buffer)
		free(buffer);
	if (pwd)
		free(pwd);
	rl_clear_history();
	if (cmd != NULL)
		free_array(&cmd);
	
}