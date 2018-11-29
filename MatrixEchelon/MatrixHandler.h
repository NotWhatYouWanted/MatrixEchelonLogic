#pragma once
class MatrixHandler
{
public:
	MatrixHandler();
	MatrixHandler(std::vector<std::vector<int>> UserMatrix);
	~MatrixHandler();

	// I/O Functions
	std::vector<std::vector<int>> InputMatrix();
	void PrintMatrix();

	//Overload for first call of function, saves user entering params they arent aware of.
	void EchelonSort(); 
	void EchelonSort(int Iterations, int AUTORowsToCheck); 

	//Logical Functions
	int GetUnsortedCol();
	void SortCol(int ColIndex);

	//ERO Functions
	std::vector<int> MultiplyRow(int Row, int Factor);
	std::vector<int> SumRows(std::vector<int> Row1, std::vector<int> Row2);
	void SetRow(int Target, std::vector<int> NewData);
	void SwapRows(int Row1, int Row2);

private:

	// Internal Logical Functions

	int CheckColSort(int ColIndex);
	int GetLowestInCol(int ColIndex);
	int GetNextInCol(int ColIndex, int PrevIndex);
	//Will return true if successfully found and reduced factor.
	bool FactorSearch(int ColIndex);

	std::vector<std::vector<int>> Matrix;
	const int MatrixRows;
	const int MatrixCols;
};
