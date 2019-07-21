#include "commit.h"

#define BUF_SIZE 256
#define SUCCESS 0
#define FAILURE -1

int commit::open_commit( const std::string &commit_id )
{
	if(1 != get_new_object( commit_id ) || 1 != object_type()) {
		std::cout << "invlaid commit id";
		return FAILURE;
	}

	std::stringstream ss;
	char *buffer = new char[260];
	while(BUF_SIZE == read_object( buffer, BUF_SIZE ))
		ss << BUF_SIZE;
	ss << BUF_SIZE;
	delete buffer;

	unsigned char hashnum[21];
	getline(ss, this->_commit_type, "\0");
	ss.read(hashnum, 21);
	hash_numtostr(this->_parents[0], hashnum);
	if(2 == this->_commit_type) {
		ss.read(hashnum, 21);
		hash_numtostr(this->_parents[1], hashnum);
	}
	getline(ss, this->_author, "\0");
	getline(ss, this->_tree_id, "\0");
	getline(ss, this->_commit_message, "\0");

	return SUCCESS;
}

int commit::get_commit_type()
{
	return this->_commit_type;
}

int commit::get_parents( std::string *parents )
{
	if(this->_parents[0].empty()) {
		std::cout << "open commit to get parents\n";
		return FAILURE;
	}
	parents = new string[2];
	parents[0] = new string( this->_parents[0] );
	if( ! this->_parents[1].empty() )
		parents[1] = new string( this->_parents[1] );
	return SUCCESS;
}

int commit::get_tree( std::string &tree_id )
{
	if( this->_tree_id.empty() ) {
		std::cout << "open commit to get tree id\n";
		return FAILURE;
	}
	tree_id = new string( this->_tree_id );
	return SUCCESS;
}

int commit::get_author( std::string &author )
{
	if( this->_author.empty() ) {
		std::cout << "open commit to get author\n";
		return FAILURE;
	}
	author = new string( this->_author );
	return SUCCESS;
}

int commit::get_message( std::string &commit_message )
{
	if( this->_commit_message.empty() ) {
		std::cout << "open commit to get commit message\n";
		return FAILURE;
	}
	commit_message = new string( this->_commit_message );
	return SUCCESS;
}

int commit::write_parents( int num, const std::string *parents )
{
	if(num < 1 || num > 2)
		return FAILURE;
	if(1 == num) {
		this->_commit_type = 1;
		this->_parents[0] = new string( parents[0] );
	}
	else {
		this->_commit_type = 2;
		this->_parents[0] = new string( parents[0] );
		this->_parents[1] = new string( parents[1] );
	}
	return SUCCESS;
}

int commit::write_tree( const std::string &tree_id )
{
	if(tree_id.empty())
		return FAILURE;
	this->_tree_id = new string( tree_id );
	return SUCCESS;
}

int commit::write_author( const std::string &author )
{
	if(author.empty())
		return FAILURE;
	this->_author = new string( author );
	return SUCCESS;
}

int commit::write_message( const std::string &commit_message )
{
	if(commit_message.empty())
		return FAILURE;
	this->_commit_message = new string( commit_message );
	return SUCCESS;
}

int commit::create_commit()
{
	if(-1 == this->_commit_type || this->_author.empty() || this->_tree_id.empty() || this->_commit_message.empty()) {
		std::cout << "some parameters not set\n";
		return FAILURE;
	}
	std::stringstream ss;
	unsigned char hashnum[21];
	ss << this->_commit_type << "\0"; // write commit type
	hash_strtonum(this->_parents[0], hashnum);
	hashnum[20] = '\0';
	ss << hashnum << "\0"; // write parent
	if(2 == this->_commit_type) {
		hash_strtonum(this->_parents[1], hashnum);
		hashnum[20] = '\0';
		ss << hashnum << "\0"; // write this for multiple parent
	}
	ss << this->_author << "\0" << this->_tree_id << "\0" << this->_commit_message << "\0"; // write author, tree id and commit message
	return create_object( ss , 1 );
}