#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "seamcarver.cpp"

int main(int argc, char *argv[]) {

  if (argc != 4) {
    // Checks for correct command line form
    std::cout << "Wrong format! 'filename.pgm (Vertical Seams)(Horizontal Seams)'\n";
    return 0;
  }

  int maxWhite, xWidth, yHeight;
  // Initializing variables
  int vertSeams = atoi(argv[2]);
  int horizSeams = atoi(argv[3]);
  std::ifstream inputFile(argv[1]);
  std::string filename = argv[1];
  std::string inputLine;
  // Gets values from command line

  getline(inputFile,inputLine);
  if(inputLine.compare("P2") != 0) std::cerr << "Version error" << std::endl;
  else std::cout << "Version : " << inputLine << std::endl;
  // Gets the version from the pgm file
  getline(inputFile,inputLine);
  std::cout << "Comment : " << inputLine << std::endl;
  // Gets the Comment from the pgm file
  inputFile >> xWidth >> yHeight;
  std::cout << xWidth << " rows " << yHeight << " columns" << std::endl;
  // Gets the Width and Height from the pgm file
  inputFile >> maxWhite;
  std::cout << "Max white value : " << maxWhite << std::endl;
  // Gets the max white value from pgm value

  std::vector<std::vector<int>> pgmMatrix(yHeight, std::vector<int> (xWidth,0 ));

  for (int i = 0; i < pgmMatrix.size(); ++i) {
    // Gets matrix from pgm file
    for (int j = 0; j < pgmMatrix[i].size(); ++j) {
      int temp;
      inputFile >> temp;
      pgmMatrix[i][j] = temp;
    }
  }

  seamCarver(yHeight, xWidth, vertSeams, pgmMatrix);
  // Carves the vertical seams
  flipMatrix(pgmMatrix);
  // Flips matrix so that seamCarver can be reused due to Vertical and Horizontal being changed
  std::swap(xWidth, yHeight);
  // Swaps the width and height of the matrix to be used
  seamCarver(yHeight, xWidth, horizSeams, pgmMatrix);
  // Carves the horizontal seams
  flipMatrix(pgmMatrix);
  // Flips matrix back to correct orientation
  
  std::size_t pos = filename.find_last_of(".");
  if (pos != std::string::npos) filename = filename.substr(0, pos) + "_processed" + filename.substr(pos, pos + 2);
  // Creates a file with _processed to end of file name
  
  std::ofstream outputFile(filename);
  
  outputFile << "P2\n";
  outputFile << pgmMatrix[0].size() << " " << pgmMatrix.size() << "\n";
  outputFile << maxWhite << "\n";
  // Puts heading information into the output file
  
  for (int i = 0; i < pgmMatrix.size(); i++) {
    // Prints the seam carved matrix to the output file
    for (int j = 0; j < pgmMatrix[i].size(); j++) {
      outputFile << pgmMatrix[i][j] << " ";
    }
    if (i != pgmMatrix.size()-1) outputFile << "\n";
  }
  
  outputFile.close();
  inputFile.close();
  // Closes files
  
  return 0;
}

