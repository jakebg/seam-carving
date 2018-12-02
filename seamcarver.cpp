#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h> 

#define INF INT_MAX

void calculateSeam(std::vector<std::vector<int> > &CumulativeMatrix, std::vector<std::vector<int> > &pgmMatrix) {
  // Finds the seam to be removed

  int yHeight = CumulativeMatrix.size();
  int xWidth  = CumulativeMatrix[0].size();
  int min = 0;
  // Getting height and width of the matrix

  for (int i = 1; i < xWidth; ++i) 
    if (CumulativeMatrix[yHeight-1][min] > CumulativeMatrix[yHeight-1][i])
      min = i;
  // Finding min value from bottom row
  
  pgmMatrix[yHeight-1][min] = -1;

  for (int i = yHeight - 1; i >= 0; --i) {
    // Find minimum value from the diagonals and up in the matrix
    bool minW  = (min == 0), maxW  = (min == xWidth - 1);
    int upL = INF, upR = INF, previous = 0;

    if (i > 0) {
      // Sets the left and right up as values
      if (!minW)  
	upL = CumulativeMatrix[i-1][min-1];
      
      if (!maxW)
	upR = CumulativeMatrix[i-1][min+1];
      
      previous = CumulativeMatrix[i-1][min];
    
      if (upL ==  std::min({upL, upR, previous})) {
	// Checks for ties in the energy matrix
	pgmMatrix[i-1][min-1] = -1;
	--min;
      }
      else if (previous ==  std::min({upL, upR, previous}))
	pgmMatrix[i-1][min] = -1;
      
      else {
	pgmMatrix[i-1][min+1] = -1;
	++min;
      }
    } 
  }
}

void flipMatrix(std::vector<std::vector<int> > &pgmMatrix)
// Swaps the matrix values so that you can carve seams of horizontal values using vertical function  without creating another function
{
  std::vector<std::vector<int>> flippedMatrix (pgmMatrix[0].size(), std::vector<int>(pgmMatrix.size(), 0));

  int xWidth = pgmMatrix[0].size();
  int yHeight = pgmMatrix.size();
  for(int i=0; i<yHeight; i++) {
    // Flips the matrix from original pgmMatrix to new flipped matrix
    for(int j=0; j<xWidth; j++) 
      flippedMatrix[j][i] = pgmMatrix[i][j];
  }
  pgmMatrix.clear();
  pgmMatrix = flippedMatrix;
}

void seamCarver(int& yHeight, int& xWidth, int seamsAmount, std::vector<std::vector<int> > &pgmMatrix) {
  
  for (int i=0; i<seamsAmount; ++i) {
    std::vector<std::vector<int>> energyVec(yHeight, std::vector<int> (xWidth, 0));
    // Vector to store energy values
    std::vector<std::vector<int>> savedVec(yHeight, std::vector<int> (xWidth, 0));
    // Vector to save matrix values

    for (int i = 0; i < pgmMatrix.size(); ++i) {
      for (int j = 0; j < pgmMatrix[i].size(); ++j) {
	int yHeight = pgmMatrix.size(), xWidth = pgmMatrix[i].size();
	bool minW   = (j == 0), maxW   = (j == xWidth - 1), minH  = (i == 0), maxH  = (i== yHeight - 1);
	int upL = INF, upR = INF, previous =  0, energyTot = 0;
	int Energy = pgmMatrix[i][j];
	// Initializing variables

	if (!maxW)
	  energyTot += abs(Energy - pgmMatrix[i][j+1]);
	if (!minW)  
	  energyTot += abs(Energy - pgmMatrix[i][j-1]); 
	if (!maxH) 
	  energyTot += abs(Energy - pgmMatrix[i+1][j]); 
	if (!minH) 
	  energyTot += abs(Energy - pgmMatrix[i-1][j]); 
	// Adds energy values 
	energyVec[i][j] = energyTot;
	savedVec[i][j] = energyTot;
 
	if (i > 0) {
	  // Calculate min and max in energy values
	  if (!minW)
	    upL = energyTot + savedVec[i-1][j-1]; 
	  if (!maxW)
	    upR = energyTot + savedVec[i-1][j+1];
	  
	  previous = energyTot + savedVec[i-1][j];

	  savedVec[i][j] = std::min( {upL, upR, previous} );
	}
      }
    }
  
    calculateSeam (savedVec, pgmMatrix);
    
    std::vector<std::vector<int>> resultVec(yHeight, std::vector<int> (xWidth-1, 0));
    // Vector to save final vec
    
    bool seamCheck = false;
    // Has seam been passed through
    for (int i = 0; i < pgmMatrix.size(); i++) {
      for (int j = 0; j < pgmMatrix[i].size(); j++) {
	if (pgmMatrix[i][j] == -1)
	  seamCheck = true;
	else 
	  resultVec[i][j-seamCheck] = pgmMatrix [i][j]; 
      }
      seamCheck = false;
    }

    --xWidth;

    pgmMatrix.clear();
    pgmMatrix = resultVec;
  }
}
