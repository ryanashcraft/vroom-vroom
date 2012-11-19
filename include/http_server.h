
#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <vector>
#include <unordered_map>

#include "server.h"
#include "http_response.h"
#include "http_exception.h"

namespace vv {
	using namespace std;

	/**
	 * Handles HTTP requests from clients, and processes response to send back generated HTTP
	 * response.
	 */
	class HTTPServer : public vv::Server {
	public:
		/**
		 * Creates an HTTPServer with a given port.
		 * @param port	the port to open for the server
		 */
		HTTPServer(unsigned short port);

		/**
		 * Handles a client request in a separate thread.
		 */
		void handle();
	protected:
		/**
		 * Accepts a client socket to receive the request string.
		 * @param  client the client socket
		 * @return        the full HTTP request message
		 */
		static string accept(const Socket& client);	

		/**
		 * Processes an HTTP request to handle the logic required to generate a response.
		 * @param  message the full HTTP request
		 * @return         the HTTP response to the client request
		 */
		static HTTPResponse process(const string& message);
	private:
		/**
		 * Determines if a given HTTP message is complete and valid.
		 * @param  message the HTTP message to validate
		 * @return         true if the given HTTP message is valid
		 */
		static bool is_valid_http_message(string& message);

		/**
		 * Parses the HTTP request to retrieve its POST data, and stores them into an unordered
		 * map to be used when interpreting the file requested.
		 */
		static unordered_map<string, string> parse_post_data(const string& message);

		/**
		 * Parses the HTTP request to retrieve its GET data, and stores them into an unordered
		 * map to be used when interpreting the file requested.
		 */
		static unordered_map<string, string> parse_get_data(const string& message);

		/**
		 * Parses the string of url-encoded key-value pairs from an HTTP request into an unordered
		 * map.
		 */
		static unordered_map<string, string> url_encoded_variables_to_map(const string& content);

		/**
		 * Convert a hexadecimal character into the ASCII value it represents.
		 * @param  ch the hexadecimal-encoded character
		 * @return    the ASCII value the character represents
		 */
		static unsigned char from_hex(unsigned char ch);

		/**
		 * Decodes a URL-encoded string.
		 * @param  str the URL-encoded string
		 * @return     the converted string in ASCII encoding
		 */
		static const string url_decode(const string& str);
	};
}

#endif
