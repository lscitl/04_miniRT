# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/11 13:53:10 by seseo             #+#    #+#              #
#    Updated: 2022/07/19 15:48:20 by seseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCM_DIR 		:=	src/mandatory
SRCB_DIR 		:=	src/bonus

OBJS_DIR		:=	obj

SRCSM			:=	$(addprefix $(SRCM_DIR)/, $(SRCM))
SRCSB			:=	$(addprefix $(SRCB_DIR)/, $(SRCB))

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

SRCS			:=	$(SRCM)

OBJM			:=	$(SRCM:%.c=$(OBJS_DIR)/%.o)
OBJB			:=	$(SRCB:%.c=$(OBJS_DIR)/%.o)

OBJS			:=	$(OBJM)

ifeq ($(BONUS), true)
	SRCS		+=	$(SRCSB)
	OBJS		+=	$(OBJB)
endif

NAME			:=	libft.a
LIBC			:=	ar -rcs
CC				:=	cc
RM				:=	rm -rf
CFLAGS			:=	-Wall -Wextra -Werror
INCLUDE			:=	-Iinclude

ifeq ($(BONUS), true)
$(OBJS_DIR)/%.o	:	$(SRCB_DIR)/%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
else
$(OBJS_DIR)/%.o	:	$(SRCM_DIR)/%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
endif

$(NAME)			:	$(OBJS)
					@$(LIBC) $(NAME) $(OBJS)

all				:	$(NAME)

clean			:
					@$(RM) $(OBJM) $(OBJB)

fclean			:	clean
					$(RM) $(NAME)

re				:	fclean
					@$(MAKE) bonus

bonus			:	all
					@$(MAKE) BONUS=true

.PHONY			:	all clean fclean re bonus
