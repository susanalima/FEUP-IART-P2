#include "DataExam.h"
#include "DataRoom.h"
#include "DataPeriod.h"
using namespace std;

int main()
{

	DataExam dataExam = DataExam();
	dataExam.writeExams();

	DataRoom dataRoom = DataRoom();
	dataRoom.writeRooms();

	DataPeriod dataPeriod = DataPeriod();
	dataPeriod.writePeriods();


}
