#include <iostream>
#include <memory>
#include <string>

#include <sqlite3.h>
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
  (void)unused;
  printf("There are %d column(s)\n", count);
  for (int idx = 0; idx < count; idx++) {
    printf("The data in column \"%s\" is: %s\n", columns[idx], data[idx]);
  }
  printf("\n");
  return 0;
}

// RAII handle so sqlite3* is always closed, even on early return.
struct Sqlite3Deleter {
  void operator()(sqlite3 *db) const { sqlite3_close(db); }
};
using Sqlite3Ptr = std::unique_ptr<sqlite3, Sqlite3Deleter>;

void SQLiteTest()
{
  sqlite3 *raw_db = nullptr;
  if (sqlite3_open("my-db.db", &raw_db) != SQLITE_OK) {
    std::cerr << "SQLiteTest: failed to open database: "
              << sqlite3_errmsg(raw_db) << "\n";
    sqlite3_close(raw_db);
    return;
  }
  Sqlite3Ptr db(raw_db);

  const char *create_sql =
      "CREATE TABLE IF NOT EXISTS COMPANY("
      "ID INT PRIMARY KEY NOT NULL,"
      "NAME TEXT NOT NULL,"
      "AGE INT NOT NULL,"
      "ADDRESS CHAR(50),"
      "SALARY REAL);";

  char *err = nullptr;
  int rc = sqlite3_exec(db.get(), create_sql, nullptr, nullptr, &err);
  if (rc != SQLITE_OK) {
    std::cerr << "SQLiteTest: CREATE TABLE failed: " << err << "\n";
    sqlite3_free(err);
    return;
  }

  err = nullptr;
  rc = sqlite3_exec(db.get(), "SELECT * FROM COMPANY", my_special_callback, nullptr, &err);
  if (rc != SQLITE_OK) {
    std::cerr << "SQLiteTest: SELECT failed: " << err << " (rc=" << rc << ")\n";
    sqlite3_free(err);
    return;
  }

  std::cout << "SQLiteTest: SELECT executed successfully\n";
}