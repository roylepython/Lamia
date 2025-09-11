#pragma once
/**
 * MEDUSA PORTAL AUTHENTICATION SYSTEM - TRIFORCE DATABASE INTEGRATION
 * ===================================================================
 * 
 * "Authentication powered by the trinity of databases!" ⚔️🛡️🏹🗄️
 * Complete portal authentication using PostgreSQL + MongoDB + Neo4j
 * 
 * TRIFORCE DATABASE ARCHITECTURE:
 * - PostgreSQL (Power): User credentials, sessions, permissions, activity logs
 * - MongoDB (Wisdom): User profiles, preferences, dynamic content, cache
 * - Neo4j (Courage): User relationships, social graph, permission inheritance
 * 
 * USER PERMISSION LEVELS:
 * - client: Basic user access to personal dashboard and public content
 * - developer: Code access, API keys, development tools, project management
 * - moderator: Content moderation, user management, system monitoring
 * - godmode: Full system access, database management, server administration
 * 
 * PRIVATE PORTAL SECTIONS:
 * - Personal Dashboard (all users)
 * - Development Console (developer+)
 * - Moderation Panel (moderator+)
 * - God Mode Interface (godmode only)
 * - Project Management (developer+)
 * - System Analytics (moderator+)
 * - Database Administration (godmode only)
 * 
 * PUBLIC SECTIONS (view-only):
 * - Store/Auction House (browse only, no purchasing without login)
 * - User Profiles (only if user enabled public visibility)
 * - Public galleries and showcases
 * 
 * "One login, three databases, infinite possibilities!" 🚀💎
 */

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <functional>
#include <thread>
#include <future>

// Include our TRIFORCE production schema engine
#include "../inc/libs/3d_generation/production_schema_engine/libproduction_schema_dynamic_engine.hpp"

// Authentication engine integration
#include "security/authentication_engine.h"

// JSON for complex data structures
#include <nlohmann/json.hpp>

// Database connections
#ifdef MEDUSA_MOCK_POSTGRES
typedef void* PGconn;
typedef void* PGresult;
#else
#include <libpq-fe.h>
#endif

namespace medusa {
namespace portal {

// ========================================
// USER PERMISSION LEVELS AND ROLES
// ========================================

enum class UserPermissionLevel {
    NONE = 0,           // No access
    CLIENT = 1,         // Basic user - personal dashboard, public content
    DEVELOPER = 2,      // Code access, API keys, development tools
    MODERATOR = 3,      // Content moderation, user management
    GODMODE = 4         // Full system access, database admin
};

enum class PortalSection {
    // Public sections (available to all, including non-logged-in users)
    PUBLIC_STORE,           // Store/auction browsing (view-only)
    PUBLIC_PROFILES,        // Public user profiles
    PUBLIC_GALLERY,         // Public showcases
    
    // Private sections (require authentication)
    PERSONAL_DASHBOARD,     // User's personal dashboard
    DEVELOPMENT_CONSOLE,    // Code editor, API management
    PROJECT_MANAGEMENT,     // Project creation and management
    MODERATION_PANEL,       // Content moderation tools
    USER_MANAGEMENT,        // User administration
    SYSTEM_ANALYTICS,       // System monitoring and analytics
    DATABASE_ADMIN,         // Database administration
    GOD_MODE_INTERFACE,     // Full system control
    
    // Hybrid sections (public view, private interaction)
    MARKETPLACE,            // Can view all, purchase requires login
    SOCIAL_FEED,           // Public posts visible, interaction requires login
    COMMUNITY_FORUMS       // Read public, post requires login
};

struct UserPermissions {
    UserPermissionLevel level;
    std::vector<PortalSection> allowed_sections;
    std::map<std::string, bool> specific_permissions;
    std::vector<std::string> api_access_keys;
    std::chrono::system_clock::time_point permissions_updated;
    
    // Permission inheritance from groups/roles
    std::vector<std::string> inherited_from_groups;
    std::map<std::string, std::vector<std::string>> custom_permissions;
};

// ========================================
// TRIFORCE DATABASE CONFIGURATION
// ========================================

struct TriforceAuthConfig {
    // PostgreSQL - Power (User data, sessions, logs)
    std::string postgres_host = "172.236.23.54";
    std::string postgres_db = "medusa_rts";
    std::string postgres_user = "medusa";
    std::string postgres_pass = "izJaRuA2kwbNwezvKsCzo7DUNnQc";
    int postgres_port = 5432;
    
    // MongoDB - Wisdom (Profiles, preferences, cache)
    std::string mongo_uri = "mongodb://medusa:izJaRuA2kwbNwezvKsCzo7DUNnQc@172.236.23.54:27017/medusa";
    std::string mongo_db = "medusa_portal";
    
    // Neo4j - Courage (Relationships, social graph)
    std::string neo4j_uri = "bolt://172.236.23.54:7687";
    std::string neo4j_user = "neo4j";
    std::string neo4j_pass = "medusa_graph";
    
    // Connection pooling
    int max_connections_per_db = 15;
    bool enable_connection_pooling = true;
    bool enable_real_time_sync = true;
    int cache_ttl_seconds = 300;
};

// ========================================
// USER DATA STRUCTURES
// ========================================

struct MedusaUser {
    std::string user_id;
    std::string username;
    std::string email;
    std::string password_hash;
    UserPermissionLevel permission_level;
    bool active;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    std::chrono::system_clock::time_point last_login;
    
    // Profile data (from MongoDB)
    struct Profile {
        std::string display_name;
        std::string bio;
        std::string avatar_url;
        std::string banner_url;
        bool public_profile = false;
        std::map<std::string, std::string> social_links;
        std::vector<std::string> interests;
        std::map<std::string, nlohmann::json> custom_fields;
        
        // Developer-specific profile data
        struct DeveloperProfile {
            std::vector<std::string> programming_languages;
            std::vector<std::string> specializations;
            std::string github_username;
            std::string portfolio_url;
            int years_experience = 0;
            std::vector<std::string> certifications;
        } developer_profile;
        
        // Moderator-specific profile data
        struct ModeratorProfile {
            std::vector<std::string> moderation_areas;
            int cases_handled = 0;
            double satisfaction_rating = 0.0;
            std::chrono::system_clock::time_point moderator_since;
        } moderator_profile;
    } profile;
    
    // Permissions and access
    UserPermissions permissions;
    
    // Activity and statistics
    struct ActivityStats {
        int login_count = 0;
        std::chrono::system_clock::time_point last_activity;
        std::vector<std::string> recent_actions;
        std::map<std::string, int> feature_usage_counts;
        double total_session_time_hours = 0.0;
    } activity_stats;
    
    // Social connections (from Neo4j)
    struct SocialConnections {
        std::vector<std::string> following;
        std::vector<std::string> followers;
        std::vector<std::string> blocked_users;
        std::vector<std::string> group_memberships;
        std::map<std::string, std::string> relationship_types;
    } social_connections;
};

struct UserSession {
    std::string session_id;
    std::string user_id;
    std::string session_token;
    UserPermissionLevel permission_level;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point last_activity;
    std::chrono::system_clock::time_point expires_at;
    bool is_active;
    
    // Session metadata
    std::string ip_address;
    std::string user_agent;
    std::string client_info;
    std::map<std::string, std::string> session_data;
    
    // Security tracking
    std::vector<std::string> accessed_sections;
    std::vector<std::string> performed_actions;
    bool elevated_privileges_used = false;
};

// ========================================
// TRIFORCE AUTHENTICATION MANAGER
// ========================================

class TriforceAuthenticationManager {
public:
    TriforceAuthenticationManager(const TriforceAuthConfig& config = TriforceAuthConfig());
    ~TriforceAuthenticationManager();
    
    // System initialization
    bool initialize_triforce_auth();
    bool shutdown_triforce_auth();
    bool is_triforce_ready() const;
    
    // User authentication (using all three databases)
    std::future<BleedingHeart::Security::AuthResult> authenticate_user_async(
        const std::string& username, 
        const std::string& password);
    
    BleedingHeart::Security::AuthResult authenticate_user_blocking(
        const std::string& username, 
        const std::string& password);
    
    // Session management (PostgreSQL + MongoDB + Neo4j)
    std::string create_user_session(const MedusaUser& user, const std::string& ip_address);
    bool validate_session(const std::string& session_token, MedusaUser& user);
    bool refresh_session(const std::string& session_token);
    bool terminate_session(const std::string& session_token);
    std::vector<UserSession> get_active_sessions(const std::string& user_id);
    
    // User management across TRIFORCE
    std::future<std::optional<MedusaUser>> get_user_async(const std::string& user_id);
    std::optional<MedusaUser> get_user_blocking(const std::string& user_id);
    std::optional<MedusaUser> get_user_by_username(const std::string& username);
    std::optional<MedusaUser> get_user_by_email(const std::string& email);
    
    bool create_user(const MedusaUser& user);
    bool update_user(const MedusaUser& user);
    bool delete_user(const std::string& user_id);
    
    // Permission management (PostgreSQL + Neo4j for inheritance)
    bool check_permission(const std::string& user_id, PortalSection section);
    bool check_custom_permission(const std::string& user_id, const std::string& permission);
    bool update_user_permissions(const std::string& user_id, const UserPermissions& permissions);
    UserPermissions calculate_effective_permissions(const std::string& user_id);
    
    // Social connections (Neo4j)
    bool add_social_connection(const std::string& user_id, const std::string& target_user_id, 
                              const std::string& relationship_type);
    bool remove_social_connection(const std::string& user_id, const std::string& target_user_id);
    std::vector<std::string> get_user_connections(const std::string& user_id, 
                                                  const std::string& relationship_type = "");
    
    // Activity tracking (PostgreSQL + MongoDB)
    void log_user_activity(const std::string& user_id, const std::string& activity_type,
                          const nlohmann::json& activity_data = nlohmann::json::object());
    std::vector<nlohmann::json> get_user_activity_log(const std::string& user_id, int limit = 100);
    
    // User statistics and analytics
    std::map<std::string, int> get_user_statistics();
    std::map<UserPermissionLevel, int> get_permission_level_distribution();
    std::vector<std::string> get_most_active_users(int limit = 10);
    
    // Component access
    std::shared_ptr<production_schema::AsyncConnectionPool> get_postgres_pool() { return postgres_pool_; }
    
    // Performance monitoring
    struct TriforceAuthMetrics {
        std::atomic<uint64_t> auth_requests{0};
        std::atomic<uint64_t> successful_auths{0};
        std::atomic<uint64_t> failed_auths{0};
        std::atomic<uint64_t> sessions_created{0};
        std::atomic<uint64_t> sessions_validated{0};
        std::atomic<double> avg_auth_time_ms{0.0};
        std::atomic<double> avg_session_validation_time_ms{0.0};
        std::atomic<int> active_sessions{0};
        std::atomic<int> total_users{0};
    } metrics;
    
private:
    TriforceAuthConfig config_;
    
    // TRIFORCE database connections
    std::shared_ptr<production_schema::AsyncConnectionPool> postgres_pool_;
    void* mongo_client_;    // MongoDB client
    void* neo4j_driver_;    // Neo4j driver
    
    // Integrated authentication engine
    std::unique_ptr<BleedingHeart::Security::IAuthenticationEngine> auth_engine_;
    
    // Cache and session management
    std::map<std::string, UserSession> active_sessions_;
    std::map<std::string, MedusaUser> user_cache_;
    mutable std::shared_mutex auth_mutex_;
    
    // Background processes
    std::thread session_cleanup_thread_;
    std::thread cache_refresh_thread_;
    std::atomic<bool> shutdown_requested_{false};
    
    // System state
    std::atomic<bool> triforce_initialized_{false};
    std::chrono::system_clock::time_point system_start_time_;
    
    // Internal TRIFORCE operations
    bool connect_to_postgres();
    bool connect_to_mongodb();
    bool connect_to_neo4j();
    void disconnect_all_databases();
    
    // PostgreSQL operations (User data, sessions, logs)
    MedusaUser load_user_from_postgres(const std::string& user_id);
    bool save_user_to_postgres(const MedusaUser& user);
    UserSession load_session_from_postgres(const std::string& session_token);
    bool save_session_to_postgres(const UserSession& session);
    void log_activity_to_postgres(const std::string& user_id, const std::string& activity_type,
                                 const nlohmann::json& activity_data);
    
    // MongoDB operations (Profiles, preferences, cache)
    nlohmann::json load_user_profile_from_mongo(const std::string& user_id);
    bool save_user_profile_to_mongo(const std::string& user_id, const nlohmann::json& profile);
    void cache_user_data_in_mongo(const std::string& user_id, const nlohmann::json& data);
    std::optional<nlohmann::json> get_cached_data_from_mongo(const std::string& cache_key);
    
    // Neo4j operations (Social connections, permission inheritance)
    std::vector<std::string> get_social_connections_from_neo4j(const std::string& user_id, 
                                                              const std::string& relationship_type);
    bool create_social_relationship_in_neo4j(const std::string& user_id, const std::string& target_user_id,
                                            const std::string& relationship_type);
    UserPermissions calculate_inherited_permissions_from_neo4j(const std::string& user_id);
    
    // Background maintenance
    void session_cleanup_worker();
    void cache_refresh_worker();
    void cleanup_expired_sessions();
    void refresh_user_cache();
    
    // Security and validation
    std::string generate_secure_session_token();
    bool validate_session_security(const UserSession& session);
    void update_session_activity(const std::string& session_token);
    
    // Performance optimization
    void update_auth_metrics(bool success, double auth_time_ms);
    std::map<std::string, uint64_t> get_triforce_metrics() const;
};

// ========================================
// PORTAL SECTION ACCESS CONTROLLER
// ========================================

class PortalAccessController {
public:
    PortalAccessController(std::shared_ptr<TriforceAuthenticationManager> auth_manager);
    ~PortalAccessController();
    
    // Access control for portal sections
    bool can_access_section(const std::string& session_token, PortalSection section);
    std::vector<PortalSection> get_accessible_sections(const std::string& session_token);
    
    // Permission-based access control
    bool has_permission(const std::string& session_token, const std::string& permission);
    bool has_minimum_level(const std::string& session_token, UserPermissionLevel min_level);
    
    // Dynamic permission checking
    bool can_view_user_profile(const std::string& session_token, const std::string& target_user_id);
    bool can_purchase_from_store(const std::string& session_token);
    bool can_moderate_content(const std::string& session_token);
    bool can_access_database(const std::string& session_token);
    
    // Portal navigation
    std::vector<std::string> get_navigation_menu(const std::string& session_token);
    std::string get_default_dashboard_url(const std::string& session_token);
    
    // Content filtering for public access
    nlohmann::json filter_content_for_access_level(const nlohmann::json& content, 
                                                   const std::string& session_token);
    
private:
    std::shared_ptr<TriforceAuthenticationManager> auth_manager_;
    
    // Access control rules
    std::map<PortalSection, UserPermissionLevel> section_requirements_;
    std::map<std::string, UserPermissionLevel> permission_requirements_;
    
    // Initialize access rules
    void initialize_access_rules();
    void setup_section_requirements();
    void setup_permission_requirements();
    
    // Internal access checking
    UserPermissionLevel get_user_level_from_session(const std::string& session_token);
    bool check_section_access(UserPermissionLevel user_level, PortalSection section);
};

// ========================================
// USER DASHBOARD GENERATOR
// ========================================

class UserDashboardGenerator {
public:
    UserDashboardGenerator(std::shared_ptr<TriforceAuthenticationManager> auth_manager,
                          std::shared_ptr<PortalAccessController> access_controller);
    ~UserDashboardGenerator();
    
    // Dashboard generation for different user levels
    nlohmann::json generate_client_dashboard(const std::string& session_token);
    nlohmann::json generate_developer_dashboard(const std::string& session_token);
    nlohmann::json generate_moderator_dashboard(const std::string& session_token);
    nlohmann::json generate_godmode_dashboard(const std::string& session_token);
    
    // Dynamic dashboard based on user level
    nlohmann::json generate_user_dashboard(const std::string& session_token);
    
    // Dashboard components
    nlohmann::json get_user_statistics(const std::string& session_token);
    nlohmann::json get_recent_activity(const std::string& session_token);
    nlohmann::json get_quick_actions(const std::string& session_token);
    nlohmann::json get_notifications(const std::string& session_token);
    nlohmann::json get_system_status(const std::string& session_token);
    
    // Specialized dashboard components
    nlohmann::json get_development_tools(const std::string& session_token);
    nlohmann::json get_moderation_queue(const std::string& session_token);
    nlohmann::json get_system_administration(const std::string& session_token);
    
private:
    std::shared_ptr<TriforceAuthenticationManager> auth_manager_;
    std::shared_ptr<PortalAccessController> access_controller_;
    
    // Dashboard component builders
    nlohmann::json build_dashboard_header(const MedusaUser& user);
    nlohmann::json build_navigation_menu(const std::string& session_token);
    nlohmann::json build_widget_grid(const std::string& session_token, UserPermissionLevel level);
    nlohmann::json build_footer_info(const MedusaUser& user);
    
    // Permission-specific components
    nlohmann::json build_client_widgets(const std::string& session_token);
    nlohmann::json build_developer_widgets(const std::string& session_token);
    nlohmann::json build_moderator_widgets(const std::string& session_token);
    nlohmann::json build_godmode_widgets(const std::string& session_token);
};

} // namespace portal
} // namespace medusa

// C API for web integration
extern "C" {
    // Portal system lifecycle
    bool medusa_portal_init_triforce_auth(const char* config_json);
    void medusa_portal_cleanup_triforce_auth();
    
    // Authentication operations
    const char* medusa_portal_authenticate_user(const char* username, const char* password, const char* ip_address);
    bool medusa_portal_validate_session(const char* session_token);
    bool medusa_portal_logout_user(const char* session_token);
    
    // Permission checking
    bool medusa_portal_can_access_section(const char* session_token, int portal_section);
    bool medusa_portal_has_permission(const char* session_token, const char* permission);
    const char* medusa_portal_get_user_level(const char* session_token);
    
    // Dashboard generation
    const char* medusa_portal_generate_dashboard(const char* session_token);
    const char* medusa_portal_get_navigation_menu(const char* session_token);
    
    // User management
    const char* medusa_portal_get_user_profile(const char* session_token, const char* user_id);
    bool medusa_portal_update_user_profile(const char* session_token, const char* profile_json);
    
    // Activity tracking
    void medusa_portal_log_user_activity(const char* session_token, const char* activity_type, const char* activity_data);
    const char* medusa_portal_get_user_activity(const char* session_token, int limit);
    
    // System monitoring
    const char* medusa_portal_get_triforce_metrics();
    const char* medusa_portal_get_system_status();
}