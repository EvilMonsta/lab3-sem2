union FloatBits {
    float floatNumber;
    struct {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } parts;
};

struct Equipment {
    char bodyType[50];
    float engineCapacity;
    int startReleaseYear;
    int endReleaseYear;
};

struct Car {
    char *model;
    struct Equipment *equipments;
    int equipmentsCount;
};
