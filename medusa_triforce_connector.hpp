#pragma once

/**
 * MEDUSA TRIFORCE UNIVERSAL DATABASE CONNECTOR v0.2.7
 * ===================================================
 * 
 * Foundation database layer - MUST BE ACTIVE before any other components!
 * 
 * Yorkshire Champion Gold Standard - Ground Up Implementation
 * Combines PostgreSQL + Neo4j + MongoDB into single .so library
 * 
 * Author: Claude Code (Yorkshire Champion) 
 * Version: 0.3.0 (Enhanced with Python v3.0 Features)
 * Purpose: Complete TRIFORCE system with Enigma Scale & Intelligent Routing
 * 
 * ENHANCEMENTS FROM PYTHON v3.0:
 * - Medusa Enigma Scale integration (0.315 universal constant)
 * - Intelligent query routing with operation type hints
 * - Cross-database synchronization capabilities
 * - Texture batch processing with AI coordination
 * - Advanced Yorkshire Champion error handling
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <future>
#include <queue>
#include <condition_variable>
#include <algorithm>
#include <numeric>
#include <cmath>

// Database driver includes
#ifdef LIBPQ_AVAILABLE
#include <libpq-fe.h>  // PostgreSQL
#endif
// Neo4j C++ driver would need to be linked
// MongoDB C++ driver would need to be linked

// Security includes
#ifdef OPENSSL_AVAILABLE
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/aes.h>
#endif

namespace medusa::database {

    /**
     * Database Types for Triforce System
     */
    enum class DatabaseType {
        POSTGRESQL,     // Structured relational data
        NEO4J,         // Graph relationships and patterns
        MONGODB        // Document storage and flexibility
    };

    /**
     * Query Type for Intelligent Routing (Python v3.0 Feature)
     */
    enum class QueryType {
        GRAPH_RELATIONSHIP,    // Neo4j
        DOCUMENT_STORAGE,      // MongoDB
        RELATIONAL_DATA,       // PostgreSQL
        TEXTURE_PROCESSING,    // PostgreSQL + Neo4j
        THREAT_ANALYSIS,       // Neo4j + PostgreSQL
        AUDIT_LOGGING,         // MongoDB + PostgreSQL
        ENIGMA_SCALE_CALC,     // PostgreSQL with special processing
        CROSS_DB_SYNC          // All databases
    };

    /**
     * Connection Status and Health
     */
    enum class ConnectionStatus {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        ERROR,
        RECONNECTING
    };

    /**
     * Texture Profile Structure (Python v3.0 Feature)
     */
    struct TextureProfile {
        std::string name;
        std::string id;
        double complexity = 1.0;
        double porosity = 0.5;
        double anisotropy = 0.3;
        double enigma_score = 0.0;
        std::unordered_map<std::string, std::string> metadata;
        std::chrono::system_clock::time_point processed_at;
        
        // Storage IDs from different databases
        std::optional<std::string> postgres_id;
        std::optional<std::string> mongo_id;
        bool neo4j_created = false;
    };

    /**
     * Medusa Enigma Scale - The Universal Constant System (Python v3.0 Feature)
     * "Champion texture scaling with the magic 0.315 constant!"
     */
    class MedusaEnigmaScale {
    public:
        static constexpr double UNIVERSAL_CONSTANT = 0.315;
        static const std::vector<int> SNAKE_SCALES;
        
        static double calculate_texture_score(const TextureProfile& texture_profile);
        static double apply_snake_scaling(double base_score);
        static std::string get_yorkshire_verdict(double score);
        
    private:
        static double calculate_base_score(const TextureProfile& profile);
    };

    /**
     * Query Result Structure (Enhanced)
     */
    struct QueryResult {
        bool success;
        std::string error_message;
        std::vector<std::unordered_map<std::string, std::string>> rows;
        size_t affected_rows;
        std::chrono::milliseconds execution_time;
        DatabaseType database_used;
        
        // Enhanced Medusa-specific metadata
        double enigma_score = 0.0;
        std::string yorkshire_status;
        std::chrono::system_clock::time_point timestamp;
        QueryType query_type = QueryType::RELATIONAL_DATA;
        
        // Cross-database sync metadata
        bool sync_triggered = false;
        std::vector<std::string> sync_operations;
    };

    /**
     * Database Configuration
     */
    struct DatabaseConfig {
        // PostgreSQL Configuration
#ifdef LIBPQ_AVAILABLE
        struct PostgreSQLConfig {
            std::string host = "172.236.23.54";
            uint16_t port = 5432;
            std::string username = "medusa";
            std::string password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
            std::string database = "medusa_rts";
            std::string sslmode = "require";
            uint32_t connection_timeout = 30;
            uint32_t max_connections = 20;
        } postgresql;
#endif

        // Neo4j Configuration  
        struct Neo4jConfig {
            std::string uri = "neo4j://172.236.23.54:7687";
            std::string username = "medusa";
            std::string password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
            std::string database = "neo4j";
            uint32_t connection_timeout = 30;
            uint32_t max_connections = 10;
        } neo4j;

        // MongoDB Configuration
        struct MongoDBConfig {
            std::string host = "172.236.23.54";
            uint16_t port = 27017;
            std::string username = "medusa";
            std::string password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
            std::string database = "medusa_rts";
            std::string auth_source = "admin";
            uint32_t connection_timeout = 30;
            uint32_t max_connections = 15;
        } mongodb;

        // Global settings
        bool enable_ssl = true;
        bool enable_connection_pooling = true;
        std::chrono::seconds health_check_interval{30};
        std::chrono::seconds connection_retry_delay{5};
        uint32_t max_retry_attempts = 3;
    };

    /**
     * Intelligent Query Router (Python v3.0 Feature)
     * "Champion intelligence for optimal database routing!"
     */
    class IntelligentQueryRouter {
    private:
        std::unordered_map<std::string, DatabaseType> routing_rules_;
        std::unordered_map<std::string, QueryType> operation_mapping_;
        
    public:
        IntelligentQueryRouter();
        
        DatabaseType route_query(const std::string& query, 
                               const std::string& operation_type = "") const;
        QueryType determine_query_type(const std::string& query,
                                     const std::string& operation_type = "") const;
        
        void update_routing_intelligence();
        std::string get_routing_explanation(const std::string& query) const;
        
    private:
        void initialize_routing_rules();
        void initialize_operation_mapping();
        bool requires_cross_database_operation(QueryType query_type) const;
    };

    /**
     * Cross-Database Synchronizer (Python v3.0 Feature)
     * "Champion consistency across all three databases!"
     */
    class CrossDBSynchronizer {
    private:
        bool sync_enabled_;
        std::thread sync_worker_thread_;
        std::atomic<bool> sync_worker_active_;
        std::queue<std::pair<QueryResult, QueryType>> sync_queue_;
        std::mutex sync_queue_mutex_;
        std::condition_variable sync_condition_;
        
    public:
        CrossDBSynchronizer(bool enable_sync = true);
        ~CrossDBSynchronizer();
        
        void trigger_sync(const QueryResult& result, QueryType operation_type);
        void enable_sync(bool enabled) { sync_enabled_ = enabled; }
        bool is_sync_enabled() const { return sync_enabled_; }
        
        size_t get_pending_sync_count() const;
        void wait_for_sync_completion();
        
    private:
        void sync_worker();
        void perform_texture_sync(const QueryResult& result);
        void perform_threat_sync(const QueryResult& result);
        void perform_audit_sync(const QueryResult& result);
    };

    /**
     * Connection Pool Management
     */
    template<typename ConnectionType>
    class ConnectionPool {
    private:
        std::vector<std::unique_ptr<ConnectionType>> available_connections_;
        std::vector<std::unique_ptr<ConnectionType>> active_connections_;
        std::mutex pool_mutex_;
        size_t max_connections_;
        std::atomic<size_t> current_connections_;

    public:
        ConnectionPool(size_t max_connections) 
            : max_connections_(max_connections), current_connections_(0) {}

        std::unique_ptr<ConnectionType> acquire_connection();
        void release_connection(std::unique_ptr<ConnectionType> connection);
        size_t get_active_count() const { return current_connections_.load(); }
        size_t get_max_connections() const { return max_connections_; }
        
        void cleanup_idle_connections();
    };

    /**
     * PostgreSQL Connection Wrapper
     */
#ifdef LIBPQ_AVAILABLE
    class PostgreSQLConnection {
    private:
        PGconn* connection_;
        std::string connection_string_;
        ConnectionStatus status_;
        std::chrono::system_clock::time_point last_activity_;
        std::atomic<bool> is_transaction_active_;

    public:
        PostgreSQLConnection(const DatabaseConfig::PostgreSQLConfig& config);
        ~PostgreSQLConnection();

        bool connect();
        void disconnect();
        bool is_connected() const;
        bool reconnect();
        
        QueryResult execute_query(const std::string& query, 
                                const std::vector<std::string>& params = {});
        bool begin_transaction();
        bool commit_transaction();
        bool rollback_transaction();
        
        ConnectionStatus get_status() const { return status_; }
        void update_last_activity() { last_activity_ = std::chrono::system_clock::now(); }
        
    private:
        void update_status();
        std::string escape_string(const std::string& input);
    };
#endif // LIBPQ_AVAILABLE

    /**
     * Neo4j Connection Wrapper (Placeholder for C++ driver)
     */
    class Neo4jConnection {
    private:
        void* driver_;  // neo4j::Driver* when neo4j C++ driver is linked
        void* session_; // neo4j::Session* 
        ConnectionStatus status_;
        std::string uri_;
        std::string username_;
        std::string password_;

    public:
        Neo4jConnection(const DatabaseConfig::Neo4jConfig& config);
        ~Neo4jConnection();

        bool connect();
        void disconnect();
        bool is_connected() const;
        bool reconnect();
        
        QueryResult execute_cypher(const std::string& cypher_query,
                                  const std::unordered_map<std::string, std::string>& params = {});
        
        ConnectionStatus get_status() const { return status_; }
        
    private:
        void update_status();
    };

    /**
     * MongoDB Connection Wrapper (Placeholder for C++ driver)
     */
    class MongoDBConnection {
    private:
        void* client_;     // mongocxx::client* 
        void* database_;   // mongocxx::database*
        ConnectionStatus status_;
        std::string connection_string_;

    public:
        MongoDBConnection(const DatabaseConfig::MongoDBConfig& config);
        ~MongoDBConnection();

        bool connect();
        void disconnect(); 
        bool is_connected() const;
        bool reconnect();
        
        QueryResult execute_operation(const std::string& collection,
                                    const std::string& operation,
                                    const std::string& document_json);
        
        ConnectionStatus get_status() const { return status_; }
        
    private:
        void update_status();
        std::string build_connection_string(const DatabaseConfig::MongoDBConfig& config);
    };

    /**
     * Main Triforce Connector - THE FOUNDATION
     */
    class TriforceConnector {
    private:
        DatabaseConfig config_;
        
        // Connection pools
#ifdef LIBPQ_AVAILABLE
        std::unique_ptr<ConnectionPool<PostgreSQLConnection>> postgresql_pool_;
#endif
        std::unique_ptr<ConnectionPool<Neo4jConnection>> neo4j_pool_;
        std::unique_ptr<ConnectionPool<MongoDBConnection>> mongodb_pool_;
        
        // Enhanced Python v3.0 Features
        std::unique_ptr<IntelligentQueryRouter> query_router_;
        std::unique_ptr<CrossDBSynchronizer> cross_db_sync_;
        bool enable_intelligent_routing_;
        bool enable_cross_db_sync_;
        
        // Health monitoring
        std::thread health_monitor_thread_;
        std::atomic<bool> health_monitoring_active_;
#ifdef LIBPQ_AVAILABLE
        std::atomic<ConnectionStatus> postgresql_status_;
#endif
        std::atomic<ConnectionStatus> neo4j_status_;
        std::atomic<ConnectionStatus> mongodb_status_;
        
        // Enhanced statistics with Python v3.0 features
        std::atomic<uint64_t> total_queries_executed_;
        std::atomic<uint64_t> successful_queries_;
        std::atomic<uint64_t> failed_queries_;
        std::atomic<uint64_t> texture_profiles_processed_;
        std::atomic<uint64_t> enigma_calculations_performed_;
        std::chrono::system_clock::time_point start_time_;
        
        // Performance tracking per database
        std::unordered_map<std::string, uint64_t> database_query_counts_;
        std::unordered_map<std::string, double> database_total_times_;
        std::mutex stats_mutex_;
        
        // Thread safety
        std::mutex connector_mutex_;
        
    public:
        TriforceConnector(const DatabaseConfig& config = DatabaseConfig{});
        ~TriforceConnector();

        // Initialization and shutdown
        bool initialize();
        void shutdown();
        bool is_initialized() const;
        
        // Core database operations
#ifdef LIBPQ_AVAILABLE
        QueryResult execute_postgresql_query(const std::string& query,
                                            const std::vector<std::string>& params = {});
#endif
        QueryResult execute_neo4j_cypher(const std::string& cypher,
                                        const std::unordered_map<std::string, std::string>& params = {});
        QueryResult execute_mongodb_operation(const std::string& collection,
                                             const std::string& operation,
                                             const std::string& document_json);
        
        // Enhanced intelligent query routing (Python v3.0 Features)
        QueryResult execute_smart_query(const std::string& query,
                                       const std::vector<std::string>& params = {},
                                       DatabaseType preferred_db = DatabaseType::POSTGRESQL);
        
        QueryResult execute_intelligent_query(const std::string& query,
                                            const std::vector<std::string>& params = {},
                                            const std::string& operation_type = "");
        
        // Texture Processing with Enigma Scale (Python v3.0 Features)
        QueryResult process_texture_batch(const std::vector<TextureProfile>& texture_profiles);
        QueryResult process_single_texture(const TextureProfile& texture_profile);
        double calculate_enigma_score_for_texture(const TextureProfile& profile);
        
        // Cross-database operations
        QueryResult execute_triforce_transaction(
#ifdef LIBPQ_AVAILABLE
            const std::function<bool(PostgreSQLConnection*, Neo4jConnection*, MongoDBConnection*)>& transaction_func);
#else
            const std::function<bool(Neo4jConnection*, MongoDBConnection*)>& transaction_func);
#endif
        
        // Cross-database synchronization control
        void enable_cross_database_sync(bool enabled);
        bool is_cross_database_sync_enabled() const;
        size_t get_pending_sync_operations() const;
        void wait_for_sync_completion();
        
        // Enhanced Health and status (Python v3.0 Features)
        struct HealthStatus {
#ifdef LIBPQ_AVAILABLE
            ConnectionStatus postgresql_status;
#endif
            ConnectionStatus neo4j_status;
            ConnectionStatus mongodb_status;
            bool overall_healthy;
            uint64_t total_queries;
            uint64_t successful_queries;
            uint64_t failed_queries;
            double success_rate;
            std::chrono::seconds uptime;
            std::string yorkshire_verdict;
            
            // Enhanced Python v3.0 features
            double enigma_constant = MedusaEnigmaScale::UNIVERSAL_CONSTANT;
            uint64_t texture_profiles_processed;
            uint64_t enigma_calculations_performed;
            bool intelligent_routing_enabled;
            bool cross_db_sync_enabled;
            size_t pending_sync_operations;
            
            // Per-database performance stats
            std::unordered_map<std::string, double> database_avg_response_times;
            std::unordered_map<std::string, uint64_t> database_query_counts;
            std::chrono::system_clock::time_point timestamp;
        };
        
        HealthStatus get_health_status() const;
        bool test_all_connections();
        
        // Configuration management
        void update_configuration(const DatabaseConfig& new_config);
        const DatabaseConfig& get_configuration() const { return config_; }
        
        // Statistics
        uint64_t get_total_queries() const { return total_queries_executed_.load(); }
        double get_success_rate() const;
        std::chrono::seconds get_uptime() const;
        
        // Yorkshire Champion specific methods
        std::string get_enigma_scale_status() const;
        void log_yorkshire_status(const std::string& operation, bool success) const;
        
    private:
        // Initialization helpers
#ifdef LIBPQ_AVAILABLE
        bool initialize_postgresql_pool();
#endif
        bool initialize_neo4j_pool();
        bool initialize_mongodb_pool();
        
        // Health monitoring
        void health_monitor_worker();
        void check_connection_health(DatabaseType db_type);
        
        // Query routing intelligence
        DatabaseType determine_optimal_database(const std::string& query) const;
        bool requires_cross_database_consistency(const std::string& query) const;
        
        // Error handling and retry logic
        QueryResult retry_query_with_backoff(
            const std::function<QueryResult()>& query_func,
            uint32_t max_retries = 3);
        
        void log_query_statistics(const QueryResult& result);
        
        // Utility methods
        std::string generate_connection_id() const;
        void cleanup_expired_connections();
    };

    /**
     * Global Triforce Instance Manager
     * Ensures single instance across the entire application
     */
    class TriforceManager {
    private:
        static std::unique_ptr<TriforceConnector> instance_;
        static std::mutex instance_mutex_;
        static std::atomic<bool> is_initialized_;

    public:
        static TriforceConnector* get_instance();
        static bool initialize_global_instance(const DatabaseConfig& config = DatabaseConfig{});
        static void shutdown_global_instance();
        static bool is_global_instance_ready() { return is_initialized_.load(); }
        
        // Yorkshire Champion convenience methods
#ifdef LIBPQ_AVAILABLE
        static QueryResult quick_postgresql_query(const std::string& query,
                                                 const std::vector<std::string>& params = {});
#endif
        static QueryResult quick_neo4j_cypher(const std::string& cypher,
                                             const std::unordered_map<std::string, std::string>& params = {});
        static QueryResult quick_mongodb_operation(const std::string& collection,
                                                  const std::string& operation,
                                                  const std::string& document_json);
    };

    // Yorkshire Champion convenience functions for external use
    extern "C" {
        // C-style API for compatibility with other languages/systems
        int triforce_initialize();
        int triforce_shutdown();
        int triforce_is_ready();
        
#ifdef LIBPQ_AVAILABLE
        const char* triforce_execute_postgresql(const char* query, const char* params_json);
#endif
        const char* triforce_execute_neo4j(const char* cypher, const char* params_json);
        const char* triforce_execute_mongodb(const char* collection, const char* operation, const char* document);
        
        const char* triforce_get_health_status();
        const char* triforce_get_yorkshire_status();
    }

} // namespace medusa::database