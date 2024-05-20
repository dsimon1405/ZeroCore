#include <ZC/Tools/SQLite3/ZC_SQLite3.h>

// #include <string>

// #include <list>
// #include <map>
// class DB
// {
// public:
// 	static void Open(const std::string& name = "Resources.db", const bool& defNaming = true) noexcept
// 	{
// 		if (sqlite3_open_v2(name.c_str(), &db, SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX, NULL))
// 		{
// 			Close();
// 			// ErrorLoger::Insert(__FILE__, __LINE__, sqlite3_errmsg(db));
// 			return;
// 		}

// 		if (!defNaming)
// 		{
// 			EditNames();
// 			defaultNaming = defNaming;
// 		}
// 		else
// 		{
// 			tableNames.clear();
// 			columnNames.clear();
// 		}
// 	}

// 	static std::list<std::list<std::string>> Select(const std::string& sql) noexcept
// 	{
// 		char* errMsg = 0;
// 		std::list<std::list<std::string>> response;
// 		int result = sqlite3_exec(db, sql.c_str(), Callback, &response, &errMsg);
// 		if (result != SQLITE_OK)
// 		{
// 			// ErrorLoger::Insert(__FILE__, __LINE__, errMsg);
// 			sqlite3_free(errMsg);
// 		}
// 		return response;
// 	}

// 	static std::string SelectSingleStr(const std::string sql) noexcept
// 	{
// 		std::list<std::list<std::string>> response = Select(sql);
// 		return response.size() == 0 ? "" : std::move(response.front().front());
// 	}

// 	static void Close() noexcept
// 	{
// 		if (db) sqlite3_close(db);
// 	}

// 	static const std::string& Table(const std::string& table) noexcept
// 	{
// 		return defaultNaming ? table : tableNames[table];
// 	}

// 	static const std::string& Column(const std::string& column, const std::string& table) noexcept
// 	{
// 		return defaultNaming ? column : columnNames[table][column];
// 	}

// private:
// 	DB() = delete;

// 	static inline sqlite3* db = nullptr;
// 	static inline bool defaultNaming = true;
// 	static inline std::map<std::string, std::string> tableNames
// 	{
// 		{ "texture", "texture" },
// 		{ "shader", "shader" },
// 		{ "font", "font" },
// 	};
// 	static inline std::map<std::string, std::map<std::string, std::string>> columnNames
// 	{
// 		{
// 			"texture",
// 			{
// 				{ "name", "name" },
// 				{ "path", "path" }
// 			}
// 		},
// 		{
// 			"shader",
// 			{
// 				{ "name", "name" },
// 				{ "code", "code" },
// 			}
// 		},
// 		{
// 			"font",
// 			{
// 				{ "name", "name" },
// 				{ "path", "path" }
// 			}
// 		}
// 	};

// 	static int Callback(void* response, int columns, char** values, char** colummNames) noexcept
// 	{
// 		for (size_t i = 0; i < columns; i++)
// 		{
// 			if (i == 0)
// 			{
// 				((std::list<std::list<std::string>>*)response)->emplace_back(std::list<std::string>{values[i]});
// 				continue;
// 			}
// 			((std::list<std::list<std::string>>*)response)->back().emplace_back(values[i]);
// 		}
// 		return 0;
// 	}

// 	static void EditNames() noexcept
// 	{
// 		std::list<std::list<std::string>> tables = Select("SELECT * FROM table_specification");
// 		for (auto& names : tables)
// 		{
// 			tableNames[names.front()] = std::move(names.back());
// 		}
// 		std::list<std::list<std::string>> columns = Select("SELECT * FROM column_specification");
// 		for (auto& column : columns)
// 		{
// 			columnNames[column.front()][*(++column.begin())] = std::move(column.back());
// 		}
// 	}
// };