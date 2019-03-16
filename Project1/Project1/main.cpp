#include <iostream>
#include <string>

#include "hash_table.h"

int main()
{
	nwacc::hash_table<double,int> table(7);

	table.insert(1.0,std::rand());
	table.insert(3.5, std::rand());
	table.print();
	table.insert(-9.0, std::rand());
	table.print();
	table.insert(5.3, std::rand());
	table.print();
	table.insert(3.2, std::rand());
	table.print();
	table.remove(1.0);
	table.print();
	std::cout << std::boolalpha << table.contains(3.5) << std::endl;

	return 0;
}
