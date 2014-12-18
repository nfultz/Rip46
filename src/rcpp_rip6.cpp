#include <Rcpp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace Rcpp;

union u_double
{
    double  dbl;
    unsigned char data[sizeof(double)];
};

//' Convert presentation IPv6 addresses to Rip6
//' 
//' @export
// [[Rcpp::export]]
ComplexVector hostToIp6(CharacterVector x) {
  
  ComplexVector ret(x.length());
  
  u_double buf[sizeof(struct in6_addr) / sizeof(u_double)];
  
  Rcomplex rc ;
  
  for(int i = 0; i < x.length(); i++){
    inet_pton(AF_INET6, x[i], buf[0].data);
    rc.r = buf[0].dbl;
    rc.i = buf[1].dbl;   
    ret[i] = rc; 
  }
  
  ret.attr("class") = "ip6";
  return ret;
  
} 

//' Convert Rip6 to presentation
//' 
//' @export
// [[Rcpp::export]]
CharacterVector ip6ToHost(ComplexVector x) {

  char str[INET6_ADDRSTRLEN];
  u_double buf[sizeof(struct in6_addr) / sizeof(u_double)];
  
  CharacterVector ret(x.length());
  Rcomplex rc ;

  struct in_addr s;
  for(int i = 0; i < x.length(); i++){
    rc = x(i);
    buf[0].dbl = rc.r;
    buf[1].dbl = rc.i;
    inet_ntop(AF_INET6, buf[0].data, str, INET6_ADDRSTRLEN);
    ret(i) = str;
  }
  return ret;
}



