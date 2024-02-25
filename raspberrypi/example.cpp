#include <iostream>
#include <curl/curl.h>

// 콜백 함수: CURL WRITEFUNCTION에 전달할 함수
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *response) {
    size_t total_size = size * nmemb;
    response->append((char *)contents, total_size);
    return total_size;
}

int main() {
    CURL *curl;
    CURLcode res;
    std::string url = "http://your-server-url.com";
    std::string post_data = "key1=value1&key2=value2"; // POST할 데이터

    // libcurl 초기화
    curl_global_init(CURL_GLOBAL_ALL);

    // CURL 핸들 생성
    curl = curl_easy_init();
    if (curl) {
        // POST 요청 설정
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        
        // 서버 응답을 저장할 문자열 초기화
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // HTTP POST 요청 보내기
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to perform HTTP POST request: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "HTTP POST request successful!" << std::endl;
            std::cout << "Response: " << response << std::endl;
        }

        // CURL 핸들 정리
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize CURL!" << std::endl;
    }

    // libcurl 종료
    curl_global_cleanup();

    return 0;
}

