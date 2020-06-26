// Schedule ADT implementation

#include <stdio.h>
#include <stdlib.h>

#include "Schedule.h"
#include "Time.h"
#include "Tree.h"

struct schedule {
    Tree times;
    int  count;
};

// Creates a new schedule
Schedule ScheduleNew(void) {
    Schedule s = malloc(sizeof(*s));
    if (s == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    s->times = TreeNew();
    s->count = 0;
    return s;
}

// Frees all memory associated with a given schedule
void ScheduleFree(Schedule s) {
    TreeFree(s->times);
    free(s);
}

// Gets the number of times added to the schedule
int  ScheduleCount(Schedule s) {
    return s->count;
}

// Attempts to schedule a new landing time. Returns true if the time was
// successfully added, and false otherwise.
bool ScheduleAdd(Schedule s, Time t) {
    // TODO: Modify the following code
		bool safe = true;
		//make a ceiling and floor time that represents the boundaries
		Time tCeiling = TimeAddMinutes(t, 10);
		Time tFloor = TimeSubtractMinutes(t,10);
		//get the actual ceiling and floor times from the tree
		Time ceiling = TreeCeiling(s->times, t);
		Time floor = TreeFloor(s->times, t);

		//if there is a ceiling
		if (ceiling != NULL){
			//check the ceiling boundary is later than or equal to the ceiling
			if (TimeCmp(tCeiling, ceiling) >= 0) safe = false;
		}
		//if there is a floor
		if (floor != NULL){
			//check the floor boundary is earlier than or equal to the floor 
			if (TimeCmp(tFloor, floor) <= 0) safe = false;
		}

		if (safe){
			TreeInsert(s->times, t);
			s->count++;
			return true;
		}
		return false;
}

// Shows  all  the landing times in the schedule. If mode is 1, only the
// times are shown. If mode is 2, the underlying data structure is shown
// as well.
void ScheduleShow(Schedule s, int mode) {
    if (mode == 1) {
        TreeList(s->times);
    } else if (mode == 2) {
        TreeShow(s->times);
    }
}
