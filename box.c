//------------------------------------------------------------------
// File: box.c
//   
// Description: This program takes in a number of boxes, along with the 
//       width and height of the boxes from the command line and draws the 
//       number of boxes of the width and height as per the specifications 
//       on the screen.  The width cannot exceed 20 and the 
//       height cannot exceed 10.
//       
// Syntax:
//      This program is used via the command-line syntax: 
//      box number width height, where: 
//	number = the number of boxes
//	width = the width of each box
//	height = the height of each box 
//
//      The width must be an integer between 2 and 20 and the height must 
//      be an integer between 2 and 10. The number of boxes must be an 
//      integer greater than 0. 
//
//	The boxes will be created using the following characters:
//	a. ‘+’ at the corners
//	b. ‘-‘ as the horizontal edgea
//	c. ‘|’ as the vertical edges
//	d. Use a space for the inside parts of the box
//
//	Each character that is used to draw the outside of a box is a unit 
//	of measure of the width and height.  
//	An example of a 4x4 box would be:
//
//		+--+
//		|  |
//		|  |
//		+--+	
//	
//	The screen will be cleared before the boxes are drawn.  If a box 
//	cannot fully fit on the screen, only the parts of the box that fit 
//	in the allowable screen space will be displayed. No part of the box
//	will be allowed in the bottom row of the screen. 
//
//-------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

//------------------------------
// Declaration of constants
//------------------------------

#define VALID_ARGS 4

//------------------------------
// Function prototypes
//------------------------------

void display(unsigned int row,
             unsigned int col,
             long int width,
             long int height);

void get_coord(unsigned int rows[],
              unsigned int columns[],
              long int number);

//------------------------------
// Global variables
//------------------------------

int Max_rows = 0;
int Max_cols = 0;

//------------------------------
// Main
//------------------------------

int main(int argc, char * argv[])
{
        //-------------------------------------
        // Creation of local variables
        //-------------------------------------

        long int box_number;
        long int width;
        long int height;
        unsigned int *rows = NULL;
        unsigned int *columns = NULL;

        //-------------------------------------
        // Checking for input validity
        //-------------------------------------

        // Verify valid number of arguments
        
        if (argc != VALID_ARGS){
                printf("Error.  Invalid number of inputs.\n");
                exit (-1);

        }

        // Verify box number, width and height are numbers
        
        for (int i=1; i<4; i++){

                for (int j=0; j < strlen(argv[i]); j++){ 
                        
                        if (!isdigit(argv[i][j])){
                                printf("Error. You must only use" 
                                        " integers for input values.\n");
                                exit(-1);
                        }
                }       
        };

        // Convert box number, width, height inputs to long ints
        
        box_number = strtol(argv[1], NULL, 10);
        width = strtol(argv[2], NULL, 10);
        height = strtol(argv[3], NULL, 10);

        // Verify that box number, width and height are in valid ranges

        if (box_number <= 0){
                printf("Error.  Number of boxes must be greater"
                        " than 0.\n");
                exit(-1);
        }
        if ((width < 2) || (width > 20)){
                printf("Error.  Width must be between 2 and 20.\n");
                exit (-1);
        }
        if ((height < 2) || (height > 10)){
                printf("Error.  Height must be between 2 and 10.\n");
                exit (-1);
        }

        // Allocate memory for arrays to hold data for corner coords

        rows = malloc(sizeof(unsigned int) * box_number);
        columns = malloc(sizeof(unsigned int) * box_number);

        // Get size of terminal

        struct winsize win;
        ioctl(0, TIOCGWINSZ, &win);
        Max_rows = (win.ws_row) - 1; // Cannot use the bottom row
        Max_cols = win.ws_col;
        
        // Get coordinates for box corners
        
        if (rows != NULL && columns != NULL){
                get_coord(rows, columns, box_number);
        
        }else{
                printf("Error.  Not enough memory.\n");
                exit (-1);
        }

        // Clear the screen and change color to blue for the boxes

        printf("\033[2J");
        printf("\033[34;1m");

        // Display the boxes
        
        for (int i=0; i < box_number; i++){
                display(rows[i], columns[i], width, height);
        }        

        // Change the color back to normal and move the cursor to the bottom

        printf("\033[0m");
        printf("\033[%i;%iH", Max_rows+1, 0);

        // Free the memory space from the malloc operation

        free(rows);
        rows = NULL;
        free(columns);
        columns = NULL;
        
        return (0);
}

void get_coord(unsigned int *rows,
                unsigned int *columns,
                long int box_number){
        
        // Seed the random number generator 

        srandom(time(NULL));
        
        // Populate the arrays with random values for first corner coords

        for (int i=0; i<box_number; i++){
                rows[i] = (random() % Max_rows) + 1;
                columns[i] = (random() % Max_cols) + 1;
        }
}

void display(unsigned int row,
             unsigned int col,
             long int width,
             long int height){

        unsigned int r = row;
        unsigned int c = col;

        // Move cursor to corner position

        printf("\033[%i;%iH", r, c);

        // Print the first row
               
        printf("+");

        for (int a=0; a<width-2; a++){
                if (c+a < Max_cols){
                       printf("-");
                }
        }

        if (c+width-1 < Max_cols){
                printf("+");
        }

        // Print row 2 to the second to last row

        for (int f=1; f<height-1; f++){
                if (r+f <= Max_rows){
                        printf("\033[%i;%iH", r+f, c);
                        printf("|");
                        for (int b=0; b<width-2; b++){
                                if (c+b < Max_cols){
                                        printf(" ");
                                }
                        } 
                

                        if (c+width-1 < Max_cols){
                                printf("|");
                        
                        }
                }       
         };

        // Print the last row

        if (r+height-1 <= Max_rows){
                printf("\033[%ld;%iH", r+height-1, c);
                printf("+");

                for (int q=0; q<width-2; q++){
                        if (c+q < Max_cols){
                                printf("-");
                        }
                }        
                if (c+width-1 < Max_cols){
                       printf("+");
                }       
        }

        // Push the print and wait one second before printing the next box 

        fflush(stdout);
        sleep(1);
}
