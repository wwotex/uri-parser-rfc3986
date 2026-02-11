FROM gcc:13.3.0

RUN apt update && apt install -y cmake

WORKDIR /home

ARG TARGET
COPY ./include /home/include
COPY ./src /home/src
COPY ./tests /home/tests
COPY ./CMakeLists.txt /home/CMakeLists.txt
RUN cmake -S . -B /home/build && cmake --build /home/build --target ${TARGET}
