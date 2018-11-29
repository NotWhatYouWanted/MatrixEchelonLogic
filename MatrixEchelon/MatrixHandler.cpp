#include "stdafx.h"
#include "MatrixHandler.h"


MatrixHandler::MatrixHandler() :
	Matrix(InputMatrix()),
	MatrixRows(Matrix.size()),
	MatrixCols(Matrix[0].size())
{
}

MatrixHandler::MatrixHandler(std::vector<std::vector<int>> UserMatrix) :
	Matrix(UserMatrix),
	MatrixRows(Matrix.size()),
	MatrixCols(Matrix[0].size())
{
}

MatrixHandler::~MatrixHandler()
{
}

//
// I/O Functions
//

std::vector<std::vector<int>> MatrixHandler::InputMatrix()
{
	std::vector<std::vector<int>> NewMatrix;

	//Using sample matrix for now, based on equations:
	// 2x + 3y - 2z = 5
	// 0x + 6y + 1z = 55
	// 1x + 2y + 4z = 71

	std::vector<int> Row1 = { 2, 5, 3, 110};
	std::vector<int> Row2 = { 4, 2, 1, 111 };
	std::vector<int> Row3 = { 1, 3, 5, 65 };

	/*
	std::vector<int> Row1 = { 1, 1, 1, 6 };
	std::vector<int> Row2 = { 0, 2, 5, -4 };
	std::vector<int> Row3 = { 2, 5, -1, 27 };
	*/

	//TODO Soft code matrix input, once input format is established, current input is hard-coded
	NewMatrix.push_back(Row1);
	NewMatrix.push_back(Row2);
	NewMatrix.push_back(Row3);
	
	return NewMatrix;
}

void MatrixHandler::PrintMatrix()
{
	//While R is inside of matrix row range
	for (int R = 0; R < MatrixRows; R++)
	{
		//Print each col entry in row R.
		for (int C = 0; C < MatrixCols; C++)
		{
			std::cout << Matrix[R][C] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


//
// LOGICAL FUNCTIONS
//


int MatrixHandler::GetUnsortedCol()
{
	for (int C = 0; C < MatrixCols; C++)
	{
		if (CheckColSort(C) > 0)
			return C;
	}

	return -1; //Negative index means all rows are sorted!
}

int MatrixHandler::CheckColSort(int ColIndex)
{
	int UnsortedRows = 0;
	//For each row in the column
	for (int R = 0; R < MatrixRows; R++)
	{
		///TODO Expand this search for irregular matrices.
		//Check if entry should be zero, which means is the row index higher than column
		if (R > ColIndex && Matrix[R][ColIndex] != 0)
			UnsortedRows++;
	}
	return UnsortedRows;
}

//Returns index of lowest non 0 value in column (will return -1 in failure state)
int MatrixHandler::GetLowestInCol(int ColIndex)
{
	int Low = 0;
	int Index = -1;
	//Set Low to first non-zero value in column
	for (int R = 0; R < MatrixRows; R++)
	{
		if (Matrix[R][ColIndex] != 0)
		{
			Low = Matrix[R][ColIndex];
			Index = R;
			break;
		}
	}
	//Set low to any ABSOLUTE values lower than low (and not equal to 0)
	for (int R = 0; R < MatrixRows; R++)
	{
		if (abs(Matrix[R][ColIndex]) < abs(Low) && Matrix[R][ColIndex] != 0)
		{
			Low = Matrix[R][ColIndex];
			Index = R;
		}
	}
	return Index;
}
int MatrixHandler::GetNextInCol(int ColIndex, int PrevIndex)
{
	//Could probably remove Low variable (in GetLowest func too), as the index is stored anyway. Keeping just for legibility. 
	int Low = 0;
	int Index = -1;
	//Set Low to non-zero value in column, that isnt the previous value.
	for (int R = 0; R < MatrixRows; R++)
	{
		//If variable at R isnt 0, use that. Unless it is the previous index or lower than previous. (stops same digit going through twice)
		if (Matrix[R][ColIndex] != 0 
			&& R !=PrevIndex 
			&& abs(Matrix[R][ColIndex]) > abs(Matrix[PrevIndex][ColIndex]))
		{
			Low = Matrix[R][ColIndex];
			Index = R;
			break;
		}
	}
	//Set low to any values lower than low, but higher than previous value (and not equal to 0)
	for (int R = 0; R < MatrixRows; R++)
	{
		if (abs(Matrix[R][ColIndex]) < abs(Low) 
			&& Matrix[R][ColIndex] != 0
			&& abs(Matrix[R][ColIndex]) > abs(Matrix[PrevIndex][ColIndex]))
		{
			Low = Matrix[R][ColIndex];
			Index = R;
		}
	}
	return Index;
}

void MatrixHandler::SortCol(int ColIndex)
{ 
	//If no common factors are found.
	if (!FactorSearch(ColIndex))
	{
		//Move to a simple reduction (A*B) - (B*A)
		int LowIndex = GetLowestInCol(ColIndex);

		//Low index cant be lower than the column we're working in, else it will be a sorted row.
		while (LowIndex < ColIndex)
			{ LowIndex = GetNextInCol(ColIndex, LowIndex); }

		int TargetIndex = -1;
		//Find next non-zero entry.
		//TODO We start these sort of loops at ColIndex, to avoid checking sorted rows in the first place
		for (int R = ColIndex; R < MatrixRows; R++)
		{
			//The row we reduce cant be == 0, cant be the row we're using as the factor, and cant be a sorted row.
			if (Matrix[R][ColIndex] != 0 && R != LowIndex && R >= ColIndex)
			{
				TargetIndex = R;
				std::cout << "Chosen Row: " << R << " to reduce. Using row: " << LowIndex << " as the lowest row we can use as the factor.\n";
				break;
			}
		}

		//Force reduction (A * B) + (-1 * B * A)
		std::vector<int> TempRowA = MultiplyRow(LowIndex, Matrix[TargetIndex][ColIndex]);
		std::vector<int> TempRowB = MultiplyRow(TargetIndex, Matrix[LowIndex][ColIndex]*-1); //We *-1 so SumRows acts as subtraction.

		std::vector<int> TempSum = SumRows(TempRowA, TempRowB);

		SetRow(TargetIndex, TempSum);
	}
	return;
}

//This checks for common factors in a specified column and reduces them, if its fails will return false
bool MatrixHandler::FactorSearch(int ColIndex)
{
	//Used to track loops, only execute this loop max the same amount of times as matrix rows. 
	int Iterations = 0;
	int LastLowIndex = -1;
	int LowIndex = 0; ///////////
	bool HitFound = false;

	//While we dont have a match, and while we havent checked each row for factors.
	while (Iterations < MatrixRows
		&& !HitFound)
	{

		//If this is the first iteration, find the lowest poss value, else find the next lowest.
		//Low is the ROW INDEX of the lowest value in col 
		//int LowIndex = 0;
		
		if (Iterations == 0)
			LowIndex = GetLowestInCol(ColIndex);
		else
			LowIndex = GetNextInCol(ColIndex, LastLowIndex);
		//This row HAS to be an unsorted row too (no non zeros to the left) or it will corrupt the row we operate on.
		while (LowIndex < ColIndex)
		{
			//Quick check if GetNextInCol has failed on the previous iteration (meaning we have gone out of bounds)
			if (LowIndex < 0)
				return false;
			LowIndex = GetNextInCol(ColIndex, LowIndex);
		}

		for (int R = 0; R < MatrixRows; R++)
		{
			//DO NOT TOUCH SORTED COLS so R must be higher than col index.
			//Unless first column, as no rows before it are sorted. Also check we're not comparing the same index. 
			//Also divding 1 by anything will give a false positive. Int portion will always == 0.
			if ((R > ColIndex || ColIndex == 0)
				&& R != LowIndex
				&& Matrix[R][ColIndex] != 1)
			{
				//Check IF LowestRow % against rest of Column == 0
				if (Matrix[R][ColIndex] % Matrix[LowIndex][ColIndex] == 0)
				{
					//If we have a pair of factors (Row % Low = 0)
					//Create a new row using lowest row, that will cancel out Row.
					//If a%b==0  then  a/b=c  and  b
					std::vector<int> TempRow = MultiplyRow(
						LowIndex,
						// Create negative compliment, hence -1*.
						-1 * (Matrix[R][ColIndex] / Matrix[LowIndex][ColIndex]));


					//Set matched row, as itself + negative match (previously lowest row)
					SetRow(R, SumRows(TempRow, Matrix[R]));
					HitFound = true;
					break;
				}
			}
		}
		//If loop has finished without HitFound being set, means LowIndex is not a factor of other members. Move to next highest no.
		if (!HitFound)
		{
			LastLowIndex = LowIndex;
			Iterations++;
		}
	}

	return HitFound;
}

//
// ERO Operations
//


std::vector<int> MatrixHandler::MultiplyRow(int RowIndex, int Factor)
{
	std::vector<int> Result;
	for (int i = 0; i < MatrixCols; i++)
		Result.push_back(Matrix[RowIndex][i] * Factor);

	return Result;
}

std::vector<int> MatrixHandler::SumRows(std::vector<int> Row1, std::vector<int> Row2)
{
	std::vector<int> Result;

	for (int C = 0; C < MatrixCols; C++)
		Result.push_back(Row1[C] + Row2[C]);

	return Result;
}

void MatrixHandler::SetRow(int TargetIndex, std::vector<int> NewData)
{
	if (NewData.size() == Matrix[TargetIndex].size())
		Matrix[TargetIndex] = NewData;
	else
		std::cout << "Set row failed. Rows did not match.\nCurrent Matrix Width : " << Matrix[TargetIndex].size() << "\nNew row width: " << NewData.size();
}

void MatrixHandler::SwapRows(int Row1, int Row2)
{
	if (Row1 < MatrixRows && Row2 < MatrixRows)
	{
		std::vector<int> Buffer = Matrix[Row1];
		Matrix[Row1] = Matrix[Row2];
		Matrix[Row2] = Buffer;
	}
	else
	{
		std::cout << "Out of bounds row swap request.\n";
		std::cout << "Highest matrix rows index: " << MatrixRows - 1 << std::endl;
		std::cout << "Row 1: " << Row1 << std::endl;
		std::cout << "Row 2: " << Row2 << std::endl;
	}
}


//Overloaded so function will handle recursion.
void MatrixHandler::EchelonSort()
{
	//std::cout << "Checking first column\n\n";
	//This overload is for the first recursion of the function, check every row
	int RowsToCheck = MatrixRows;
	int Iterations = 0;
	//Counter to pass on to next recursion, means only rows that have been moved are checked. therefore init to 0
	int RowsMoved = 0;

	//This is the index of the last row that needs checking (the lowest index 0-value row from the previous iteration)
	int LastRow = MatrixRows - RowsToCheck;
	std::vector<int> TrashRows;

	//Scan matrix bottom to top vertically a maximum of RowsToCheck times
	for (int R = MatrixRows - 1; R >= LastRow; R--)
	{
		//If any 0's found in column, move to bottom.
		if (Matrix[R][Iterations] == 0)
		{
			//Insert a copy at the end and save index to delete later, to avoid moving unchecked row to current pos
			Matrix.push_back(Matrix[R]);
			TrashRows.push_back(R);
			//Break from loop if all rows have been checked
			if (++RowsMoved >= MatrixRows)
				break;
		}
	}

	//For every member in TrashRows
	for (size_t i = 0; i < TrashRows.size(); i++)
		Matrix.erase(Matrix.begin() + TrashRows[i]); //Delete the MatrixRow index contained 

	//If any rows have been changed, scan next col.
	if (RowsMoved > 1)
		EchelonSort(++Iterations, RowsMoved);

	return;
}

void MatrixHandler::EchelonSort(int Iterations, int RowsToCheck)
{
	//std::cout << "Checking column: " << Iterations << std::endl << std::endl;

	//Counter to pass on to next recursion, means only rows that have been moved are checked , and therefore start with 0
	int RowsMoved = 0;

	//Scan matrix bottom to top horizontally a maximum of RowsToCheck times
	int LastRow = MatrixRows - RowsToCheck;
	std::vector<int> TrashRows;
	for (int R = MatrixRows - 1; R >= LastRow; R--)
	{
		//std::cout << "Checking Row: " << R << std::endl;
		//If any 0's found in column, move to bottom
		if (Matrix[R][Iterations] == 0)
		{
			//Add row to end of matrix, also add index to TrashRows
			Matrix.push_back(Matrix[R]);
			TrashRows.push_back(R);
			//Break from loop if all rows have been checked
			if (++RowsMoved >= MatrixRows)
				break;
		}
	}

	//For every member in TrashRows
	for (size_t i = 0; i < TrashRows.size(); i++)
		Matrix.erase(Matrix.begin() + TrashRows[i]); //Delete the MatrixRow index contained 

	//If next column flagged, scan next column, unless it is the last column
	if (RowsMoved > 1 && !(Iterations >= MatrixCols - 1))
		EchelonSort(++Iterations, RowsMoved);

	return;
}

