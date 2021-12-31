#pragma once
#include "Engine.hpp"

void Engine::CreateTime()
{
	cout << "Creating time domain..." << endl;
	times.push_back(0);
	while (times.back() < time_length)
	{
		times.push_back(times.back() + time_step);
	}
	cout << "Time Created" << endl;
}

tuple<double, double> Engine::GetRecordb(int state_position, int record_position) const
{
	vector<tuple<double, double>> state;
	tuple<double, double > record;
	state = results.at(state_position);
	record = state.at(record_position);
	return record;
}