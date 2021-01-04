# Ancestral-Speciation
INCOMPLETE AND UNMAINTAINED

Monte Carlo Tree Search with complexification to map the fitness landscape using the NEAT algorithm's augmentation operations.
Builds a tree structure reminiscent of a phylogenetic tree for mapping innovations discovered.

Ancestral Speciation aims to speciate over time instead of population, using a swap method much as modern computers do for problems 
larger than memory to increase scale of search. With the Ancestry tree, it is hoped that island (distributed) models,
offline and asynchronous learning can be implemented within a unified backing representation. More importantly, Ancestral Speciation wont
get stuck in a part of the fitness landscape due to being rooted at the initial topology and able to repeat all samples discovered.

Monte Carlo Tree Search is realized by automating the NEAT algorithm, using each evaluation run as a roll out and updating the MCTS based on
*justified complexification*, or when the innovations discovered by the current evaluation are better than previous representations
discovered (coined as "Points of Mutation"). A timeout/stagnation timer is used to set how long roll out can occur per evaluation.
This allows multiple hyperparameter configurations and re sampling of the evolutionary routes taken by the NEAT algorithm
to occur asynchronously without losing discoveries already found.

This is aimed to further research biological models for learning, knowledge and intelligence using
artificial neural networks as the universal function approximator and Ancestral Speciation as a platform for scale.
