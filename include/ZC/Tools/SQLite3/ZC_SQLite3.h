#pragma once

#include <ZC/Tools/Container/ZC_Tuple.h>

#include <sqlite3.h>

#include <string>
#include <list>
#include <cassert>
#include <iostream>
struct ZC_SQLite3
{
	static bool Open(std::string&& _name, int _flags)
    {
        // name = std::move(_name);
        // flags = _flags;
        // if (sqlite3_open_v2(name.c_str(), &db, flags, NULL) != SQLITE_OK)
        // {
        //     assert(false);
        //     return false;
        // }
        // // return true;




        // char* err;
        // std::list<std::list<std::string>> resp {};
        // auto q = sqlite3_exec(db, "SELECT * FROM t1", Callback, &resp, &err);
        // if (q != SQLITE_OK)
        // {
        //     std::cout<<sqlite3_errmsg(db)<<std::endl;
        //     int a = 3;
        // }
        // int a = 3;
        return true;
    }


	static int Callback(void* response, int columns, char** values, char** colummNames) noexcept
	{
        auto container = (std::list<std::list<std::string>>*)response;
		for (int i = 0; i < columns; i++)
		{
			if (i == 0)
			{
				container->emplace_back(std::list<std::string>{values[i]});
				continue;
			}
			container->back().emplace_back(values[i]);
		}
		return 0;
	}

	static void Close()
	{
		if (db)
        {
            sqlite3_close(static_cast<sqlite3*>(db));
            db = nullptr;
        }
	}

    // template<typename... TParams>
    // static bool Exec(const std::string& request, std::list<ZC_Tuple<TParams...>>& response)
    // {
    //     if (!db) return false;
        
    //     // if (!sqlite3_mutex_enter(db->mutex)) return false;
        
    //     sqlite3_stmt* stmt = nullptr;
    //     if (sqlite3_prepare_v2(db, request.c_str(), request.size(), &stmt, nullptr) != SQLITE_OK) return false;

    //     bool wasError = false,
    //         makeStep = true;
    //     while (makeStep)
    //     {
    //         switch (sqlite3_step(stmt))
    //         {
    //         case SQLITE_DONE: makeStep = false; break;
    //         case SQLITE_ROW:
    //         {
    //             auto newTuple = response.emplace_back(std::tuple<TParams...>());
    //             auto columns = sqlite3_column_count(stmt);
    //             for (int i = 0; i < columns; ++i)
    //             {
    //                 switch (sqlite3_column_type(stmt, i))
    //                 {
    //                 case SQLITE_INTEGER: std::get<(size_t)i>(newTuple) = sqlite3_column_int(stmt, i); break;
    //                 case SQLITE_FLOAT: std::get<i>(newTuple) = sqlite3_column_double(stmt, i); break;
    //                 case SQLITE_TEXT: std::get<i>(newTuple) = sqlite3_column_text16(stmt, i); break;
    //                 case SQLITE3_TEXT: std::get<i>(newTuple) = sqlite3_column_text16(stmt, i); break;
    //                 case SQLITE_BLOB: std::get<i>(newTuple) = sqlite3_column_text16(stmt, i); break;
    //                 case SQLITE_NULL: std::get<i>(newTuple) = sqlite3_column_text16(stmt, i); break;
    //                 default: assert(false);
    //                 }
    //             }
    //         } break;
    //         default:
    //         {
    //             makeStep = false;
    //             wasError = true;
    //         }
    //         } 
    //     }
    //     sqlite3_finalize(stmt);
            
    //     // sqlite3_mutex_leave(db->mutex);
    // }

private:
    static inline std::string name;
    static inline int flags = 0;
    static inline sqlite3* db;



public:
	// static void Open(const std::string& name = "/home/dmitry/projects/ZCreator/data.db", const bool& defNaming = true) noexcept
	// {
	// 	if (sqlite3_open_v2(name.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL))
	// 	{
	// 		// ErrorLoger::Insert(__FILE__, __LINE__, sqlite3_errmsg(db));
    //         auto q = sqlite3_errmsg(db);
	// 		Close();
	// 		return;
	// 	}
	// 	char* errMsg = 0;
	// 	std::list<std::list<std::string>> response;
	// 	int result = sqlite3_exec(db, "SELECT * FROM t1", Callback, &response, &errMsg);
	// 	if (result != SQLITE_OK)
	// 	{
	// 		sqlite3_free(errMsg);
	// 	}
    // }
        // sqlite3_stmt* stmt = nullptr;
        // sqlite3_prepare_v2(db, "SELECT * FROM lol", -1, &stmt, nullptr);

		// std::list<std::string> glist;
        // bool contin = true;
        // while (contin)
        // {
        //     auto stepRes = sqlite3_step(stmt);
        //     switch (stepRes)
        //     {
        //     case SQLITE_BUSY:
        //         {int q = 4;}
        //         break;
        //     case SQLITE_DONE:
        //         contin = false;
        //         break;
        //     case SQLITE_ERROR:
        //         {int q = 4;}
        //         break;
        //     case SQLITE_MISUSE:
        //         {int q = 4;}
        //         break;
        //     case SQLITE_ROW:
        //     {
        //         auto clms = sqlite3_column_count(stmt);
        //         for (int i = 0; i < clms; i++)
        //         {
        //             auto q = sqlite3_column_text(stmt, i);
        //             glist.emplace_back(reinterpret_cast<const char*>(q));
        //         }
        //     }
        //         break;
        //     default:
        //         {int q = 4;}
        //         break;
        //     } 
        // }
        // sqlite3_finalize(stmt);
        




		// if (!defNaming)
		// {
		// 	EditNames();
		// 	defaultNaming = defNaming;
		// }
		// else
		// {
		// 	tableNames.clear();
		// 	columnNames.clear();
		// }

// 	static std::list<std::list<std::string>> Select(const std::string& sql) noexcept
// 	{
// 		char* errMsg = 0;
// 		std::list<std::list<std::string>> response;
		// int result = sqlite3_exec(db, sql.c_str(), Callback, &response, &errMsg);
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
};
