#pragma once
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include<optional>
#include "nlohmann.hpp"
using namespace nlohmann;

class MyDB
{
public:
	using Record = std::vector<std::string>;
	using Records = std::vector<Record>;

public:
	MyDB() = default;
	
	void Open(const std::string& database);
	void Close();

	std::vector<json> Select(const std::string& selectStatement) const;
	void PrintResult(const Records& result)const;

	void Insert(const std::string& statement);
	
	void Delete(const std::string& deleteStatement);
	void Update(const std::string& updateStatement);

private:
	sqlite3* m_db;
private:
	static int select_callback_null(void* p_data, int num_fields, char** p_fields, char** p_col_names);
};

