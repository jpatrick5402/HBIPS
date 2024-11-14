// basic hash table
// uses the function h(k) = k mod m
// where m is the size of the table
struct hash_table {
    int m;
    int * table;
};

typedef struct hash_table HT;

void init_0_ht(HT * t) {
    for (int i = 0; i < t->m; i++) {
        t->table[i] = 0;
    }
}

int get_place(HT * t, char * s) {
    long int sum = 1;
    int i = 0;
    while (s[i] != '\0') {
        sum *= s[i];
        i++;
    }
    int place = sum % t->m;
    if (place < 0) {
        place *= -1;
    }
    return place;
}

void insert_ht(HT * t, char * s) {
    int b = get_place(t, s);
    if (b != 1) {
        t->table[b] += 1;
    }
}
