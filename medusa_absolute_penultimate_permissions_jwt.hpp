/*
 * MEDUSA ABSOLUTE PEN-ULTIMATE PERMISSIONS & JWT SYSTEM
 * Cross-references ALL permission systems, JWT implementations, and real-time administration
 * The FINAL never-to-be-superseded permissions and authentication system
 * Combines all features from back-office auth systems and MCP integrations
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <fstream>
#include <sstream>
#include <array>
#include <random>
#include <bitset>
#include <nlohmann/json.hpp>
#include <jwt-cpp/jwt.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <bcrypt/BCrypt.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <postgresql/libpq-fe.h>
#include <neo4j-client.h>
#include <mongocxx/client.hpp>
#include <redis++.h>

namespace MedusaServer {

using namespace std::chrono;
using json = nlohmann::json;
using websocket_server = websocketpp::server<websocketpp::config::asio>;

// ============================================================================
// COMPREHENSIVE PERMISSION SYSTEM ENUMS & CONSTANTS
// ============================================================================

// Permission Types (Extended from all back-office systems)
enum class PermissionType {
    // System Administration
    SYSTEM_ADMIN = 1,
    SYSTEM_CONFIG = 2,
    SYSTEM_MONITORING = 3,
    SYSTEM_BACKUP = 4,
    SYSTEM_RESTORE = 5,
    SYSTEM_UPDATE = 6,
    SYSTEM_SHUTDOWN = 7,
    SYSTEM_RESTART = 8,
    
    // User Management
    USER_CREATE = 100,
    USER_READ = 101,
    USER_UPDATE = 102,
    USER_DELETE = 103,
    USER_MANAGE_ROLES = 104,
    USER_MANAGE_PERMISSIONS = 105,
    USER_VIEW_SESSIONS = 106,
    USER_TERMINATE_SESSIONS = 107,
    USER_PASSWORD_RESET = 108,
    USER_ACCOUNT_LOCK = 109,
    
    // Role Management
    ROLE_CREATE = 200,
    ROLE_READ = 201,
    ROLE_UPDATE = 202,
    ROLE_DELETE = 203,
    ROLE_ASSIGN_PERMISSIONS = 204,
    ROLE_MANAGE_HIERARCHY = 205,
    ROLE_VIEW_MEMBERS = 206,
    ROLE_BULK_OPERATIONS = 207,
    
    // Callback Administration (From back-office)
    CALLBACK_CREATE = 300,
    CALLBACK_READ = 301,
    CALLBACK_UPDATE = 302,
    CALLBACK_DELETE = 303,
    CALLBACK_EXECUTE = 304,
    CALLBACK_ANALYZE = 305,
    CALLBACK_OPTIMIZE = 306,
    CALLBACK_CONFIGURE = 307,
    CALLBACK_VIEW_LOGS = 308,
    CALLBACK_MANAGE_QUEUE = 309,
    
    // API Gateway & Security
    API_GATEWAY_ADMIN = 400,
    API_RATE_LIMIT_CONFIG = 401,
    API_SECURITY_CONFIG = 402,
    API_MONITORING = 403,
    API_THROTTLING = 404,
    API_CORS_CONFIG = 405,
    API_JWT_MANAGEMENT = 406,
    API_SESSION_MANAGEMENT = 407,
    
    // Database Operations
    DATABASE_READ = 500,
    DATABASE_WRITE = 501,
    DATABASE_ADMIN = 502,
    DATABASE_BACKUP = 503,
    DATABASE_RESTORE = 504,
    DATABASE_SCHEMA_CHANGE = 505,
    DATABASE_PERFORMANCE_TUNING = 506,
    DATABASE_REPLICATION = 507,
    MCP_POSTGRESQL_ACCESS = 508,
    TRIFORCE_DATABASE_ADMIN = 509,
    
    // File & Resource Management
    FILE_READ = 600,
    FILE_WRITE = 601,
    FILE_DELETE = 602,
    FILE_ADMIN = 603,
    RESOURCE_ALLOCATION = 604,
    RESOURCE_MONITORING = 605,
    NAS_ACCESS = 606,
    BACKUP_MANAGEMENT = 607,
    
    // Security & Audit
    SECURITY_ADMIN = 700,
    SECURITY_VIEW_LOGS = 701,
    SECURITY_CONFIGURE_RULES = 702,
    AUDIT_VIEW = 703,
    AUDIT_EXPORT = 704,
    THREAT_MANAGEMENT = 705,
    VULNERABILITY_ASSESSMENT = 706,
    COMPLIANCE_MANAGEMENT = 707,
    PENETRATION_TESTING = 708,
    INCIDENT_RESPONSE = 709,
    
    // Real-time Features
    REALTIME_MONITORING = 800,
    WEBSOCKET_ADMIN = 801,
    LIVE_DASHBOARD_ACCESS = 802,
    REAL_TIME_ALERTS = 803,
    STREAMING_DATA_ACCESS = 804,
    EVENT_SUBSCRIPTION = 805,
    NOTIFICATION_MANAGEMENT = 806,
    
    // Plugin System
    PLUGIN_INSTALL = 900,
    PLUGIN_UNINSTALL = 901,
    PLUGIN_CONFIGURE = 902,
    PLUGIN_DEVELOP = 903,
    PLUGIN_TEST = 904,
    PLUGIN_PUBLISH = 905,
    PLUGIN_SECURITY_SCAN = 906,
    PLUGIN_DEPENDENCY_MANAGE = 907,
    
    // Advanced Features
    AI_MODEL_ACCESS = 1000,
    MACHINE_LEARNING_CONFIG = 1001,
    ANALYTICS_ADVANCED = 1002,
    PREDICTIVE_ANALYSIS = 1003,
    CUSTOM_ALGORITHMS = 1004,
    QUANTUM_FEATURES = 1005,
    NEURAL_NETWORK_ACCESS = 1006,
    
    // GDPR & Privacy
    GDPR_DATA_ACCESS = 1100,
    GDPR_DATA_EXPORT = 1101,
    GDPR_DATA_DELETE = 1102,
    PRIVACY_SETTINGS = 1103,
    DATA_RETENTION_MANAGE = 1104,
    CONSENT_MANAGEMENT = 1105,
    
    // Custom & Dynamic Permissions
    CUSTOM_PERMISSION = 9000,
    WILDCARD_ACCESS = 9999
};

// Permission Scopes (Context-aware permissions)
enum class PermissionScope {
    GLOBAL,           // System-wide access
    ORGANIZATION,     // Organization-level access
    DEPARTMENT,       // Department-level access
    TEAM,            // Team-level access
    PROJECT,         // Project-specific access
    RESOURCE,        // Resource-specific access
    USER_SPECIFIC,   // Individual user access
    TIME_BOUND,      // Temporal permissions
    CONDITION_BASED, // Conditional access
    DYNAMIC          // Runtime-evaluated permissions
};

// User Roles (Comprehensive hierarchy from back-office analysis)
enum class UserRole {
    // System Roles
    SUPER_ADMIN = 0,        // Ultimate system access
    SYSTEM_ADMIN = 1,       // System administration
    SECURITY_ADMIN = 2,     // Security-focused admin
    DATABASE_ADMIN = 3,     // Database administration
    
    // Management Roles
    ORGANIZATION_ADMIN = 10, // Organization management
    DEPARTMENT_MANAGER = 11, // Department management
    TEAM_LEAD = 12,         // Team leadership
    PROJECT_MANAGER = 13,   // Project management
    
    // Operational Roles
    SENIOR_OPERATOR = 20,   // Advanced operations
    OPERATOR = 21,          // Standard operations
    ANALYST = 22,           // Data analysis
    DEVELOPER = 23,         // Development access
    TESTER = 24,           // Testing access
    
    // Specialized Roles
    SECURITY_ANALYST = 30,  // Security analysis
    AUDIT_SPECIALIST = 31,  // Audit operations
    COMPLIANCE_OFFICER = 32, // Compliance management
    DATA_SCIENTIST = 33,    // Advanced analytics
    
    // Standard Users
    POWER_USER = 40,        // Enhanced user privileges
    STANDARD_USER = 41,     // Regular user access
    LIMITED_USER = 42,      // Restricted access
    GUEST_USER = 43,        // Guest access
    
    // Service Accounts
    SERVICE_ACCOUNT = 50,   // Automated services
    API_CLIENT = 51,        // API access
    INTEGRATION_USER = 52,  // External integrations
    
    // Special Roles
    GODMODE = 99,          // Ultimate access (from back-office)
    EMERGENCY_ACCESS = 98,  // Emergency situations
    TEMPORARY_ADMIN = 97,   // Temporary elevated access
    
    CUSTOM_ROLE = 100      // Dynamic custom roles
};

// JWT Token Types
enum class TokenType {
    ACCESS_TOKEN,       // Short-lived access token
    REFRESH_TOKEN,      // Long-lived refresh token
    ID_TOKEN,          // Identity token
    SESSION_TOKEN,     // Session-specific token
    API_KEY,           // API key token
    SERVICE_TOKEN,     // Service-to-service token
    TEMPORARY_TOKEN,   // Single-use token
    MAGIC_LINK_TOKEN,  // Magic link authentication
    MFA_TOKEN,         // Multi-factor authentication
    RESET_TOKEN        // Password reset token
};

// Authentication Methods (Multi-factor support)
enum class AuthMethod {
    PASSWORD,          // Traditional password
    BIOMETRIC,         // Fingerprint, face recognition
    HARDWARE_KEY,      // YubiKey, FIDO2
    SMART_CARD,        // Smart card authentication
    CERTIFICATE,       // Digital certificates
    OTP_SMS,          // SMS one-time password
    OTP_EMAIL,        // Email one-time password
    TOTP_APP,         // Time-based OTP app
    PUSH_NOTIFICATION, // Push-based approval
    VOICE_CALL,       // Voice verification
    BACKUP_CODES,     // Emergency backup codes
    SSO_OAUTH,        // OAuth SSO
    SSO_SAML,         // SAML SSO
    LDAP,             // LDAP authentication
    KERBEROS,         // Kerberos authentication
    RADIUS            // RADIUS authentication
};

// Session Status
enum class SessionStatus {
    ACTIVE,           // Currently active
    INACTIVE,         // Temporarily inactive
    EXPIRED,          // Session expired
    TERMINATED,       // Manually terminated
    SUSPENDED,        // Suspended due to security
    LOCKED,           // Account locked
    COMPROMISED,      // Security compromise detected
    MAINTENANCE       // System maintenance
};

// ============================================================================
// ADVANCED DATA STRUCTURES
// ============================================================================

// Permission Definition (Complete from back-office analysis)
struct Permission {
    PermissionType type;
    std::string key;                    // Unique permission key
    std::string name;                   // Human-readable name
    std::string description;            // Detailed description
    PermissionScope scope;              // Permission scope
    std::vector<std::string> resources; // Applicable resources
    
    // Hierarchical & Dependencies
    std::vector<PermissionType> parent_permissions;  // Parent permissions
    std::vector<PermissionType> child_permissions;   // Child permissions
    std::vector<PermissionType> required_permissions; // Prerequisites
    std::vector<PermissionType> conflicting_permissions; // Mutually exclusive
    
    // Contextual Information
    std::map<std::string, std::string> metadata;
    std::vector<std::string> tags;
    std::string category;
    int priority = 0;
    
    // Constraints & Conditions
    std::string conditions;             // JSON conditions for dynamic evaluation
    std::chrono::system_clock::time_point valid_from;
    std::chrono::system_clock::time_point valid_until;
    std::vector<std::string> ip_restrictions;
    std::vector<std::string> time_restrictions;
    
    // Audit & Tracking
    bool requires_approval = false;     // Requires approval for grant/revoke
    bool audit_required = true;         // Requires audit logging
    std::string created_by;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    
    // Risk Assessment
    int risk_level = 1;                 // 1-10 risk scale
    std::string risk_justification;
    bool requires_mfa = false;          // Requires multi-factor authentication
    
    // Advanced Features
    bool is_delegatable = false;        // Can be delegated to others
    bool is_inheritable = true;         // Inheritable by child roles
    bool is_revocable = true;           // Can be revoked
    bool is_transferable = false;       // Can be transferred
    
    std::string toJson() const;
    bool isValid() const;
    bool conflictsWith(const Permission& other) const;
    int calculateRiskScore() const;
};

// Role Definition (Enhanced from back-office)
struct Role {
    UserRole type;
    std::string id;
    std::string name;
    std::string display_name;
    std::string description;
    
    // Permissions
    std::unordered_set<PermissionType> permissions;
    std::unordered_set<std::string> dynamic_permissions;
    
    // Hierarchical Structure
    std::string parent_role_id;
    std::vector<std::string> child_role_ids;
    int hierarchy_level = 0;
    
    // Role Properties
    bool is_system_role = false;        // System-defined role
    bool is_active = true;
    bool is_assignable = true;
    int max_assignments = -1;           // -1 = unlimited
    int current_assignments = 0;
    
    // Constraints
    std::chrono::system_clock::time_point valid_from;
    std::chrono::system_clock::time_point valid_until;
    std::vector<std::string> assignment_conditions;
    
    // Metadata
    std::map<std::string, std::string> metadata;
    std::string created_by;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    
    // Advanced Features
    bool allows_delegation = false;
    bool requires_approval = false;
    int approval_threshold = 1;         // Number of approvals required
    
    std::string toJson() const;
    bool hasPermission(PermissionType permission) const;
    bool canAssignTo(const std::string& user_id) const;
    int calculatePrivilegeLevel() const;
};

// User Account (Complete authentication profile)
struct UserAccount {
    std::string id;
    std::string username;
    std::string email;
    std::string display_name;
    std::string first_name;
    std::string last_name;
    
    // Authentication Data
    std::string password_hash;          // bcrypt hash
    std::string encryption_key;         // Fernet encryption key
    std::string master_key_ref;         // Reference to master key
    std::vector<AuthMethod> enabled_auth_methods;
    
    // Account Status
    bool is_active = true;
    bool is_verified = false;
    bool is_locked = false;
    bool requires_password_change = false;
    std::chrono::system_clock::time_point locked_until;
    
    // Roles & Permissions
    std::unordered_set<std::string> role_ids;
    std::unordered_set<PermissionType> direct_permissions;
    std::unordered_map<std::string, std::string> permission_grants; // Permission ID -> Grant reason
    
    // Security Information
    std::string totp_secret;            // TOTP secret for 2FA
    std::vector<std::string> backup_codes;
    std::vector<std::string> trusted_devices;
    int failed_login_attempts = 0;
    std::chrono::system_clock::time_point last_failed_login;
    std::chrono::system_clock::time_point last_successful_login;
    std::chrono::system_clock::time_point password_changed_at;
    
    // Session Management
    std::vector<std::string> active_session_ids;
    int max_concurrent_sessions = 5;
    bool force_single_session = false;
    
    // Audit Trail
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    std::chrono::system_clock::time_point last_login_at;
    std::string created_by;
    std::string last_modified_by;
    
    // Profile Information
    std::string avatar_url;
    std::string timezone;
    std::string locale;
    std::map<std::string, std::string> preferences;
    std::map<std::string, std::string> custom_fields;
    
    // Compliance & Privacy
    bool gdpr_consent = false;
    std::chrono::system_clock::time_point gdpr_consent_date;
    std::vector<std::string> data_processing_purposes;
    bool marketing_consent = false;
    
    std::string toJson() const;
    bool hasPermission(PermissionType permission) const;
    bool hasRole(const std::string& role_id) const;
    std::vector<PermissionType> getAllPermissions() const;
    bool canAuthenticate() const;
    bool requiresMFA() const;
};

// JWT Token Structure (Enhanced from back-office JWT implementation)
struct JWTToken {
    std::string token_id;               // Unique token identifier
    TokenType type;
    std::string user_id;
    std::string session_id;
    
    // Token Content
    std::string issuer;                 // Token issuer
    std::string audience;               // Intended audience
    std::string subject;                // Subject (usually user_id)
    std::chrono::system_clock::time_point issued_at;
    std::chrono::system_clock::time_point expires_at;
    std::chrono::system_clock::time_point not_before;
    
    // Claims & Permissions
    std::map<std::string, json> claims; // Custom claims
    std::vector<PermissionType> permissions;
    std::vector<std::string> roles;
    std::string scope;                  // OAuth-style scopes
    
    // Security Information
    std::string device_fingerprint;
    std::string ip_address;
    std::string user_agent;
    std::vector<std::string> allowed_origins;
    
    // Token Metadata
    std::string algorithm = "RS256";    // Signing algorithm
    bool is_refresh_token = false;
    std::string parent_token_id;        // For refresh token chains
    int refresh_count = 0;
    
    // Rate Limiting & Usage
    int usage_count = 0;
    int max_usage = -1;                 // -1 = unlimited
    std::chrono::system_clock::time_point last_used;
    
    // Revocation
    bool is_revoked = false;
    std::chrono::system_clock::time_point revoked_at;
    std::string revoked_by;
    std::string revocation_reason;
    
    std::string encode(const std::string& private_key) const;
    static JWTToken decode(const std::string& token, const std::string& public_key);
    bool isValid() const;
    bool hasPermission(PermissionType permission) const;
    json getClaims() const;
    std::string toJson() const;
};

// Session Management (Real-time session tracking)
struct UserSession {
    std::string session_id;
    std::string user_id;
    std::string device_id;
    
    // Session State
    SessionStatus status;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_activity;
    std::chrono::system_clock::time_point expires_at;
    
    // Authentication Details
    std::vector<AuthMethod> auth_methods_used;
    bool mfa_completed = false;
    std::string initial_ip;
    std::string current_ip;
    std::string user_agent;
    std::string device_fingerprint;
    
    // Session Data
    std::map<std::string, json> session_data;
    std::string access_token_id;
    std::string refresh_token_id;
    
    // Security Tracking
    std::vector<std::string> suspicious_activities;
    int risk_score = 0;
    bool requires_reauth = false;
    std::chrono::system_clock::time_point last_security_check;
    
    // Activity Tracking
    int request_count = 0;
    std::chrono::system_clock::time_point last_request;
    std::vector<std::string> accessed_resources;
    std::map<std::string, int> permission_usage;
    
    // Geographic Information
    std::string country;
    std::string city;
    std::string timezone;
    
    bool isActive() const;
    bool isExpired() const;
    bool requiresRefresh() const;
    void updateActivity();
    int calculateRiskScore() const;
    std::string toJson() const;
};

// Real-time Permission Event (WebSocket notifications)
struct PermissionEvent {
    std::string event_id;
    std::string event_type;             // "permission_granted", "role_assigned", etc.
    std::chrono::system_clock::time_point timestamp;
    
    // Event Subject
    std::string user_id;
    std::string role_id;
    PermissionType permission_type;
    std::string permission_key;
    
    // Event Details
    std::string action;                 // "grant", "revoke", "update", "create", "delete"
    std::string initiated_by;           // Who initiated the change
    std::string reason;                 // Reason for the change
    
    // Change Information
    json old_value;                     // Previous state
    json new_value;                     // New state
    std::map<std::string, std::string> metadata;
    
    // Propagation
    std::vector<std::string> affected_users;
    std::vector<std::string> notification_channels;
    bool requires_user_notification = false;
    
    std::string toJson() const;
    void broadcast() const;
};

// Rate Limiting Configuration (From back-office security middleware)
struct RateLimitConfig {
    std::string resource_pattern;       // Resource pattern (regex)
    int requests_per_minute = 60;
    int requests_per_hour = 1000;
    int requests_per_day = 10000;
    int burst_capacity = 10;
    
    // Advanced Rate Limiting
    std::map<UserRole, int> role_based_limits;
    std::map<PermissionType, int> permission_based_limits;
    
    // Sliding Window Configuration
    int window_size_seconds = 60;
    int max_requests_per_window = 100;
    
    // Penalty System
    int lockout_duration_seconds = 300;
    int progressive_penalty_multiplier = 2;
    
    bool matches(const std::string& resource) const;
    int getLimitForUser(const UserAccount& user) const;
};

// ============================================================================
// CORE SYSTEM CLASSES
// ============================================================================

// Revolutionary Permission Manager (Ultimate version)
class AbsolutePenultimatePermissionManager {
private:
    // Database Connections (Triforce Integration)
    PGconn* postgresql_connection_;
    neo4j_connection_t* neo4j_connection_;
    mongocxx::client mongodb_client_;
    sw::redis::Redis redis_client_;
    
    // In-Memory Caches
    std::unordered_map<std::string, Permission> permission_cache_;
    std::unordered_map<std::string, Role> role_cache_;
    std::unordered_map<std::string, UserAccount> user_cache_;
    
    // Thread Safety
    mutable std::shared_mutex cache_mutex_;
    mutable std::shared_mutex permission_mutex_;
    
    // Configuration
    std::string database_connection_string_;
    std::string redis_connection_string_;
    bool enable_caching_ = true;
    int cache_ttl_seconds_ = 3600;
    
public:
    AbsolutePenultimatePermissionManager(
        const std::string& postgresql_conn = "postgresql://medusa:izJaRuA2kwbNwezvKsCzo7DUNnQc@172.236.23.54:5432/medusa_rts",
        const std::string& redis_conn = "tcp://172.236.23.54:6379"
    );
    ~AbsolutePenultimatePermissionManager();
    
    // Initialization & Configuration
    bool initialize();
    void shutdown();
    bool loadPermissionSchema();
    void configureCache(int ttl_seconds, bool enable = true);
    
    // Permission Management
    bool createPermission(const Permission& permission);
    bool updatePermission(const std::string& permission_key, const Permission& permission);
    bool deletePermission(const std::string& permission_key);
    Permission getPermission(const std::string& permission_key);
    std::vector<Permission> getAllPermissions();
    std::vector<Permission> getPermissionsByCategory(const std::string& category);
    std::vector<Permission> searchPermissions(const std::string& query);
    
    // Role Management
    bool createRole(const Role& role);
    bool updateRole(const std::string& role_id, const Role& role);
    bool deleteRole(const std::string& role_id);
    Role getRole(const std::string& role_id);
    std::vector<Role> getAllRoles();
    std::vector<Role> getRoleHierarchy(const std::string& parent_role_id = "");
    
    // Permission Assignment
    bool grantPermissionToUser(const std::string& user_id, PermissionType permission, const std::string& granted_by, const std::string& reason = "");
    bool revokePermissionFromUser(const std::string& user_id, PermissionType permission, const std::string& revoked_by, const std::string& reason = "");
    bool grantPermissionToRole(const std::string& role_id, PermissionType permission, const std::string& granted_by);
    bool revokePermissionFromRole(const std::string& role_id, PermissionType permission, const std::string& revoked_by);
    
    // Role Assignment
    bool assignRoleToUser(const std::string& user_id, const std::string& role_id, const std::string& assigned_by, const std::string& reason = "");
    bool removeRoleFromUser(const std::string& user_id, const std::string& role_id, const std::string& removed_by, const std::string& reason = "");
    
    // Permission Checking (High-performance)
    bool hasPermission(const std::string& user_id, PermissionType permission, const std::string& context = "");
    bool hasPermission(const std::string& user_id, const std::string& permission_key, const std::string& context = "");
    bool hasAnyPermission(const std::string& user_id, const std::vector<PermissionType>& permissions);
    bool hasAllPermissions(const std::string& user_id, const std::vector<PermissionType>& permissions);
    
    // Advanced Permission Queries
    std::vector<PermissionType> getUserPermissions(const std::string& user_id, bool include_inherited = true);
    std::vector<std::string> getUserRoles(const std::string& user_id);
    std::vector<std::string> getUsersWithPermission(PermissionType permission);
    std::vector<std::string> getUsersWithRole(const std::string& role_id);
    
    // Permission Analytics
    std::map<PermissionType, int> getPermissionUsageStats();
    std::map<std::string, int> getRoleAssignmentStats();
    std::vector<std::string> getUnusedPermissions();
    std::vector<std::string> getOverPrivilegedUsers();
    json generatePermissionReport(const std::string& user_id = "");
    
    // Bulk Operations
    bool bulkAssignPermissions(const std::string& user_id, const std::vector<PermissionType>& permissions, const std::string& assigned_by);
    bool bulkRevokePermissions(const std::string& user_id, const std::vector<PermissionType>& permissions, const std::string& revoked_by);
    bool bulkAssignRole(const std::vector<std::string>& user_ids, const std::string& role_id, const std::string& assigned_by);
    
    // Cache Management
    void clearCache();
    void refreshCache();
    void refreshUserCache(const std::string& user_id);
    void refreshRoleCache(const std::string& role_id);
    
    // Event Publishing
    void publishPermissionEvent(const PermissionEvent& event);
    void subscribeToPermissionEvents(std::function<void(const PermissionEvent&)> callback);
    
private:
    // Internal Methods
    bool validatePermission(const Permission& permission);
    bool validateRole(const Role& role);
    void cachePermission(const Permission& permission);
    void cacheRole(const Role& role);
    void cacheUser(const UserAccount& user);
    void invalidateUserCache(const std::string& user_id);
    bool loadFromDatabase();
    bool saveToDatabase();
    std::string generatePermissionKey(PermissionType type, const std::string& context = "");
};

// Ultimate JWT Manager (Enhanced from back-office auth.js)
class AbsolutePenultimateJWTManager {
private:
    // Cryptographic Keys
    std::string private_key_;           // RSA private key for signing
    std::string public_key_;            // RSA public key for verification
    std::string symmetric_secret_;      // HMAC secret key
    
    // Token Storage
    std::unordered_map<std::string, JWTToken> active_tokens_;
    std::unordered_set<std::string> revoked_tokens_;
    sw::redis::Redis redis_client_;
    
    // Configuration
    std::string issuer_;
    std::string default_audience_;
    int default_expiry_minutes_ = 15;   // Access token expiry
    int refresh_expiry_days_ = 30;      // Refresh token expiry
    std::string algorithm_ = "RS256";
    
    // Thread Safety
    mutable std::shared_mutex token_mutex_;
    
    // Rate Limiting & Security
    std::unordered_map<std::string, int> token_generation_counts_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> last_generation_time_;
    
public:
    AbsolutePenultimateJWTManager(
        const std::string& redis_conn = "tcp://172.236.23.54:6379",
        const std::string& issuer = "medusa-ultimate-auth"
    );
    ~AbsolutePenultimateJWTManager();
    
    // Initialization
    bool initialize();
    bool loadKeys(const std::string& private_key_path, const std::string& public_key_path);
    bool generateKeys();
    void configureDefaults(int access_expiry_minutes, int refresh_expiry_days, const std::string& algorithm);
    
    // Token Generation
    JWTToken generateAccessToken(const UserAccount& user, const std::string& session_id, const std::map<std::string, json>& custom_claims = {});
    JWTToken generateRefreshToken(const std::string& user_id, const std::string& session_id, const std::string& device_id);
    JWTToken generateAPIToken(const std::string& user_id, const std::vector<PermissionType>& permissions, int expiry_days = 365);
    JWTToken generateTemporaryToken(const std::string& user_id, const std::string& purpose, int expiry_minutes = 15);
    JWTToken generateMagicLinkToken(const std::string& user_id, const std::string& action, int expiry_minutes = 30);
    
    // Token Verification & Validation
    bool verifyToken(const std::string& token);
    JWTToken parseToken(const std::string& token);
    bool isTokenRevoked(const std::string& token_id);
    bool isTokenExpired(const JWTToken& token);
    bool hasRequiredClaims(const JWTToken& token, const std::vector<std::string>& required_claims);
    
    // Token Management
    bool revokeToken(const std::string& token_id, const std::string& reason, const std::string& revoked_by);
    bool revokeUserTokens(const std::string& user_id, const std::string& reason, const std::string& revoked_by);
    bool revokeSessionTokens(const std::string& session_id, const std::string& reason, const std::string& revoked_by);
    JWTToken refreshAccessToken(const std::string& refresh_token);
    
    // Token Queries
    std::vector<JWTToken> getUserTokens(const std::string& user_id, TokenType type = TokenType::ACCESS_TOKEN);
    std::vector<JWTToken> getActiveTokens();
    int getActiveTokenCount(const std::string& user_id);
    std::chrono::system_clock::time_point getTokenExpiry(const std::string& token);
    
    // Security Features
    bool validateTokenFingerprint(const JWTToken& token, const std::string& current_fingerprint);
    bool checkTokenRateLimit(const std::string& user_id);
    void updateTokenUsage(const std::string& token_id);
    bool isTokenFromTrustedDevice(const JWTToken& token);
    
    // Token Blacklisting
    void addToBlacklist(const std::string& token_id, const std::string& reason);
    bool isBlacklisted(const std::string& token_id);
    void cleanupExpiredTokens();
    void cleanupBlacklist();
    
    // Analytics & Monitoring
    json getTokenStatistics();
    std::map<std::string, int> getTokenUsageByUser();
    std::vector<std::string> getSuspiciousTokenActivity();
    json generateSecurityReport();
    
private:
    // Internal Methods
    std::string signToken(const json& payload);
    bool verifySignature(const std::string& token, const json& payload);
    std::string generateTokenId();
    std::string createDeviceFingerprint(const std::string& user_agent, const std::string& ip);
    bool validateTokenStructure(const json& payload);
    void storeTokenInRedis(const JWTToken& token);
    void removeTokenFromRedis(const std::string& token_id);
    JWTToken loadTokenFromRedis(const std::string& token_id);
};

// Real-time Session Manager (WebSocket + Redis integration)
class AbsolutePenultimateSessionManager {
private:
    // Storage & Caching
    sw::redis::Redis redis_client_;
    std::unordered_map<std::string, UserSession> active_sessions_;
    
    // WebSocket Server for real-time updates
    websocket_server websocket_server_;
    std::thread websocket_thread_;
    std::unordered_map<std::string, websocketpp::connection_hdl> user_connections_;
    
    // Configuration
    int session_timeout_minutes_ = 30;
    int max_sessions_per_user_ = 5;
    bool enable_concurrent_sessions_ = true;
    bool force_single_session_admin_ = false;
    
    // Thread Safety
    mutable std::shared_mutex session_mutex_;
    
    // Security & Monitoring
    std::unordered_map<std::string, int> failed_auth_attempts_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> lockout_times_;
    
public:
    AbsolutePenultimateSessionManager(
        const std::string& redis_conn = "tcp://172.236.23.54:6379",
        int websocket_port = 8028
    );
    ~AbsolutePenultimateSessionManager();
    
    // Initialization
    bool initialize();
    void shutdown();
    void startWebSocketServer();
    void stopWebSocketServer();
    
    // Session Management
    UserSession createSession(const UserAccount& user, const std::string& device_id, const std::string& ip, const std::string& user_agent);
    bool validateSession(const std::string& session_id);
    bool updateSessionActivity(const std::string& session_id);
    bool terminateSession(const std::string& session_id, const std::string& reason);
    bool terminateUserSessions(const std::string& user_id, const std::string& reason, const std::string& exclude_session = "");
    
    // Session Queries
    UserSession getSession(const std::string& session_id);
    std::vector<UserSession> getUserSessions(const std::string& user_id);
    std::vector<UserSession> getActiveSessions();
    int getActiveSessionCount(const std::string& user_id = "");
    
    // Session Security
    bool isSessionCompromised(const std::string& session_id);
    void flagSuspiciousSession(const std::string& session_id, const std::string& reason);
    bool requiresReauthentication(const std::string& session_id);
    void updateSessionRisk(const std::string& session_id, int risk_score);
    
    // Multi-factor Authentication Integration
    bool requiresMFA(const std::string& session_id);
    bool completeMFA(const std::string& session_id, const std::string& method, const std::string& token);
    bool isMFAVerified(const std::string& session_id);
    void resetMFAStatus(const std::string& session_id);
    
    // Real-time Features
    void broadcastToUser(const std::string& user_id, const json& message);
    void broadcastToSession(const std::string& session_id, const json& message);
    void broadcastToAll(const json& message);
    void subscribeToSessionEvents(const std::string& user_id, websocketpp::connection_hdl connection);
    void unsubscribeFromSessionEvents(const std::string& user_id);
    
    // Session Analytics
    json getSessionStatistics();
    std::map<std::string, int> getSessionsByDevice();
    std::map<std::string, int> getSessionsByLocation();
    std::vector<std::string> getAnomalousSession();
    
    // Configuration
    void configureSessionTimeout(int minutes);
    void configureMaxSessions(int max_sessions, bool allow_concurrent = true);
    void enableSingleSessionMode(bool force_admin = false);
    
    // Cleanup & Maintenance
    void cleanupExpiredSessions();
    void performSessionMaintenance();
    bool backupSessionData(const std::string& backup_path);
    bool restoreSessionData(const std::string& backup_path);
    
private:
    // Internal Methods
    std::string generateSessionId();
    void storeSessionInRedis(const UserSession& session);
    void removeSessionFromRedis(const std::string& session_id);
    UserSession loadSessionFromRedis(const std::string& session_id);
    bool enforceSessionLimits(const std::string& user_id);
    void publishSessionEvent(const std::string& event_type, const UserSession& session);
    void handleWebSocketMessage(websocketpp::connection_hdl hdl, websocket_server::message_ptr msg);
    void onWebSocketOpen(websocketpp::connection_hdl hdl);
    void onWebSocketClose(websocketpp::connection_hdl hdl);
    int calculateSessionRisk(const UserSession& session);
    bool isFromTrustedLocation(const std::string& ip, const std::string& user_id);
};

// Advanced Rate Limiter (From back-office security middleware)
class AbsolutePenultimateRateLimiter {
private:
    // Storage
    sw::redis::Redis redis_client_;
    std::vector<RateLimitConfig> rate_limit_configs_;
    
    // In-memory tracking for performance
    std::unordered_map<std::string, std::queue<std::chrono::steady_clock::time_point>> request_windows_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> lockout_times_;
    
    // Configuration
    bool use_sliding_window_ = true;
    bool enable_progressive_penalties_ = true;
    int default_window_size_ = 60;
    
    mutable std::mutex limiter_mutex_;
    
public:
    AbsolutePenultimateRateLimiter(const std::string& redis_conn = "tcp://172.236.23.54:6379");
    ~AbsolutePenultimateRateLimiter();
    
    bool initialize();
    void addRateLimitConfig(const RateLimitConfig& config);
    void removeRateLimitConfig(const std::string& pattern);
    
    // Rate Limiting
    struct RateLimitResult {
        bool allowed;
        int remaining_requests;
        std::chrono::seconds retry_after;
        std::string reason;
        int current_usage;
        int limit;
    };
    
    RateLimitResult checkRateLimit(const std::string& identifier, const std::string& resource, const UserAccount* user = nullptr);
    bool isRateLimited(const std::string& identifier, const std::string& resource);
    void resetRateLimit(const std::string& identifier);
    void addPenalty(const std::string& identifier, int penalty_seconds);
    
    // Analytics
    json getRateLimitStatistics();
    std::vector<std::string> getMostLimitedResources();
    std::map<std::string, int> getTopOffenders();
    
private:
    RateLimitConfig* findMatchingConfig(const std::string& resource);
    int calculateLimit(const RateLimitConfig& config, const UserAccount* user);
    bool checkSlidingWindow(const std::string& key, int limit, int window_seconds);
    void cleanupOldEntries();
};

// Ultimate Authentication Engine (Combining all back-office features)
class AbsolutePenultimateAuthEngine {
private:
    // Core Managers
    std::unique_ptr<AbsolutePenultimatePermissionManager> permission_manager_;
    std::unique_ptr<AbsolutePenultimateJWTManager> jwt_manager_;
    std::unique_ptr<AbsolutePenultimateSessionManager> session_manager_;
    std::unique_ptr<AbsolutePenultimateRateLimiter> rate_limiter_;
    
    // Database Connections
    PGconn* postgresql_connection_;
    sw::redis::Redis redis_client_;
    
    // Configuration
    std::string master_key_;            // Master encryption key
    bool enable_mfa_ = true;
    bool enable_rate_limiting_ = true;
    bool enable_audit_logging_ = true;
    int max_failed_attempts_ = 5;
    
    // Security Features
    std::unordered_set<std::string> trusted_devices_;
    std::unordered_map<std::string, std::vector<AuthMethod>> user_mfa_methods_;
    
public:
    AbsolutePenultimateAuthEngine();
    ~AbsolutePenultimateAuthEngine();
    
    // Initialization
    bool initialize(const std::string& config_path = "");
    void shutdown();
    void loadConfiguration(const std::string& config_path);
    
    // User Management
    bool createUser(const UserAccount& user, const std::string& password);
    bool updateUser(const std::string& user_id, const UserAccount& user);
    bool deleteUser(const std::string& user_id);
    UserAccount getUser(const std::string& user_id);
    UserAccount getUserByEmail(const std::string& email);
    UserAccount getUserByUsername(const std::string& username);
    
    // Authentication
    struct AuthenticationResult {
        bool success;
        std::string error_message;
        UserSession session;
        JWTToken access_token;
        JWTToken refresh_token;
        std::vector<AuthMethod> required_mfa_methods;
        bool requires_password_change;
        bool account_locked;
    };
    
    AuthenticationResult authenticate(const std::string& identifier, const std::string& password, const std::string& device_id, const std::string& ip, const std::string& user_agent);
    AuthenticationResult authenticateWithToken(const std::string& token);
    AuthenticationResult refreshAuthentication(const std::string& refresh_token);
    bool logout(const std::string& session_id);
    bool logoutAllSessions(const std::string& user_id);
    
    // Multi-factor Authentication
    bool setupMFA(const std::string& user_id, AuthMethod method, const std::map<std::string, std::string>& config);
    bool verifyMFA(const std::string& session_id, AuthMethod method, const std::string& token);
    bool disableMFA(const std::string& user_id, AuthMethod method, const std::string& admin_user_id);
    std::vector<AuthMethod> getEnabledMFAMethods(const std::string& user_id);
    
    // Password Management
    bool changePassword(const std::string& user_id, const std::string& old_password, const std::string& new_password);
    bool resetPassword(const std::string& user_id, const std::string& reset_token, const std::string& new_password);
    std::string generatePasswordResetToken(const std::string& user_id);
    bool validatePasswordStrength(const std::string& password);
    
    // Authorization
    bool hasPermission(const std::string& user_id, PermissionType permission, const std::string& context = "");
    bool hasPermission(const std::string& session_id, PermissionType permission, const std::string& context = "");
    std::vector<PermissionType> getUserPermissions(const std::string& user_id);
    std::vector<std::string> getUserRoles(const std::string& user_id);
    
    // Session Management Integration
    bool validateSession(const std::string& session_id);
    UserSession getSession(const std::string& session_id);
    std::vector<UserSession> getUserSessions(const std::string& user_id);
    bool terminateSession(const std::string& session_id, const std::string& reason);
    
    // Security Features
    bool isAccountLocked(const std::string& user_id);
    bool lockAccount(const std::string& user_id, const std::string& reason, int duration_minutes = 30);
    bool unlockAccount(const std::string& user_id, const std::string& admin_user_id);
    void recordFailedAttempt(const std::string& identifier, const std::string& ip);
    bool isIPBlocked(const std::string& ip);
    
    // Device Trust Management
    bool trustDevice(const std::string& user_id, const std::string& device_id, const std::string& device_name);
    bool revokeDeviceTrust(const std::string& user_id, const std::string& device_id);
    std::vector<std::string> getTrustedDevices(const std::string& user_id);
    bool isDeviceTrusted(const std::string& user_id, const std::string& device_id);
    
    // Real-time Features
    void broadcastSecurityAlert(const std::string& user_id, const std::string& alert_type, const json& details);
    void subscribeToAuthEvents(std::function<void(const std::string&, const json&)> callback);
    json getRealTimeSecurityStatus();
    
    // Analytics & Reporting
    json getAuthenticationStatistics();
    json getSecurityReport();
    std::vector<std::string> getSecurityAlerts();
    std::map<std::string, int> getFailedLoginsByIP();
    
    // Component Access
    AbsolutePenultimatePermissionManager* getPermissionManager() { return permission_manager_.get(); }
    AbsolutePenultimateJWTManager* getJWTManager() { return jwt_manager_.get(); }
    AbsolutePenultimateSessionManager* getSessionManager() { return session_manager_.get(); }
    AbsolutePenultimateRateLimiter* getRateLimiter() { return rate_limiter_.get(); }
    
private:
    // Internal Methods
    bool validateUserCredentials(const UserAccount& user, const std::string& password);
    std::string hashPassword(const std::string& password);
    bool verifyPassword(const std::string& password, const std::string& hash);
    std::string generateDeviceId(const std::string& user_agent, const std::string& ip);
    void auditAuthEvent(const std::string& event_type, const std::string& user_id, const json& details);
    bool checkAccountSecurity(const UserAccount& user, const std::string& ip);
    void updateLastLogin(const std::string& user_id);
    bool requiresMFAVerification(const UserAccount& user, const std::string& device_id);
};

// Production Setup Function
void setupAbsolutePenultimateProductionAuth(AbsolutePenultimateAuthEngine& engine);

} // namespace MedusaServer