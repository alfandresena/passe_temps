// dbmanager.cpp
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QDebug>

DBManager::DBManager() {}

DBManager::~DBManager() {}

DBManager& DBManager::getInstance()
{
    static DBManager instance;
    return instance;
}

bool DBManager::open(QSqlDatabase& db, const std::string& dbName)
{
    if (db.isOpen()) {
        // Si la base de données est déjà ouverte, on la ferme d'abord
        db.close();
    }

    db.setDatabaseName(QString::fromStdString(dbName));
    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }
    return true;
}

void DBManager::close(QSqlDatabase& db)
{
    if (db.isOpen()) {
        QString connectionName = db.connectionName();
        db.close();
        // Supprime la connexion du pool de connexions de Qt
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connectionName);
    }
}

bool DBManager::executeSQL(QSqlDatabase& db, const std::string& sql)
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(QString::fromStdString(sql))) {
        qDebug() << "Error executing SQL:" << query.lastError().text();
        qDebug() << "Query was:" << QString::fromStdString(sql);
        return false;
    }
    return true;
}

bool DBManager::executeQuery(QSqlDatabase& db, const std::string& query)
{
    // Dans cette implémentation, executeQuery est similaire à executeSQL
    // mais pourrait être étendu pour gérer la récupération de résultats
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery sqlQuery(db);
    if (!sqlQuery.exec(QString::fromStdString(query))) {
        qDebug() << "Error executing query:" << sqlQuery.lastError().text();
        qDebug() << "Query was:" << QString::fromStdString(query);
        return false;
    }
    return true;
}

bool DBManager::deleteDatabase(QSqlDatabase& db, const std::string& dbName)
{
    // Fermer la base de données si elle est ouverte
    if (db.isOpen() && db.databaseName() == QString::fromStdString(dbName)) {
        close(db);
    }

    // Supprimer le fichier de la base de données (pour SQLite)
    QFile dbFile(QString::fromStdString(dbName));
    if (dbFile.exists()) {
        if (!dbFile.remove()) {
            qDebug() << "Failed to delete database file:" << dbFile.errorString();
            return false;
        }
    } else {
        qDebug() << "Database file does not exist:" << QString::fromStdString(dbName);
        return false;
    }

    return true;
}

bool DBManager::createTable(QSqlDatabase& db, const std::string& tableScript)
{
    // Vérifier que la DB est ouverte
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    // Exécuter le script de création de table
    return executeSQL(db, tableScript);
}

bool DBManager::dropTable(QSqlDatabase& db, const std::string& tableName)
{
    // Vérifier que la DB est ouverte
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    // Vérifier d'abord si la table existe
    if (!tableExists(db, tableName)) {
        qDebug() << "Table does not exist:" << QString::fromStdString(tableName);
        return false;
    }

    // Créer et exécuter la requête DROP TABLE
    std::string dropSQL = "DROP TABLE " + tableName;
    return executeSQL(db, dropSQL);
}

bool DBManager::tableExists(QSqlDatabase& db, const std::string& tableName)
{
    // Vérifier que la DB est ouverte
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    // Récupérer la liste des tables de la base de données
    QStringList tables = db.tables();
    return tables.contains(QString::fromStdString(tableName));
}
