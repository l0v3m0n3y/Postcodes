#ifndef POSTCODES_H
#define POSTCODES_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Postcodes {
private:
    std::string api_base = "https://api.postcodes.io";
    http_client_config client_config;
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("api.postcodes.io"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Postcodes(){
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> random_postcodes() {
        return make_api_call("/random/postcodes","GET");
    }

    pplx::task<json::value> random_places(const std::string& title) {
        return make_api_call("/random/places","GET");
    }

    pplx::task<json::value> postcodes_by_id(const std::string& postcodes_id) {
        return make_api_call("/postcodes/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(postcodes_id))),"GET");
    }

    pplx::task<json::value> scotland_postcodes_by_id(const std::string& character_id) {
        return make_api_call("/scotland/postcodes/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(character_id))),"GET");
    }

    pplx::task<json::value> terminated_postcodes_by_id(const std::string& character_id) {
        return make_api_call("/terminated_postcodes/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(character_id))),"GET");
    }

    pplx::task<json::value> places_by_id(const std::string& character_id) {
        return make_api_call("/places/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(character_id))),"GET");
    }

    pplx::task<json::value> search_places(const std::string& city,int limit) {
        return make_api_call("/places?q=" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(city))) + "&limit=" + std::to_string(limit),"GET");
    }
};

#endif
