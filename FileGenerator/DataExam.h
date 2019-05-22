#include "Exam.h"

#include <set>
#include <map>

class DataExam {

	int nrExams;
	std::pair<int, int> nrStudents;
	std::pair<int, int> duration;
	std::vector<Exam> exams;

	std::map<std::pair<int, int>, int> incompatibilitiesTable;

public:
	DataExam();
	DataExam(int nrExams, std::pair<int, int> nrStudents, std::pair<int, int> duration);
	
	void readExamInfo();
	void writeExams();
	void buildExams();
	void buildIncompatibilitiesTable();
	void writeIncompatibilitiesTable();

};