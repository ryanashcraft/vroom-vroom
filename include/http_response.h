
#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <string>
#include <vector>

namespace vv {
	using namespace std;

	/**
	 * Defines logic and holds attributes required to generate HTTP response strings.
	 */
	class HTTPResponse {
	public:
		/**
		 * Creates an HTTPResponse instance with the an HTTP code, response content, and other attributes.
		 */
		HTTPResponse(int code, string& mime, string& content, vector<string>& headers, bool head=false);

		/**
		 * Creates an HTTPResponse instance with a custom HTTP status string, response content, and other
		 * attributes.
		 */
		HTTPResponse(string& status, string& mime, string& content, vector<string>& headers, bool head=false);

		/**
		 * Creates an HTTPResponse instance with an HTTP code and no content.
		 */
		HTTPResponse(int code, bool head=false);

		/**
		 * Return the generated HTTP response string.
		 * @return the generated HTTP response
		 */
		string str();

		/**
		 * Retursn the integer status code form an HTTP string.
		 * @param  status the HTTP status code string
		 * @return        the integer HTTP status code
		 */
		static int status_to_code(string& status);

		/**
		 * Sets if generated string should include the content in the response.
		 * @param head_only if true, does not include the content in the response
		 */
		void set_head_only(bool head_only) {
			head_ = head_only;
		}

		/**
		 * Sets if generated string should contain header for client to close connection.
		 * @param close if true, includes header to signal end of connection
		 */
		void set_connection_close(bool close) {
			close_ = close;
		}

		/**
		 * Returns the HTTP code for the response.
		 * @return the HTTP code
		 */
		int get_code() {
			return code_;
		}
	private:
		/**
		 * The HTTP code.
		 */
		int code_;

		/**
		 * The HTTP status code string.
		 */
		string status_;

		/**
		 * The content mime type string.
		 */
		string mime_;

		/**
		 * The response content string.
		 */
		string content_;

		/**
		 * Additional headers for the response.
		 */
		vector<string> headers_;

		/**
		 * If true, does not include content in response.
		 */
		bool head_;

		/**
		 * If true, includes a header to signal end of connection with client.
		 */
		bool close_;
	};
}

#endif
