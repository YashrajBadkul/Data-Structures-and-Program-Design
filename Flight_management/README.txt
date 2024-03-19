Flight Management System
This C program implements a Flight Management System that allows users to manage flight schedules by performing various operations such as adding new flights, canceling flights, displaying flight details, and rearranging flights based on specified criteria.

Overview
The Flight Management System consists of the following key components:

Structures:

TIME: Represents a time structure with hours and minutes.
flight_plan: Represents a flight plan with flight ID, departure time, arrival time, and a pointer to the next flight plan.
bucket: Represents a bucket containing a list of flight plans and the start and end estimated time of arrival (ETA) for flights in the bucket. It also includes a pointer to the next bucket.
Functions:

maxTime: Compares two TIME structures and returns 1 if the first time is greater, -1 if the second time is greater, and 0 if they are equal.
insert: Inserts a new flight plan into the flight schedule, maintaining sorted order based on arrival time.
show_status: Displays details of a flight based on its ID.
delete: Cancels a flight plan with the specified ID.
getNewHead: Returns a new head bucket with updated ETA based on a new time input.
time_diff: Calculates the time difference between two TIME structures.
update: Updates the flight schedule by removing empty buckets.
insertSorted: Inserts a flight plan into a bucket's flight list in sorted order based on departure time.
reArrange: Rearranges flights based on a new specified time, updating bucket start and end ETAs and adjusting flight plans accordingly.
outputData: Outputs flight schedule data to a CSV file.
Main Functionality:

Reads flight data from a CSV file and populates the flight schedule.
Provides a menu for users to perform operations such as displaying flight status, adding new flights, canceling flights, and rearranging flights.
Implements error checking for input validation.
