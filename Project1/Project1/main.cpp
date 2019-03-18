// lewis nordan
// hash table

#include <iostream>
#include <string>
#include "hash_table.h"


/**
 *this largest ascii value desired
 * 
 */
const int upper = 126;
/**
 *this smallest ascii value desired
 *
 */
const int lower = 33;
/**
 *the number of elements to be inserted for testing
 *
 */
const int test_size = 50;
/**
 *the starting siz of the map
 *
 */
const int size = 7;
/**
 *this is zero
 *
 */
const int zero = 0;
/**
 *this is one
 *
 */
const int one = 1;
/**
 *this some arbitrary number for testing the subscript operator
 *
 */
const int test_value = 56;
/**
 *the main function of this program.
 *
 *@returns int 
 */
int main()
{
	nwacc::hash_table<char,double> table(size);
	std::vector<char> characters;
	for (auto counter=lower;counter<=upper;counter++)
	{
		characters.push_back(counter);
	}
	char random_char=' ';
	int inserted = 0;
	while (table.size() < test_size && characters.size() > zero)
	{
		auto value = characters.size() > one? rand() % characters.size(): zero ;
		random_char = characters[value];
		std::swap(characters[value], characters.back());
		characters.pop_back();
		table.insert(random_char, ((double)rand()*(lower - upper) / (double)upper - lower));
		inserted++;		
	}
	std::cout << table;
	std::cout << table[random_char] << "---";
	std::cout << (table[random_char] = test_value) << std::endl;

	system("pause");
	return 0;
}
