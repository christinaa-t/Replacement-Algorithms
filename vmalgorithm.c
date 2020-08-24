/*
 * Names & Class Accounts: Christina Tran, cssc2367
 * Class Information: CS 570 Section #02, Summer 2020
 * Assignment Information: Assignment #4
 * Filename: vmalgorithm.c 
 * 
 * Note:
 *  -Implementation of FIFO and LRU page replacement algorithm
 *  Please add appropriate level of comments in this file 
 */

#include "vmalgorithm.h"
#include <stdio.h>

/* Generate an access pattern
 * Example: 3, 2, 2, 1, 1  indicates the page 3, 2, 2, 1, 1 in order
 */
void generateAccessPattern()
{
   int i;
   srand(time(0));
   accessPattern = (int *)malloc( sizeof(int) * AccessPatternLength);   
   printf("The randomized Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
		     accessPattern[i] = rand() % ReferenceSZ;
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");
}

/*
 * Initialize the parameters of simulated memory
 */
void initializePageFrame()
{
   int i;
   memory.PageFrameList = (int *)malloc( sizeof(int)* FrameNR );    // dynamic allocated FrameNR frames to be used in memory
   memory.nextToReplaced =0;          // indicate the new frame to be replaced as 0
   for(i=0; i< FrameNR; i++)
   {
			memory.PageFrameList[i] = -1;  // initialization to -1 indicating all frames are unused 
   }

}

// Print the pages loaded in memory
void printPageFrame()
{
   int i;
   for(i=0; i< FrameNR; i++)
   {
			printf("%2d ",memory.PageFrameList[i]);       
   }
   printf("\n");
}

/*
 *  Print the access pattern in order for FIFO
 */
void printAccessPattern()
{
   int i;
   printf("The Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");

}


/*
 *  Print the access pattern in order for LRU 
 */
void printAccessPattern2()
{
   int i;
   printf("The Same Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");

}


/*
 * Return: number of the page fault using FIFO page replacement algorithm
 */
int FIFO() {
   //pageHit has values of 0 or 1 
      //pageHit = 1 means that its a page hit 
      //pageHit = 0 means that its a page fault
   int pageHit, pageFaults;
   int n, m;
   
   //AccessPatternLength is the total number of memory access
      //Hard coded with a length of 20
   //Iterates through all the values in accessPattern
   for (n = 0; n < AccessPatternLength; n++) {
      //Reset the pageHit to 0 for each iteratation of values
      pageHit = 0;
      for (m = 0; m < FrameNR; m++) {
         //Check to see if the page is in memory; if it is, then it's a page hit
         if (accessPattern[n] == memory.PageFrameList[m]) {
            pageHit = 1;
         }
      }
      if (pageHit ==  1) {
         printPageFrame();
      }
      //Keeping track of the number of pageFaults
      else if (pageHit == 0) {
         pageFaults++;
         //Since its a page fault, the page will be loaded next into memory
         memory.PageFrameList[memory.nextToReplaced] = accessPattern[n];
         printPageFrame();
         //Keeping track of the next frame to be replaced
         memory.nextToReplaced++;
         //When we reach the frame size of FrameNR, reset back to 0 
         //Starts over to the beginning of PageFrameList
         if (memory.nextToReplaced == FrameNR) {
            memory.nextToReplaced = 0;
         }
      }  
   }
   return pageFaults;
}

/*
 * Return: number of the page fault using LRU page replacement algorithm
 */

int LRU() {

   int pageHit, pageFaults;
   int n, m, i;
   //index is the reference to the frames
   int index = 0;
   //clockTime checks for what is the least recently used 
   int clockTime;
   //realTime keeps track of every page added into the frame
   int realTime = 0;

   //Dynamically-allocated array representing memory pages
   frameList = malloc(sizeof(PAGE)* FrameNR); 
   //Initializing the values 
   for (m = 0; m < FrameNR; m++) {
      frameList[m].value = -1;
      frameList[m].clock = 0;
   }

   for (n = 0; n < AccessPatternLength; n++) {
      pageHit = 0;
      for (m = 0; m < FrameNR; m++) {
         if (accessPattern[n] == frameList[m].value) {
            pageHit = 1; 
         }
      }
      //Work for when it's a page fault
      if (pageHit == 0) {
         pageFaults++; 
         //When there is a pageFault, clockTime is updated 
         clockTime = ++realTime;
         for (m = 0; m < FrameNR; m++) {
            //Searches for the index to replace by looking for page frame
            //With least amount of clock "ticks"
            if (frameList[m].clock < clockTime) {
               clockTime = frameList[m].clock;
               index = m;
            }
         }
         //Updates the index of the frame with new page from accessPattern
         frameList[index].value = accessPattern[n]; 
         //Updates to realTime
         frameList[index].clock = ++realTime; 
      }
      //Work for when it's a page hit
      else { 
         for (m = 0; m < FrameNR; m++) {
            if (accessPattern[n] == frameList[m].value) {
               index = m;
            }
            frameList[index].value = accessPattern[n];
            frameList[index].clock = ++realTime; 
         }
      }
      //Prints the frameList 
      for (i = 0; i < FrameNR; i++) {
         printf("%d ", frameList[i].value);
      }
      printf("\n");
   }
   
   free(frameList);
   return pageFaults;        

}

