#include "shell.h"

int main(int ac __attribute__((unused)), char **av, char **ev)
{
    char *arg, **argv, temp[120], bin[6] = "/bin/";
    size_t len = 0;
    ssize_t nread;
    pid_t id;
    int status, i;

    _memset(temp, 0, 120);
    _puts("$ ");
    while ((nread = getline(&arg, &len, stdin)) != -1)
    {
        if (*arg == '\n')
            continue;
        if (!_strcmp(arg, "exit\n"))
        {
            free(arg);
            exit(EXIT_SUCCESS);
        }
        arg[nread - 1] = '\0';
        if (*arg != '/')
        {
            _strcpy(temp, arg);
            free(arg);
            arg = _strcat(_strncpy(malloc(_strlen(temp) + 6), bin, 5), temp);
        }
        argv = tokenize(arg, " ");
        if (!argv && (free(arg), dprintf(2, "Error parsing commands\n")))
            continue;
        if ((id = fork()) == -1)
            perror(av[0]);
        if (id == 0 && (execve(argv[0], argv, ev) == -1))
            perror(av[0]), exit(EXIT_FAILURE);
        wait(&status);
        for (i = 0; argv[i]; free(argv[i++]))
            argv[i] = NULL;
        free(argv);
        free(arg);
        _puts("$ ");
    }
    free(arg);
    _putchar('\n');
    exit(EXIT_SUCCESS);
}
