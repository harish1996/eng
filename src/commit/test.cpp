#include <iostream>
#include <string>
#include "commit.h"

using namespace std;

int main()
{
	// Empty commit object should print noting
	commit c1;
	c1.cat();

	// Create a new commit object
	commit c2;
	int num = 2;
	string parents[2] = {"ac123ac12345ac12345790790457ac1234579057", "de54321790ac12345790ac12345790ac12345745"};
	string tree_id = "deab32144842904855445";
	string author = "Ankan Kumar Das";
	string message = "Test Message";
	string hash;
	cout << "Write Parents Status : " << c2.write_parents(num, parents) << "\n";
	cout << "Write Tree Status : " << c2.write_tree(tree_id) << "\n";
	cout << "Write Author Status : " << c2.write_author(author) << "\n";
	cout << "Write Message Status : " << c2.write_message(message)<< "\n";
	cout << "Create Commit Status : " << c2.create_commit(hash) << "\n";
	cout << "Hash Value : " << hash << "\n";
	cout << "\nCommit Object parameters written -->\n\n";
	c2.cat();

	// Open an existing commit object
	commit c3(hash);
	cout << "\n\nCommit Object parameters fetched -->\n\n";
	c3.cat();
	return 0;
}