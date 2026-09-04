#include <stdio.h>
#include <string.h>
#include "enigma.h"

// Setting up Enigma Models
void init_mapping(Mapping mapping, const char *alphabet){
    for(size_t i = 0; i< ALPHABET_SIZE; i++){
        mapping[i] = (uint8_t) (alphabet[i] - 'A');
    }
}

void init_reverse_mapping(Mapping revese_mapping, Mapping mapping){
    for(size_t i = 0; i < ALPHABET_SIZE; i++){
       revese_mapping[mapping[i]] = (uint8_t) i;
    }
}

void init_rotor(Rotor * r, const char * rotor_label, const uint8_t rotor_ring, const uint8_t rotor_position){
    r->label = malloc(strlen(rotor_label) + 1);
    if(strcmp(rotor_label, "I") == 0){
        init_mapping(r->forward_mapping, "EKMFLGDQVZNTOWYHXUSPAIBRCJ");
        r->notch = 'Q' - 'A';
        strncpy(r->label, rotor_label , strlen(rotor_label));
    }else if(strcmp(rotor_label, "II") == 0){
        init_mapping(r->forward_mapping, "AJDKSIRUXBLHWTMCQGZNPYFVOE");
        r->notch = 'E' - 'A';
        strncpy(r->label, rotor_label , strlen(rotor_label));
    } else if(strcmp(rotor_label, "III") == 0){
        init_mapping(r->forward_mapping, "BDFHJLCPRTXVZNYEIWGAKMUSQO");
        r->notch = 'V' - 'A';
        strncpy(r->label, rotor_label , strlen(rotor_label));
    } else if(strcmp(rotor_label, "IV") == 0){
        init_mapping(r->forward_mapping, "ESOVPZJAYQUIRHXLNFTGKDCMWB");
        r->notch = 'J' - 'A';
        strncpy(r->label, rotor_label , strlen(rotor_label));
    }else if(strcmp(rotor_label, "V") == 0){
        init_mapping(r->forward_mapping, "VZBRGITYUPSDNHLXAWMJQOFECK");
        r->notch = 'Z' - 'A';
        strncpy(r->label, rotor_label , strlen(rotor_label));
    }
    init_reverse_mapping(r->backward_mapping, r->forward_mapping);
    r->position = rotor_position;
    r->ring = rotor_ring;
}

void init_reflector(Reflector * reflector, const char reflector_label){
    switch (reflector_label) {
        case 'A':{
            init_mapping(reflector->mapping, "EJMZALYXVBWFCRQUONTSPIKHGD");
        }break;
        case 'B':{
            init_mapping(reflector->mapping, "YRUHQSLDPXNGOKMIEBFZCWVJAT");
        }break;
        case 'C':{
            init_mapping(reflector->mapping, "FVPJIAOYEDRZXWGCTKUQSBNMHL");
        }break;
        default:init_mapping(reflector->mapping, "YRUHQSLDPXNGOKMIEBFZCWVJAT");
    }
    reflector->label = reflector_label;
}

Enigma* init_enigma(const char *rotor_labels[3],const uint8_t *rotor_ring, const uint8_t *rotor_position, const char reflector_label, Plugboard plugboard){
    Enigma * m = malloc(sizeof(Enigma));
    init_rotor(&m->rotors[0], rotor_labels[2], rotor_ring[2], rotor_position[2]);
    init_rotor(&m->rotors[1], rotor_labels[1], rotor_ring[1], rotor_position[1]);
    init_rotor(&m->rotors[2], rotor_labels[0], rotor_ring[0], rotor_position[0]);
    init_reflector(&m->reflector, reflector_label);
    if(plugboard.board_size > PLUGBOARD_SIZE){
        printf("An error occurred: the given plugboard_size is greater than the maximum allowed\n");
        exit(0);
    }
    m->plugboard = plugboard;
    return m;
}

void destroy_all(Enigma*m){
    if(m){
     if(m->rotors[0].label){
         free(m->rotors[0].label);
    }
    if(m->rotors[1].label){
        free(m->rotors[1].label);
    }
    if(m->rotors[2].label){
        free(m->rotors[2].label);
    }
     free(m);
    }
}

/// Useful Functions
uint8_t c_to_int(char c){
    return c - 'A';
}

char int_to_c(uint8_t n){
    return n + 'A';
}

/// Main Logic
void move_rotors(Enigma * m){
    if(m->rotors[1].position == m->rotors[1].notch){
        m->rotors[2].position = (m->rotors[2].position + 1) % ALPHABET_SIZE;
        m->rotors[1].position = (m->rotors[1].position + 1) % ALPHABET_SIZE; //double stepping mechanism
    } else if(m->rotors[0].position == m->rotors[0].notch){
        m->rotors[1].position = (m->rotors[1].position + 1) % ALPHABET_SIZE;
    }
    // moving the first rotor of one step
    m->rotors[0].position = (m->rotors[0].position + 1) % ALPHABET_SIZE;
}

uint8_t use_rotor(Rotor * r,  uint8_t n, RotorOrder movement_order){
    n = (n + r->position - r->ring + ALPHABET_SIZE) % ALPHABET_SIZE;
    if (movement_order == RO_FORWARD) {
        n = r->forward_mapping[n];
    }

    if (movement_order == RO_BACKWARD) {
    n = r->backward_mapping[n];
    }
    n = (n - r->position + r->ring + ALPHABET_SIZE) % ALPHABET_SIZE;
    return n;
}

uint8_t use_rotors(Enigma * m, uint8_t n, RotorOrder movement_order){
    switch (movement_order) {
        case RO_FORWARD:{
            n = use_rotor(&m->rotors[0], n, RO_FORWARD);
            n = use_rotor(&m->rotors[1], n, RO_FORWARD);
            n = use_rotor(&m->rotors[2], n, RO_FORWARD);
        }break;

        case RO_BACKWARD:{
            n = use_rotor(&m->rotors[2], n, RO_BACKWARD);
            n = use_rotor(&m->rotors[1], n, RO_BACKWARD);
            n = use_rotor(&m->rotors[0], n, RO_BACKWARD);
        }break;
};
    return n;
}



uint8_t use_plugboard(Enigma * m, const uint8_t n){
    for(uint8_t i = 0; i < m->plugboard.board_size && i < PLUGBOARD_SIZE; i++){
        if(m->plugboard.pairs[i][0] == n){
            return m->plugboard.pairs[i][1];
        } else if(m->plugboard.pairs[i][1] == n){
            return m->plugboard.pairs[i][0];
        }
    }
    return n;
}

uint8_t use_reflector(Enigma* m, const uint8_t n){
    return m->reflector.mapping[n];
}

void make_enigma(Enigma * m, const char * input, size_t len, char * output){
    for(size_t i = 0; i < len; i++){
        uint8_t n = c_to_int(input[i]);
        move_rotors(m);
        n = use_plugboard(m,n);
        n = use_rotors(m, n, RO_FORWARD);
        n = use_reflector(m, n);
        n = use_rotors(m, n, RO_BACKWARD);
        n = use_plugboard(m, n);
        output[i] = int_to_c(n);
    }
    output[len+1] = '\0';
}

void enigma_encrypt(Enigma* m, const char * plaintext,  size_t plaintext_len,  char * ciphertext){
    make_enigma(m,plaintext, plaintext_len , ciphertext);
}

void enigma_decrypt(Enigma* m, const char * ciphertext, size_t ciphertext_len, char * plaintext){
    make_enigma(m,ciphertext, ciphertext_len , plaintext);
}
