
#' @export
as.data.frame.ip4 <- as.data.frame.numeric

#' @export
format.ip4 <- function(x, ...) ip4ToHost(x)

#' @export
print.ip4 <- function(x) print(ip4ToHost(x))

#' @export
`[.ip4` <- function(x, ...) `oldClass<-`( unclass(x)[...], oldClass(x))

#' @export
`[<-.ip4` <- function(x,i, value) {
  f <- switch(storage.mode(value), integer=identity, double=mySqlToIp4, character=hostToIp4, stop)
  `oldClass<-`( `[<-`(unclass(x), i, f(value)), oldClass(x))
}


#' @export
Ops.ip4 <- function(x, ...) 
  stop(gettextf("%s not defined for \"ip4\" objects", .Generic), domain = NA)

#' @export
Math.ip4 <- function(x, ...) 
  stop(gettextf("%s not defined for \"ip4\" objects", .Generic), domain = NA)