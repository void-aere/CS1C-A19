#include <algorithm>			// Needed for sort
#include "bubbleSort.h"		// Needed for bubbleSort
#include <chrono>				// Needed to time and uint64 definition
#include <dirent.h>			// Needed to read directories
#include "insertionSort.h"	// Needed for insertionSort
#include "selectionSort.h" // Needed for selectionSort
#include <iomanip>			// Needed for setw
#include <iostream>			// Needed for cout
#include "mergeSort.h"		// Needed for mergeSort
#include <pwd.h>  	   	// Needed for struct passwd
#include <sys/stat.h>		// Needed for struct stat
#include <string.h>			// Needed for strcmp, strcap

using namespace std;

using namespace std::chrono;
namespace {
  /* Adds commas to a numeric value to make it easier to read. */

  template <typename Integer> string addCommasTo(Integer n) {
    /* Negative numbers don't play well with mods. */
    if (n < 0) return "-" + addCommasTo(-n);

    /* Build up our target string, but in reverse. */
    string result;

    /* Handle all but the first digit, which eliminates an edge case of having
     * a comma at the front.
     */
    for (size_t i = 0; n >= 10; i++) {
      result += static_cast<char>('0' + n % 10);
      n /= 10;

      if (i % 3 == 2) result += ',';
    }

    /* Handle the first digit. */
    result += static_cast<char>('0' + n);

    /* Reverse and return the result. */
    reverse(result.begin(), result.end());

    return result;
  }

}

enum algorithm {BUBBLE, SELECTION, INSERTION, MERGE};

/* Type: FileData
 * ---------------------------------------------------------------------------
 * A type representing tree data read from the file. It consists of a pair of
 * a number of elements and the elements themselves, in order.
 */
struct FileData {
  int num_elems;
  long long int* elems;
};

/* Constant: DATA_DIRECTORY
 * ---------------------------------------------------------------------------
 * Name of the directory containing the data. This must end with a
 * slash.
 */
#define DATA_DIRECTORY "/Documents/CS1C/A19/data/"
const char *env_var[1] = {"HOME"};
/* Macro: error(msg)
 * ---------------------------------------------------------------------------
 * Reports an error and terminates the program.
 */
#define error(msg) do_error(msg, __FILE__, __LINE__)
static void do_error(const char* msg, const char* file, unsigned line) {
  fprintf(stderr, "ERROR: %s\nFile: %s\nLine: %u\n", msg, file, line);
  abort();
}

/* Function: read_file(filename)
 * ---------------------------------------------------------------------------
 * Returns the contents of a data file as a FileData object. On error,
 * aborts the program.
 */
static struct FileData read_file(const char* test_file) {
	struct FileData result;

	FILE* f = fopen(test_file, "r");

	if (f == NULL)
		error("Can't open data file.");

  	/* Read the data elements into a separate buffer so that we can see whether everything worked
    * properly.
    */
	if (fscanf(f, "%lld", &result.num_elems) != 1)
		error("Failed to read number of elements from data file.");

	result.elems = static_cast<long long int*>(calloc(result.num_elems, sizeof(long long int)));

	for (size_t i = 0; i < result.num_elems; i++) {
		if (fscanf(f, "%lld", &result.elems[i]) != 1)
			error("Failed to read long long integer from file.");
	}

	/* Confirm there's nothing left by trying to read another long long integer and watching it fail. */
	if (fscanf(f, "%*lld") != EOF)
		error("Unexpected file contents found after end of tree data.");

	fclose(f);

	return result;
}

/* Function: compare_ints(&i1, &i2)
 * ---------------------------------------------------------------------------
 * Comparison function for long long integers, for use in qsort.
 */
int compare_ints(const void *left, const void *right) {

	/* We can't just do right - left, since that might overflow. */
	if ( *(long long int*)left < *(long long int*)right){
		return -1;
	}
	if ( *(long long int*)left > *(long long int*)right){
		return +1;
	}

  return 0;
}

/* Function: assert_monotone_increasing(data, size)
 * ---------------------------------------------------------------------------
 * Asserts that the range pointed at by data is monotonically increasing (each
 * element is strictly greater or equal to the previous.)
 */
static void assert_monotone_increasing(const long long int data[], size_t size) {
	for (size_t i = 1; i < size; i++) {
		if (data[i - 1] >= data[i]) {
			fprintf(stderr, "Elements not strictly increasing: %lld >= %lld\n", data[i-1], data[i]);
			abort();
		}
  	}
}

/* Function: assert_sets_equal(data1, size1, data2, size2);
 * ---------------------------------------------------------------------------
 * Checks that the elements pointed at by data1 and data2 are equal to one
 * another, ignoring duplicates. The elements must be in sorted order. If the
 * ranges aren't equal, this function signals an error.
 */
static void assert_sets_equal(const long long int data1[], size_t size1, const long long int data2[], size_t size2) {
	/* Loop over the ranges until at least one is exhausted. */
	size_t i1 = 0;
	while (i1 < size1) {
		/* Confirm the elements here match. */
		if (data1[i1] != data2[i1]) {
			fprintf(stderr, "Mismatch found: answer key[%d] = %lld and student array[%d] = %lld", i1, data1[i1],i1, data2[i1]);
			abort();
		}

		/* Advance the index
	 	 */
		i1++;
	}

}


void grader(string grade, uint64_t duration, int num_bytes){
	uint64_t hours = duration/3600000;
	duration = duration - (hours*3600000);
	uint64_t minutes = duration/60000;
	duration = duration - (minutes*60000);
	uint64_t seconds = duration/1000;
	duration = duration - (seconds*1000);


	cout << setw(13) << right << grade << setfill('0') << setw(2) << hours << ":" << setw(2) << minutes << ":" << setw(2) << seconds << ":" << setw(3) << duration << setfill(' ') << flush;

	cout << flush << setw(13) << right << addCommasTo(num_bytes) << setw(7) << right << " Bytes" << flush;

	cout << " =" << endl;


	return;
}


static void run_test(string fileNames[], size_t numFiles, passwd *pw) {


	string sorting_algorithm {};
	algorithm a {BUBBLE};


	while (a <= MERGE){
		string pass {"FALSE"};

		char* files = new char[numFiles];

		/* Picks which sorting algorithm to run. */
		switch(a){
			case BUBBLE:
				sorting_algorithm = "BUBBLE SORT";
				break;
			case SELECTION:
				sorting_algorithm = "SELECTION SORT";
				break;
			case INSERTION:
				sorting_algorithm = "INSERTION SORT";
				break;
			case MERGE:
				sorting_algorithm = "MERGE SORT";
				break;
			default:
				error("Invalid Sorting Algorithm chosen");
		}
		cout << "= " << setw(27) << left <<  sorting_algorithm << setw(30) << left <<  "GRADE  HH:MM:SS:mmm" 
			<< setw(7) << right << "RAM" << setw(4) << right << "=" << endl;


		for (size_t i = 0; i < numFiles; i++){
			cout << "=  " << setw(10) << left <<  "ELEMENTS:" << flush;

			char* data_file = new char[fileNames[i].length() + 1]; 

			// make sure that the new string is null terminated 
			data_file[fileNames[i].length()] = '\0'; 

			for (int j = 0; j < fileNames[i].length(); j++)
        		data_file[j] = fileNames[i][j]; 

			cout << " READING FILE" << flush;
			struct FileData data = read_file(data_file);
			cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b" << flush;
			cout << setw(10) << right <<  addCommasTo(data.num_elems) << flush;

			/* Construct a dynamic array from this data. */
			long long int *studentArray = NULL;

			studentArray = static_cast<long long int*>(calloc(data.num_elems, sizeof(long long int)));

			float num_bytes = data.num_elems * sizeof(long long int);

			/* Copy unsorted array into studentArray */
			for (size_t i = 0; i < data.num_elems; i++) {
				studentArray[i] = data.elems[i];
			}

			/**** Here is where we call the students' insertionSort Algorithm ****/


			cout << " SORTING" << flush;
			uint64_t b4 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			switch(a){
				case BUBBLE:
					bubbleSort(studentArray, data.num_elems);
					break;
				case SELECTION:
					selectionSort(studentArray, data.num_elems);
					break;
				case INSERTION:
					insertionSort(studentArray, data.num_elems);
					break;
				case MERGE:
					mergeSort(studentArray, 1, data.num_elems);
					break;
			}

			uint64_t after = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			uint64_t duration = after - b4;

			cout << "\b\b\b\b\b\b\b\b" << flush;




			/* Use system qsort to sort array to verify student has correct solution
			 *
			 */


			qsort(data.elems, data.num_elems, sizeof(long long int), compare_ints);


			/* Confirm sorted array of student matches answer key */
			cout << " VERIFYING" << flush;
			assert_sets_equal(data.elems, data.num_elems, studentArray, data.num_elems);
			cout << "\b\b\b\b\b\b\b\b\b\b" << flush;
			pass = " PASSED ";

			grader(pass,duration,num_bytes);

			free(studentArray);
			free(data.elems);
			delete[] data_file;

		}// end of For loop

		delete[] files;

		a = static_cast<algorithm>(a + 1);
	} // End of while loop
} // End of function run_test

void grade_header(string assignment, passwd *pw){
	cout << "\033[H\033[2J" << flush; //Clear the Screen
	cout << setw(71) << setfill('=') << "\n" << setfill(' ');
	cout << setw(69) << left << "= CS1C AUTO GRADER" << "=" << endl;
	cout << "= ASSIGNMENT: " << setw(55) << left << assignment << "=" << endl;
	cout << "= STUDENT:    " << setw(55) << left << pw->pw_name << "=" << endl;
	time_t now = time(0); // get current dat/time with respect to system.
	char* dt = ctime(&now); // convert it into string.
	cout << "= LOCAL DATE AND TIME: " << setw(25) << left << dt;
	cout << "= " << setw(66) << setfill('-') << "-" <<  setfill(' ') << " =" << endl;;
	string str = "diff -q -s  ./main.cpp /usr/share/CS1C/A19/main.cpp";
   // Convert string to const char * as system requires
   // parameter of type const char *
	const char* command = str.c_str();
 
	system(command);

	str = "diff -q -s  ./bubbleSort.h /usr/share/CS1C/A19/bubbleSort.h";
	const char* command2 = str.c_str();
	system(command2);

	str = "diff -q -s  ./insertionSort.h /usr/share/CS1C/A19/insertionSort.h";
	const char* command3 = str.c_str();
	system(command3);

	str = "diff -q -s  ./mergeSort.h /usr/share/CS1C/A19/mergeSort.h";
	const char* command4 = str.c_str();
	system(command4);

	str = "diff -q -s  ./merge.h /usr/share/CS1C/A19/merge.h";
	const char* command5 = str.c_str();
	system(command5);
	cout << "= " << setw(66) << setfill('-') << "-" <<  setfill(' ') << " =" << endl;;


	return;
}

int main() {
	size_t numFiles = 0;
	struct stat info;
	struct passwd *pw = NULL;

	char *dataDirectory[1];
	dataDirectory[0] = getenv(env_var[0]);
	strcat(*dataDirectory,DATA_DIRECTORY);
	cout << *dataDirectory << endl;

	DIR* dataFiles = opendir(*dataDirectory);
	if (dataFiles == NULL)
		error("Could not open " DATA_DIRECTORY " for reading.");

	for (struct dirent* entry; (entry = readdir(dataFiles)) != NULL; ) {
   	/* Make sure this isn't a dotted file (hidden file, ., or ..) */
		if (entry->d_name[0] == '.')
			continue;
		numFiles++;
	}
	closedir(dataFiles);
	string fileNames[numFiles];
	size_t counter = 0;

	dataFiles = opendir(*dataDirectory);
	if (dataFiles == NULL)
		error("Could not open " DATA_DIRECTORY " for reading.");

	char owner[numFiles][20] {};


	for (struct dirent* entry; (entry = readdir(dataFiles)) != NULL; ) {
   	/* Make sure this isn't a dotted file (hidden file, ., or ..) */
		if (entry->d_name[0] == '.')
			continue;


		char test_file_name[strlen(entry->d_name) + strlen(*dataDirectory) + 1];
		strcpy(test_file_name, *dataDirectory);
		strcat(test_file_name, entry->d_name);
		fileNames[counter] = test_file_name;

		stat(test_file_name, &info);
		pw = getpwuid(info.st_uid);

/*		if(strcmp(pw->pw_name,"instructor") != 0){
			char error_message[100] = "Owner of ";
			strcat(error_message,test_file_name);
			strcat(error_message," must be instructor! Not ");
			strcat(error_message,pw->pw_name);

			error(error_message);
		}*/
		strcpy(owner[counter], pw->pw_name);

		if (pw != 0){
			strcpy(owner[counter], pw->pw_name);
		}
		else
			error("Owner does not exist ...  should not be possible.");

		counter++;
	}
	if (errno != 0)
		error("Error traversing the " DATA_DIRECTORY " directory.");

	closedir(dataFiles);

	/* Print grade report header */
	grade_header("A19", pw);


	std::sort(fileNames, fileNames + numFiles);
	run_test(fileNames, numFiles, pw);


	return 0;
}
