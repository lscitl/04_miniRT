# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seseo <seseo@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/11 13:53:10 by seseo             #+#    #+#              #
#    Updated: 2022/08/02 16:03:39 by seseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR 		:=	src
OBJS_DIR		:=	obj

SRCM			:=	ft_isalpha.c \
					ft_isdigit.c \
					ft_isalnum.c \
					ft_isascii.c \
					ft_isprint.c \
					ft_strlen.c \
					ft_memset.c \
					ft_bzero.c \
					ft_memcpy.c \
					ft_memmove.c \
					ft_strlcpy.c \
					ft_strlcat.c \
					ft_toupper.c \
					ft_tolower.c \
					ft_strchr.c \
					ft_strrchr.c \
					ft_strncmp.c \
					ft_memchr.c \
					ft_memcmp.c \
					ft_strnstr.c \
					ft_atoi.c \
					ft_calloc.c \
					ft_strdup.c \
					ft_substr.c \
					ft_strjoin.c \
					ft_strtrim.c \
					ft_split.c \
					ft_itoa.c \
					ft_strmapi.c \
					ft_striteri.c \
					ft_putchar_fd.c \
					ft_putstr_fd.c \
					ft_putendl_fd.c \
					ft_putnbr_fd.c

SRCB			:=	ft_lstnew.c \
					ft_lstadd_front.c \
					ft_lstsize.c \
					ft_lstlast.c \
					ft_lstadd_back.c \
					ft_lstdelone.c \
					ft_lstclear.c \
					ft_lstiter.c \
					ft_lstmap.c \
					ft_malloc_bonus.c \
					ft_strtod_bonus.c \
					ft_atol_bonus.c \
					ft_strndup_bonus.c \
					split_quote_bonus.c \
					str_buffer_bonus.c \
					ft_hextoi_bonus.c \
					get_next_line_bonus.c \
					split_wildcard_bonus.c \
					split_wildcard_utils_bonus.c \
					free_strs_bonus.c

SRCS			:=	$(SRCM) $(SRCB)
OBJS			:=	$(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME			:=	libft.a
LIBC			:=	ar -rcs
CC				:=	cc
RM				:=	rm -rf
CFLAGS			:=	-Wall -Wextra -Werror -g
INCLUDE			:=	-Iinclude

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME)			:	$(OBJS)
					@$(LIBC) $(NAME) $(OBJS)

all				:	$(NAME)

clean			:
					@$(RM) $(OBJS)

fclean			:	clean
					$(RM) $(NAME)

re				:	fclean
					@$(MAKE) bonus

bonus			:	all

.PHONY			:	all clean fclean re bonus
