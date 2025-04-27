// dbmanager_test.cpp
#include <gtest/gtest.h>
#include "dbmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QDir>

class DBManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Créer une nouvelle connexion pour les tests
        db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
        dbName = "test_database.db";

        // Supprimer la base de données si elle existe déjà
        QFile file(dbName.c_str());
        if (file.exists()) {
            file.remove();
        }
    }

    void TearDown() override {
        // Nettoyer après les tests
        if (db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase("test_connection");

        // Supprimer la base de données de test
        QFile file(dbName.c_str());
        if (file.exists()) {
            file.remove();
        }
    }

    QSqlDatabase db;
    std::string dbName;
    DBManager& dbManager = DBManager::getInstance();
};

// Test d'ouverture de base de données
TEST_F(DBManagerTest, OpenDatabase) {
    ASSERT_TRUE(dbManager.open(db, dbName));
    ASSERT_TRUE(db.isOpen());
}

// Test de fermeture de base de données
TEST_F(DBManagerTest, CloseDatabase) {
    ASSERT_TRUE(dbManager.open(db, dbName));
    ASSERT_TRUE(db.isOpen());

    dbManager.close(db);
    ASSERT_FALSE(db.isOpen());
}

// Test d'exécution de SQL
TEST_F(DBManagerTest, ExecuteSQL) {
    ASSERT_TRUE(dbManager.open(db, dbName));

    std::string createSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT)";
    ASSERT_TRUE(dbManager.executeSQL(db, createSQL));

    // Vérifier que la table a été créée
    ASSERT_TRUE(dbManager.tableExists(db, "test_table"));
}

// Test d'exécution de requête
TEST_F(DBManagerTest, ExecuteQuery) {
    ASSERT_TRUE(dbManager.open(db, dbName));

    // Créer une table
    std::string createSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT)";
    ASSERT_TRUE(dbManager.executeSQL(db, createSQL));

    // Insérer des données
    std::string insertSQL = "INSERT INTO test_table (id, name) VALUES (1, 'Test')";
    ASSERT_TRUE(dbManager.executeQuery(db, insertSQL));

    // Vérifier que les données ont été insérées
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM test_table");
    ASSERT_TRUE(query.next());
    ASSERT_EQ(1, query.value(0).toInt());
}

// Test de création de table
TEST_F(DBManagerTest, CreateTable) {
    ASSERT_TRUE(dbManager.open(db, dbName));

    std::string createSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT)";
    ASSERT_TRUE(dbManager.createTable(db, createSQL));

    // Vérifier que la table a été créée
    ASSERT_TRUE(dbManager.tableExists(db, "test_table"));
}

// Test de vérification d'existence de table
TEST_F(DBManagerTest, TableExists) {
    ASSERT_TRUE(dbManager.open(db, dbName));

    // La table ne devrait pas exister au début
    ASSERT_FALSE(dbManager.tableExists(db, "test_table"));

    // Créer la table
    std::string createSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT)";
    ASSERT_TRUE(dbManager.createTable(db, createSQL));

    // La table devrait maintenant exister
    ASSERT_TRUE(dbManager.tableExists(db, "test_table"));
}

// Test de suppression de table
TEST_F(DBManagerTest, DropTable) {
    ASSERT_TRUE(dbManager.open(db, dbName));

    // Créer la table
    std::string createSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT)";
    ASSERT_TRUE(dbManager.createTable(db, createSQL));

    // Vérifier que la table a été créée
    ASSERT_TRUE(dbManager.tableExists(db, "test_table"));

    // Supprimer la table
    ASSERT_TRUE(dbManager.dropTable(db, "test_table"));

    // Vérifier que la table n'existe plus
    ASSERT_FALSE(dbManager.tableExists(db, "test_table"));
}

// Test de suppression de base de données
TEST_F(DBManagerTest, DeleteDatabase) {
    ASSERT_TRUE(dbManager.open(db, dbName));

    // Fermer la base de données avant de la supprimer
    dbManager.close(db);

    // Supprimer la base de données
    ASSERT_TRUE(dbManager.deleteDatabase(db, dbName));

    // Vérifier que le fichier n'existe plus
    QFile file(dbName.c_str());
    ASSERT_FALSE(file.exists());
}
