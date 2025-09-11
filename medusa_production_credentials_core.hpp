/*
 * Medusa Production Credentials - Interface Definition
 * Bleeding Heart Foundation - Security & Encryption Manager Library
 * 
 * PRODUCTION INTEGRATION: Interface for backend server and current codebase integration
 * Triforce authentication, texture engine security, and production deployment support
 */

#pragma once

#include "credential_manager.h"
#include "yorkshire_encryption_vault.h"
#include <string>
#include <vector>
#include <map>

#ifdef _WIN32
    #ifdef SECURITY_MANAGER_LIBRARY
        #define PRODUCTION_CREDS_API __declspec(dllexport)
    #else
        #define PRODUCTION_CREDS_API __declspec(dllimport)
    #endif
#else
    #define PRODUCTION_CREDS_API __attribute__((visibility("default")))
#endif

namespace BleedingHeart {
namespace Security {
namespace Production {

// Forward declaration
class MedusaProductionCredentials;

// Server connection information structure
struct ServerConnection {
    std::string server_id;
    std::string public_ip;
    std::string vpc_ip;
    std::string username;
    std::string password;
    std::string additional_info;
    bool connection_ready = false;
};

/*
 * Production Credentials Factory - For library/DLL usage
 */
extern "C" {
    PRODUCTION_CREDS_API MedusaProductionCredentials* CreateMedusaProductionCredentials();
    PRODUCTION_CREDS_API void DestroyMedusaProductionCredentials(MedusaProductionCredentials* credentials);
    PRODUCTION_CREDS_API const char* GetMedusaProductionCredentialsName();
    PRODUCTION_CREDS_API const char* GetMedusaProductionCredentialsVersion();
}

} // namespace Production
} // namespace Security
} // namespace BleedingHeart