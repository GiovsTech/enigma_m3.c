#ifndef ROTORS_H
#define ROTORS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#define ALPHABET_SIZE 26
#define PLUGBOARD_SIZE 10

typedef uint8_t Mapping[ALPHABET_SIZE];

typedef enum {
    RO_FORWARD,
    RO_BACKWARD
} RotorOrder;

typedef struct{
    Mapping forward_mapping;
    Mapping backward_mapping;
    uint8_t position; // rotor position during rotation
    uint8_t ring; // indica l'inizio dell'offset del mapping
    uint8_t notch; // indica quando fa muovere un altro rotore (da destra a sinistra)
    char * label;
} Rotor;

typedef struct{
    Mapping mapping;
    char  label;
} Reflector; // fixed direction: from right to left

typedef struct{
     uint8_t board_size;
     uint8_t (*pairs)[2];
} Plugboard;

typedef struct {
    Plugboard plugboard;
    Reflector reflector;
    Rotor rotors[3];
} Enigma;


// Setting up Enigma Models
Enigma* init_enigma(const char *rotor_labels[3], const uint8_t rotor_ring, const uint8_t rotor_position, const char reflector_label, Plugboard plugboard);
void init_mapping(Mapping mapping, const char *alphabet);
void init_reverse_mapping(Mapping reverse_mapping, Mapping mapping);
void init_rotor(Rotor * r, const char * rotor_label, const uint8_t rotor_ring, const uint8_t rotor_position);
void init_reflector(Reflector * reflector, const char reflector_label);
void destroy_all(Enigma * m);
/// Utility functions
uint8_t use_rotor(Rotor * r,  uint8_t n, RotorOrder movement_order);
uint8_t use_rotors(Enigma * m,  uint8_t n, RotorOrder movement_order);
uint8_t use_plugboard(Enigma * m, const uint8_t n);
uint8_t use_reflector(Enigma * m, const uint8_t n);
void move_rotors(Enigma * m);
uint8_t c_to_int(char c);
char int_to_c(uint8_t n);
void make_enigma(Enigma * m, const char * input, size_t len, char * output);
///

// APIs
void enigma_encrypt(Enigma* m, const char * plaintext,  size_t plaintext_len,  char * ciphertext);
void enigma_decrypt(Enigma* m, const char * ciphertext, size_t ciphertext_len, char * plaintext);
//
#endif



