#include <iostream>
#include <string>
#include "commit.h"

using namespace std;

int main()
{
	commit c1;
	c1.cat();

	commit c2;
	int num = 2;
	string parents[2] = {"ac123ac12345ac12345790790457ac1234579090", "ac12345790ac12345790ac12345790ac12345790"};
	string tree_id = "deab32144842904855445";
	string author = "Ankan Kumar Das";
	string message = "Test Message";
	string hash;
	c2.write_parents(num, parents);
	c2.write_tree(tree_id);
	c2.write_author(author);
	c2.write_message(message);
	c2.create_commit(hash);
	cout << hash << "\n";
	return 0;
}