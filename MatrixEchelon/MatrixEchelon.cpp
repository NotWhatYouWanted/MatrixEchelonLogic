// MatrixEchelon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MatrixHandler.h"

MatrixHandler GetUserMatrix();

int main()
{

	MatrixHandler MATRIX = GetUserMatrix();

	//Apply sort before checking for available operations.
	MATRIX.EchelonSort();
	MATRIX.PrintMatrix();

	//For each column to be reduced (doesnt satisfy echelon form (0 Vals after Row > Column))
	while (MATRIX.GetUnsortedCol() >= 0)
	{
		std::cout << "Next unsorted column is: " << MATRIX.GetUnsortedCol() << std::endl;
		std::cout << "Applying sort...\n";
		MATRIX.SortCol(MATRIX.GetUnsortedCol());
		MATRIX.EchelonSort();
		std::cout << "New Matrix Produced...\n";
		MATRIX.PrintMatrix();
	}

    return 0;
}

MatrixHandler GetUserMatrix()
{
	std::cout << "Would you like to input a custom matrix?Y/N\n";
	std::string Input = "";
	std::cin >> Input;

	if (Input == "Y" || Input == "y")
	{
		std::vector<std::vector<int>> NewMatrix;
		int Rows = 0, Cols = 0;

		std::cout << "Please enter the dimensions of your *AUGMENTED* matrix (requires an end column for the equation values):\n";
		std::cout << "Rows: ";
		std::cin >> Rows;
		std::cout << "Columns: ";
		std::cin >> Cols;

		//For each row to enter, get the row data
		for (int R = 0; R < Rows; R++)
		{
			std::cout << "Inputting row " << R << std::endl;
			std::vector<int> NewRow;
			//Populate column data for rows
			for (int C = 0; C < Cols; C++)
			{
				std::cin >> Input;
				NewRow.push_back(std::stoi(Input));
			}
			NewMatrix.push_back(NewRow);
		}

		std::cout << "Input complete. Constructing matrix.\n";
		return MatrixHandler(NewMatrix);

	}

	else
		//Default matrix constructor handles filling vectors with example (hard-coded) equation.
		return MatrixHandler();
}