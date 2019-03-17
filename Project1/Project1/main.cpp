#include <iostream>
#include <string>
#include "hash_table.h"



const int upper = 126;

const int lower = 33;

int main()
{
	nwacc::hash_table<char,double> table(7);
	std::vector<char> characters;
	for (auto counter=lower;counter<=upper;counter++)
	{
		characters.push_back(counter);
	}
	char random_char=' ';
	int inserted = 0;
	while (table.size() < 50 && characters.size() > 0)
	{
		auto value = characters.size() > 1? rand() % characters.size(): 0 ;
		random_char = characters[value];
		std::swap(characters[value], characters.back());
		characters.pop_back();
		table.insert(random_char, ((double)rand()*(lower - upper) / (double)upper - lower));
		inserted++;		
	}
	table.print();

	std::cout << table[random_char] << "---";
	std::cout << (table[random_char] = 56) << std::endl;

	system("pause");
	return 0;
}
