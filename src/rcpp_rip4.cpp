#include <Rcpp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace Rcpp;

union un_int
{
    int i;
    unsigned int ui;
    unsigned char data[sizeof(int)];
};

//' Convert MySQL ATONs to Rip4
//'
//' When you query a MySQL db, you should use INET_ATON(ip_field) to convert a string IP 
//' to an (unsigned) int.
//' 
//' Unfortunately, the RMySQL driver will convert these to doubles because not all
//' unsigned ints are representable as ints. Because doubles get normalized, we 
//' can't mask subnets directly.
//'  
//' Instead, we can map the top half of unsigned integers to the negative half of signed
//' ints using 2s-complement.  
//'
//' @param x input numeric vector
//' @return integer format IP addresses
//'
//'
// [[Rcpp::export]]
IntegerVector mySqlToIp4(NumericVector x) {
  unsigned int u = 1U << 31;
   IntegerVector ret(x.length());
   for(int i = 0; i < x.length(); i++) {
     if(R_IsNA(x[i])){
       ret[i] = NA_INTEGER;
     }
     else {
       unsigned int xi = (unsigned int) x[i];
       ret[i] = (int) htonl(xi < u ?  xi : xi - u);
     }
   }
   ret.attr("class") = "ip4";
   return ret;
}

// Convert dotted-quad IP addresses to Rip4
// 
// [[Rcpp::export]]
IntegerVector hostToIp4(CharacterVector x) {
  IntegerVector ret(x.length());
  
  un_int buf;
  
  
  for(int i = 0; i < x.length(); i++){
    if(x[i] == NA_STRING) 
      ret[i] = NA_INTEGER;
    else if(inet_pton(AF_INET, x[i], buf.data) == 1)
      ret[i] = buf.i; 
    else ret[i] = NA_INTEGER;  
  }
  
  ret.attr("class") = "ip4";
  return ret;   
  
} 

// Convert Rip4 to dotted-quad IP addresses
// 
// [[Rcpp::export]]
CharacterVector ip4ToHost(IntegerVector x) {
  char str[INET_ADDRSTRLEN];
  un_int buf;
  
  CharacterVector ret(x.length());

  for(int i = 0; i < x.length(); i++){
    buf.i = x[i];
    if(buf.i == NA_INTEGER) 
      ret[i] = NA_STRING;
    else if( inet_ntop(AF_INET, buf.data, str, INET_ADDRSTRLEN) != NULL)
      ret[i] =  str; 
    else 
      ret[i] = NA_STRING;
  }
  return ret;


}

//' Extract Networks from IP addresses
//'
//' @rdname network-mask
//' @param x an \code{ip4} object
//' @param m a bit mask
//' @export
// [[Rcpp::export]]
IntegerVector mask(IntegerVector x,  int m) {
  IntegerVector ret(x.length()); 
   for(int i = 0; i < x.length(); i++)
     ret[i] = m & x[i];
       
   ret.attr("class") = "ip4";
   return ret;
  
}


//' @rdname network-mask
//' @export
// [[Rcpp::export]]
IntegerVector classA(IntegerVector x) {
     return mask(x, 0x000000ff);
}

//' @rdname network-mask 
//' @export
// [[Rcpp::export]]
IntegerVector classB(IntegerVector x) {
     return mask(x, 0x0000ffff);
}

//' @rdname network-mask
//' @export
// [[Rcpp::export]]
IntegerVector classC(IntegerVector x) {
     return mask(x, 0x00ffffff);
}
