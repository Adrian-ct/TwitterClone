#include "MyDB.h"

void MyDB::Open(const std::string& database)
{	
	if (sqlite3_open(database.c_str(), &m_db) != SQLITE_OK)
	{
		throw "database could not be opened";
	}
}

void MyDB::Close()
{
	sqlite3_close(m_db);
}


void MyDB::PrintResult(const Records& result) const
{
	for (Record record : result)
	{
		for (const std::string& s : record)
			std::cout << s << " ";
		std::cout << "\n";
	}
}

void MyDB::Insert(const std::string& statement)
{
	const std::string _insertStatement = "INSERT " + statement;

	int ret = sqlite3_exec(m_db, _insertStatement.c_str(), NULL, NULL, NULL);

	if (ret != SQLITE_OK) {
		std::cerr << "Error in insert statement " << "'" << _insertStatement << "'" << "\n";;
	}
}

void MyDB::Delete(const std::string& deleteStatement)
{
	const std::string _deleteStatement = "DELETE " + deleteStatement;

	int ret = sqlite3_exec(m_db, _deleteStatement.c_str(), NULL, NULL, NULL);

	if (ret != SQLITE_OK) {
		std::cerr << "Error in delete statement " << "'" << _deleteStatement << "'" << "\n";;
	}

}

void MyDB::Update(const std::string& updateStatement)
{
	const std::string _updateStatement = "UPDATE " + updateStatement;

	int ret = sqlite3_exec(m_db, _updateStatement.c_str(), NULL, NULL, NULL);

	if (ret != SQLITE_OK) {
		std::cerr << "Error in update statement " << "'" << _updateStatement << "'" << "\n";;
	}
}

std::vector<json>  MyDB::Select(const std::string& selectStatement) const
{
	const std::string _selectStatement = "SELECT " + selectStatement;
	std::vector<json> records;

	char* errmsg;
	int ret = sqlite3_exec(m_db, _selectStatement.c_str(), select_callback_null, &records, &errmsg);
	if (ret != SQLITE_OK) {
		std::cerr << "Error in select statement " << _selectStatement << "[" << errmsg << "]\n";
	}
	else {
		//std::cerr << records.size() << " records returned.\n";
	}

	return records;
}

int MyDB::select_callback_null(void* p_data, int num_fields, char** p_fields, char** p_col_names)
{
	Records recordsString;
	std::vector<json>* records = static_cast<std::vector<json>*>(p_data);

	try {
		
		json currentRecord;
		for(int currentField = 0; currentField<num_fields;currentField++)
		{
			if (p_fields[currentField] != nullptr)
			{
				currentRecord[p_col_names[currentField]] = p_fields[currentField];
			}
			else
			{
				currentRecord[p_col_names[currentField]] = nullptr;
			}
		}

		records->push_back(currentRecord);
		
	}
	catch (...) {
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return 1;
	}

	return 0;
}