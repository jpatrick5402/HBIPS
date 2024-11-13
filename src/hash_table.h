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

void insert_ht(HT * t, char * s) {
    long int sum = 1;
    for (int i = 0; i < 16; i++) {
        sum *= s[i] - '0';
    }
    int place = sum % t->m;
    t->table[place] += 1;
}
