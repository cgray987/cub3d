/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:14:15 by cgray             #+#    #+#             */
/*   Updated: 2024/03/28 15:15:00 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* dupes 'n' chars from str 's' into new string to be freed later */
char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	i;

	new = (char *)malloc(sizeof(char) * (n + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
