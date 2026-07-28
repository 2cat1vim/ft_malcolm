FROM debian:12
RUN apt-get update
RUN apt-get install -y iproute2
RUN apt-get install -y net-tools
RUN apt-get install -y make
RUN apt-get install -y clang
RUN mkdir /malcolm
ADD Makefile /malcolm/
ADD src/ /malcolm/src/
