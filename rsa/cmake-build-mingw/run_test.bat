@ECHO OFF
ECHO ==== RSA factorization attack test suite ====
SET /P key_bits=Enter key pair size in bits to generate: 
ECHO ==== generate key pair ====
rsa-keygen %key_bits%
ECHO ==== key pair generated ====
ECHO ==== running tests with %key_bits% bits key (no wiener key) ====
ECHO ==== running knj attack ====
knj public.pem
ECHO ==== running pollard rho attack ====
pollard public.pem
ECHO ==== running quadratic sieve attack ====
sieve public.pem
ECHO ==== generate wiener key pair ====
rsa-keygen %key_bits% --wiener
ECHO ==== key pair generated ====
ECHO ==== running tests with %key_bits% bits key (wiener key) ====
ECHO ==== running knj attack ====
knj public.pem
ECHO ==== running pollard rho attack ====
pollard public.pem
ECHO ==== running quadratic sieve attack ====
sieve public.pem
