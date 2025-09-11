/*
 * Authentication Engine - Interface Definition
 * Bleeding Heart Foundation - Security & Encryption Manager Library
 * 
 * PERFORMANCE NOTICE: C++ interfaces for absolute lightspeed authentication
 * ENIGMA Scale integration with Yorkshire Champion performance
 */

#pragma once

#include "../foundation/brass_plate.h"
#include <string>
#include <chrono>

#ifdef _WIN32
    #ifdef SECURITY_MANAGER_LIBRARY
        #define AUTH_ENGINE_API __declspec(dllexport)
    #else
        #define AUTH_ENGINE_API __declspec(dllimport)
    #endif
#else
    #define AUTH_ENGINE_API __attribute__((visibility("default")))
#endif

namespace BleedingHeart {
namespace Security {

/*
 * Authentication Result Structure
 * Lightweight result structure for absolute lightspeed
 */
struct AuthResult {
    bool success = false;
    std::string username;
    std::string session_token;
    int auth_level = 0;          // 0=none, 1=basic, 2=elevated
    std::string error_message;
    double processing_time_ms = 0.0;
    
    // Yorkshire Champion performance indicators
    bool is_lightspeed_performance() const {
        return processing_time_ms < 3.15;  // ENIGMA_CONSTANT * 10
    }
};

/*
 * Authentication Statistics
 * Real-time performance metrics for absolute lightspeed monitoring
 */
struct AuthStats {
    uint64_t total_requests = 0;
    uint64_t successful_requests = 0;
    uint64_t failed_requests = 0;
    double average_processing_time_ms = 0.0;
    int active_sessions = 0;
    int locked_users = 0;
    bool is_lightspeed_performance = false;
    
    // Calculate success rate
    double GetSuccessRate() const {
        return total_requests > 0 ? 
               (static_cast<double>(successful_requests) / total_requests) * 100.0 : 0.0;
    }
};

/*
 * IAuthenticationEngine - Absolute lightspeed authentication interface
 * 
 * Provides enterprise-grade authentication with Yorkshire Champion performance
 * Integrated with ENIGMA Scale (0.315) for optimal speed
 */
class AUTH_ENGINE_API IAuthenticationEngine : public IComponent {
public:
    virtual ~IAuthenticationEngine() = default;
    
    // Authenticate user with lightspeed performance
    virtual AuthResult AuthenticateUser(const std::string& username, 
                                       const std::string& password,
                                       const std::string& additional_data = "") = 0;
    
    // Validate session token (ultra-fast validation)
    virtual bool ValidateSession(const std::string& session_token, 
                                std::string& username, 
                                int& auth_level) = 0;
    
    // Logout user and invalidate session
    virtual bool LogoutUser(const std::string& session_token) = 0;
    
    // Generate authentication challenge for enhanced security
    virtual std::string GenerateChallenge(const std::string& username) = 0;
    
    // Validate challenge response
    virtual bool ValidateChallengeResponse(const std::string& username,
                                          const std::string& challenge,
                                          const std::string& response) = 0;
    
    // Get real-time authentication statistics
    virtual AuthStats GetAuthenticationStats() const = 0;
    
    // Get Yorkshire component name
    virtual std::string GetYorkshireName() const = 0;
    
    // Check if can run as standalone application
    virtual bool CanRunStandalone() const = 0;
    
    // Secure shutdown with cleanup
    virtual void Shutdown() = 0;
};

/*
 * Authentication Engine Factory - For library/DLL usage
 */
extern "C" {
    AUTH_ENGINE_API IAuthenticationEngine* CreateAuthenticationEngine();
    AUTH_ENGINE_API void DestroyAuthenticationEngine(IAuthenticationEngine* engine);
    AUTH_ENGINE_API const char* GetAuthenticationEngineName();
    AUTH_ENGINE_API const char* GetAuthenticationEngineVersion();
    AUTH_ENGINE_API bool TestAuthenticationEngine();
}

} // namespace Security
} // namespace BleedingHeart