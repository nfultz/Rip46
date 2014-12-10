
#' @export
as.data.frame.ip4 <- as.data.frame.numeric

#' @export
format.ip4 <- function(x, ...) ip4ToHost(x)

#' @export
print.ip4 <- function(x) print(ip4ToHost(x))