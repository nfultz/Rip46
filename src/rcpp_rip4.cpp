#include <Rcpp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace Rcpp;

//' Convert MySQL ATONs to Rip4
//'
//' When you quert a MySQL db, you should call INET_ATON(ip_field) to convert to an
//' (unsigned) int.
//' 
//' Unfortunately, the RMySQL driver will convert these to doubles because not all
//' unsigned ints are representable as ints. Because doubles get normalized, we 
//' can't apply masks directly.
//'  
//' Instead, we can use signed integers.  
//'
//' @param str input numeric vector
//' @return integer format IP addresses
//'
//' @export
// [[Rcpp::export]]
IntegerVector mySqlToIp4(NumericVector x) {
  long u = 1L << 32;
   IntegerVector ret(x.length());
   for(int i = 0; i < x.length(); i++) {
     long xi = (long) x[i];
     ret[i] = (int) (xi < u ?  xi : xi - u);
   }
   ret.attr("class") = "ip4";
   return ret;
}

//' Convert dotted-quad IP addresses to Rip4
//' 
//' @export
// [[Rcpp::export]]
IntegerVector hostToIp4(CharacterVector x) {
   IntegerVector ret(x.length());
  
   for(int i = 0; i < x.length(); i++)
     ret[i] = (int) ntohl(inet_addr(x[i]));
  
   ret.attr("class") = "ip4";
   return ret;
  
} 

//' Convert Rip4 to dotted-quad IP addresses
//' 
//' @export
// [[Rcpp::export]]
CharacterVector ip4ToHost(IntegerVector x) {
  CharacterVector ret(x.length());
  struct in_addr s;
  for(int i = 0; i < x.length(); i++){
    s.s_addr = htonl((unsigned int) x[i]);
    ret(i) = inet_ntoa(s);
  }
  return ret;
}


IntegerVector applyMask(IntegerVector x,  int mask) {
  IntegerVector ret(x.length()); 
   for(int i = 0; i < x.length(); i++)
     ret[i] = mask & x[i];
       
   ret.attr("class") = "ip4";
   return ret;
  
}


//' Extract Networks from IP addresses
//'
//' @rdname network-mask
//' @export
// [[Rcpp::export]]
IntegerVector classA(IntegerVector x) {
     return applyMask(x, 0xff000000);
}

//' @rdname network-mask 
//' @export
// [[Rcpp::export]]
IntegerVector classB(IntegerVector x) {
     return applyMask(x, 0xffff0000);
}

//' @rdname network-mask
//' @export
// [[Rcpp::export]]
IntegerVector classC(IntegerVector x) {
     return applyMask(x, 0xffffff00);
}
