#include <random>
#include <iostream>
#include <iterator>     
#include <chrono>

class Exam {


public:

	int duration;
	std::vector<int> students;

	Exam(int duration, std::vector<int> students);

	int getDuration() ;
	std::vector<int> getStudents() ;
};