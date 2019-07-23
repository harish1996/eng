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
	while(BUF_SIZE == read_object( buffer, BUF_SIZE )) {
		ss << buffer;
		buffer[0] = '\0';
	}
	ss << buffer;
	delete buffer;

	char hashnum[21];
	ss >> this->_commit_type;
	ss.ignore(1);
	ss.read(hashnum, 20);
	hash_numtostr(this->_parents[0], (unsigned char *) hashnum);
	if(2 == this->_commit_type) {
		ss.read(hashnum, 20);
		hash_numtostr(this->_parents[1], (unsigned char *) hashnum);
	}
	getline(ss, this->_author);
	getline(ss, this->_tree_id);
	getline(ss, this->_commit_message);
	ss.read( (char *)&this->_timestamp,sizeof(time_t));
	// getline(ss, this->_timestamp);

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
	parents = new std::string[2];
	parents[0] = std::string( this->_parents[0] );
	if( ! this->_parents[1].empty() )
		parents[1] = std::string( this->_parents[1] );
	return SUCCESS;
}

int commit::get_tree( std::string &tree_id )
{
	if( this->_tree_id.empty() ) {
		std::cout << "open commit to get tree id\n";
		return FAILURE;
	}
	tree_id = std::string( this->_tree_id );
	return SUCCESS;
}

int commit::get_author( std::string &author )
{
	if( this->_author.empty() ) {
		std::cout << "open commit to get author\n";
		return FAILURE;
	}
	author = std::string( this->_author );
	return SUCCESS;
}

int commit::get_message( std::string &commit_message )
{
	if( this->_commit_message.empty() ) {
		std::cout << "open commit to get commit message\n";
		return FAILURE;
	}
	commit_message = std::string( this->_commit_message );
	return SUCCESS;
}

int commit::get_timestamp( std::string &timestamp )
{
	timestamp = ctime(&this->_timestamp);
	return 1;
}

int commit::write_parents( int num, const std::string *parents )
{
	if(num < 1 || num > 2)
		return FAILURE;
	if(1 == num) {
		this->_commit_type = 1;
		this->_parents[0] = std::string( parents[0] );
	}
	else {
		this->_commit_type = 2;
		this->_parents[0] = std::string( parents[0] );
		this->_parents[1] = std::string( parents[1] );
	}
	return SUCCESS;
}

int commit::write_tree( const std::string &tree_id )
{
	if(tree_id.empty())
		return FAILURE;
	this->_tree_id = std::string( tree_id );
	return SUCCESS;
}

int commit::write_author( const std::string &author )
{
	if(author.empty())
		return FAILURE;
	this->_author = std::string( author );
	return SUCCESS;
}

int commit::write_message( const std::string &commit_message )
{
	if(commit_message.empty())
		return FAILURE;
	this->_commit_message = std::string( commit_message );
	return SUCCESS;
}

int commit::create_commit( std::string &hash )
{
	if(-1 == this->_commit_type || this->_parents[0].empty() || this->_author.empty() || this->_tree_id.empty() || this->_commit_message.empty()) {
		std::cout << "some parameters not set\n";
		return FAILURE;
	}
	std::stringstream ss;
	unsigned char hashnum[21];
	this->_timestamp = time( NULL );
	ss << this->_commit_type << " "; // write commit type
	hash_strtonum(this->_parents[0], hashnum);
	hashnum[20] = '\0';
	ss << hashnum; // write parent
	if(2 == this->_commit_type) {
		hash_strtonum(this->_parents[1], hashnum);
		hashnum[20] = '\0';
		ss << hashnum; // write this for multiple parent
	}

	// write author, tree id and commit message
	ss << this->_author << '\n' << this->_tree_id << '\n' << this->_commit_message << '\n' ; 
	ss.write( (const char *)&this->_timestamp, sizeof(time_t) );
	int ret = create_object( ss , 1 );
	if(ret == 0)
		hash = get_hash();
	return ret;
}

void commit::cat()
{
	if(-1 == this->_commit_type || this->_parents[0].empty() || this->_author.empty() || this->_tree_id.empty() || this->_commit_message.empty()) {
		std::cout << "some parameters not set\n\n";
		// return;
	}
	std::cout << "Commit Type : " << this->_commit_type << "\n";
	std::cout << "Parent(s) : " << this->_parents[0] << " " << this->_parents[1] << "\n";
	std::cout << "Author : " << this->_author << "\n";
	std::cout << "Time : " << ctime(&this->_timestamp) << "\n";
	std::cout << "Tree Id : " << this->_tree_id << "\n";
	std::cout << "Commit Message : " << this->_commit_message << "\n";

}