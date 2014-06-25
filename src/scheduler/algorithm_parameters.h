/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexander Sazonov (sazon666@mail.ru) and 
 *                      Alexey N. Salnikov (salnikov@cmc.msu.ru).
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#ifndef __ALGORITHM_PARAMETERS_H__
#define __ALGORITHM_PARAMETERS_H__

/**
 * This class contains parameters for Genetic Algorithm (GA)
 */
class Algorithm_parameters
{
public:
	/**
	 * Number of chromosomes. It is used to define the upper border 
	 * number of chromosomes in current generation of the population. 
	 */
	int num_chromosomes;
	/**
	 * Size of the first generation in Genetic Algorithm 
	 */
	int start_generation_size;
	/**
	 * Number of chromosomes that will be selected for new generation of
	 * Genetic Algorithm
	 */
	int num_selecting_chromosomes;
	
	/*
	 *
	 * Stop parameters
	 *
	 */

	/**
	 * Number of generations in genetic algorithm.
	 *
	 * Algorithm will be stopped after it create generation with
	 * number num_generations
	 */
	int num_generations;
	/**
	 * If the difference between values of the fitness functions for the best 
	 * chromosomes of both generations in genetic algorithm is lower than 
	 * futility_barrier then a latest generation of chromosomes is declared as
	 * futility.
	 */
	double futility_barrier;
	/**
	 *	Number of futility generations before algorithm
	 *	will be stopped.
	 */
	int allowed_futile_generations;
	
	/*
	 *
	 * Mutation options
	 *
	 */

	/**
	 * Ratio mutants in generation to genration size.
	 */
	double ratio_mutants;
	/**
	 * Ratio mutant genes to num genes in chromosome.
	 */
	double ratio_genes;
	/**
	 * Probability of proc_number mutation
	 */
	double proc_mutate_probability;
	/**
	 * Probability of jobs order for fixed MPI-process (processor)
	 * mutation.
	 */
	double order_mutate_probability;

	/*
	 *
	 * Recombination options
	 *
	 */
	
	/**
	 * Ratio recombinats to generation size
	 */
	double ratio_recombinats;
	/**
	 * Number of genes in  chromosome 
	 * where recombination hapens.
	 */
	int num_recombinate_points;

	/*
	 *
	 * Methods
	 *
	 */
	
	/**
	 * Sets default values 
	 */
	Algorithm_parameters();
	int read(char *file_name);

};

#endif /*  __ALGORITHM_PARAMETERS_H__ */

