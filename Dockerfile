FROM alpine:3.8
MAINTAINER Salvador Guzman <guzmansalv@gmail.com>

WORKDIR /root
ADD . dbase-channel

RUN apk add --no-cache --virtual build-dependencies build-base cmake git libtool automake autoconf curl-dev postgresql-dev \
    && cd /root \
    && git clone https://github.com/google/gumbo-parser \
    && git clone https://github.com/lazytiger/gumbo-query \
    && git clone https://github.com/jtv/libpqxx \
    && cd libpqxx \
    && mkdir build \
    && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make \
    && make install \
    && cd ../../ \
    && cd gumbo-parser \
    && sh autogen.sh \
    && ./configure \
    && make \
    && make install \
    && cd ../gumbo-query/build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make \
    && make install \
    && cd ../../ \
    && cd dbase-channel \
    && mkdir build \
    && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make \
    && mv dbase_channels ../../ \
    && cd ../../ \
    && rm -r dbase-channel gumbo-parser gumbo-query libpqxx \
    && apk del build-dependencies

ENTRYPOINT ['/root//dbase-channel/dbase_channel']