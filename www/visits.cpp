#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

inline void visit_page(const string& filename, const string& remote_addr) {
    ofstream out(filename, ios_base::app);
    out << remote_addr << endl;
}

inline int count_visits(const string& filename, const string& remote_addr) {
    int count = 0;

    ifstream in(filename); 
    string line;
    while (getline(in, line))
        if (line == remote_addr)
            ++count;

    return count;
}

inline int calculate_content_length(const string& body) {
    int content_lenght = body.length() * sizeof(char); 
    return content_lenght;
}

void write_html_response(const string& body_content) {
    string body = "<!DOCTYPE html>\n<html style='height: 100%; display: table; margin: auto;'>"
        "<head><title>IP-based Visits Counter</title></head>"
        "<body style='font-weight:bold; font-size:5.5em; height: 100%; display:table-cell; vertical-align:middle;'>" 
        + body_content + 
        "</body></html>\n";
    
    int content_length = calculate_content_length(body);

    cout << "Content-Type: text/html; charset=utf-8\n"
        "Content-Length: " + to_string(content_length) + "\n"
        "\n" + body;
}

int main() {
    const string FILENAME = "visits.txt";
    const string REMOTE_ADDR = getenv("REMOTE_ADDR") == 0 ? "" : getenv("REMOTE_ADDR");

    if (REMOTE_ADDR.empty()) {
        write_html_response("Can`t locate remote address.");
             
        return EXIT_FAILURE;
    }

    visit_page(FILENAME, REMOTE_ADDR);
    int count = count_visits(FILENAME, REMOTE_ADDR);

    string body_content = "Visitor: " + REMOTE_ADDR + "\nVisits: " + to_string(count) +  "\n";
    write_html_response(body_content);

    return EXIT_SUCCESS;
}
