#include <iostream>
#include <string>
#include "sqlite.hh"

/*
 * Arguments:
 *
 *   unused - Ignored in this case, see the documentation for sqlite3_exec
 *    count - The number of columns in the result set
 *     data - The row's data
 *  columns - The column names
 */
static int my_special_callback(void *unused, int count, char **data, char **columns)
{
  int idx;

  printf("There are %d column(s)\n", count);

  for (idx = 0; idx < count; idx++) {
      printf("The data in column \"%s\" is: %s\n", columns[idx], data[idx]);
  }

  printf("\n");

  return 0;
}

void SQLiteTest()
{
  char* err;
  sqlite3* db;
  sqlite3_stmt* stmt;
  sqlite3_open("my-db.db", &db);
  char* sql_exec_str;
  sql_exec_str = "CREATE TABLE IF NOT EXISTS COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AGE            INT     NOT NULL," \
      "ADDRESS        CHAR(50)," \
      "SALARY         REAL );";
  std::string insert_str = "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) VALUES (2, 'Nam', 26, '446', 1);";
  auto sql_rc = sqlite3_exec(db, "SELECT * FROM COMPANY", my_special_callback, NULL, &err);
  if (sql_rc != SQLITE_OK)
  {
    std::cout << "SQLite execution error " << err << " with rc: " << sql_rc << std::endl;
  }
  else
  {
    std::cout << "SQLite execution successful with rc: " << sql_rc << std::endl;
  }
  sqlite3_close(db);
}