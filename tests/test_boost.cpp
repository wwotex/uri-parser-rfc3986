#include <iostream>
#include <boost/url.hpp>

int main() {
    // boost::core::string_view s = "https://user:pass@example.com:443/path/to/my%2dfile.txt?id=42&name=John%20Doe+Jingleheimer%2DSchmidt#page%20anchor";
    // boost::system::result<boost::url_view> r = boost::urls::parse_uri( s );
    // boost::url_view u = r.value();
    //
    boost::url_view u("https://user@[0012:01:002::]:12/path");
    std::cout <<
            "url       : " << u << "\n"
            "scheme    : " << u.scheme() << "\n"
            "authority : " << u.authority() << "\n"
            "userinfo  : " << u.userinfo() << "\n"
            "host      : " << u.host() << "\n"
            "ipv4      : " << u.host_ipv4_address() << "\n"
            "ipv6      : " << u.host_ipv6_address() << "\n"
            "hostname  : " << u.host_name() << "\n"
            "port      : " << u.port() << "\n";
}
