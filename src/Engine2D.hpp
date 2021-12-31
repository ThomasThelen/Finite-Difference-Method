#include "Engine.hpp"

class Engine2D : public Engine
{
public:
	vector<vector<tuple<double, double>>> Instance;

	// !This is ugly
	vector<vector<vector<tuple<double, double>>>> Results;

	void CreateMesh(Mesh2D&);

	void CreateInitialState(Mesh2D&);

	void CreateCSV(Mesh2D);

	void Solve(Mesh2D&);

	bool CheckStability(Mesh2D);

	double RetrieveTemperature(int, int, int);

	double GetPreviousTemperature(int, int, double, char);
};