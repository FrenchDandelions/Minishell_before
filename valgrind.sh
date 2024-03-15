valgrind -q --suppressions=suppress.txt --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --leak-resolution=high --trace-children=yes ./minishell
