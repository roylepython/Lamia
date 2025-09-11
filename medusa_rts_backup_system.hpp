#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <nlohmann/json.hpp>

// P001-P019 Library Integration
#ifdef P001_AI_COMMAND_SYSTEM_ENABLED
#include "libai_command_system.hpp"
#endif

#ifdef P002_AI_COMMAND_V2_ENABLED
#include "libai_command_v2.hpp"
#endif

#ifdef P003_CLAUDE_CLI_ENABLED
#include "libclaude_cli.hpp"
#endif

#ifdef P004_MEDUSA_GOLD_STANDARD_ENABLED
#include "libmedusa_gold_standard.hpp"
#endif

#ifdef P005_UNIVERSAL_DB_CONNECTOR_ENABLED
#include "libuniversal_db_connector.hpp"
#endif

#ifdef P006_CURSOR_INTEGRATION_ENABLED
#include "libcursor_integration.hpp"
#endif

#ifdef P007_AI_CERT_DEPLOYMENT_ENABLED
#include "libai_cert_deployment.hpp"
#endif

#ifdef P008_AI_PORTAL_CERT_ENABLED
#include "libai_portal_cert.hpp"
#endif

#ifdef P009_GOLD_STANDARD_PROCESSOR_ENABLED
#include "libgold_standard_processor.hpp"
#endif

#ifdef P011_DOCUMENT_FILTERING_DAEMON_ENABLED
#include "libdocument_filtering_daemon.hpp"
#endif

#ifdef P012_GRAPHQL_TOURNAMENT_ENGINE_ENABLED
#include "libgraphql_tournament_engine.hpp"
#endif

#ifdef P014_DOCUMENT_PROCESSOR_ENABLED
#include "libdocument_processor.hpp"
#endif

#ifdef P015_MODULAR_CORE_ENGINE_ENABLED
#include "libmodular_core_engine.hpp"
#endif

#ifdef P016_ANTI_GUMPH_FILTER_ENABLED
#include "libanti_gumph_filter.hpp"
#endif

#ifdef P017_RAG_KNOWLEDGE_ENGINE_ENABLED
#include "librag_knowledge_engine.hpp"
#endif

#ifdef P018_QUALITY_ANALYZER_ENABLED
#include "libquality_analyzer.hpp"
#endif

#ifdef P019_MEDUSA_ENCRYPTION_ENABLED
#include "libmedusa_encryption.hpp"
#endif

namespace MedusaServ {
namespace Database {

// Forward declarations for P001-P019 libraries
#ifdef P001_AI_COMMAND_SYSTEM_ENABLED
class AICommandSystem;
#endif

#ifdef P002_AI_COMMAND_V2_ENABLED
class AICommandV2;
#endif

#ifdef P003_CLAUDE_CLI_ENABLED
class ClaudeCLI;
#endif

#ifdef P004_MEDUSA_GOLD_STANDARD_ENABLED
class MedusaGoldStandard;
#endif

#ifdef P005_UNIVERSAL_DB_CONNECTOR_ENABLED
class UniversalDBConnector;
#endif

#ifdef P006_CURSOR_INTEGRATION_ENABLED
class CursorIntegration;
#endif

#ifdef P007_AI_CERT_DEPLOYMENT_ENABLED
class AICertificateDeployment;
#endif

#ifdef P008_AI_PORTAL_CERT_ENABLED
class AIPortalCertificate;
#endif

#ifdef P009_GOLD_STANDARD_PROCESSOR_ENABLED
class GoldStandardProcessor;
#endif

#ifdef P011_DOCUMENT_FILTERING_DAEMON_ENABLED
class DocumentFilteringDaemon;
#endif

#ifdef P012_GRAPHQL_TOURNAMENT_ENGINE_ENABLED
class GraphQLTournamentEngine;
#endif

#ifdef P014_DOCUMENT_PROCESSOR_ENABLED
class DocumentProcessor;
#endif

#ifdef P015_MODULAR_CORE_ENGINE_ENABLED
class ModularCoreEngine;
#endif

#ifdef P016_ANTI_GUMPH_FILTER_ENABLED
class AntiGumphFilter;
#endif

#ifdef P017_RAG_KNOWLEDGE_ENGINE_ENABLED
class RAGKnowledgeEngine;
#endif

#ifdef P018_QUALITY_ANALYZER_ENABLED
class QualityAnalyzer;
#endif

#ifdef P019_MEDUSA_ENCRYPTION_ENABLED
class MedusaEncryption;
#endif

// Database table information structure
struct TableInfo {
    std::string name;
    std::string schema;
    std::string data;
    std::string indexes;
    std::string constraints;
    std::string foreign_keys;
    int64_t row_count;
    size_t size_bytes;
    std::chrono::system_clock::time_point last_modified;
    bool is_accessible;
    std::string error_message;
    
    nlohmann::json toJson() const;
    static TableInfo fromJson(const nlohmann::json& j);
};

// Backup configuration
struct BackupConfig {
    std::string host = "172.236.23.54";
    int port = 5432;
    std::string database = "medusa_rts";
    std::string username = "medusa";
    std::string password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    std::string output_directory = "./database/backup_tables";
    bool include_schema = true;
    bool include_data = true;
    bool include_indexes = true;
    bool include_constraints = true;
    bool include_foreign_keys = true;
    bool preserve_order = true;
    bool validate_data = true;
    bool encrypt_backup = false;
    std::string encryption_key;
    int max_concurrent_downloads = 5;
    int64_t max_table_size_bytes = 1024 * 1024 * 1024; // 1GB
    std::chrono::seconds timeout = std::chrono::seconds(300);
    
    nlohmann::json toJson() const;
    static BackupConfig fromJson(const nlohmann::json& j);
};

// Performance metrics
struct BackupMetrics {
    std::atomic<int64_t> tables_processed{0};
    std::atomic<int64_t> tables_successful{0};
    std::atomic<int64_t> tables_failed{0};
    std::atomic<int64_t> total_rows_downloaded{0};
    std::atomic<int64_t> total_bytes_downloaded{0};
    std::atomic<int64_t> total_download_time_ms{0};
    std::atomic<int64_t> total_processing_time_ms{0};
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    
    double getSuccessRate() const;
    double getAverageDownloadSpeedMBps() const;
    double getAverageProcessingSpeedMBps() const;
    std::chrono::milliseconds getTotalDuration() const;
    
    nlohmann::json toJson() const;
};

// Backup progress callback
using BackupProgressCallback = std::function<void(const std::string& table_name, 
                                                  int progress_percent, 
                                                  const std::string& status)>;

// Main backup system class
class MedusaRTSBackupSystem {
public:
    MedusaRTSBackupSystem();
    ~MedusaRTSBackupSystem();
    
    // Initialization
    bool initialize(const BackupConfig& config);
    bool shutdown();
    bool isInitialized() const;
    
    // Core backup functionality
    bool backupAllTables(const BackupProgressCallback& progress_callback = nullptr);
    bool backupTable(const std::string& table_name, const BackupProgressCallback& progress_callback = nullptr);
    bool backupTables(const std::vector<std::string>& table_names, const BackupProgressCallback& progress_callback = nullptr);
    
    // Analysis and discovery
    std::vector<TableInfo> discoverTables();
    std::vector<std::string> getAccessibleTables();
    std::vector<std::string> getTableDependencies();
    TableInfo analyzeTable(const std::string& table_name);
    
    // Validation and verification
    bool validateBackup(const std::string& backup_directory);
    bool verifyTableIntegrity(const std::string& table_name);
    bool compareBackups(const std::string& backup1, const std::string& backup2);
    
    // Restoration
    bool restoreBackup(const std::string& backup_directory, const std::string& target_database);
    bool restoreTable(const std::string& table_name, const std::string& backup_directory, const std::string& target_database);
    
    // Configuration and management
    void setConfig(const BackupConfig& config);
    BackupConfig getConfig() const;
    BackupMetrics getMetrics() const;
    void resetMetrics();
    
    // P001-P019 Enhanced methods
    bool backupWithAICommand(const std::string& table_name, const nlohmann::json& ai_config);
    bool backupWithGoldStandard(const std::string& table_name, const nlohmann::json& gold_standard_config);
    bool backupWithUniversalDB(const std::string& table_name, const nlohmann::json& db_config);
    bool backupWithDocumentProcessor(const std::string& table_name, const nlohmann::json& processor_config);
    bool backupWithAntiGumphFilter(const std::string& table_name, const nlohmann::json& filter_config);
    bool backupWithRAGKnowledge(const std::string& table_name, const nlohmann::json& rag_config);
    bool backupWithQualityAnalysis(const std::string& table_name, const nlohmann::json& quality_config);
    bool backupWithEncryption(const std::string& table_name, const nlohmann::json& encryption_config);
    
    // Utility methods
    std::string generateBackupReport() const;
    bool exportBackupReport(const std::string& filename) const;
    std::vector<std::string> getBackupHistory() const;
    bool cleanupOldBackups(int days_to_keep);

private:
    // Core components
    BackupConfig config_;
    BackupMetrics metrics_;
    std::atomic<bool> is_initialized_{false};
    std::atomic<bool> shutdown_requested_{false};
    mutable std::shared_mutex config_mutex_;
    mutable std::shared_mutex metrics_mutex_;
    
    // Database connection
    PGconn* conn_{nullptr};
    
    // P001-P019 Enhanced components
#ifdef P001_AI_COMMAND_SYSTEM_ENABLED
    std::unique_ptr<AICommandSystem> ai_command_system_;
#endif

#ifdef P002_AI_COMMAND_V2_ENABLED
    std::unique_ptr<AICommandV2> ai_command_v2_;
#endif

#ifdef P003_CLAUDE_CLI_ENABLED
    std::unique_ptr<ClaudeCLI> claude_cli_;
#endif

#ifdef P004_MEDUSA_GOLD_STANDARD_ENABLED
    std::unique_ptr<MedusaGoldStandard> gold_standard_;
#endif

#ifdef P005_UNIVERSAL_DB_CONNECTOR_ENABLED
    std::unique_ptr<UniversalDBConnector> universal_db_connector_;
#endif

#ifdef P006_CURSOR_INTEGRATION_ENABLED
    std::unique_ptr<CursorIntegration> cursor_integration_;
#endif

#ifdef P007_AI_CERT_DEPLOYMENT_ENABLED
    std::unique_ptr<AICertificateDeployment> ai_cert_deployment_;
#endif

#ifdef P008_AI_PORTAL_CERT_ENABLED
    std::unique_ptr<AIPortalCertificate> ai_portal_cert_;
#endif

#ifdef P009_GOLD_STANDARD_PROCESSOR_ENABLED
    std::unique_ptr<GoldStandardProcessor> gold_standard_processor_;
#endif

#ifdef P011_DOCUMENT_FILTERING_DAEMON_ENABLED
    std::unique_ptr<DocumentFilteringDaemon> document_filtering_daemon_;
#endif

#ifdef P012_GRAPHQL_TOURNAMENT_ENGINE_ENABLED
    std::unique_ptr<GraphQLTournamentEngine> tournament_engine_;
#endif

#ifdef P014_DOCUMENT_PROCESSOR_ENABLED
    std::unique_ptr<DocumentProcessor> document_processor_;
#endif

#ifdef P015_MODULAR_CORE_ENGINE_ENABLED
    std::unique_ptr<ModularCoreEngine> modular_core_engine_;
#endif

#ifdef P016_ANTI_GUMPH_FILTER_ENABLED
    std::unique_ptr<AntiGumphFilter> anti_gumph_filter_;
#endif

#ifdef P017_RAG_KNOWLEDGE_ENGINE_ENABLED
    std::unique_ptr<RAGKnowledgeEngine> rag_knowledge_engine_;
#endif

#ifdef P018_QUALITY_ANALYZER_ENABLED
    std::unique_ptr<QualityAnalyzer> quality_analyzer_;
#endif

#ifdef P019_MEDUSA_ENCRYPTION_ENABLED
    std::unique_ptr<MedusaEncryption> medusa_encryption_;
#endif

    // Private helper methods
    bool initializeP001P019Libraries();
    bool connectToDatabase();
    bool disconnectFromDatabase();
    bool testDatabaseConnection();
    
    std::string downloadTableSchema(const std::string& table_name);
    std::string downloadTableData(const std::string& table_name);
    std::string downloadTableIndexes(const std::string& table_name);
    std::string downloadTableConstraints(const std::string& table_name);
    std::string downloadTableForeignKeys(const std::string& table_name);
    
    bool saveTableBackup(const std::string& table_name, const TableInfo& table_info);
    bool validateTableBackup(const std::string& table_name, const TableInfo& table_info);
    
    std::vector<std::string> getTableDependenciesInternal(const std::string& table_name);
    std::vector<std::string> sortTablesByDependencies(const std::vector<std::string>& tables);
    
    bool executeWithTimeout(const std::function<bool()>& operation, std::chrono::seconds timeout);
    std::string sanitizeTableName(const std::string& table_name);
    std::string generateBackupId() const;
    
    // P001-P019 specific helper methods
    bool initializeAICommandSystem();
    bool initializeGoldStandard();
    bool initializeUniversalDBConnector();
    bool initializeDocumentProcessor();
    bool initializeAntiGumphFilter();
    bool initializeRAGKnowledgeEngine();
    bool initializeQualityAnalyzer();
    bool initializeEncryption();
    
    nlohmann::json getEnhancedMetrics() const;
    void updateMetrics(const std::string& table_name, int64_t rows, int64_t bytes, int64_t time_ms);
};

// Utility functions
namespace Utils {
    std::string formatBytes(int64_t bytes);
    std::string formatDuration(std::chrono::milliseconds duration);
    std::string getCurrentTimestamp();
    bool createDirectory(const std::string& path);
    bool fileExists(const std::string& path);
    int64_t getFileSize(const std::string& path);
    std::string calculateFileHash(const std::string& path);
    bool validateJsonSchema(const nlohmann::json& data, const nlohmann::json& schema);
}

} // namespace Database
} // namespace MedusaServ 