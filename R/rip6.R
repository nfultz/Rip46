
#' @export
as.data.frame.ip6 <- as.data.frame.complex

#' @export
format.ip6 <- function(x, ...) ip6ToHost(x)

#' @export
print.ip6 <- function(x) print(ip6ToHost(x))

#' @export
`[.ip6` <- function(x, ...) `oldClass<-`( unclass(x)[...], oldClass(x))

#' @export
`[<-.ip6` <- function(x,i, value) {
  f <- switch(storage.mode(value), complex=identity, character=hostToIp6, stop)
  `oldClass<-`( `[<-`(unclass(x), i, f(value)), oldClass(x))
}

#' @export
Ops.ip6 <- function(x, ...) 
  stop(gettextf("%s not defined for \"ip6\" objects", .Generic), domain = NA)

#' @export
Math.ip6 <- function(x, ...) 
  stop(gettextf("%s not defined for \"ip6\" objects", .Generic), domain = NA)