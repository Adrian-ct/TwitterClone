#include "Validation.h"

bool Validation::ExistsUser(const MyDB& database,const std::string& username)
{   
	std::string statement="* FROM User WHERE username='"+username+"'";
	std::vector<json> check=database.Select(statement);
 
	return !check.empty();
}

bool Validation::VerifyPassword(const MyDB& database,const std::string& username, const std::string& password)
{  
	std::string statement = "password FROM User WHERE username='" + username + "'";
	std::vector<json> passwordDB= database.Select(statement);

	/*if (passwordDB.size() == 0)
		return true;*/

	if (passwordDB[0]["password"] != password)
		return false;
	 
    return true;
}
