#include <QCoreApplication>

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     // Set up code that uses the Qt event loop here.
//     // Call a.quit() or a.exit() to quit the application.
//     // A not very useful example would be including
//     // #include <QTimer>
//     // near the top of the file and calling
//     // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
//     // which quits the application after 5 seconds.

//     // If you do not need a running Qt event loop, remove the call
//     // to a.exec() or use the Non-Qt Plain C++ Application template.

//     return a.exec();
// }

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);

    // Créer la connexion à la base de données SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test_connection");
    std::string dbName = "test_database.db";

    // Obtenir l'instance du DBManager
    DBManager& dbManager = DBManager::getInstance();

    // Ouvrir la base de données
    if (dbManager.open(db, dbName)) {
        qDebug() << "Database opened successfully";

        // 1. Créer une table si elle n'existe pas déjà
        std::string createTableSQL = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)";
        if (dbManager.createTable(db, createTableSQL)) {
            qDebug() << "Table 'users' created or already exists";
        } else {
            qDebug() << "Failed to create table 'users'";
        }

        // 2. Insérer des données dans la table
        std::string insertSQL = "INSERT INTO users (name) VALUES ('John Doe')";
        if (dbManager.executeSQL(db, insertSQL)) {
            qDebug() << "Data inserted successfully";
        } else {
            qDebug() << "Failed to insert data";
        }

        // 3. Vérifier l'existence d'une table
        if (dbManager.tableExists(db, "users")) {
            qDebug() << "Table 'users' exists in the database";
        } else {
            qDebug() << "Table 'users' does not exist";
        }

        // 4. Sélectionner les données
        QSqlQuery query(db);
        query.exec("SELECT * FROM users");

        // Afficher les résultats de la sélection
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            qDebug() << "User ID:" << id << "Name:" << name;
        }

        // 5. Mettre à jour des données
        std::string updateSQL = "UPDATE users SET name = 'Jane Doe' WHERE name = 'John Doe'";
        if (dbManager.executeSQL(db, updateSQL)) {
            qDebug() << "Data updated successfully";
        } else {
            qDebug() << "Failed to update data";
        }

        // 6. Supprimer des données
        std::string deleteSQL = "DELETE FROM users WHERE name = 'Jane Doe'";
        if (dbManager.executeSQL(db, deleteSQL)) {
            qDebug() << "Data deleted successfully";
        } else {
            qDebug() << "Failed to delete data";
        }

        // 7. Supprimer la table
        if (dbManager.dropTable(db, "users")) {
            qDebug() << "Table 'users' dropped successfully";
        } else {
            qDebug() << "Failed to drop table 'users'";
        }

        // Fermer la connexion à la base de données
        dbManager.close(db);
        qDebug() << "Database connection closed";
    } else {
        qDebug() << "Failed to open the database";
    }

    return 0;
}
