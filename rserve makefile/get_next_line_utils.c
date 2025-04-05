/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:36:14 by mohchams          #+#    #+#             */
/*   Updated: 2024/12/18 21:06:36 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	const unsigned char	*str;

	str = (const unsigned char *)s;
	while (*str)
	{
		if (*str == (unsigned char)c)
			return ((char *)str);
		str++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)str);
	return (NULL);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	j;
	size_t	long_src;

	j = 0;
	while (src[j])
		j++;
	long_src = j;
	if (siz == 0)
		return (long_src);
	i = 0;
	while (src[i] && i < (siz - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (long_src);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	tail_s1_s2;
	char	*dest;

	tail_s1_s2 = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc((tail_s1_s2 + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, s1, ft_strlen(s1) + 1);
	ft_strlcat(dest, s2, tail_s1_s2 + 1);
	return (dest);
}

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	long_src;
	size_t	long_dst;
	size_t	i;
	size_t	j;

	long_dst = ft_strlen(dst);
	long_src = ft_strlen(src);
	if (siz <= long_dst)
		return (long_src + siz);
	i = 0;
	j = long_dst;
	while (src[i] && j < (siz - 1))
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (long_dst + long_src);
}
