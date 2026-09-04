#include <stdint.h>
#include <stdio.h>
#include "enigma.h"

/// ENCRYPTION /// ./enigma_m3 encrypt "PLAINTEXT"
/// DECRYPTION /// ./enigma_m3 decrypt "CIPHERTEXT"

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("ERROR: invalid arguments\n");
        exit(0);
    }
    const char *rotors[3] = {"III", "II", "I"};
    uint8_t rotor_ring[3] = {  2,    1,    1};
    uint8_t rotor_position[3] = {1,  1,    1};
    const char reflector_label = 'B';
    uint8_t pairs[10][2] = {
        {0, 25},  // A <-> Z
        {1, 23},  // B <-> X
        {2, 12},  // C <-> M
        {3, 16},  // D <-> Q
        {4, 22},  // E <-> W
        {5, 11},  // F <-> L
        {6, 17},  // G <-> R
        {7, 19},  // H <-> T
        {8, 24},  // I <-> Y
        {9, 20}   // J <-> U
    };
    Plugboard plugboard = {PLUGBOARD_SIZE, pairs};
    Enigma * m = init_enigma(rotors,rotor_ring,rotor_position,reflector_label, plugboard);
    char * plaintext = malloc(sizeof(char)*strlen(argv[2]));
    char * ciphertext = malloc(sizeof(char)*strlen(argv[2]));
    if (strcmp(argv[1], "encrypt") == 0){
        strncpy(plaintext, argv[2], strlen(argv[2]));
        enigma_encrypt(m, plaintext, strlen(argv[2]), ciphertext);
        printf("%s\n", ciphertext);
    } else if (strcmp(argv[1], "decrypt") == 0) {
        strncpy(ciphertext, argv[2], strlen(argv[2]));
        enigma_decrypt(m, ciphertext, strlen(argv[2]), plaintext);
        printf("%s\n", plaintext);
    } else {
        printf("ERROR: invalid arguments\n");
        exit(0);
    }
    destroy_all(m);
    free(plaintext);
    free(ciphertext);
    return 1;
}


