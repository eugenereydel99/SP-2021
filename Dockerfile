FROM debian
WORKDIR /docs
COPY . .
RUN chmod +x script.sh && ./script.sh
