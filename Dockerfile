FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    libssl-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Build with OpenSSL support (needed for HTTPS calls to Groq/Gemini)
# Compiles main.cpp plus every .cpp file under src/ together
RUN g++ -std=c++17 -O2 main.cpp $(find src -name '*.cpp') -o db \
    -DCPPHTTPLIB_OPENSSL_SUPPORT \
    -lssl -lcrypto -lpthread

# Render sets $PORT at runtime; our app reads it via getenv
EXPOSE 8080

CMD ["./db"]