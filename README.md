# FEUP-IART-P2

Second project developed for Artificial Intelligence class.

Made in colaboration with [João Vieira](https://github.com/vieirajlt).

## Specification

Implementation of a system to solve an optimization problem using different optimization algorithms giving special relevance to Genetic Algorithms, Hill-Climbing and Simulated Annealing.

The problem in question is the scheduling of exams in periods of an exam season, satisfying a set of restrictions, according with the 
[International Timetabling Competition]( http://www.cs.qub.ac.uk/itc2007/index.htm).
 
### Hard Constraints

* there cannot be two exams, with common enrolled students, in the same period of time (a student cannot take two exams simultaneously);
* the capacity of a room can never be exceeded during the exam;
* the duration of the periods is fulfilled; 
* restrictions related to the sequence of exams are satisfied (if it is specified that exam A must be carried out before exam B, this condition must be fulfilled).

Compliance with this type of restrictions is considered mandatory for a solution to be valid.

### Soft Constraints

* scheduling exams, with students in common, in two consecutive periods or on the same day should be avoided, as well as scheduling exams with a longer duration in later periods of the season (it is preferable that longer exams are scheduled in the initial periods of the exam season); 
* the allocation of different exams, with different durations, in the same room and in the same period of time should also be avoided; 
* the distance between the exams that each student has to take is also counted (number of periods between the periods of each exam). 

These restrictions are not mandatory to obtain a valid solution, but their compliance improves the quality of the solution.

## Execution

In order to facilitate interaction with the user, a menu is available that allows, among others, the choice of the algorithm to be executed and its parameterizations. At the end of the execution of the algorithm, and if it is only executed once (options 1,2 and 3 of the menu), a `Schedule.html` file is generated that allows an easy visualization and understanding of the obtained solution.
