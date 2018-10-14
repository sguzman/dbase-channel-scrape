FROM alpine
MAINTAINER Salvador Guzman <guzmansalv@gmail.com>

RUN apk add --no-cache build-base cmake git libtool automake autoconf libtoolize curl-dev
