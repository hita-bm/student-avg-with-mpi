#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_STUDENT_COUNT 100

typedef struct student {
	int stdnum;
	float sum;
	int count;
}Student;

int main() {
	int comm_sz;
	int my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	Student st[MAX_STUDENT_COUNT];
	for (int i = 0; i < MAX_STUDENT_COUNT; i++) {
		st[i].stdnum = 0;
		st[i].sum = 0;
		st[i].count = 0;
	}

	int studentNumber;
	float grade;

	if (my_rank == 0) {
		printf("Enter student number, then grade with one space between them:\n");
		while (1) {
			scanf("%d %f", &studentNumber, &grade);
			MPI_Send(&studentNumber, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Send(&grade, 1, MPI_FLOAT, 1, 1, MPI_COMM_WORLD);
		}
	}
	else if (my_rank == 1) {
		while (1) {
			MPI_Recv(&studentNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&grade, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			int found = 0;
			for (int i = 0; i < MAX_STUDENT_COUNT; i++) {
				if (st[i].stdnum == studentNumber) {
					st[i].sum += grade;
					st[i].count++;
					printf("The new average for student %d is: %f\n", st[i].stdnum, st[i].sum / st[i].count);
					found = 1;
				}
			}
			if (found == 0) {
				for (int i = 0; i < MAX_STUDENT_COUNT; i++) {
					if (st[i].stdnum == 0) {
						st[i].stdnum = studentNumber;
						st[i].sum += grade;
						st[i].count++;
						printf("New student %d with first average %f was added.", studentNumber, grade);
						break;
					}
				}
			}
		}
	}
	MPI_Finalize();
	return 0;
}

