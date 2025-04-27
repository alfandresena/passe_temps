// dbmanager.h
#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <string>

class DBManager
{
public:
    static DBManager& getInstance();

    bool open(QSqlDatabase& db, const std::string& dbName);
    void close(QSqlDatabase& db);
    bool executeSQL(QSqlDatabase& db, const std::string& sql);
    bool executeQuery(QSqlDatabase& db, const std::string& query);
    bool deleteDatabase(QSqlDatabase& db, const std::string& dbName);
    bool createTable(QSqlDatabase& db, const std::string& tableScript);
    bool dropTable(QSqlDatabase& db, const std::string& tableName);
    bool tableExists(QSqlDatabase& db, const std::string& tableName);

private:
    DBManager(); // Constructeur privé (pattern Singleton)
    ~DBManager();

    // Interdire la copie (pattern Singleton)
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;
};

#endif // DBMANAGER_H
