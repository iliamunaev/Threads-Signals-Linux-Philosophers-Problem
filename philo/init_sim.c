#include "philo.h"

static t_sim	*alloc_sim_struct(char **av)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
	{
		print_err("Error: malloc of t_sim failed.");
		return (NULL);
	}
	sim->ph_count = ft_atol(av[1]);
	sim->all_dead = false;
	sim->ph_threads = NULL;
	sim->mtx_forks = NULL;
	return (sim);
}

static int	init_main_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->mtx_is_dead, NULL) != 0 ||
		pthread_mutex_init(&sim->mtx_log, NULL) != 0 ||
		pthread_mutex_init(&sim->mtx_last_meal_time, NULL) != 0)
	{
		print_err("Error: init_sim() could not init a main mutex.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	init_fork_mutexes(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->ph_count)
	{
		if (pthread_mutex_init(&sim->mtx_forks[i], NULL) != 0)
		{
			print_err("Error: init_sim() could not init fork mutex.");
			i--;
			while (i >= 0)
			{
				if (pthread_mutex_destroy(&sim->mtx_forks[i]) != 0)
					return (EXIT_FAILURE);
				i--;
			}
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	alloc_forks_and_threads(t_sim *sim)
{
	sim->mtx_forks = malloc(sim->ph_count * sizeof(pthread_mutex_t));
	if (!sim->mtx_forks)
	{
		print_err("Error: malloc of mtx_forks failed.");
		return (EXIT_FAILURE);
	}
	sim->ph_threads = malloc(sim->ph_count * sizeof(pthread_t));
	if (!sim->ph_threads)
	{
		print_err("Error: malloc of ph_threads failed.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_sim	*init_sim(char **av)
{
	t_sim	*sim;

	sim = alloc_sim_struct(av);
	if (!sim)
		return (NULL);
	if (alloc_forks_and_threads(sim) == EXIT_FAILURE)
		return (NULL);
	if (init_main_mutexes(sim) == EXIT_FAILURE)
		return (NULL);
	if (init_fork_mutexes(sim) == EXIT_FAILURE)
		return (NULL);
	return (sim);
}
