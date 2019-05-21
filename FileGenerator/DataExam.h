#include "Exam.h"


class DataExam {

	int nrExams;
	std::pair<int, int> nrStudents;
	std::pair<int, int> duration;
	std::vector<Exam> exams;

public:
	DataExam();
	DataExam(int nrExams, std::pair<int, int> nrStudents, std::pair<int, int> duration);
	
	void readExamInfo();
	void writeExams();
	void buildExams();
};