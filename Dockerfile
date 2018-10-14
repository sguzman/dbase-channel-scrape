FROM alpine
MAINTAINER Salvador Guzman <guzmansalv@gmail.com>

RUN apk add --no-cache build-base cmake git libtool automake autoconf curl-dev postgresql-dev

RUN git clone https://github.com/google/gumbo-parser
RUN git clone https://github.com/lazytiger/gumbo-query
RUN git clone https://github.com/sguzman/dbase-channel-scrape

WORKDIR ../../

WORKDIR gumbo-parser
RUN sh autogen.sh
RUN ./configure
RUN make
RUN make install

WORKDIR ../../

WORKDIR gumbo-query
WORKDIR build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make
RUN make install

WORKDIR ../../

WORKDIR dbase-channel-scrape
RUN mkdir build
WORKDIR build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make

RUN mv dbase-channel-scrape ../../
WORKDIR /root

RUN rm -rf dbase-channel-scrape gumbo-parser gumbo-query libpqxx
RUN RUN apk del build-base cmake git libtool automake autoconf curl-dev postgresql-dev

ENTRYPOINT ['dbase-channel-scrape']