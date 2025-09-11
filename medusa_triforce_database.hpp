/*
 * MEDUSA TRIFORCE HYBRID DATABASE SYSTEM
 * Yorkshire Champion three-tier database architecture from Medusa++
 * PostgreSQL + SQLite + Redis hybrid for optimal performance
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <functional>
#include <queue>

namespace MedusaServer {

// Forward declarations
class PostgreSQLConnection;
class SQLiteConnection;
class RedisConnection;

enum class TriforceLayer {
    PERSISTENCE = 1,    // PostgreSQL - Production data, ACID compliance
    OPERATIONAL = 2,    // SQLite - Local operations, fast queries
    CACHE = 3          // Redis - High-speed caching, sessions
};

enum class DataSyncStrategy {
    IMMEDIATE,          // Sync immediately across all layers
    BATCH,             // Batch sync at intervals
    LAZY,              // Sync when accessed
    WRITE_THROUGH,     // Write to all layers simultaneously
    WRITE_BACK         // Write to cache first, sync later
};

struct TriforceConfig {
    // PostgreSQL (Persistence Layer)
    std::string postgres_host = "172.236.23.54";
    int postgres_port = 5432;
    std::string postgres_database = "medusa_rts";
    std::string postgres_username = "medusa";
    std::string postgres_password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    
    // SQLite (Operational Layer)
    std::string sqlite_path = "/home/medusa/data/operational.db";
    bool sqlite_wal_mode = true;
    
    // Redis (Cache Layer)
    std::string redis_host = "localhost";
    int redis_port = 6379;
    std::string redis_password = "";
    int redis_database = 0;
    
    // Sync configuration
    DataSyncStrategy sync_strategy = DataSyncStrategy::WRITE_THROUGH;
    std::chrono::seconds sync_interval{60};
    int max_cache_size_mb = 256;
    std::chrono::seconds cache_ttl{3600};
    
    // Performance settings
    int postgres_connection_pool_size = 10;
    int sqlite_busy_timeout_ms = 5000;
    int redis_connection_pool_size = 5;
    bool enable_query_optimization = true;
    bool enable_automatic_failover = true;
};

struct QueryResult {
    bool success = false;
    std::string error_message;
    std::vector<std::map<std::string, std::string>> rows;
    int affected_rows = 0;
    std::chrono::microseconds execution_time{0};
    TriforceLayer executed_on = TriforceLayer::PERSISTENCE;
    bool from_cache = false;
};

struct DatabaseStats {
    std::atomic<uint64_t> postgres_queries{0};
    std::atomic<uint64_t> sqlite_queries{0};
    std::atomic<uint64_t> redis_queries{0};
    std::atomic<uint64_t> cache_hits{0};
    std::atomic<uint64_t> cache_misses{0};
    std::atomic<uint64_t> sync_operations{0};
    std::atomic<uint64_t> failed_operations{0};
    std::chrono::system_clock::time_point last_sync;
    std::atomic<double> avg_response_time_ms{0.0};
};

class TriforceDatabase {
private:
    TriforceConfig config_;
    DatabaseStats stats_;
    
    // Connection pools for each layer
    std::unique_ptr<PostgreSQLConnection> postgres_primary_;
    std::vector<std::unique_ptr<PostgreSQLConnection>> postgres_pool_;
    std::unique_ptr<SQLiteConnection> sqlite_connection_;
    std::unique_ptr<RedisConnection> redis_connection_;
    
    // Synchronization
    std::mutex postgres_pool_mutex_;
    std::mutex sync_mutex_;
    std::thread sync_thread_;
    std::atomic<bool> running_;
    std::queue<std::string> sync_queue_;
    std::mutex sync_queue_mutex_;
    
    // Query optimization
    std::map<std::string, std::chrono::system_clock::time_point> query_cache_;
    std::map<std::string, QueryResult> result_cache_;
    std::mutex cache_mutex_;
    
public:
    TriforceDatabase(const TriforceConfig& config = TriforceConfig{});
    ~TriforceDatabase();
    
    // Lifecycle management
    bool initialize();
    void shutdown();
    bool isConnected() const;
    
    // Query interface - auto-routing based on query type and strategy
    QueryResult execute(const std::string& query, TriforceLayer preferred_layer = TriforceLayer::PERSISTENCE);
    QueryResult select(const std::string& query, bool allow_cache = true);
    QueryResult insert(const std::string& query, bool sync_immediately = true);
    QueryResult update(const std::string& query, bool sync_immediately = true);
    QueryResult delete_query(const std::string& query, bool sync_immediately = true);
    
    // Prepared statements
    QueryResult executePrepared(const std::string& statement_id, const std::vector<std::string>& parameters);
    bool preparStatement(const std::string& statement_id, const std::string& query);
    
    // Transaction support
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    // Layer-specific operations
    QueryResult executeOnPostgres(const std::string& query);
    QueryResult executeOnSQLite(const std::string& query);
    QueryResult executeOnRedis(const std::string& command);
    
    // Cache management
    bool cacheResult(const std::string& query_hash, const QueryResult& result);
    QueryResult getCachedResult(const std::string& query_hash);
    void invalidateCache(const std::string& pattern = "");
    void flushCache();
    
    // Data synchronization
    bool syncData(TriforceLayer from, TriforceLayer to);
    bool syncTable(const std::string& table_name);
    bool syncAllTables();
    void schedulSync(const std::string& sync_operation);
    
    // High-level data operations
    std::vector<std::map<std::string, std::string>> getUserData(const std::string& user_id);
    bool updateUserData(const std::string& user_id, const std::map<std::string, std::string>& data);
    std::vector<std::map<std::string, std::string>> getAuditLogs(const std::chrono::system_clock::time_point& since);
    bool logAuditEvent(const std::map<std::string, std::string>& event_data);
    
    // Performance monitoring
    DatabaseStats getStats() const { return stats_; }
    std::map<std::string, double> getPerformanceMetrics();
    std::string getHealthStatus();
    
    // Configuration management
    bool updateConfig(const TriforceConfig& new_config);
    TriforceConfig getConfig() const { return config_; }
    
    // Maintenance operations
    bool vacuum();
    bool analyze();
    bool reindex();
    bool backup(const std::string& backup_path);
    bool restore(const std::string& backup_path);
    
private:
    // Connection management
    PostgreSQLConnection* acquirePostgresConnection();
    void releasePostgresConnection(PostgreSQLConnection* conn);
    bool initializePostgresPool();
    bool initializeSQLite();
    bool initializeRedis();
    
    // Query optimization
    std::string optimizeQuery(const std::string& query);
    TriforceLayer determineOptimalLayer(const std::string& query);
    std::string generateQueryHash(const std::string& query);
    
    // Synchronization thread
    void syncWorkerLoop();
    bool performDataSync();
    
    // Utility methods
    void updateStats(TriforceLayer layer, std::chrono::microseconds execution_time, bool success);
    void logError(const std::string& operation, const std::string& error);
    void logInfo(const std::string& operation, const std::string& info);
};

// Individual connection classes

class PostgreSQLConnection {
private:
    void* connection_; // PGconn pointer
    std::string connection_string_;
    bool connected_;
    
public:
    PostgreSQLConnection(const std::string& connection_string);
    ~PostgreSQLConnection();
    
    bool connect();
    void disconnect();
    bool isConnected() const { return connected_; }
    
    QueryResult execute(const std::string& query);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
private:
    QueryResult processResult(void* result); // PGresult pointer
};

class SQLiteConnection {
private:
    void* database_; // sqlite3 pointer
    std::string database_path_;
    bool connected_;
    std::map<std::string, void*> prepared_statements_; // sqlite3_stmt pointers
    
public:
    SQLiteConnection(const std::string& database_path);
    ~SQLiteConnection();
    
    bool connect();
    void disconnect();
    bool isConnected() const { return connected_; }
    
    QueryResult execute(const std::string& query);
    QueryResult executePrepared(const std::string& statement_id, const std::vector<std::string>& parameters);
    bool prepareStatement(const std::string& statement_id, const std::string& query);
    
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    bool vacuum();
    bool enableWAL();
};

class RedisConnection {
private:
    void* context_; // redisContext pointer
    std::string host_;
    int port_;
    std::string password_;
    int database_;
    bool connected_;
    
public:
    RedisConnection(const std::string& host, int port, const std::string& password = "", int database = 0);
    ~RedisConnection();
    
    bool connect();
    void disconnect();
    bool isConnected() const { return connected_; }
    
    QueryResult execute(const std::string& command);
    
    // Redis-specific operations
    bool set(const std::string& key, const std::string& value, std::chrono::seconds ttl = std::chrono::seconds{0});
    std::string get(const std::string& key);
    bool exists(const std::string& key);
    bool del(const std::string& key);
    bool expire(const std::string& key, std::chrono::seconds ttl);
    
    // Hash operations
    bool hset(const std::string& key, const std::string& field, const std::string& value);
    std::string hget(const std::string& key, const std::string& field);
    std::map<std::string, std::string> hgetall(const std::string& key);
    
    // List operations
    bool lpush(const std::string& key, const std::string& value);
    bool rpush(const std::string& key, const std::string& value);
    std::string lpop(const std::string& key);
    std::string rpop(const std::string& key);
    
private:
    QueryResult processReply(void* reply); // redisReply pointer
};

// Triforce Database Factory
class TriforceFactory {
public:
    static std::shared_ptr<TriforceDatabase> createProductionInstance();
    static std::shared_ptr<TriforceDatabase> createDevelopmentInstance();
    static std::shared_ptr<TriforceDatabase> createTestInstance();
    
    static TriforceConfig getProductionConfig();
    static TriforceConfig getDevelopmentConfig();
    static TriforceConfig getTestConfig();
};

// Query Builder for Triforce
class TriforceQueryBuilder {
private:
    std::string table_name_;
    std::vector<std::string> select_fields_;
    std::vector<std::string> where_conditions_;
    std::vector<std::string> join_clauses_;
    std::string order_by_;
    std::string group_by_;
    std::string having_;
    int limit_ = -1;
    int offset_ = 0;
    
public:
    TriforceQueryBuilder(const std::string& table_name);
    
    TriforceQueryBuilder& select(const std::vector<std::string>& fields);
    TriforceQueryBuilder& where(const std::string& condition);
    TriforceQueryBuilder& whereEquals(const std::string& field, const std::string& value);
    TriforceQueryBuilder& whereIn(const std::string& field, const std::vector<std::string>& values);
    TriforceQueryBuilder& whereBetween(const std::string& field, const std::string& start, const std::string& end);
    TriforceQueryBuilder& join(const std::string& table, const std::string& on_condition);
    TriforceQueryBuilder& leftJoin(const std::string& table, const std::string& on_condition);
    TriforceQueryBuilder& orderBy(const std::string& field, const std::string& direction = "ASC");
    TriforceQueryBuilder& groupBy(const std::string& field);
    TriforceQueryBuilder& having(const std::string& condition);
    TriforceQueryBuilder& limit(int limit);
    TriforceQueryBuilder& offset(int offset);
    
    std::string buildSelect();
    std::string buildInsert(const std::map<std::string, std::string>& data);
    std::string buildUpdate(const std::map<std::string, std::string>& data);
    std::string buildDelete();
    
private:
    std::string escapeValue(const std::string& value);
};

// Production Triforce setup for Medusa
inline std::shared_ptr<TriforceDatabase> createMedusaTriforce() {
    TriforceConfig config;
    
    // Production PostgreSQL
    config.postgres_host = "172.236.23.54";
    config.postgres_port = 5432;
    config.postgres_database = "medusa_rts";
    config.postgres_username = "medusa";
    config.postgres_password = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    
    // Local SQLite for operations
    config.sqlite_path = "/home/medusa/data/triforce_operational.db";
    config.sqlite_wal_mode = true;
    
    // Redis cache
    config.redis_host = "localhost";
    config.redis_port = 6379;
    config.redis_database = 1; // Use database 1 for Triforce
    
    // Optimized settings
    config.sync_strategy = DataSyncStrategy::WRITE_THROUGH;
    config.sync_interval = std::chrono::seconds{30};
    config.max_cache_size_mb = 512;
    config.cache_ttl = std::chrono::seconds{1800}; // 30 minutes
    config.postgres_connection_pool_size = 15;
    config.enable_query_optimization = true;
    config.enable_automatic_failover = true;
    
    return std::make_shared<TriforceDatabase>(config);
}

} // namespace MedusaServer