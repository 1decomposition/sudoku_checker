#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

int grid[9][9] = 
{
	{6,5,3,1,2,8,7,9,4},
	{1,7,4,3,5,9,6,8,2},
	{9,2,8,4,6,7,5,3,1},
	{2,8,6,5,1,4,3,7,9},
	{3,9,1,7,8,2,4,5,6},
	{5,4,7,6,9,3,2,1,8},
	{8,6,5,2,3,1,9,4,7},
	{4,1,2,9,7,5,8,6,3},
	{7,3,9,8,4,6,1,2,5}
};

/*structure for passing data to threats*/
typedef struct
{
	int row;
	int column;
} parameters;

void print(void)
{
	int i, j = 0;

	while(i < 9)
	{
		while(j < 9)
		{
			printf("%d", grid[i][j]);
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
}

/* Check every 3x3 box */
void	*boxy(void* param)
{
	parameters *data = (parameters *) param;
	int row_Start = data -> row;
	int col_Start = data -> column;	
	int i = row_Start;
	int j = col_Start;
	int box[10] = {0};
	
	while(i < row_Start + 3)
	{
		while(j < col_Start  + 3)
		{
			int value = grid[i][j];
			if (box[value] != 0)
				return (void *) 0;
			else
				box[value] = 1;
			j++;
		}
		j = data -> column;
		i++;
	}
	return (void *) 1;
}	

/* Checks every row in puzzle */
void	*check_rows(void * param)
{
	parameters *data = (parameters *) param;
	int i = data -> row;
	int j = data -> column;

	while(i < 9)
	{
		int row[10] = {0};
		while( j < 9)
		{
			int value = grid[i][j];
			if(row[value] != 0)
				return (void *) 0;
			else
				row[value] = 1;
			j++;
		}
		j = data -> column;
		i++;
	}
	return (void *) 1;
}

/* Checks every column in puzzle */
void	*check_cols(void * param)
{
	parameters *data = (parameters *) param;
	int j = data -> row;
	int i = data -> column;

	while (i < 9)
	{
		int col[10] = {0};
		while (j < 9)
		{
			int value = grid[j][i];
			if(col[value] != 0)
				return (void*) 0;
			else
				col[value] = 1;
			j++;
		}
		j = data -> row;
		i++;
	}
	return (void *) 1;
}

int		main(void)
{
	pthread_t thread_row, thread_col, thread_box1, thread_box2, thread_box3, thread_box4, thread_box5, thread_box6, thread_box7, thread_box8, thread_box9;
	
	write(1, "CS149 Sudoku from Justin Coy\n",29);

	/* Create data structure + threads for each "check" throuh puzzle */
	parameters * row_col = (parameters *) malloc(sizeof(parameters));
	row_col -> row = 0;
	row_col -> column = 0;
	pthread_create(&thread_row, NULL, check_rows,(void *) row_col);
	pthread_create(&thread_col, NULL, check_cols,(void *) row_col);
	void * every_row;
	void * every_col;

	parameters * box1 = (parameters *) malloc(sizeof(parameters));
	box1 -> row = 0;
	box1 -> column = 0;
	pthread_create(&thread_box1, NULL, boxy,(void *) box1);
	void * sq1;

	parameters * box2 = (parameters *) malloc(sizeof(parameters));
	box2 -> row = 0;
	box2 -> column = 3;
	pthread_create(&thread_box2, NULL, boxy,(void *) box2);
	void * sq2;

	parameters * box3 = (parameters *) malloc(sizeof(parameters));
	box3 -> row = 0;
	box3 -> column = 6;
	pthread_create(&thread_box3, NULL, boxy,(void *) box3);
	void * sq3;

	parameters * box4 = (parameters *) malloc(sizeof(parameters));
	box4 -> row = 3;
	box4 -> column = 0;
	pthread_create(&thread_box4, NULL, boxy,(void *) box4);
	void * sq4;

	parameters * box5 = (parameters *) malloc(sizeof(parameters));
	box5 -> row = 3;
	box5 -> column = 3;
	pthread_create(&thread_box5, NULL, boxy,(void *) box5);
	void * sq5;

	parameters * box6 = (parameters *) malloc(sizeof(parameters));
	box6 -> row = 3;
	box6 -> column = 6;
	pthread_create(&thread_box6, NULL, boxy,(void *) box6);
	void * sq6;

	parameters * box7 = (parameters *) malloc(sizeof(parameters));
	box7 -> row = 6;
	box7 -> column = 0;
	pthread_create(&thread_box7, NULL, boxy,(void *) box7);
	void * sq7;

	parameters * box8 = (parameters *) malloc(sizeof(parameters));
	box8 -> row = 6;
	box8 -> column = 3;
	pthread_create(&thread_box8, NULL, boxy,(void *) box8);
	void * sq8;

	parameters * box9 = (parameters *) malloc(sizeof(parameters));
	box9 -> row = 6;
	box9 -> column = 6;
	pthread_create(&thread_box9, NULL, boxy,(void *) box9);
	void * sq9;

	/* Wait for threads to finish */
	pthread_join(thread_row, &every_row);
	pthread_join(thread_col, &every_col);
	pthread_join(thread_box1, &sq1);
	pthread_join(thread_box2, &sq2);
	pthread_join(thread_box3, &sq3);
	pthread_join(thread_box4, &sq4);
	pthread_join(thread_box5, &sq5);
	pthread_join(thread_box6, &sq6);
	pthread_join(thread_box7, &sq7);
	pthread_join(thread_box8, &sq8);
	pthread_join(thread_box9, &sq9);

	/* Deallocate memory used */
	free(row_col);
	free(box1);
	free(box2);
	free(box3);
	free(box4);
	free(box5);
	free(box6);
	free(box7);
	free(box8);
	free(box9);

	print();

	/* If every row, col, and 3x3 is valid, the whole puzzle is valid */
	if( (int)every_row == 1 && (int)every_col == 1 && (int)sq1 == 1 &&
			(int)sq2 == 1 && (int)sq3 == 1 && (int)sq4 == 1 &&
			(int)sq5 == 1 && (int)sq6 == 1 && (int)sq7 == 1 &&
			(int)sq8 == 1 && (int)sq9 == 1)
	{
		write(1, "Valid Solution!\n", 17);
	}
	else
	{
		write(1, "Sorry, Invalid Solution\n", 26); 
	}

	return (0);
}

