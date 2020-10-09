#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int main()
{
	int height, width;
	int i, j, k, l = 0;
	int pos = -1;
	int buf;
	string line, ss;
	//файл с матрицей цены
	ifstream ifs("C:\\Users\\Crois-San\\Desktop\\gameTheoryData.txt");
	
	//ищем длину и ширину матрицы
	while (getline(ifs, line))
	{
		k = 1;
		for (i = 0; i < line.length(); i++)
		{
			if (line.at(i) == ' ')
				k++;
		}
		width = k;
		l++;
	}
	ifs.close();
	height = l;

	//создаем дин. массив с длиной и шириной матрицы цены
	int **costMatrix = new int*[height];
	for (i = 0; i < height; i++)
	{
		costMatrix[i] = new int[width];
	}

	ifstream ifs2("C:\\Users\\Crois-San\\Desktop\\gameTheoryData.txt");

	//заполняем массив данными из файла
	for (i = 0; i < height; i++)
	{
		getline(ifs2, line);
		for (j = 0; j < width; j++)
		{
			buf = pos + 1;
			pos = line.find(" ", pos + 1);
			ss = line.substr(buf, pos - buf);
			costMatrix[i][j] = stoi(ss);
		}
	}

	ifs.close();
	int *maxMin = new int[width];
	int *minMax = new int[height];
	//int *initCost = new int[height];
	//double *y = new double[height];
	int min = 10;
	int max = 0;
	bool *domLine = new bool[height];
	bool *domCol = new bool[width];
	int saddlePointCoords[2] = {0,0};
	VectorXd P(width), Q(height);
	int s = 0;
	
	for (i = 0; i < width; i++)
	{
		maxMin[i] = 0;
		domCol[i] = true;
	}
	for (i = 0; i < height; i++)
	{
		minMax[i] = 100;
		domLine[i] = true;
		//initCost[i] = -1;
		//y[i] = 0;
		s += costMatrix[i][j];
	}
	srand(s);
	//вывод матрицы цены
	printf("Your matrix: \n");
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			printf("%i\t", costMatrix[i][j]);
			//поиск минмаксных значений
			if (costMatrix[i][j] < minMax[i])
				minMax[i] = costMatrix[i][j];

		}
		printf("\n");
	}

	//поиск максиминных значений
	printf("maxMin: \n");
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height; i++)
		{
			if (costMatrix[i][j] > maxMin[j])
				maxMin[j] = costMatrix[i][j];
		}
		printf("%i\t", maxMin[j]);
	}

	//поиск максимального значения среди минмаксных
	printf("\nminMax: \n");
	for (i = 0; i < height; i++)
	{
		printf("%i\t", minMax[i]);
		if (max < minMax[i])
		{
			max = minMax[i];
			saddlePointCoords[1] = i;
		}
	}
	//поиск минимального значения среди максиминных
	for (i = 0; i < width; i++)
	{
		if (min > maxMin[i])
		{
			min = maxMin[i];
			saddlePointCoords[0] = i;
		}
	}
	printf("\nLow game cost: %i.\n", min);
	printf("High game cost: %i.\n", max);
	//поиск седловой точки
	if (min == max)
	{
		printf("There is a saddle point.\n");
		for (i = 0; i < height; i++)
		{
			if (saddlePointCoords[1] == i)
				Q(i) = 1;
			else
				Q(i) = 0;
		}
		for (i = 0; i < width; i++)
		{
			if (saddlePointCoords[0] == i)
				P(i) = 1;
			else
				P(i) = 0;
		}
		cout << "The optimal strategy of player A is:\n" << P << endl;
		cout << "The optimal strategy of player B is:\n" << Q <<endl;
	}
	else
	{
		printf("There is no saddle point.\n");
		cout << "dominant row:" << endl;
		//поиск доминирующей строки
		for (i = 0; i < height; i++)
		{
			for (k = 0; k < height; k++)
				for (j = 0; j < width; j++)
				{
					if (costMatrix[i][j] < costMatrix[k][j] && k != i)
					{
						domLine[i] = false;
						break;
					}
				}
			std::cout << domLine[i] << "\t";
		}
		std::cout << "\n";

		//обнуление доминирующей строки
		for (i = 0; i < height; i++)
		{
			if (domLine[i])
			{
				for (j = 0; j < width; j++)
				{
					costMatrix[i][j] = 0;
				}
			}

		}

		cout << "dominant column:" << endl;
		//поиск доминирующего столбца
		for (i = 0; i < width; i++)
		{
			for (k = 0; k < width; k++)
				for (j = 0; j < height; j++)
				{
					if (costMatrix[j][i] < costMatrix[j][k] && k != i)
					{
						domCol[i] = false;
						break;
					}
				}
			std::cout << domCol[i] << "\t";
		}
		std::cout << "\n";

		//обнуление доминирующего столбца
		for (j = 0; j < width; j++)
		{
			if (domCol[j])
			{
				for (i = 0; i < height; i++)
				{
					costMatrix[i][j] = 0;
				}
			}

		}
		std::cout << "\n";
		
		
		for (i = 0; i < width; i++)
		{
			cout << "V" << i+1 << "(x)= " << costMatrix[0][i] << " + (" << costMatrix[1][i] << " - " << costMatrix[0][i] << ")x" << endl;

		}
		std::cout << "\n";


		MatrixXd costM(width, height);
		MatrixXd Pstrat(height+1,width+1), Qstrat(width + 1, height + 1);
		VectorXd Y1(height), Y2(width);
		VectorXd initCostA(height), initCostB(width);
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				costM(j, i) = costMatrix[i][j];
			}
		}
		for (i = 0; i < height; i++)
		{
			Y1(i) = 1;
			initCostA(i) = -1;
		}
		for (i = 0; i < width; i++)
		{
			Y2(i) = 1;
			initCostB(i) = -1;
		}
		for (i = 0; i < height + 1; i++)
			for (j = 0; j < width + 1; j++)
			{
				if (i < height && j < width)
				{
					Qstrat(j, i) = costM(j, i);
				}
				if (j == width && i!= height)
				{
					Qstrat(j, i) = initCostA(i);
				}
				if (i == height)
				{
					if (j == width)
					{
						Qstrat(j, i) = 0;
					}
					else
					{
						Qstrat(j, i) = 1;
					}
				}
			}

		for (i=0;i< width+1;i++)
			for (j = 0; j < height + 1; j++)
			{
				if (j < height && i < width)
				{
					Pstrat(j, i) = costM(i, j);
				}
				if (j == height && i != width)
				{
					Pstrat(j, i) = initCostB(i);
				}
				if (i == width)
				{
					if (j == height)
					{
						Pstrat(j, i) = 0;
					}
					else
					{
						Pstrat(j, i) = 1;
					}
				}
			}
		VectorXd x1, x2;
		IOFormat CommaInitFmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", "(", ")");
		x1 = costM.fullPivLu().solve(Y2);
		x2 = costM.transpose().fullPivLu().solve(Y1);
		double fCost=0;
		for (i = 0; i < height; i++)
		{
			fCost += x1(i);
		}
		cout << "The optimal strategy of player A is:\np*= "  << x2.format(CommaInitFmt) << endl;
		cout << "The optimal strategy of player B is:\nq*= " << x1.format(CommaInitFmt) << endl;
		cout << "V = " << fCost << endl;
	}

	

	return 0;
}