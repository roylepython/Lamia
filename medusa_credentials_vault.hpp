/*
 * MEDUSA PRODUCTION CREDENTIALS VAULT
 * AES-256-GCM encrypted storage for all production server credentials
 * Yorkshire Champion Security Implementation
 */

#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>

namespace MedusaServer {

struct ServerCredentials {
    std::string server_name;
    std::string ip_address;
    std::string username;
    std::string password;
    int port = 22;
    std::string purpose;
    std::map<std::string, std::string> additional_info;
};

struct DatabaseCredentials {
    std::string db_name;
    std::string host;
    int port = 5432;
    std::string username;
    std::string password;
    std::string connection_string;
};

struct NASCredentials {
    std::string nas_ip;
    std::string username;
    std::string password;
    std::string shared_folder;
    std::string protocol; // "SMB", "NFS", "FTP"
};

class CredentialsVault {
private:
    std::string master_key_;
    std::string vault_file_path_;
    std::map<std::string, ServerCredentials> servers_;
    std::map<std::string, DatabaseCredentials> databases_;
    std::map<std::string, NASCredentials> nas_systems_;
    bool initialized_;

public:
    CredentialsVault(const std::string& vault_path = "/home/medusa/vault/credentials.enc");
    ~CredentialsVault();

    // Initialization
    bool initialize(const std::string& master_password);
    bool createVault(const std::string& master_password);
    bool loadVault();
    bool saveVault();

    // Server credential management
    bool addServerCredentials(const std::string& key, const ServerCredentials& creds);
    bool getServerCredentials(const std::string& key, ServerCredentials& creds);
    bool removeServerCredentials(const std::string& key);
    std::vector<std::string> listServers();

    // Database credential management
    bool addDatabaseCredentials(const std::string& key, const DatabaseCredentials& creds);
    bool getDatabaseCredentials(const std::string& key, DatabaseCredentials& creds);
    std::vector<std::string> listDatabases();

    // NAS credential management
    bool addNASCredentials(const std::string& key, const NASCredentials& creds);
    bool getNASCredentials(const std::string& key, NASCredentials& creds);
    std::vector<std::string> listNAS();

    // Quick access methods for production servers
    ServerCredentials getBackendServer();
    ServerCredentials getFrontendServer();
    ServerCredentials getDatabaseServer();
    ServerCredentials getMedlabServer();
    DatabaseCredentials getProductionDatabase();
    NASCredentials getBackupNAS();

private:
    // Encryption/Decryption
    std::string encryptData(const std::string& plaintext);
    std::string decryptData(const std::string& ciphertext);
    bool generateSalt(unsigned char* salt, int length);
    bool deriveKey(const std::string& password, const unsigned char* salt, unsigned char* key);
    
    // Serialization
    std::string serializeCredentials();
    bool deserializeCredentials(const std::string& data);
    std::string credentialsToJson(const ServerCredentials& creds);
    bool jsonToCredentials(const std::string& json, ServerCredentials& creds);
};

// Production credentials initialization
inline void initializeProductionCredentials(CredentialsVault& vault) {
    // Backend Server (172.236.28.155)
    ServerCredentials backend;
    backend.server_name = "medusa-backend";
    backend.ip_address = "172.236.28.155";
    backend.username = "root";
    backend.password = "7303nmU8Q29WjXpPlumsBAXyJjMt";
    backend.port = 22;
    backend.purpose = "Application backend, file storage";
    backend.additional_info["medusa_user"] = "medusa";
    backend.additional_info["medusa_password"] = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    vault.addServerCredentials("backend", backend);

    // Frontend Server (172.236.20.28)
    ServerCredentials frontend;
    frontend.server_name = "medusa-frontend";
    frontend.ip_address = "172.236.20.28";
    frontend.username = "root";
    frontend.password = "FBvWxPuPAWoIM6LPyJ621sYVaE4k";
    frontend.port = 22;
    frontend.purpose = "Front end website";
    frontend.additional_info["medusa_user"] = "medusa";
    frontend.additional_info["medusa_password"] = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    vault.addServerCredentials("frontend", frontend);

    // Database Server (172.236.23.54)
    ServerCredentials database_server;
    database_server.server_name = "medusa-database";
    database_server.ip_address = "172.236.23.54";
    database_server.username = "root";
    database_server.password = "aAB9drHKjqfRORhGnAvpEbVJpo31";
    database_server.port = 22;
    database_server.purpose = "PostgreSQL database (medusa, medusa_rts)";
    database_server.additional_info["medusa_user"] = "medusa";
    database_server.additional_info["medusa_password"] = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    vault.addServerCredentials("database_server", database_server);

    // Medlab Server (172.237.111.158)
    ServerCredentials medlab;
    medlab.server_name = "medlab-developersx1";
    medlab.ip_address = "172.237.111.158";
    medlab.username = "root";
    medlab.password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    medlab.port = 22;
    medlab.purpose = "Development and testing server";
    vault.addServerCredentials("medlab", medlab);

    // Production Database
    DatabaseCredentials prod_db;
    prod_db.db_name = "medusa_rts";
    prod_db.host = "172.236.23.54";
    prod_db.port = 5432;
    prod_db.username = "medusa";
    prod_db.password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    prod_db.connection_string = "postgresql://medusa:izJaRuA2kwbNwezvKsCzo7DUNnQc@172.236.23.54:5432/medusa_rts";
    vault.addDatabaseCredentials("production", prod_db);

    // Backup NAS (192.168.1.168)
    NASCredentials nas;
    nas.nas_ip = "192.168.1.168";
    nas.username = "medusa_backup";
    nas.password = "BackupSecure2025!";
    nas.shared_folder = "/medusa_backups";
    nas.protocol = "SMB";
    vault.addNASCredentials("backup_nas", nas);
}

} // namespace MedusaServer