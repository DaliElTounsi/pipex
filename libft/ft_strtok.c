#include "libft.h"

char *ft_strtok(char *str, const char *delim)
{
    static char *next_token = NULL;
    char *token;

    if (str != NULL)
        next_token = str;
    else if (next_token == NULL)
        return (NULL);
    token = next_token;
    while (*next_token)
    {
        if (ft_strchr(delim, *next_token))
        {
            *next_token = '\0';
            next_token++;
            return (token);
        }
        next_token++;
    }
    next_token = NULL;
    return (token);
}